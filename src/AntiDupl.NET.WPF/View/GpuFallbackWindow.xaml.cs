using System;
using System.Windows;
using Microsoft.Win32;
using AntiDupl.NET.WPF.Helper;
using AntiDupl.NET.WPF.Model;

namespace AntiDupl.NET.WPF.View
{
    /// <summary>
    /// Диалог выбора каталогов CUDA/cuDNN, когда GPU недоступен
    /// (ONNX Runtime не нашёл CUDA/cuDNN, поиск идёт на CPU).
    /// Кнопка «Сохранить и перезапустить» записывает пути в Configuration → Deep;
    /// сами пути вступают в силу после перезапуска приложения
    /// (PATH процесса выставляется на старте, нативные DLL ORT уже загружены).
    /// </summary>
    public partial class GpuFallbackWindow : Window
    {
        public GpuFallbackWindow(IConfigurationModel configuration = null)
        {
            InitializeComponent();
            // Сначала — сохранённые значения (до DataContext): иначе конструктор видел бы
            // пустые поля, детекция запускалась бы всегда и могла перезаписать явно
            // заданные пути, а боксы показывали бы пусто до завершения детекции.
            CudaPath = configuration?.CudaPath ?? "";
            CudnnPath = configuration?.CudnnPath ?? "";
            DataContext = this;
            _configuration = configuration;

            // Автодетекция: пустые поля предзаполняем найденными типовыми путями
            // (реестр NVIDIA / Program Files / поиск по дискам) на фоне — обход диска
            // может занять секунды, UI не блокируем. Найденные пути сразу сохраняются
            // в настройку, чтобы эффект был после перезапуска даже без нажатия «Сохранить».
            if (string.IsNullOrWhiteSpace(CudaPath))
                DetectInBackground(true);
            if (string.IsNullOrWhiteSpace(CudnnPath))
                DetectInBackground(false);
        }

        /// <summary>Каталог bin CUDA (cudart64_*.dll и т.п.).</summary>
        public string CudaPath { get; set; } = "";

        /// <summary>Каталог bin cuDNN (cudnn*.dll).</summary>
        public string CudnnPath { get; set; } = "";

        /// <summary>True, если пользователь сохранил пути (DialogResult = true).</summary>
        public bool Saved { get; private set; }

        readonly IConfigurationModel _configuration;
        string _cudaStatus = "";
        string _cudnnStatus = "";

        void DetectInBackground(bool isCuda)
        {
            string label = isCuda ? LocalizationHelper.S("gpufallback_cuda") : LocalizationHelper.S("gpufallback_cudnn");
            SetStatus(isCuda, label + " " + LocalizationHelper.S("gpufallback_detecting"));
            System.Threading.Tasks.Task.Run(() =>
            {
                string found = isCuda
                    ? GpuLibraryDetector.DetectCudaPath()
                    : GpuLibraryDetector.DetectCudnnPath();
                Dispatcher.BeginInvoke(new Action(() =>
                {
                    if (found != null)
                    {
                        // Прямо в TextBox: CLR-свойство без INPC, а binding поднимет
                        // значение в свойство сам (UpdateSourceTrigger=PropertyChanged).
                        (isCuda ? cudaPathBox : cudnnPathBox).Text = found;
                        string status = label + " " + LocalizationHelper.S("gpufallback_detected") + ": " + found;
                        if (_configuration != null)
                        {
                            // Найденные пути — машина-локальный факт: сохраняем сразу,
                            // чтобы перезапуск работал без дополнительных действий.
                            if (isCuda)
                                _configuration.CudaPath = found;
                            else
                                _configuration.CudnnPath = found;
                            try
                            {
                                _configuration.Save();
                                status += " — " + LocalizationHelper.S("gpufallback_saved");
                            }
                            catch
                            {
                                status += " — " + LocalizationHelper.S("gpufallback_saveFailed");
                            }
                        }
                        SetStatus(isCuda, status);
                    }
                    else
                    {
                        SetStatus(isCuda, label + " " + LocalizationHelper.S("gpufallback_notFound"));
                    }
                }));
            });
        }

        void SetStatus(bool isCuda, string text)
        {
            if (isCuda)
                _cudaStatus = text;
            else
                _cudnnStatus = text;
            statusBox.Text = string.Join("\n", new[] { _cudaStatus, _cudnnStatus })
                .Replace("\n\n", "\n").Trim('\n');
        }

        void BrowseCudaClick(object sender, RoutedEventArgs e)
            => cudaPathBox.Text = PickFolder(cudaPathBox.Text);

        void BrowseCudnnClick(object sender, RoutedEventArgs e)
            => cudnnPathBox.Text = PickFolder(cudnnPathBox.Text);

        void SaveClick(object sender, RoutedEventArgs e)
        {
            Saved = true;
            DialogResult = true;
        }

        void CancelClick(object sender, RoutedEventArgs e)
        {
            Saved = false;
            DialogResult = false;
        }

        static string PickFolder(string initial)
        {
            var dialog = new OpenFolderDialog();
            if (!string.IsNullOrWhiteSpace(initial))
                dialog.InitialDirectory = initial;
            return dialog.ShowDialog() == true ? dialog.FolderName : "";
        }
    }
}