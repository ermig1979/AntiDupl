using System;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using AntiDupl.NET.Core;
using AntiDupl.NET.Core.Original;
using AntiDupl.NET.Deep.Core;
using AntiDupl.NET.Deep.Infra;
using AntiDupl.NET.Deep.Models;
using AntiDupl.NET.WPF.Core;
using AntiDupl.NET.WPF.ViewModel;
using Microsoft.Extensions.Logging;

namespace AntiDupl.NET.WPF.Command
{
    /// <summary>
    /// Deep (DINOv2) поиск: адаптер use case'а <see cref="DeepSearchService"/> к UI.
    /// Владеет «политикой» запуска: сборка <see cref="DeepSettings"/> из Configuration,
    /// разрешение/авто-скачивание моделей, маппинг фатальных ошибок в MessageBox,
    /// конвертация <see cref="DeepPair"/>[] → <see cref="DuplPairViewModel"/>[].
    /// Из SearchDllCommand вынесено как часть clean-arch рефакторинга: команда
    /// осталась тонким «ICommand + прогресс-презентер + state-машина».
    /// </summary>
    public interface IDeepSearchRunner
    {
        /// <summary>Сервис deep-поиска зарегистрирован (AddDeepSearch в хосте).</summary>
        bool IsAvailable { get; }

        /// <summary>
        /// Блокирующий запуск: модели (скачивание при отсутствии) → поиск.
        /// Фатальные ошибки маппятся в MessageBox здесь же; результат — в свойстве-возврате.
        /// </summary>
        DeepPair[] Run(IProgress<DeepProgress> progress, CancellationToken ct);

        /// <summary>
        /// Конвертация результата последнего Run в UI-модели и порционный SetResult
        /// (вызывается командой в UI-потоке после Finish; маппинг — фоновым Task).
        /// </summary>
        void DeliverResults();
    }

    public sealed class DeepSearchRunner : IDeepSearchRunner
    {
        private readonly MainViewModel _vm;
        private readonly CoreLib _core;
        private readonly ILogger _logger;
        private DeepPair[] _lastResult = System.Array.Empty<DeepPair>();

        public DeepSearchRunner(MainViewModel vm, CoreLib core, ILogger logger)
        {
            _vm = vm;
            _core = core;
            _logger = logger;
        }

        public bool IsAvailable => _vm.DeepSearchService != null;

