using System;
using System.ComponentModel;
using System.Windows;
using AntiDupl.NET.WPF.Core;
using AntiDupl.NET.WPF.Model;
using AntiDupl.NET.WPF.Properties;
using AntiDupl.NET.WPF.Service;
using AntiDupl.NET.WPF.View;
using AntiDupl.NET.WPF.ViewModel;
using TinyIoC;

using AntiDupl.NET.Core;

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

            ConfigurationModel confModel = ConfigurationModel.Load();
            // Register config
            TinyIoCContainer.Current.Register<IConfigurationModel, ConfigurationModel>(confModel);

            ILanguageService languageService = new LanguageService(confModel);
            TinyIoCContainer.Current.Register<ILanguageService>(languageService);

            AntiDupl.NET.WPF.Resources.Resources.UserPath = AntiDupl.NET.WPF.Resources.Resources.GetDefaultUserPath();
            CoreLib core = new CoreLib(AntiDupl.NET.WPF.Resources.Resources.UserPath);
            TinyIoC.TinyIoCContainer.Current.Register<CoreLib>(core);

            ImageLoader imageLoader = new ImageLoader(core);
            TinyIoC.TinyIoCContainer.Current.Register<IImageLoader>(imageLoader);

            ThumbnailProvider thumbnailProvider = new ThumbnailProvider(imageLoader);
            TinyIoC.TinyIoCContainer.Current.Register<IThumbnailProvider>(thumbnailProvider);

            //IWindowService windowService = new WindowService();
            MainViewModel viewModel = TinyIoCContainer.Current.Resolve<MainViewModel>();
            //var viewModel = new MainViewModel(languageService);

            var shell = new MainWindow();
            //windowService.MainWindow = shell;
            shell.DataContext = viewModel;
            shell.Loaded += new RoutedEventHandler(viewModel.OnLoaded);
            shell.Closing += new CancelEventHandler(viewModel.OnClosing);
            shell.Show();
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
