using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;
using AntiDupl.NET.WPF.Core;
using AntiDupl.NET.WPF.View;
using AntiDupl.NET.WPF.ViewModel;
using Microsoft.Extensions.Logging;

using AntiDupl.NET.Core;
using AntiDupl.NET.Core.Original;
using System.Runtime.InteropServices;
using System.Drawing;
using System.ComponentModel.Composition.Primitives;
using MS.WindowsAPICodePack.Internal;

namespace AntiDupl.NET.WPF.Command
{
   public class SearchDllCommand : ICommand
    {
        public event EventHandler CanExecuteChanged;
        private MainViewModel _mainViewModel;
        private CoreLib _core;
        private IWindowService _windowService;
        private ILogger _logger;
        private volatile bool _running;
        private int _lastLoggedPct10 = -1;
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

        public SearchDllCommand(MainViewModel mainViewModel, CoreLib core, IWindowService windowService, ILogger logger)
        {
            this._mainViewModel = mainViewModel;
            this._core = core;
            this._windowService = windowService;
            this._logger = logger;

            //  установка таймера (250 мс — плавность, как в референс-проектах)
            _timer = new DispatcherTimer();
            _timer.Tick += OnTimerTick;
            _timer.Interval = TimeSpan.FromMilliseconds(250);
        }

        public DuplPairViewModel[] GetResults()
        {
            uint resultSize = _core.GetResultSize();
            CoreResult[] core_result = _core.GetResult(0, resultSize);

            List<DuplPairViewModel> result = new List<DuplPairViewModel>();
            if (resultSize > 0)
            {
                for (uint i = 0; i < resultSize; i++)
                {
                    result.Add(new DuplPairViewModel(core_result[i]));
                }
            }
            return result.ToArray();
        }

        void OnTimerTick(object sender, EventArgs e)
        {
            if (_state == StateEnum.Finish)
            {
                _timer.Stop();
                _running = false;
                RaiseCanExecuteChanged();
                _logger.LogInformation("Поиск: завершён, получение результатов");
                _mainViewModel.SetStatus("Поиск завершён, получение результатов…");

                var closeProgressWindow = new Action(_windowService.CloseProgressWindow);
                Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, closeProgressWindow);

                GetResultsAndSetResult();
            }
            if (_state == StateEnum.Stopped)
            {
                _progressDialogViewModel.State = "Stopped";
                _progressDialogViewModel.ProgressMessage = "Stopped";
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
                        if (_mainViewModel.Options.compareOptions.checkOnEquality)
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
                if (mainThreadStatus.total != (int)_progressDialogViewModel.ProgressMax ||
                    currentFirst != (int)_progressDialogViewModel.Progress ||
                    currentSecond != (int)_progressDialogViewModel.CurrentSecond ||
                    path != _progressDialogViewModel.ProgressMessage)
                {
                    _progressDialogViewModel.ProgressMax = (uint)(mainThreadStatus.total);
                    _progressDialogViewModel.Progress = (uint)(currentFirst);
                    _progressDialogViewModel.CurrentSecond = (uint)currentSecond;
                    _progressDialogViewModel.ProgressMessage = path;

                    if (total > 0)
                    {
                        int pct = currentFirst * 100 / total;
                        if (pct / 10 != _lastLoggedPct10)
                        {
                            _lastLoggedPct10 = pct / 10;
                            _logger.LogInformation("Поиск: прогресс {Pct}% ({Cur}/{Tot}), файл: {Path}",
                                pct, currentFirst, total, path);
                            _mainViewModel.SetStatus(string.Format("Идёт поиск… {0}% ({1}/{2})",
                                pct, currentFirst, total));
                        }
                    }
                }
            }
        }

        public bool CanExecute(object parameter)
        {
            return !_running;
        }

        void RaiseCanExecuteChanged()
        {
            if (CanExecuteChanged != null)
                CanExecuteChanged(this, EventArgs.Empty);
        }

        public void Execute(object parameter)
        {
            if (_running)
                return;

            // Initialize
            _progressDialogViewModel = new ProgressDialogViewModel();
            // Set the view model's token source
            _progressDialogViewModel.TokenSource = new CancellationTokenSource();
            _progressDialogViewModel.TokenSource.Token.Register(() => OnCancel());

            _state = StateEnum.Start;
            _lastLoggedPct10 = -1;
            _running = true;
            RaiseCanExecuteChanged();
            _logger.LogInformation("Поиск: старт");
            _mainViewModel.SetStatus("Подготовка поиска…");

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
        }

        /// <summary>
        /// Method for ThreadStart delegate
        /// </summary>
        public void RunProcess()
        {
            var watch = Stopwatch.StartNew();

            _progressDialogViewModel.State = "Search";
            _mainViewModel.SetStatus("Подготовка: передача параметров ядру…");

            // ВАЖНО: НЕ делать здесь _mainViewModel.Options = new CoreOptions(_core) —
            // конструктор CoreOptions(CoreLib) временно сбрасывает опции ядра к дефолтам,
            // и снимок оказывается заполнен дефолтными значениями (алгоритм/порог),
            // что после поиска приводит к сбросу выпадающих списков. Снимок Options
            // синхронизируется с ядром в ApplyCompareOptions, а пути передаются CopyToDll.
            _mainViewModel.LocationsModel.CopyToDll();
            _logger.LogInformation("Поиск: параметры переданы ядру за {Ms} мс (режим={Algo}, порог={Th}%)",
                watch.ElapsedMilliseconds,
                _mainViewModel.Options.compareOptions.AlgorithmComparing,
                _mainViewModel.Options.compareOptions.ThresholdDifference);

            _mainViewModel.SetStatus("Идёт поиск…");
            try
            {
                var result = _core.Search();
            }
            finally
            {
                _logger.LogInformation("Поиск: ядро завершено за {Ms} мс", watch.ElapsedMilliseconds);
            }

            _state = StateEnum.Finish;
        }

        /// <summary>
        /// Получение результатов в фоновом потоке (P/Invoke + построение VM-шек),
        /// затем порционный SetResult в UI-потоке.
        /// </summary>
        void GetResultsAndSetResult()
        {
            Task.Run(() =>
            {
                var sw = Stopwatch.StartNew();
                DuplPairViewModel[] result2 = GetResults();
                _logger.LogInformation("Поиск: получено результатов: {Count} пар за {Ms} мс",
                    result2.Length, sw.ElapsedMilliseconds);
                // SetResult сам маршализует обновление в UI-поток
                _mainViewModel.SetResult(result2);
            });
        }

        private void OnCancel()
        {
            // Поиск уже завершён (окно закрыто штатно после Finish) или уже остановлен —
            // Stop() вызывать нельзя: ядро сбросит результат.
            if (_state == StateEnum.Finish || _state == StateEnum.Stopped)
                return;

            _logger.LogWarning("Поиск: остановлен пользователем");
            _core.Stop();
            _state = StateEnum.Stopped;
            _progressDialogViewModel.IsCancelled = true;
            _running = false;
            RaiseCanExecuteChanged();
            _mainViewModel.SetStatus("Остановлено");
        }
    }
}
