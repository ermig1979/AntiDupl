using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;
using AntiDuplWPF.Core;
using AntiDuplWPF.View;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.Command
{
   public class SearchDllCommand : ICommand
    {
        public event EventHandler CanExecuteChanged;
        private MainViewModel _mainViewModel;
        private CoreLib _core;
        private IWindowService _windowService;
        DispatcherTimer _timer;
        ProgressDialogViewModel _progressDialogViewModel;
        public enum StateEnum
        {
            Start,
            Work,
            Stopped,
            Finish
        }
        StateEnum _state = StateEnum.Start;

        public SearchDllCommand(MainViewModel mainViewModel, CoreLib core, IWindowService windowService)
        {
            this._mainViewModel = mainViewModel;
            this._core = core;
            this._windowService = windowService;

            //  установка таймера
            _timer = new DispatcherTimer();
            _timer.Tick += OnTimerTick;
            _timer.Interval = new TimeSpan(0, 0, 1);
        }

        void OnTimerTick(object sender, EventArgs e)
        {
            if (_state == StateEnum.Finish)
            {
                _timer.Stop();
                var closeProgressWindow = new Action(_windowService.CloseProgressWindow);
                Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, closeProgressWindow);
            }
            else
            {
                double progress = 0;
                int total = 0, currentFirst = 0, currentSecond = 0;
                string path = "";

                CoreStatus mainThreadStatus = _core.StatusGet(CoreDll.ThreadType.Main, 0);
                if (mainThreadStatus != null)
                {
                    total = mainThreadStatus.total;
                    if (mainThreadStatus.current > 0)
                    {
                        if (_mainViewModel.Options.CompareOptions.CheckOnEquality)
                        {
                            for (int i = 0; ; i++)
                            {
                                CoreStatus compareThreadStatus = _core.StatusGet(CoreDll.ThreadType.Compare, i);
                                if (compareThreadStatus == null)
                                    break;
                                if (i == 0)
                                {
                                    path = compareThreadStatus.path;
                                }
                                currentFirst += compareThreadStatus.current;
                                currentSecond += compareThreadStatus.total;
                            }
                        }
                        else
                        {
                            currentFirst = mainThreadStatus.current;
                            for (int i = 0; ; i++)
                            {
                                CoreStatus collectThreadStatus = _core.StatusGet(CoreDll.ThreadType.Collect, i);
                                if (collectThreadStatus == null)
                                    break;
                                if (i == 0)
                                {
                                    path = collectThreadStatus.path;
                                }
                                currentFirst += collectThreadStatus.current;
                                currentFirst -= collectThreadStatus.total;
                            }
                        }
                    }
                    else
                    {
                        path = mainThreadStatus.path;
                    }
                }
                _progressDialogViewModel.ProgressMax = (uint)(mainThreadStatus.total);
                _progressDialogViewModel.Progress = (uint)(currentFirst);
                _progressDialogViewModel.CurrentSecond = (uint)currentSecond;
                //_progressDialogViewModel.ProgressMessage = status.path;
            }
        }

        public bool CanExecute(object parameter)
        {
            return true;
        }

        public void Execute(object parameter)
        {
            // Initialize
            _progressDialogViewModel = new ProgressDialogViewModel();
            // Set the view model's token source
            _progressDialogViewModel.TokenSource = new CancellationTokenSource();

            Thread thread = new Thread(new ThreadStart(RunProcess));
            thread.IsBackground = true; //make them a daemon - prevent thread callback issues
            thread.Name = "DllThread";
            thread.Start();

            _timer.Start();

            Thread.Sleep(200);

            if (_state != StateEnum.Finish)
            {
                // Announce that work is starting
                //_mainViewModel.RaiseWorkStartedEvent();
                _windowService.OpenProgressWindow(_progressDialogViewModel);
            }
            else
            {
            }
        }

        /// <summary>
        /// Method for ThreadStart delegate
        /// </summary>
        public void RunProcess()
        {
            var progressDialogViewModel = _progressDialogViewModel;

            var watch = System.Diagnostics.Stopwatch.StartNew();

            progressDialogViewModel.State = "progressWindow_ComputeHashes2";

            _mainViewModel.Options.CopyToDll();
            _mainViewModel.LocationsModel.CopyToDll();

            var result = _core.Search();

            watch.Stop();
            var elapsedMs = watch.ElapsedMilliseconds;
            //MessageBox.Show(elapsedMs.ToString());


            DuplPairViewModel[] result2 = _core.GetResult();

            if (result2.Any())
                _mainViewModel.SetResult(result2);

            _state = StateEnum.Finish;
        }

    }
}
