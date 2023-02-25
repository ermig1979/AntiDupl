using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;
using AntiDuplWPF.Core;
using AntiDuplWPF.ViewModel;

using AntiDupl.NET.Core;
using AntiDupl.NET.Core.Original;

namespace AntiDuplWPF.Command
{
    class CalculateHistogramPeaksCommand :  ICommand
    {
        private ViewModel.MainViewModel _mainViewModel;
        private CoreLib _core;
        private View.IWindowService _windowService;
        //private IEnumerable<DuplPairViewModel> _results;
        System.Collections.ObjectModel.ObservableCollection<DuplPairViewModel> _resultList;
        ProgressDialogViewModel _progressDialogViewModel;

        CoreDll.adImageInfoW[] _arrayInfo;

        DispatcherTimer _timer = new DispatcherTimer();
        StateEnum _state = StateEnum.Start;
        public enum StateEnum
        {
            Start,
            Work,
            Stopped,
            Finish
        }

        public CalculateHistogramPeaksCommand(ViewModel.MainViewModel mainViewModel,
            CoreLib core, View.IWindowService windowService, 
            System.Collections.ObjectModel.ObservableCollection<DuplPairViewModel> resultList)
        {
            _mainViewModel = mainViewModel;
            _core = core;
            this._windowService = windowService;
            _resultList = resultList;
            
            if (_resultList == null)
                throw new NullReferenceException("_results");

            _resultList.CollectionChanged += _resultList_CollectionChanged;

            //  установка таймера
            _timer = new DispatcherTimer();
            _timer.Tick += OnTimerTick;
            _timer.Interval = new TimeSpan(0, 0, 1);
        }

        void _resultList_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            if (CanExecuteChanged != null)
                CanExecuteChanged(this, e);
        }

        void OnTimerTick(object sender, EventArgs e)
        {
            if (_state == StateEnum.Finish
                || _state == StateEnum.Stopped)
            {
                _timer.Stop();
                var closeProgressWindow = new Action(_windowService.CloseProgressWindow);
                Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, closeProgressWindow);


                //if (_arrayInfo.All(i => i.jpegPeaks == 0))
                //    throw new Exception("All jpegPeaks == 0");

                foreach (var result in _resultList)
                {
                    var first = _arrayInfo.Single(i => i.path == result.FirstFile.Path);
                    var second = _arrayInfo.Single(i => i.path == result.SecondFile.Path);

                    result.FirstFile.JpegPeaks = first.jpegPeaks;
                    result.SecondFile.JpegPeaks = second.jpegPeaks;
                }
            }
            else
            {
                CoreStatus mainThreadStatus = _core.StatusGet(CoreDll.ThreadType.Main, 0);
                if (mainThreadStatus != null)
                {

                    _progressDialogViewModel.ProgressMax = (uint)(mainThreadStatus.total);
                    _progressDialogViewModel.Progress = (uint)(mainThreadStatus.current);
                    // _progressDialogViewModel.CurrentSecond = (uint)currentSecond;
                    _progressDialogViewModel.ProgressMessage = mainThreadStatus.path;
                }

            }
        }

        /// <summary>
        /// Method for ThreadStart delegate
        /// </summary>
        public void RunProcess()
        {
            _progressDialogViewModel.State = "Calculate DCT Histogram Peaks";

            // TODO: -> New feature for Net.Core from WPF branch, not jet implemented
            // -> Core and Anitdupl nativ update needed
            /*
            CoreDll.WorkProgressInteropNegotiator negotiator = new CoreDll.WorkProgressInteropNegotiator();
            negotiator.cancellationPending = new CoreDll.CancellationPendingCallback(() => { return _progressDialogViewModel.IsCancel; });

            // Refer for details to
            // "How to: Marshal Callbacks and Delegates Using C++ Interop" 
            // http://msdn.microsoft.com/en-us/library/367eeye0%28v=vs.100%29.aspx
            GCHandle gch = GCHandle.Alloc(negotiator);

            _mainViewModel.Options = new CoreOptions(_core);

            _mainViewModel.LocationsModel.CopyToDll();

            var array = _resultList.Select(r => r.FirstFile).Union(_resultList.Select(r2 => r2.SecondFile));
            var arrayDistinct = array.Distinct();
            _arrayInfo = arrayDistinct.Select(i => new CoreDll.adImageInfoW(i)).ToArray();
                //new CoreDll.adResultW(r)).ToArray();

            //foreach (var item in arrayInfo)
            //{
            //    var sear = arrayInfo.Where(i => i.path == item.path).ToArray();
            //     var first = arrayInfo.Single(i => i.path == item.path);
            //}

            _core.CalculateHistogramPeaks(_arrayInfo, negotiator);

            // DuplPairViewModel[] result2 = _core.GetResult();
            //if (result2.Any())
           //     _mainViewModel.SetResult(result2);
            */

            _state = StateEnum.Finish;
        }


        public event EventHandler CanExecuteChanged;

        public bool CanExecute(object parameter)
        {
            return _resultList.Any();
            //return true;
        }

        public void Execute(object parameter)
        {
            // Initialize
            _progressDialogViewModel = new ProgressDialogViewModel();
            // Set the view model's token source
            _progressDialogViewModel.TokenSource = new CancellationTokenSource();
            //_progressDialogViewModel.TokenSource.Token.Register(() => OnCancel());

            _state = StateEnum.Start;

            Thread thread = new Thread(new ThreadStart(RunProcess));
            thread.IsBackground = true; //make them a daemon - prevent thread callback issues
            thread.Name = "CalculateHistogramPeaksThread";
            thread.Start();

            _timer.Start();

            Thread.Sleep(200);

            if (_state != StateEnum.Finish)
            {
                // Announce that work is starting
                //_mainViewModel.RaiseWorkStartedEvent();
                _windowService.OpenProgressWindow(_progressDialogViewModel);
            }
        }
     

        //private void OnCancel()
        //{
        //    _core.Stop();
        //    _state = StateEnum.Stopped;
        //}
    }
}
