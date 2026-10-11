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
using AntiDupl.NET.WPF.Helper;
using AntiDupl.NET.WPF.View;
using AntiDupl.NET.WPF.ViewModel;
using Microsoft.Extensions.Logging;

using AntiDupl.NET.Core;
using AntiDupl.NET.Core.Enums;
using AntiDupl.NET.Core.Original;
using AntiDupl.NET.Deep.Models;
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

        // ==== Deep (DINOv2) поиск (C#-пайплайн AntiDupl.NET.Deep) ====
        private bool _deepMode;
        private CancellationTokenSource _deepCts;
        /// <summary>Последний прогресс, записанный фоновым потоком (читает UI-таймер).</summary>
        private volatile DeepProgress _deepProgress;
        /// <summary>Синхронизация «воркер начал поиск» → окно прогресса (заменяет Thread.Sleep).</summary>
        private TaskCompletionSource<bool> _startedTcs;
        private IDeepSearchRunner _deepRunner;
        /// <summary>Адаптер deep-политики (конфигурация/модели/ошибки/результаты) — F4.</summary>
        private IDeepSearchRunner DeepRunner => _deepRunner ??= new DeepSearchRunner(_mainViewModel, _core, _logger);
        public enum StateEnum
        {
            Start,
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
                _mainViewModel.SetStatus(_progressDialogViewModel.IsCancelled
                    ? "Поиск остановлен, получение результатов…"
                    : "Поиск завершён, получение результатов…");

                var closeProgressWindow = new Action(_windowService.CloseProgressWindow);
                Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal, closeProgressWindow);

                if (_deepMode)
                {
                    DeepRunner.DeliverResults();
                }
                else
                    GetResultsAndSetResult();
            }
            if (_state == StateEnum.Stopped)
            {
                _progressDialogViewModel.State = "Stopped";
                _progressDialogViewModel.ProgressMessage = "Stopped";
            }
            else if (_deepMode)
            {
                // Deep-прогресс: проценты C#-пайплайна (ProgressMax=100),
                // статус + текущий файл из DeepProgress.
                var dp = _deepProgress;
                if (dp != null)
                {
                    if (_progressDialogViewModel.ProgressMax != 100)
                        _progressDialogViewModel.ProgressMax = 100;
                    _progressDialogViewModel.Progress = (uint)Math.Clamp(dp.Percent, 0, 100);
                    _progressDialogViewModel.CurrentSecond = 0;
                    _progressDialogViewModel.ProgressMessage =
                        dp.Status + (string.IsNullOrEmpty(dp.FilePath) ? string.Empty : "  [" + dp.FilePath + "]");

                    int pct = (int)dp.Percent;
                    if (pct / 10 != _lastLoggedPct10)
                    {
                        _lastLoggedPct10 = pct / 10;
                        _logger.LogInformation("Deep-поиск: прогресс {Pct}%: {Status}", pct, dp.Status);
                        _mainViewModel.SetStatus(LocalizationHelper.S("status_searching_deep", pct));
                    }
                }
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
                            _mainViewModel.SetStatus(LocalizationHelper.S("status_searching", pct, currentFirst, total));
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

            _startedTcs = new TaskCompletionSource<bool>(TaskCreationOptions.RunContinuationsAsynchronously);
            _state = StateEnum.Start;
            _lastLoggedPct10 = -1;
            _running = true;
            _deepMode = _mainViewModel.AlgorithmComparing == AlgorithmComparing.Dinov2;
            _deepProgress = null;
            _deepCts = new CancellationTokenSource();
            RaiseCanExecuteChanged();
            _logger.LogInformation("Поиск: старт");
            // Deep-режим: в статусе — ключевые параметры запуска, чтобы в UI сразу было
            // видно, с каким порогом мин. уверенности LightGlue идёт поиск (диагностика
            // «настроил 0.6, а ложные пары на месте» — сверить со строкой minMatch в логе).
            _mainViewModel.SetStatus(_deepMode
                ? LocalizationHelper.S("status_preparing_deep",
                    _mainViewModel.Configuration.DeepMinMatchScore.ToString("0.###",
                        System.Globalization.CultureInfo.InvariantCulture))
                : LocalizationHelper.S("status_preparing"));

            Thread thread = new Thread(new ThreadStart(RunProcess));
            thread.IsBackground = true; //make them a daemon - prevent thread callback issues
            thread.Name = "DllThread";
            thread.Start();

            _timer.Start();

            // Окно прогресса открывается, когда воркер сообщил о старте (сигнал в
            // RunProcess/DeepRunProcess после State="Search"); вместо Thread.Sleep(200).
            // Если поиск завершился раньше — окно не показываем (таймер закроет штатно).
            var uiContext = System.Windows.Threading.Dispatcher.CurrentDispatcher;
            _startedTcs.Task.ContinueWith(_ =>
            {
                if (_state != StateEnum.Finish)
                    uiContext.BeginInvoke(new Action(() => _windowService.OpenProgressWindow(_progressDialogViewModel)));
            });
        }

        /// <summary>
        /// Method for ThreadStart delegate
        /// </summary>
        public void RunProcess()
        {
            var watch = Stopwatch.StartNew();

            if (_deepMode)
            {
                DeepRunProcess();
                return;
            }

            _progressDialogViewModel.State = "Search";
            _startedTcs?.TrySetResult(true);
            _mainViewModel.SetStatus("Подготовка: передача параметров ядру…");

            // ВАЖНО: НЕ делать здесь _mainViewModel.Options = new CoreOptions(_core) —
            // конструктор CoreOptions(CoreLib) временно сбрасывает опции ядра к дефолтам,
            // и снимок оказывается заполнен дефолтными значениями (алгоритм/порог),
            // что после поиска приводит к сбросу выпадающих списков. Снимок Options
            // синхронизируется с ядром в ApplyCompareOptions, а пути передаются CopyToDll.
            // Полный комплект опций (search/compare/defect/advanced) передаём в ядро перед
            // поиском: иначе чекбоксы настроек (System/Hidden, defect, useLibJpegTurbo и др.)
            // не отражались бы в DLL. Типы файлов — всегда все (см. CoreOptions.ApplyOptionsTo).
            _mainViewModel.Options.ApplyOptionsTo(_core);
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
        /// Deep (DINOv2) поиск: политика (конфигурация, модели/скачивание, маппинг фатальных
        /// ошибок, конвертация результатов) — <see cref="IDeepSearchRunner"/>;
        /// команда — только окно прогресса, state-машина и отмена.
        /// Алгоритм/порог ядру НЕ передаются (C++ adAlgorithmComparing не знает Dinov2).
        /// </summary>
        void DeepRunProcess()
        {
            var watch = Stopwatch.StartNew();

            _progressDialogViewModel.State = "Search";
            _startedTcs?.TrySetResult(true);

            // Прогресс общий для скачивания моделей и самого поиска.
            var progress = new Progress<DeepProgress>(p => _deepProgress = p);
            try
            {
                DeepRunner.Run(progress, _deepCts.Token);
                _logger.LogInformation("Deep-поиск: завершён за {Ms} мс", watch.ElapsedMilliseconds);
            }
            catch (OperationCanceledException)
            {
                // OnCancel перевёл состояние в Stopped, а deep-пайплайн остановлен токеном:
                // переводим в Finish (как в core-пути — RunProcess безоговорочно ставит Finish
                // после возврата _core.Search()), чтобы OnTimerTick закрыл окно прогресса
                // и вернул кнопку поиска. Частичного результата у deep-пайплайна нет
                // (пары строятся в конце).
                _logger.LogWarning("Deep-поиск: остановлен пользователем");
            }
            catch (Exception ex)
            {
                // Известные ошибки раннер уже маппировал в MessageBox — здесь непредвиденные.
                _logger.LogError(ex, "Deep-поиск: ошибка");
                _mainViewModel.SetStatus("Ошибка deep-поиска: " + ex.Message);
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
            if (_deepMode)
            {
                // Deep-поиск выполняется в C#: останавливаем токенотменением. Остановка
                // действует на границах этапов (скан ядра неделим — завершится сам).
                _deepCts?.Cancel();
            }
            else
            {
                _core.Stop();
            }
            _state = StateEnum.Stopped;
            _progressDialogViewModel.IsCancelled = true;
            _running = false;
            RaiseCanExecuteChanged();
            _mainViewModel.SetStatus("Остановлено");
        }
    }
}
