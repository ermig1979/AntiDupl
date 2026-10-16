using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Threading;
using AntiDupl.NET.WPF.Command;
using AntiDupl.NET.WPF.Core;
using AntiDupl.NET.WPF.Model;
using AntiDupl.NET.WPF.Service;
using AntiDupl.NET.WPF.View;

using AntiDupl.NET.Core;
using AntiDupl.NET.Deep.Logging;

namespace AntiDupl.NET.WPF.ViewModel
{
    public class ConfigurationViewModel : PropertyChangedBase
    {
        private IConfigurationModel _configuration;
        public IConfigurationModel Configuration
        {
            get { return _configuration; }
        }

        public int Delay { get; set; }
        private DispatcherTimer timer;
        IWindowService _windowService;
        CoreLib _core;
        public CoreOptions Option { get; set; }
        IThumbnailProvider _thumbnailProvider;
        IDeepLogger _deepLogger;

        public ConfigurationViewModel(IConfigurationModel configuration, IWindowService windowService, CoreLib core, CoreOptions option, 
            IThumbnailProvider thumbnailProvider, IDeepLogger deepLogger)
        {
            this._configuration = configuration;
            _windowService = windowService;
            _core = core;
            Delay = 500;
            _thumbnailWidth = _configuration.ThumbnailWidth;
            Option = option;
            _thumbnailProvider = thumbnailProvider;
            _deepLogger = deepLogger;
        }

        int _thumbnailWidth;
        public int ThumbnailWidth
        {
            get { return _thumbnailWidth; }
            set
            {
                if (_thumbnailWidth != value)
                {
                    _thumbnailWidth = value;
                    //_configuration.ThumbnailWidth = value;
                    //ThumbnailProvider.Instance.ClearThumbnailCache();

                    if (this.Delay > 0)
                    {
                        if (timer != null)
                            timer.Stop();

                        if (timer == null)
                        {
                            timer = new DispatcherTimer();
                            timer.Interval = TimeSpan.FromMilliseconds(this.Delay);
                            timer.Tick += (object s, EventArgs ea) =>
                            {
                                DispatcherTimer t = s as DispatcherTimer;
                                t.Stop();

                                _configuration.ThumbnailWidth = _thumbnailWidth;
                                _thumbnailProvider.ClearThumbnailCache();
                            };
                        }

                        timer.Start();
                    }
                    else
                    {
                        _configuration.ThumbnailWidth = value;
                        _thumbnailProvider.ClearThumbnailCache();
                    }
                }
            }
        }

        //System.Windows.Media.Color _testColor;
        //public System.Windows.Media.Color TestColor
        //{
        //    get { return _testColor; }
        //    set
        //    {
        //        _testColor = value;
        //        RaisePropertyChangedEvent("TestColor");
        //    }
        //}


        private bool _deleteResized;
        public bool DeleteResized
        {
            get { return _deleteResized; }
            set
            {
                _deleteResized = value;
                this.RaisePropertyChangedEvent();
            }
        }

        ICommand _changeGoodColorCommand;
        public ICommand ChangeGoodColorCommand
        {
            get
            {
                return _changeGoodColorCommand ?? (_changeGoodColorCommand = new RelayCommand(arg =>
                {
                    ColorViewModel vm = new ColorViewModel(Configuration.GoodColor);
                    _windowService.ShowDialogWindow<ColorWindow>(vm);
                    Configuration.GoodColor = vm.Color;
                }));
            }
        }

        ICommand _changeBadColorCommand;
        public ICommand ChangeBadColorCommand
        {
            get
            {
                return _changeBadColorCommand ?? (_changeBadColorCommand = new RelayCommand(arg =>
                {
                    ColorViewModel vm = new ColorViewModel(Configuration.BadColor);
                    _windowService.ShowDialogWindow<ColorWindow>(vm);
                    Configuration.BadColor = vm.Color;
                }));
            }
        }
        
        // ==== Deep (DINOv2) search ====

        ICommand _downloadLightGlueModelCommand;
        public ICommand DownloadLightGlueModelCommand =>
            _downloadLightGlueModelCommand ??= new RelayCommand(_ => DownloadLightGlueModel(), _ => true);

