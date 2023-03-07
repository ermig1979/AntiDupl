using System.Collections.ObjectModel;
using System.Threading;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;
using AntiDupl.NET.WPF.Command;
using AntiDupl.NET.WPF.ObjectModel;

namespace AntiDupl.NET.WPF.ViewModel;

public class ProgressDialogViewModel : PropertyChangedBase
{
    //private MainViewModel _parentViewModel;
    //private string _progressMessageTemplate;
    private readonly string _cancellationMessage;
    private ICommand _cancelCommand;

    // Property variables
    private uint _progress;
    private uint _progressMax;
    private string _progressMessage;

    //public LogEntry SelectedLogItem { get; set; }

    //public ProgressDialogViewModel(MainViewModel mainViewModel)
    public ProgressDialogViewModel()
    {
        //_parentViewModel = mainViewModel;
        //_progressMessageTemplate = "Simulated work {0}% complete";
        _cancellationMessage = "Cancellation";
        //"Simulated work cancelled";
        //Log = new ObservableCollection<LogEntry>();
        //LogItems = new ObservableCollection<LogEntry>();
        ClearViewModel();

        //Trace.AutoFlush = true;
        //Trace.Listeners.Add(new TextWriterTraceListener("Log.txt"));
    }

    /// <summary>
    ///     Whether the operation in progress has been cancelled.
    /// </summary>
    /// <remarks>
    ///     The Cancel command is invoked by the Cancel button, and on the window
    ///     close (in case the user clicks the close box to cancel. The Cancel
    ///     command sets this property and checks it to make sure that the command
    ///     isn't run twice when the user clicks the Cancel button (once for the
    ///     button-click, and once for the window-close.
    /// </remarks>
    public bool IsCancelled { get; set; }

    /// <summary>
    ///     A cancellation token source for the background operations.
    /// </summary>
    internal CancellationTokenSource TokenSource { get; set; }

    public ObservableCollection<LogEntry> LogItems { get; set; }

    /// <summary>
    ///     The Cancel command.
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
            return _cancelCommand ??= new RelayCommand(arg =>
            {
                // Cancel all pending background tasks
                TokenSource?.Cancel();
                ShowCancellationMessage();
                IsCancelled = true;
            }, arg => !IsCancelled);
        }
    }

    /// <summary>
    ///     Clears the view model.
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
        _progressMessage = "Preparing";
        IsCancelled = false;
    }


    /// <summary>
    ///     Sets the progreess message to show that processing was cancelled.
    /// </summary>
    internal void ShowCancellationMessage()
    {
        ProgressMessage = _cancellationMessage;
    }


    public void ReportProgress(uint currentStep, uint maxStep, string message)
    {
        Application.Current.Dispatcher.Invoke(() =>
        {
            Progress = currentStep;
            ProgressMax = maxStep;
            ProgressMessage = message;
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

    #region Data Properties

    /// <summary>
    ///     The progress of an image processing job.
    /// </summary>
    /// <remarks>
    ///     The setter for this property also sets the ProgressMessage property.
    /// </remarks>
    public uint Progress
    {
        get => _progress;
        set
        {
            _progress = value;
            RaisePropertyChangedEvent(nameof(Progress));
        }
    }

    /// <summary>
    ///     The maximum progress value.
    /// </summary>
    /// <remarks>
    ///     The
    /// </remarks>
    public uint ProgressMax
    {
        get => _progressMax;
        set
        {
            _progressMax = value;
            RaisePropertyChangedEvent(nameof(ProgressMax));
        }
    }

    /// <summary>
    ///     The status message to be displayed in the View.
    /// </summary>
    public string ProgressMessage
    {
        get => _progressMessage;
        set
        {
            _progressMessage = value;
            RaisePropertyChangedEvent(nameof(ProgressMessage));
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


    private string _state;

    public string State
    {
        get => _state;
        set
        {
            _state = value;
            RaisePropertyChangedEvent(nameof(State));
        }
    }

    private uint _currentSecond;

    public uint CurrentSecond
    {
        get => _currentSecond;
        set
        {
            _currentSecond = value;
            RaisePropertyChangedEvent(nameof(CurrentSecond));
        }
    }

    #endregion

    //public bool Stop { get; set; }
}