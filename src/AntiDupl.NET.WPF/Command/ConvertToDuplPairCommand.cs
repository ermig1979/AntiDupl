using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;
using System.Windows.Input;
using AntiDuplWPF.Helper;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.Command
{
    class ConvertToDuplPairCommand : ICommand
    {
        public event EventHandler CanExecuteChanged;
        private MainViewModel _mainViewModel;
        private View.IWindowService _windowService;
        public enum StateEnum
        {
            Start,
            Finish
        }
        StateEnum _state = StateEnum.Start;
        ProgressDialogViewModel _progressDialogViewModel;

        public ConvertToDuplPairCommand(MainViewModel mainViewModel, View.IWindowService _windowService)
        {
            this._mainViewModel = mainViewModel;
            this._windowService = _windowService;
        }

        public void Execute(object parameter)
        {
            // Initialize
            _progressDialogViewModel = new ProgressDialogViewModel();

            Thread thread = new Thread(new ThreadStart(RunProcess));
            thread.IsBackground = true; //make them a daemon - prevent thread callback issues
            thread.Name = "ConvertToDuplPairThread";
            thread.Start();

            Thread.Sleep(200);

            if (_state != StateEnum.Finish)
            {
                // Announce that work is starting
                _windowService.OpenProgressWindow(_progressDialogViewModel);
            }
        }

        /// <summary>
        /// Method for ThreadStart delegate
        /// </summary>
        public void RunProcess()
        {
           Application.Current.Dispatcher.Invoke(() =>
           {
               _mainViewModel.ResultList.Clear();
           });
           GroupHelper.GroupToList(_mainViewModel.Groups, _mainViewModel.ResultList);
           Application.Current.Dispatcher.Invoke(() =>
           {
                var res = CollectionViewSource.GetDefaultView(_mainViewModel.ResultList);
                _mainViewModel.Result = (IEditableCollectionView)res;
                _mainViewModel.Groups.Clear();
                _mainViewModel.SelectedTabPageIndex = 0;
           });

            _state = StateEnum.Finish;

        }

        public bool CanExecute(object parameter)
        {
            return _mainViewModel.Groups != null && _mainViewModel.Groups.Any();
        }
    }
}