        bool _isDownloadingLightGlue;
        public bool IsDownloadingLightGlue
        {
            get { return _isDownloadingLightGlue; }
            set { _isDownloadingLightGlue = value; RaisePropertyChangedEvent(); }
        }

        string _lightGlueDownloadStatus = "";
        public string LightGlueDownloadStatus
        {
            get { return _lightGlueDownloadStatus; }
            set { _lightGlueDownloadStatus = value; RaisePropertyChangedEvent(); }
        }

        /// <summary>Path to the SuperPoint+LightGlue model (empty = default folder).</summary>
        public string LightGlueModelPath
        {
            get { return Configuration.LightGlueModelPath; }
            set
            {
                Configuration.LightGlueModelPath = value;
                RaisePropertyChangedEvent();
                RaisePropertyChangedEvent(nameof(LightGlueResolvedPath));
            }
        }

        /// <summary>Path to the DINOv2-base model (empty = default folder; the file is placed manually).</summary>
        public string Dinov2ModelPath
        {
            get { return Configuration.Dinov2ModelPath; }
            set
            {
                Configuration.Dinov2ModelPath = value;
                RaisePropertyChangedEvent();
                RaisePropertyChangedEvent(nameof(Dinov2ResolvedPath));
            }
        }

        /// <summary>Models folder for deep search: &lt;UserPath&gt;\models.</summary>
        private string DeepModelsDirectory =>
            AntiDupl.NET.Deep.Infra.DeepModelManager.GetModelsDirectory(AntiDupl.NET.WPF.Resources.Resources.UserPath);

        /// <summary>CUDA toolkit bin directory (added to PATH at app start; restart required after change).</summary>
        public string CudaPath
        {
            get { return Configuration.CudaPath; }
            set
            {
                Configuration.CudaPath = value;
                RaisePropertyChangedEvent();
            }
        }

        /// <summary>cuDNN bin directory (added to PATH at app start; restart required after change).</summary>
        public string CudnnPath
        {
            get { return Configuration.CudnnPath; }
            set
            {
                Configuration.CudnnPath = value;
                RaisePropertyChangedEvent();
            }
        }

        // ==== Параметры deep-пайплайна (текстовые поля; некорректный ввод игнорируется) ====

        /// <summary>Minimum pair similarity (0..1). Text form for the settings TextBox.</summary>
        public string DeepMinSimilarityText
        {
            get { return Configuration.DeepMinSimilarityForPair.ToString("0.###", System.Globalization.CultureInfo.InvariantCulture); }
            set
            {
                if (TryParseDouble(value, out double v))
                {
                    Configuration.DeepMinSimilarityForPair = Math.Clamp(v, 0.0, 1.0);
                    RaisePropertyChangedEvent();
                }
            }
        }

        /// <summary>Maximum cluster size (≥ 1). Text form for the settings TextBox.</summary>
        public string DeepMaxClusterSizeText
        {
            get { return Configuration.DeepMaxClusterSize.ToString(); }
            set
            {
                if (int.TryParse(value, out int v) && v >= 1)
                {
                    Configuration.DeepMaxClusterSize = v;
                    RaisePropertyChangedEvent();
                }
            }
        }

        /// <summary>Transitivity ratio (0..1, × distance threshold). Text form for the settings TextBox.</summary>
        public string DeepTransitivityRatioText
        {
            get { return Configuration.DeepTransitivityRatio.ToString("0.###", System.Globalization.CultureInfo.InvariantCulture); }
            set
            {
                if (TryParseDouble(value, out double v))
                {
                    Configuration.DeepTransitivityRatio = Math.Clamp(v, 0.0, 1.0);
                    RaisePropertyChangedEvent();
                }
            }
        }

        /// <summary>WGC parallelism (0 = auto). Text form for the settings TextBox.</summary>
        public string DeepWgcParallelismText
        {
            get { return Configuration.DeepWgcParallelism.ToString(); }
            set
            {
                if (int.TryParse(value, out int v) && v >= 0)
                {
                    Configuration.DeepWgcParallelism = v;
                    RaisePropertyChangedEvent();
                }
            }
        }