        public DeepPair[] Run(IProgress<DeepProgress> progress, CancellationToken ct)
        {
            var watch = System.Diagnostics.Stopwatch.StartNew();

            DeepSearchService deep = _vm.DeepSearchService;
            if (deep == null)
            {
                _logger.LogError("Deep-поиск: сервис DeepSearchService не зарегистрирован (App.xaml.cs)");
                _vm.SetStatus("Deep-поиск недоступен: сервис не зарегистрирован");
                ShowDeepFatalError("Deep-поиск недоступен: сервис DeepSearchService не зарегистрирован.\n\n"
                    + "Пайплайн подключается вызовом AddDeepSearch в App.xaml.cs.\n"
                    + "Проверьте, что приложение собрано из текущей кодовой базы.");
                return System.Array.Empty<DeepPair>();
            }

            _vm.SetStatus("Подготовка deep-поиска…");

            // Ядру передаются пути и фильтры — как и при обычном поиске.
            _vm.LocationsModel.CopyToDll();

            // Каталог моделей — единственный источник раскладки: DeepModelManager.
            // Модели — рядом с бинарником (<BaseDirectory>\models); одинаковы для всех
            // пользователей, поэтому не зависят от user-каталога (CLI -s).
            string modelsDir = DeepModelManager.GetModelsDirectory(System.AppContext.BaseDirectory);

            var settings = new DeepSettings
            {
                // Порог в UI: 0..100 (%) → DistanceThreshold = T/100 (косинусная дистанция)
                DistanceThreshold = (float)Math.Clamp(_vm.ThresholdDifference, 0, 100) / 100f,
                GeometricCheckEnabled = _vm.Configuration.VerifyByLightGlue,
                Dinov2ModelPath = DeepModelManager.ResolveDinov2Path(_vm.Configuration.Dinov2ModelPath, modelsDir),
                LightGlueModelPath = _vm.Configuration.VerifyByLightGlue
                    ? DeepModelManager.ResolveLightGluePath(_vm.Configuration.LightGlueModelPath, modelsDir)
                    : null,
                // Параметры пайплайна из Configuration → Deep (паритет SearchSettings оригинала).
                MinSimilarityForPair = (float)Math.Clamp((double)_vm.Configuration.DeepMinSimilarityForPair, 0.0, 1.0),
                MaxClusterSize = Math.Max(1, _vm.Configuration.DeepMaxClusterSize),
                TransitivityRatio = (float)Math.Clamp((double)_vm.Configuration.DeepTransitivityRatio, 0.0, 1.0),
                // 0 = авто: половина числа процессоров (как в оригинале).
                WgcParallelism = _vm.Configuration.DeepWgcParallelism > 0
                    ? Math.Min(_vm.Configuration.DeepWgcParallelism, Environment.ProcessorCount)
                    : Math.Max(1, Environment.ProcessorCount / 2),
                MinMatchScore = (float)Math.Clamp((double)_vm.Configuration.DeepMinMatchScore, 0.0, 1.0),
                // Производительность (паритет оригинала: batchSize=32, prefetch=2).
                BatchSize = Math.Max(1, _vm.Configuration.DeepBatchSize),
                PrefetchCount = Math.Max(0, _vm.Configuration.DeepPrefetchCount),
            };

            // Единый информационный блок: что приложение видит до старта пайплайна
            // (models-каталог, разрешённые пути моделей, WGC, порог, кэш, поддержка ядром).
            _logger.LogInformation(
                "Deep-поиск: конфигурация — modelsDir={ModelsDir}, dinov2={Dinov2}, lightglue={LightGlue}, WGC={Wgc}, порог={Threshold}%, minSim={MinSim}, maxCluster={MaxCluster}, transRatio={TransRatio}, wgcPar={WgcPar}, minMatch={MinMatch}, batch={Batch}, prefetch={Prefetch}, кэш={CacheDir}, adScanCandidatesW={Deep}",
                modelsDir,
                settings.Dinov2ModelPath,
                settings.GeometricCheckEnabled ? settings.LightGlueModelPath : "off",
                settings.GeometricCheckEnabled,
                _vm.ThresholdDifference,
                settings.MinSimilarityForPair,
                settings.MaxClusterSize,
                settings.TransitivityRatio,
                settings.WgcParallelism,
                settings.MinMatchScore,
                settings.BatchSize,
                settings.PrefetchCount,
                System.IO.Path.Combine(AppContext.BaseDirectory, "deep"),
                _core.DeepSearchSupported);

            // DINOv2: при отсутствии модели пробуем скачать (URL — Configuration → Deep).
            // Официальный dinov2-base.onnx не распространяется — URL пользовательский.
            if (string.IsNullOrEmpty(settings.Dinov2ModelPath) || !System.IO.File.Exists(settings.Dinov2ModelPath))
            {
                string url = (_vm.Configuration.Dinov2ModelUrl ?? "").Trim();
                if (url.Length > 0)
                {
                    string target = string.IsNullOrWhiteSpace(_vm.Configuration.Dinov2ModelPath)
                        ? DeepModelManager.GetDefaultDinov2Path(modelsDir)
                        : _vm.Configuration.Dinov2ModelPath;
                    try
                    {
                        _logger.LogInformation("Deep-поиск: скачивание модели DINOv2: {Url} -> {Target}", url, target);
                        _vm.SetStatus("Скачивание модели DINOv2…");
                        DeepModelManager.DownloadModel(url, target, _vm.DeepLogger, progress, ct);
                        if (System.IO.File.Exists(target))
                        {
                            settings.Dinov2ModelPath = target;
                            _logger.LogInformation("Deep-поиск: модель DINOv2 скачана: {Path}", target);
                        }
                    }
                    catch (OperationCanceledException) when (ct.IsCancellationRequested)
                    {
                        // Отмена поиска — не фатальная ошибка скачивания.
                        throw;
                    }
                    catch (Exception ex)
                    {
                        _logger.LogError(ex, "Deep-поиск: не удалось скачать модель DINOv2: {Url}", url);
                        ShowDeepFatalError("Не удалось скачать модель DINOv2:\n\n"
                            + ex.Message + "\n\nURL: " + url + "\nНазначение: " + target);
                        return System.Array.Empty<DeepPair>();
                    }
                }

                if (string.IsNullOrEmpty(settings.Dinov2ModelPath) || !System.IO.File.Exists(settings.Dinov2ModelPath))
                {
                    string expectedDefault = DeepModelManager.GetDefaultDinov2Path(modelsDir);
                    _logger.LogError("Deep-поиск: не найдена модель DINOv2: {Path}", settings.Dinov2ModelPath);
                    _vm.SetStatus("Deep-поиск: не найдена модель DINOv2. Положите файл в каталог моделей или укажите путь/URL в Configuration.");
                    ShowDeepFatalError("Модель DINOv2 не найдена — deep-поиск не запускается.\n\n"
                        + (string.IsNullOrWhiteSpace(_vm.Configuration.Dinov2ModelPath)
                            ? string.Empty
                            : "Путь из Configuration (файл не найден): " + _vm.Configuration.Dinov2ModelPath + "\n")
                        + "Ожидаемый файл: " + expectedDefault + "\n"
                        + "Каталог моделей: " + modelsDir + "\n\n"
                        + "Положите dinov2-base.onnx в каталог моделей,\n"
                        + "укажите полный путь к модели или URL для скачивания: Configuration → Deep.");
                    return System.Array.Empty<DeepPair>();
                }
            }

            // LightGlue (WGC): при отсутствии модели — авто-скачивание со стандартного GitHub Releases.
            if (settings.GeometricCheckEnabled
                && (string.IsNullOrEmpty(settings.LightGlueModelPath) || !System.IO.File.Exists(settings.LightGlueModelPath)))
            {
                string target = DeepModelManager.GetDefaultLightGluePath(modelsDir);
                try
                {
                    _logger.LogInformation("Deep-поиск: скачивание модели SuperPoint+LightGlue: {Url}", DeepModelManager.LightGluePipelineUrl);
                    _vm.SetStatus("Скачивание модели SuperPoint+LightGlue…");
                    DeepModelManager.DownloadModel(DeepModelManager.LightGluePipelineUrl, target, _vm.DeepLogger, progress, ct);
                    if (System.IO.File.Exists(target))
                    {
                        settings.LightGlueModelPath = target;
                        _logger.LogInformation("Deep-поиск: модель SuperPoint+LightGlue скачана: {Path}", target);
                    }
                }
                catch (OperationCanceledException) when (ct.IsCancellationRequested)
                {
                    // Отмена поиска — не фатальная ошибка скачивания.
                    throw;
                }
                catch (Exception ex)
                {
                    _logger.LogError(ex, "Deep-поиск: не удалось скачать модель SuperPoint+LightGlue");
                    ShowDeepFatalError("Не удалось скачать модель SuperPoint+LightGlue (WGC-верификация):\n\n"
                        + ex.Message + "\n\nURL: " + DeepModelManager.LightGluePipelineUrl + "\nНазначение: " + target);
                    return System.Array.Empty<DeepPair>();
                }
                if (string.IsNullOrEmpty(settings.LightGlueModelPath) || !System.IO.File.Exists(settings.LightGlueModelPath))
                {
                    ShowDeepFatalError("Модель SuperPoint+LightGlue не найдена — WGC-верификация не запускается.\n\n"
                        + "Ожидаемый файл: " + target + "\nКаталог моделей: " + modelsDir + "\n\n"
                        + "Скачайте модель в Configuration → Deep или отключите верификацию LightGlue.");
                    return System.Array.Empty<DeepPair>();
                }
            }

            _vm.SetStatus("Идёт deep-поиск…");
            _lastResult = deep.Search(settings, progress, ct);
            _logger.LogInformation("Deep-поиск: завершён за {Ms} мс (пар: {Count}, верификация: {Backend})",
                watch.ElapsedMilliseconds, _lastResult.Length, deep.LastVerificationBackend);
            return _lastResult;
        }

