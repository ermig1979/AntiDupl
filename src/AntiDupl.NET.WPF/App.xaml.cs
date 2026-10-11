using System;
using System.ComponentModel;
using System.IO;
using System.Windows;
using AntiDupl.NET.Core;
using AntiDupl.NET.WPF.Core;
using AntiDupl.NET.WPF.Helper;
using AntiDupl.NET.WPF.Model;
using AntiDupl.NET.WPF.Properties;
using AntiDupl.NET.WPF.Service;
using AntiDupl.NET.WPF.UndoRedo;
using AntiDupl.NET.WPF.View;
using AntiDupl.NET.WPF.ViewModel;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using AntiDupl.NET.Deep.Core;
using AntiDupl.NET.Deep.Extensions;

namespace AntiDupl.NET.WPF
{
    public partial class App
    {
        static ServiceProvider _serviceProvider;

        public App()
        {
            Startup += Application_Startup;
        }

        /// <summary>
        /// Читает параметр вида "&lt;name&gt; &lt;value&gt;" из аргументов командной строки
        /// (зеркало WF Program.GetParameter).
        /// </summary>
        static string GetParameter(string[] args, string name)
        {
            if (args == null)
                return null;
            for (int i = 0; i < args.Length - 1; i++)
            {
                if (String.Compare(args[i], name, true) == 0)
                    return args[i + 1];
            }
            return null;
        }

        private void Application_Startup(object sender, StartupEventArgs e)
        {
            AppDomain.CurrentDomain.UnhandledException +=
                   new UnhandledExceptionEventHandler(CurrentDomain_UnhandledException);
            Application.Current.DispatcherUnhandledException += Current_DispatcherUnhandledException;

            // Стандартное логирование (ILogger) в файл: <BaseDirectory>\logs
            FileLoggerProvider fileLogProvider =
                new FileLoggerProvider(Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "logs"));

            // Стандартный DI-контейнер (Microsoft.Extensions.DependencyInjection)
            ServiceCollection services = new ServiceCollection();
            services.AddLogging(builder =>
            {
                builder.SetMinimumLevel(LogLevel.Information);
                builder.AddProvider(fileLogProvider);
            });

            services.AddSingleton<IConfigurationModel>(ConfigurationModel.Load());
            services.AddSingleton<ILanguageService, LanguageService>();

            // CLI: -s <dir> — кастомный user path (зеркало WF Program.cs); иначе — дефолтный.
            string customSavePath = GetParameter(e.Args, "-s");
            if (customSavePath != null)
            {
                if (!Directory.Exists(customSavePath))
                    throw new Exception(string.Format("The directory '{0}' is not exists!", customSavePath));
                AntiDupl.NET.WPF.Resources.Resources.UserPath = customSavePath;
            }
            else
            {
                AntiDupl.NET.WPF.Resources.Resources.UserPath = AntiDupl.NET.WPF.Resources.Resources.GetDefaultUserPath();
            }
            services.AddSingleton(new CoreLib(AntiDupl.NET.WPF.Resources.Resources.UserPath));
            services.AddSingleton<IImageLoader, ImageLoader>();
            services.AddSingleton<IThumbnailProvider, ThumbnailProvider>();

            services.AddSingleton<IUndoRedoEngine, UndoRedoEngine>();
            services.AddSingleton<IViewModeModel, ViewModeModel>();
            // Синхронизация списков путей (search/ignore/valid/delete) UI → ядро (окно путей).
            services.AddSingleton<ICoreLocationSync, CoreLibLocationSync>();

            // Deep (DINOv2) поиск: C#-пайплайн (AntiDupl.NET.Deep).
            // Модели: <UserPath>\models (DINOv2 — путь или пользовательский URL, LightGlue —
            // авто-скачивание); кэш эмбеддингов/вердиктов: рядом с программой,
            // <BaseDirectory>\deep; логи deep-пайплайна — в общий каталог logs.
            // (решение пользователя: не в %TEMP% и не в AppData).
            string userPath = AntiDupl.NET.WPF.Resources.Resources.UserPath;
            services.AddDeepSearch(
                () =>
                {
                    // Единственный источник раскладки моделей: DeepModelManager.GetModelsDirectory.
                    string modelsDirectory = AntiDupl.NET.Deep.Infra.DeepModelManager.GetModelsDirectory(userPath);
                    Directory.CreateDirectory(modelsDirectory);
                    string cacheDirectory = Path.Combine(AppContext.BaseDirectory, "deep");
                    Directory.CreateDirectory(cacheDirectory);
                    string logDirectory = Path.Combine(AppContext.BaseDirectory, "logs");
                    return (ModelsDirectory: modelsDirectory, CacheDirectory: cacheDirectory, LogDirectory: logDirectory);
                },
                // Явное уведомление (MessageBox на UI-потоке), если GPU недоступен
                // и поиск идёт на CPU; текст дублируется в deep.log.
                message =>
                {
                    try
                    {
                        Application.Current?.Dispatcher.BeginInvoke(new Action(() =>
                            MessageBox.Show(message, "Deep-поиск", MessageBoxButton.OK, MessageBoxImage.Information)));
                    }
                    catch
                    {
                        // UI недоступен — сообщение остаётся в deep.log.
                    }
                });

