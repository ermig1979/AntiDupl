using System;
using System.ComponentModel;
using System.Windows;
using AntiDuplWPF.Core;
using AntiDuplWPF.Model;
using AntiDuplWPF.Service;
using AntiDuplWPF.View;
using AntiDuplWPF.ViewModel;
using TinyIoC;

namespace AntiDuplWPF
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

            CoreLib core = new CoreLib();
            TinyIoC.TinyIoCContainer.Current.Register<ICoreLib>(core);

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
