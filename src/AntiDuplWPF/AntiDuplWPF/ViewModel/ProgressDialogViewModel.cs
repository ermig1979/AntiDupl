using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;
using AntiDuplWPF.Command;
using AntiDuplWPF.ObjectModel;

namespace AntiDuplWPF.ViewModel
{
    public class ProgressDialogViewModel : PropertyChangedBase
    {
        //private MainViewModel _parentViewModel;
        //private string _progressMessageTemplate;
        private string _cancellationMessage;

        // Property variables
        private uint _progress;
        private string _progressMessage;
        private uint _progressMax;

        /// <summary>
        /// Whether the operation in progress has been cancelled.
        /// </summary>
        /// <remarks> 
        /// The Cancel command is invoked by the Cancel button, and on the window
        /// close (in case the user clicks the close box to cancel. The Cancel 
        /// command sets this property and checks it to make sure that the command 
        /// isn't run twice when the user clicks the Cancel button (once for the 
        /// button-click, and once for the window-close.
        /// </remarks>
        public bool IsCancelled { get; set; }

        public bool IsCancel { get; set; }

        /// <summary>
        /// A cancellation token source for the background operations.
        /// </summary>
        internal CancellationTokenSource TokenSource { get; set; }

        public ObservableCollection<LogEntry> LogItems { get; set; }

        //public LogEntry SelectedLogItem { get; set; }

        //public ProgressDialogViewModel(MainViewModel mainViewModel)
        public ProgressDialogViewModel()
        {
            //_parentViewModel = mainViewModel;
            //_progressMessageTemplate = "Simulated work {0}% complete";
            _cancellationMessage = "progressWindow_CancellationMessage";
            //"Simulated work cancelled";
            //Log = new ObservableCollection<LogEntry>();
            LogItems = new ObservableCollection<LogEntry>();
            this.ClearViewModel();

            //Trace.AutoFlush = true;
            //Trace.Listeners.Add(new TextWriterTraceListener("Log.txt"));
        }

        ICommand _cancelCommand;
        /// <summary>
        /// The Cancel command.
        /// </summary>
        public ICommand CancelCommand
        {
            get
            {
                //return _cancelCommand ?? (_cancelCommand = new RelayCommand(arg =>
                //{
                //    /* The Cancel command is invoked by the Cancel button, and on the window
                //      * close (in case the user clicks the close box to cancel. The Cancel 
                //      * command sets this property and checks it to make sure that the command 
                //      * isn't run twice when the user clicks the Cancel button (once for the 
                //      * button-click, and once for the window-close. */

                //    // Exit if dialog has already been cancelled
                //    if (IsCancelled) 
                //        return;

                //    /* The DoDemoWorkCommand.Execute() method defines a cancellation token source and
                //     * passes it to the Progress Dialog view model. The token itself is passed to the 
                //     * parallel image processing loop defined in the GoCommand.DoWork()  method. We 
                //     * cancel the loop by calling the TokenSource.Cancel() method. */

                //    // Validate TokenSource object
                //    if (TokenSource == null)
                //    {
                //        throw new ApplicationException("ProgressDialogViewModel.TokenSource property is null");
                //    }

                //    // Cancel all pending background tasks
                //    TokenSource.Cancel();
                //}, arg => true));
                return _cancelCommand ?? (_cancelCommand = new RelayCommand(arg =>
                {
                    IsCancel = true;
                    ShowCancellationMessage();
                }, arg => !IsCancelled));
            }
        }

        #region Data Properties

        /// <summary>
        /// The progress of an image processing job.
        /// </summary>
        /// <remarks>
        /// The setter for this property also sets the ProgressMessage property.
        /// </remarks>
        public uint Progress
        {
            get { return _progress; }

            set
            {
                //base.RaisePropertyChangingEvent("Progress");
                _progress = value;
                //base.RaisePropertyChangedEvent("Progress");
                RaisePropertyChangedEvent("Progress");
            }
        }

        /// <summary>
        /// The maximum progress value.
        /// </summary>
        /// <remarks>
        /// The 
        /// </remarks>
        public uint ProgressMax
        {
            get { return _progressMax; }

            set
            {
                //base.RaisePropertyChangingEvent("ProgressMax");
                _progressMax = value;
                //base.RaisePropertyChangedEvent("ProgressMax");
                RaisePropertyChangedEvent("ProgressMax");
            }
        }

        /// <summary>
        /// The status message to be displayed in the View.
        /// </summary>
        public string ProgressMessage
        {
            get { return _progressMessage; }

            set
            {
                //base.RaisePropertyChangingEvent("ProgressMessage");
                _progressMessage = value;
                //base.RaisePropertyChangedEvent("ProgressMessage");
                RaisePropertyChangedEvent("ProgressMessage");
            }
        }

        //string _logMessage;
        //public string LogMessage
        //{
        //    get { return _logMessage; }

        //    set
        //    {
        //        base.RaisePropertyChangingEvent("LogMessage");
        //        _logMessage = value;
        //        base.RaisePropertyChangedEvent("LogMessage");
        //    }
        //}


        string _state;
        public string State
        {
            get { return _state; }
            set
            {
                _state = value;
                RaisePropertyChangedEvent("State");
            }
        }

        uint _currentSecond;
        public uint CurrentSecond
        {
            get { return _currentSecond; }
            set
            {
                _currentSecond = value;
                RaisePropertyChangedEvent("CurrentSecond");
            }
        }

        #endregion

        /// <summary>
        /// Clears the view model.
        /// </summary>
        internal void ClearViewModel()
        {
            //Application.Current.Dispatcher.Invoke(() =>
            //{
            //    Progress = 0;
            //    ProgressMax = 0;
            //    ProgressMessage = "Preparing to perform simulated work.";
            //    this.IsCancelled = false;
            //}, DispatcherPriority.Normal);

            _progress = 0;
            //Progress = 0;
            _progressMax = 0;
            _progressMessage = "progressWindow_PreparingMessage";
            this.IsCancelled = false;
        }


        /// <summary>
        /// Sets the progreess message to show that processing was cancelled.
        /// </summary>
        internal void ShowCancellationMessage()
        {
            this.ProgressMessage = _cancellationMessage;
        }


        public void ReportProgress(uint currentStep, uint maxStep, string message)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                Progress = currentStep;
                ProgressMax = maxStep;
                this.ProgressMessage = message;
            }, DispatcherPriority.Normal);
        }

        public void ReportLog(string message)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                LogItems.Add(new LogEntry(message));
                //Trace.TraceInformation(message);
            }, DispatcherPriority.Normal);
        }

        //public bool Stop { get; set; }

    }
}