        /// <summary>Minimum LightGlue match score (0..1). Text form for the settings TextBox.</summary>
        public string DeepMinMatchScoreText
        {
            get { return Configuration.DeepMinMatchScore.ToString("0.###", System.Globalization.CultureInfo.InvariantCulture); }
            set
            {
                if (TryParseDouble(value, out double v))
                {
                    Configuration.DeepMinMatchScore = Math.Clamp(v, 0.0, 1.0);
                    RaisePropertyChangedEvent();
                }
            }
        }

        /// <summary>DINOv2 inference batch size (≥ 1). Text form for the settings TextBox.</summary>
        public string DeepBatchSizeText
        {
            get { return Configuration.DeepBatchSize.ToString(); }
            set
            {
                if (int.TryParse(value, out int v) && v >= 1)
                {
                    Configuration.DeepBatchSize = v;
                    RaisePropertyChangedEvent();
                }
            }
        }

        /// <summary>Prefetch queue depth (0 = off). Text form for the settings TextBox.</summary>
        public string DeepPrefetchCountText
        {
            get { return Configuration.DeepPrefetchCount.ToString(); }
            set
            {
                if (int.TryParse(value, out int v) && v >= 0)
                {
                    Configuration.DeepPrefetchCount = v;
                    RaisePropertyChangedEvent();
                }
            }
        }

        /// <summary>Размер очереди отмены (Advanced, зеркало WF UndoQueueSize). Текстовая форма.</summary>
        public string UndoRedoStackSizeText
        {
            get { return Configuration.UndoRedoStackSize.ToString(); }
            set
            {
                if (uint.TryParse(value, out uint v))
                {
                    Configuration.UndoRedoStackSize = v;
                    RaisePropertyChangedEvent();
                }
            }
        }

        /// <summary>Разбор числа из TextBox: допускает запятую как десятичный разделитель.</summary>
        private static bool TryParseDouble(string? text, out double value)
        {
            value = 0;
            if (string.IsNullOrWhiteSpace(text))
                return false;
            return double.TryParse(text.Replace(',', '.'),
                System.Globalization.NumberStyles.Float, System.Globalization.CultureInfo.InvariantCulture, out value);
        }

        /// <summary>Resolved DINOv2 model path (for display; hint if the file is missing).</summary>
        public string Dinov2ResolvedPath
        {
            get
            {
                string modelsDir = DeepModelsDirectory;
                string? path = AntiDupl.NET.Deep.Infra.DeepModelManager.ResolveDinov2Path(Dinov2ModelPath, modelsDir);
                if (path == null)
                    return AntiDupl.NET.Deep.Infra.DeepModelManager.GetDefaultDinov2Path(modelsDir) +
                           "  <not found: place the file manually (not downloadable)>";
                return path;
            }
        }

        /// <summary>Resolved LightGlue model path (default = models folder; downloaded by the button).</summary>
        public string LightGlueResolvedPath =>
            AntiDupl.NET.Deep.Infra.DeepModelManager.ResolveLightGluePath(LightGlueModelPath, DeepModelsDirectory);

        /// <summary>
        /// Downloads superpoint_lightglue_pipeline.onnx into the models folder
        /// (background thread; status goes to LightGlueDownloadStatus).
        /// dinov2-base.onnx is never downloaded.
        /// </summary>
        private void DownloadLightGlueModel()
        {
            if (_isDownloadingLightGlue)
                return;
            _isDownloadingLightGlue = true;
            RaisePropertyChangedEvent(nameof(IsDownloadingLightGlue));
            LightGlueDownloadStatus = "";
            string target = LightGlueResolvedPath;
            var dispatcher = System.Windows.Application.Current.Dispatcher;
            Task.Run(() =>
            {
                if (System.IO.File.Exists(target))
                {
                    dispatcher.Invoke(() =>
                    {
                        LightGlueDownloadStatus = "Model is up to date: " + target;
                        RaisePropertyChangedEvent(nameof(LightGlueResolvedPath));
                        RaisePropertyChangedEvent(nameof(Dinov2ResolvedPath));
                        RaisePropertyChangedEvent(nameof(Configuration));
                    });
                    _isDownloadingLightGlue = false;
                    RaisePropertyChangedEvent(nameof(IsDownloadingLightGlue));
                    return;
                }
                try
                {
                    var progress = new Progress<AntiDupl.NET.Deep.Models.DeepProgress>(
                        p => dispatcher.Invoke(() => LightGlueDownloadStatus = "Downloading model... " + p.Status));
                    AntiDupl.NET.Deep.Infra.DeepModelManager.DownloadModel(
                        AntiDupl.NET.Deep.Infra.DeepModelManager.LightGluePipelineUrl, target, _deepLogger, progress);
                    dispatcher.Invoke(() =>
                    {
                        LightGlueDownloadStatus = "Model downloaded: " + target;
                        RaisePropertyChangedEvent(nameof(LightGlueResolvedPath));
                    });
                }
                catch (System.Exception ex)
                {
                    dispatcher.Invoke(() => LightGlueDownloadStatus = "Model download failed: " + ex.Message);
                }
                finally
                {
                    _isDownloadingLightGlue = false;
                    RaisePropertyChangedEvent(nameof(IsDownloadingLightGlue));
                }
            });
        }

