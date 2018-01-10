using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using AntiDuplWPF.Model;
using AntiDuplWPF.Service;
using AntiDuplWPF.View;
using AntiDuplWPF.ViewModel;
using TinyIoC;

namespace AntiDuplWPF
{
    /// <summary>
    /// Логика взаимодействия для App.xaml
    /// </summary>
    public partial class App : Application
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

            LanguageService languageService = new LanguageService(confModel);
            TinyIoCContainer.Current.Register<ILanguageService, LanguageService>(languageService);

            var viewModel = new MainViewModel();

            var shell = new MainWindow();
            shell.DataContext = viewModel;
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