            services.AddSingleton<MainViewModel>();

            _serviceProvider = services.BuildServiceProvider();
            ServiceLocator.Provider = _serviceProvider;

            // Повреждённые файлы настроек: лог + сообщение пользователю.
            // SerializeHelper сам диалог не показывает (аудит зеркала WF: диалог → событие) —
            // обработка здесь, в UI-слое.
            Helper.SerializeHelperEvents.LoadErrorOccurred += (s, e) =>
            {
                if (_serviceProvider != null)
                    _serviceProvider.GetRequiredService<ILoggerFactory>()
                        .CreateLogger("SerializeHelper")
                        .LogError("SerializeHelper failed to load {File}: {Message}", e.FilePath, e.Message);
                string format = Application.Current?.Resources["serialize_load_error"] as string
                    ?? "File {0} is corrupted and has been reset to default settings";
                MessageBox.Show(string.Format(format, e.FilePath),
                    Application.Current?.Resources["mainwindow_title"] as string ?? "AntiDupl",
                    MessageBoxButton.OK, MessageBoxImage.Warning);
            };

            ILoggerFactory loggerFactory = _serviceProvider.GetRequiredService<ILoggerFactory>();
            loggerFactory.CreateLogger<App>()
                .LogInformation("Запуск приложения. Лог: {LogPath}", fileLogProvider.LogFilePath);

            MainViewModel viewModel = _serviceProvider.GetRequiredService<MainViewModel>();

            // Каталог лога deep-пайплайна задаётся в resolvePaths (FileDeepLogger
            // регистрируется в AddDeepSearch); статический DeepLog удалён.

            // CUDA/cuDNN (как в оригинале): каталоги bin из Configuration → Deep добавляются
            // в начало PATH процесса ДО первой загрузки нативного ORT (первый deep-поиск).
            AddCudaPaths(viewModel.Configuration, loggerFactory.CreateLogger<App>());

            var shell = new MainWindow();
            shell.DataContext = viewModel;
            shell.Loaded += new RoutedEventHandler(viewModel.OnLoaded);
            shell.Closing += new CancelEventHandler(viewModel.OnClosing);
            shell.Show();
        }

        /// <summary>
        /// Как в оригинальном DinoDuplicateSearch (App.AddCudaPaths): каталоги bin CUDA/cuDNN
        /// из Configuration → Deep добавляются в начало PATH процесса. Вызывается на старте,
        /// до первой загрузки ONNX Runtime (CUDA EP ищет cudart/cublas/cudnn через PATH).
        /// После изменения путей требуется перезапуск приложения.
        /// </summary>
        static void AddCudaPaths(IConfigurationModel configuration, ILogger log)
        {
            try
            {
                System.Collections.Generic.List<string> paths = new System.Collections.Generic.List<string>();
                if (!string.IsNullOrWhiteSpace(configuration.CudaPath))
                    paths.Add(configuration.CudaPath.Trim());
                if (!string.IsNullOrWhiteSpace(configuration.CudnnPath))
                    paths.Add(configuration.CudnnPath.Trim());

                if (paths.Count == 0)
                {
                    log.LogInformation("CUDA/cuDNN: пути не заданы (если ORT не найдёт их сам, поиск пойдёт на CPU)");
                    return;
                }

                string currentPath = Environment.GetEnvironmentVariable("PATH") ?? "";
                Environment.SetEnvironmentVariable("PATH", string.Join(";", paths) + ";" + currentPath);
                log.LogInformation("CUDA/cuDNN: добавлено в PATH: {Paths}", string.Join(" | ", paths));
            }
            catch (Exception ex)
            {
                log.LogWarning(ex, "CUDA/cuDNN: не удалось установить PATH");
            }
        }

        void Current_DispatcherUnhandledException(object sender, System.Windows.Threading.DispatcherUnhandledExceptionEventArgs e)
        {
            LogError("DispatcherUnhandledException", e.Exception);
            MessageBox.Show(e.Exception.Message, "Uncaught Thread Exception",
                            MessageBoxButton.OK, MessageBoxImage.Error);
        }

        void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            Exception ex = e.ExceptionObject as Exception;
            LogError("AppDomain.UnhandledException", ex);
            MessageBox.Show(ex.Message, "Uncaught Thread Exception",
                            MessageBoxButton.OK, MessageBoxImage.Error);
        }

        static void LogError(string source, Exception ex)
        {
            if (_serviceProvider == null || ex == null)
                return;
            _serviceProvider.GetRequiredService<ILoggerFactory>()
                .CreateLogger(source).LogError(ex, "{Source}: {Message}", source, ex.Message);
        }
    }
}