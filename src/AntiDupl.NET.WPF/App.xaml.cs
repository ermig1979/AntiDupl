using System;
using System.ComponentModel;
using System.Windows;
using Microsoft.Extensions.DependencyInjection;
using AntiDupl.NET.WPF.Model;
using AntiDupl.NET.WPF.Service;
using AntiDupl.NET.WPF.View;
using AntiDupl.NET.WPF.ViewModel;
using AntiDupl.NET.Core;
using AntiDupl.NET.WPF.UndoRedo;

namespace AntiDupl.NET.WPF
{
    public partial class App
    {
        public App()
        {
            Startup += Application_Startup;
        }

        private void Application_Startup(object sender, StartupEventArgs e)
        {
            AppDomain.CurrentDomain.UnhandledException +=
                   new UnhandledExceptionEventHandler(CurrentDomain_UnhandledException);
            Application.Current.DispatcherUnhandledException += Current_DispatcherUnhandledException;

            var services = new Microsoft.Extensions.DependencyInjection.ServiceCollection();

            ResgisterServices(services);

            using (ServiceProvider serviceProvider = services.BuildServiceProvider())
            {
                //IWindowService windowService = new WindowService();
                MainViewModel viewModel = serviceProvider.GetRequiredService<MainViewModel>();

                var shell = new MainWindow();
                //windowService.MainWindow = shell;
                shell.DataContext = viewModel;
                shell.Loaded += new RoutedEventHandler(viewModel.OnLoaded);
                shell.Closing += new CancelEventHandler(viewModel.OnClosing);
                shell.Show();
            }
        }

        private void ResgisterServices(Microsoft.Extensions.DependencyInjection.ServiceCollection services)
        {
            ConfigurationModel confModel = ConfigurationModel.Load();
            // Register config
            services.AddSingleton<IConfigurationModel>(confModel);

            ILanguageService languageService = new LanguageService(confModel);
            services.AddSingleton<ILanguageService>(languageService);

            AntiDupl.NET.WPF.Resources.Resources.UserPath = AntiDupl.NET.WPF.Resources.Resources.GetDefaultUserPath();
            CoreLib core = new CoreLib(AntiDupl.NET.WPF.Resources.Resources.UserPath);
            services.AddSingleton<CoreLib>(core);

            ImageLoader imageLoader = new ImageLoader(core);
            services.AddSingleton<IImageLoader>(imageLoader);

            ThumbnailProvider thumbnailProvider = new ThumbnailProvider(imageLoader);
            services.AddSingleton<IThumbnailProvider>(thumbnailProvider);

            services
                .AddSingleton<IUndoRedoEngine, UndoRedoEngine>()
                .AddSingleton<IViewModeModel,ViewModeModel>();
        }

        void Current_DispatcherUnhandledException(object sender, System.Windows.Threading.DispatcherUnhandledExceptionEventArgs e)
        {
            MessageBox.Show(e.Exception.Message, "Uncaught Thread Exception",
                            MessageBoxButton.OK, MessageBoxImage.Error);
        }

        void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            Exception ex = e.ExceptionObject as Exception;
            MessageBox.Show(ex.Message, "Uncaught Thread Exception",
                            MessageBoxButton.OK, MessageBoxImage.Error);
        }
    }
}