        // ==== Скачивание модели DINOv2 (пользовательский URL; официальный файл не распространяется) ====

        ICommand _downloadDinov2ModelCommand;
        public ICommand DownloadDinov2ModelCommand =>
            _downloadDinov2ModelCommand ??= new RelayCommand(_ => DownloadDinov2Model(), _ => true);

        bool _isDownloadingDinov2;
        public bool IsDownloadingDinov2
        {
            get { return _isDownloadingDinov2; }
            set { _isDownloadingDinov2 = value; RaisePropertyChangedEvent(); }
        }

        string _dinov2DownloadStatus;
        public string Dinov2DownloadStatus
        {
            get { return _dinov2DownloadStatus; }
            set { _dinov2DownloadStatus = value; RaisePropertyChangedEvent(); }
        }

        /// <summary>URL скачивания модели DINOv2 (пусто = авто-скачивание при поиске отключено).</summary>
        public string Dinov2ModelUrl
        {
            get { return Configuration.Dinov2ModelUrl; }
            set { Configuration.Dinov2ModelUrl = value; RaisePropertyChangedEvent(); }
        }

        private void DownloadDinov2Model()
        {
            if (_isDownloadingDinov2)
                return;
            string url = (Configuration.Dinov2ModelUrl ?? "").Trim();
            if (url.Length == 0)
            {
                Dinov2DownloadStatus = "URL для скачивания не задан (Configuration → Deep).";
                return;
            }
            _isDownloadingDinov2 = true;
            RaisePropertyChangedEvent(nameof(IsDownloadingDinov2));
            Dinov2DownloadStatus = "";
            string target = string.IsNullOrWhiteSpace(Dinov2ModelPath)
                ? AntiDupl.NET.Deep.Infra.DeepModelManager.GetDefaultDinov2Path(DeepModelsDirectory)
                : Dinov2ModelPath;
            var dispatcher = System.Windows.Application.Current.Dispatcher;
            Task.Run(() =>
            {
                if (System.IO.File.Exists(target))
                {
                    dispatcher.Invoke(() =>
                    {
                        Dinov2DownloadStatus = "Модель актуальна: " + target;
                        RaisePropertyChangedEvent(nameof(Dinov2ResolvedPath));
                        RaisePropertyChangedEvent(nameof(Configuration));
                    });
                    _isDownloadingDinov2 = false;
                    RaisePropertyChangedEvent(nameof(IsDownloadingDinov2));
                    return;
                }
                try
                {
                    var progress = new Progress<AntiDupl.NET.Deep.Models.DeepProgress>(
                        p => dispatcher.Invoke(() => Dinov2DownloadStatus = "Скачивание модели... " + p.Status));
                    AntiDupl.NET.Deep.Infra.DeepModelManager.DownloadModel(url, target, _deepLogger, progress);
                    dispatcher.Invoke(() =>
                    {
                        Dinov2DownloadStatus = "Модель скачана: " + target;
                        RaisePropertyChangedEvent(nameof(Dinov2ResolvedPath));
                    });
                }
                catch (System.Exception ex)
                {
                    dispatcher.Invoke(() => Dinov2DownloadStatus = "Не удалось скачать модель: " + ex.Message);
                }
                finally
                {
                    _isDownloadingDinov2 = false;
                    RaisePropertyChangedEvent(nameof(IsDownloadingDinov2));
                }
            });
        }
    }
}
