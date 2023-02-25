using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Threading;
using AntiDupl.NET.WPF.Helper;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.Command
{
    class ConvertToGroupCommand : ICommand
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

        public ConvertToGroupCommand(MainViewModel mainViewModel, View.IWindowService _windowService)
        {
            this._mainViewModel = mainViewModel;
            this._windowService = _windowService;
        }

        public void Execute(object parameter)
        {
            Thread thread = new Thread(new ThreadStart(RunProcess));
            thread.IsBackground = true; //make them a daemon - prevent thread callback issues
            thread.Name = "ConvertToGroupThread";
            thread.Start();

            Thread.Sleep(200);

            if (_state != StateEnum.Finish)
            {
                _progressDialogViewModel = new ProgressDialogViewModel();
                // Announce that work is starting
                _windowService.OpenProgressWindow(_progressDialogViewModel);
            }
        }

        /// <summary>
        /// Method for ThreadStart delegate
        /// </summary>
        public void RunProcess()
        {
            _mainViewModel.Groups.Clear();
            GroupHelper.ConvertToGroup(_mainViewModel.Groups, _mainViewModel.ResultList);

            Application.Current.Dispatcher.Invoke(() =>
            {
                if (_mainViewModel.GroupsCollection == null)
                    _mainViewModel.GroupsCollection = CollectionViewSource.GetDefaultView(_mainViewModel.Groups);
                _mainViewModel.ResultList.Clear();
                _mainViewModel.SelectedTabPageIndex = 1;
            });

            _state = StateEnum.Finish;
            if (_progressDialogViewModel != null)
                Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, (Action)(() => _windowService.CloseProgressWindow()));
        }

        public bool CanExecute(object parameter)
        {
            return _mainViewModel.ResultList != null && _mainViewModel.ResultList.Any();
        }

    }
}
