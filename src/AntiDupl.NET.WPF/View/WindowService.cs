using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using AntiDupl.NET.WPF.Helper;
using AntiDupl.NET.WPF.ViewModel;
using Microsoft.Extensions.Logging;

namespace AntiDupl.NET.WPF.View
{
    public class WindowService : IWindowService
    {
        ProgressDialog _viewOfProgress;
        ComparatorWindow _viewComparator;
        readonly ILogger _logger;

        public WindowService()
            : this(null)
        {
        }

        public WindowService(ILogger logger)
        {
            _logger = logger;
        }

        public Window ActiveWindow
        {
            get
            {
                return Application.Current.Windows.OfType<Window>().SingleOrDefault(x => x.IsActive);
            }
        }

        //public Window MainWindow { get; set; }

        public ProgressDialogViewModel OpenProgressWindow(ProgressDialogViewModel vm)
        {
            //var vm = new ProgressDialogViewModel(mainViewModel);
            //vm.Image = captcha.Image;
            // vm.WindowService = this;

            bool? result = null;

            if (ActiveWindow == null)
                throw new NullReferenceException("ActiveWindow");

            Window activeWindow = ActiveWindow;
            if (activeWindow != null)
            {
                _logger?.LogInformation("Окно прогресса: открыто (активное окно '{Window}', поток {Thread})",
                    activeWindow.Title, ThreadHelper.CurrentThreadName());
                //throw new NullReferenceException("ActiveWindow");

                //Application.Current.Dispatcher.Invoke(() =>
                //{
                //Window activeWindow2 = ActiveWindow;

                _viewOfProgress = new ProgressDialog();
                _viewOfProgress.Owner = activeWindow;
                _viewOfProgress.DataContext = vm;
                _viewOfProgress.WindowStartupLocation = WindowStartupLocation.CenterOwner;
                ApplyEffect(_viewOfProgress.Owner);
                result = _viewOfProgress.ShowDialog();
                ClearEffect(_viewOfProgress.Owner);
                //}, DispatcherPriority.Normal);

                //view.Activated += (a, b) => { vm.Load(); };
                //view.Closed += (a, b) => { vm.Dispose(); };
            }
            //else
            //{
            //    _viewOfProgress = new ProgressDialog();
            //    _viewOfProgress.DataContext = vm;
            //    result = _viewOfProgress.ShowDialog();
            //}

            return result == true ? vm : null;
        }

        public void CloseProgressWindow()
        {
            if (_viewOfProgress != null)
            {
                // Программное закрытие: окно не должно трактовать его как отмену
                var vm = _viewOfProgress.DataContext as ProgressDialogViewModel;
                if (vm != null)
                    vm.IsProgrammaticClose = true;

                _logger?.LogInformation("Окно прогресса: закрыто (поток {Thread})", ThreadHelper.CurrentThreadName());
                //Application.Current.Dispatcher.Invoke(() =>
                //{
                _viewOfProgress.Close();
                //}, DispatcherPriority.Normal);
            }
        }

        public void CloseActiveWindow()
        {
            var window = ActiveWindow;
            //window.DialogResult = dialogResult;
            window.Close();
        }

        //System.Windows.Media.Brush _beforeBackground;

        /// <summary>
        /// Apply Blur Effect on the window
        /// </summary>
        /// <param name=”win”></param>
        private void ApplyEffect(Window win)
        {
            System.Windows.Media.Effects.BlurEffect objBlur = new System.Windows.Media.Effects.BlurEffect();
            objBlur.Radius = 4;
            win.Effect = objBlur;
            //_beforeBackground = win.Background;
            //win.Background = System.Windows.Media.Brushes.Black;
            win.Opacity = 0.7;
        }

        /// <summary>
        /// Remove Blur Effects
        /// </summary>
        /// <param name=”win”></param>
        private void ClearEffect(Window win)
        {
            win.Effect = null;
            //win.Background = _beforeBackground;
            win.Opacity = 1.0;
        }

        public void ShowDialogWindow<T>(object dataContext) where T : Window, new()
        {
            bool? result = null;

            Window activeWindow = ActiveWindow;
            if (activeWindow == null)
                throw new NullReferenceException("ActiveWindow");

            var view = new T();
            view.Owner = activeWindow;
            view.DataContext = dataContext;
            view.WindowStartupLocation = WindowStartupLocation.CenterOwner;
            ApplyEffect(view.Owner);
            result = view.ShowDialog();
            ClearEffect(view.Owner);
        }

        public bool? ShowDialogWindow<T>(CloseableViewModel dataContext) where T : Window, new()
        {
            bool? result = null;

            Window activeWindow = ActiveWindow;
            if (activeWindow == null)
                throw new NullReferenceException("ActiveWindow");

            var view = new T();
            view.Owner = activeWindow;
            view.DataContext = dataContext;
            view.WindowStartupLocation = WindowStartupLocation.CenterOwner;
            dataContext.ClosingRequest += (sender, e) =>
            {
                view.DialogResult = e.DialogResult;
                view.Close();
            };
            view.Closing += new System.ComponentModel.CancelEventHandler(dataContext.OnClosing);
            ApplyEffect(view.Owner);
            result = view.ShowDialog();
            ClearEffect(view.Owner);
            return result;
        }
                    

        public void OpenComparatorWindow(ComparatorViewModel vm)
        {
            bool? result = null;

            Window activeWindow = ActiveWindow;
            if (activeWindow == null)
                throw new NullReferenceException("ActiveWindow");

            _viewComparator = new ComparatorWindow();
            _viewComparator.Owner = activeWindow;
            _viewComparator.DataContext = vm;
            _viewComparator.WindowStartupLocation = WindowStartupLocation.CenterOwner;
            ApplyEffect(_viewComparator.Owner);
            result = _viewComparator.ShowDialog();
            ClearEffect(_viewComparator.Owner);
        }

        public void CloseComparatorWindow()
        {
            if (_viewComparator != null)
            {
                _viewComparator.Close();
            }
        }

        public void ShowPleaseWait()
        {
            Window activeWindow = ActiveWindow;
            if (activeWindow != null)
                ApplyEffect(activeWindow);
        }

        public void ClosePleaseWait()
        {
            Window activeWindow = ActiveWindow;
            if (activeWindow != null)
               ClearEffect(activeWindow);
        }


        public void ShowMessage(string message)
        {
            MessageBox.Show(message, "Info", MessageBoxButton.OK, MessageBoxImage.Information);
        }
    }
}
