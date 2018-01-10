using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.View
{
    public class WindowService : IWindowService
    {
        ProgressDialog _viewOfProgress;
        ComparatorWindow _viewComparator;

        public Window ActiveWindow
        {
            get
            {
                return Application.Current.Windows.OfType<Window>().SingleOrDefault(x => x.IsActive);
            }
        }

        public ProgressDialogViewModel OpenProgressWindow(ProgressDialogViewModel vm)
        {
            //var vm = new ProgressDialogViewModel(mainViewModel);
            //vm.Image = captcha.Image;
            // vm.WindowService = this;

            bool? result = null;

            Window activeWindow = ActiveWindow;
            if (activeWindow == null)
                throw new NullReferenceException("ActiveWindow");

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

            return result == true ? vm : null;
        }

        public void CloseProgressWindow()
        {
            if (_viewOfProgress != null)
            {
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

    }
}
