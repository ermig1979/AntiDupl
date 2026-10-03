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

namespace AntiDupl.NET.WPF
{
    public partial class App
    {
        static ServiceProvider _serviceProvider;

        public App()
        {
            Startup += Application_Startup;
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

            AntiDupl.NET.WPF.Resources.Resources.UserPath = AntiDupl.NET.WPF.Resources.Resources.GetDefaultUserPath();
            services.AddSingleton(new CoreLib(AntiDupl.NET.WPF.Resources.Resources.UserPath));
            services.AddSingleton<IImageLoader, ImageLoader>();
            services.AddSingleton<IThumbnailProvider, ThumbnailProvider>();

            services.AddSingleton<IUndoRedoEngine, UndoRedoEngine>();
            services.AddSingleton<IViewModeModel, ViewModeModel>();
            services.AddSingleton<MainViewModel>();

            _serviceProvider = services.BuildServiceProvider();
            ServiceLocator.Provider = _serviceProvider;

            ILoggerFactory loggerFactory = _serviceProvider.GetRequiredService<ILoggerFactory>();
            loggerFactory.CreateLogger<App>()
                .LogInformation("Запуск приложения. Лог: {LogPath}", fileLogProvider.LogFilePath);

            MainViewModel viewModel = _serviceProvider.GetRequiredService<MainViewModel>();

            var shell = new MainWindow();
            shell.DataContext = viewModel;
            shell.Loaded += new RoutedEventHandler(viewModel.OnLoaded);
            shell.Closing += new CancelEventHandler(viewModel.OnClosing);
            shell.Show();
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