        public void DeliverResults()
        {
            Task.Run(() =>
            {
                var sw = System.Diagnostics.Stopwatch.StartNew();
                DeepPair[] pairs = _lastResult;
                var result = new DuplPairViewModel[pairs.Length];
                for (int i = 0; i < pairs.Length; i++)
                {
                    DeepPair p = pairs[i];
                    var vm = new DuplPairViewModel();
                    // Ядро уже проверило пути/форматы при скане; deep-пайплайн возвращает
                    // пути + статистику файлов (размер из скана ядра, нативные размеры и
                    // blurring/blockiness — побочный продукт декодирования ядра).
                    vm.FirstFile.Path = p.Path1;
                    vm.SecondFile.Path = p.Path2;
                    if (p.Size1 > 0) vm.FirstFile.FileSize = (ulong)p.Size1;
                    if (p.Size2 > 0) vm.SecondFile.FileSize = (ulong)p.Size2;
                    // Время изменения файлов (сырой FILETIME из скана ядра) — в колонку «Time».
                    if (p.Time1 > 0) vm.FirstFile.Time = (ulong)p.Time1;
                    if (p.Time2 > 0) vm.SecondFile.Time = (ulong)p.Time2;
                    if (p.Width1 > 0) { vm.FirstFile.Width = (uint)p.Width1; vm.FirstFile.Height = (uint)p.Height1; }
                    if (p.Width2 > 0) { vm.SecondFile.Width = (uint)p.Width2; vm.SecondFile.Height = (uint)p.Height2; }
                    // Метрики качества (blurring/blockiness) — только если ядро их посчитало
                    // (WGC-декод с метриками); иначе остаются 0, как «недоступно».
                    vm.FirstFile.Bluring = p.Bluring1;
                    vm.FirstFile.Blockiness = p.Blockiness1;
                    vm.SecondFile.Bluring = p.Bluring2;
                    vm.SecondFile.Blockiness = p.Blockiness2;
                    // Difference = (1 − косинусное сходство) × 100.
                    vm.Difference = Math.Round((1.0 - p.Similarity) * 100.0);
                    vm.Type = CoreDll.ResultType.DuplImagePair;
                    // Номер группы (клика) из поиска: в groups-режиме группы собираются строго
                    // по нему, без транзитивного слияния (семантика референса).
                    vm.DeepGroupId = p.GroupId;
                    // Метрики LightGlue/WGC в колонку результатов.
                    // Верификатор: 4-й элемент кортежа = число совпадений, 5-й = avgScore×100.
                    vm.DeepWgcVerified = p.GeometricVerified;
                    vm.DeepWgcMatches = p.GeometricAngleVotes;
                    vm.DeepWgcScorePercent = p.GeometricScaleVotes;
                    result[i] = vm;
                }
                _logger.LogInformation("Deep-поиск: получено результатов: {Count} пар за {Ms} мс",
                    result.Length, sw.ElapsedMilliseconds);
                _vm.SetResult(result);
            });
        }

        /// <summary>
        /// Фатальная ошибка deep-поиска — MessageBox в UI с полным текстом.
        /// Вызывается из фонового потока: рендерится через Dispatcher UI-потока.
        /// </summary>
        private void ShowDeepFatalError(string message)
        {
            try
            {
                var dispatcher = System.Windows.Application.Current?.Dispatcher;
                if (dispatcher == null) return; // UI недоступен (закрытие) — ошибка уже в логe
                dispatcher.BeginInvoke(new Action(() =>
                    System.Windows.MessageBox.Show(message, "Deep-поиск",
                        System.Windows.MessageBoxButton.OK, System.Windows.MessageBoxImage.Error)));
            }
            catch
            {
                // Ошибка показа диалога не должна ронять поиск — текст уже в логe.
            }
        }
    }
}