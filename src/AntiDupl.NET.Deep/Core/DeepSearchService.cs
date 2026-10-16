using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using AntiDupl.NET.Deep.Cache;
using AntiDupl.NET.Deep.Cv;
using AntiDupl.NET.Deep.Image;
using AntiDupl.NET.Deep.Logging;
using AntiDupl.NET.Deep.Models;
using AntiDupl.NET.Deep.Ml;

namespace AntiDupl.NET.Deep.Core;

/// <summary>
/// Оркестратор deep-поиска (перенос DuplicatesFinder из DinoDuplicateSearchWPF).
///
/// Отличия от референса (гибридная архитектура):
///  - кандидаты: не собственный ListImages, а <c>CoreLib.ScanCandidates()</c>
///    (adScanCandidatesW): ядро AntiDupl.dll прогоняет свои фильтры (пути/подпапки,
///    скрытые/системные, ignore/valid, фильтр форматов, размер) и возвращает path/size/time;
///  - декодирование: IDeepImageSource (adLoadBitmapW) вместо OpenCV;
///  - кэш: управляемый бинарный FeatureCache вместо SQLite;
///  - нет SIFT-верификатора и WGC-порога: только LightGlue или «без верификации»;
///  - модели создаются фабриками (путь разрешает хост): dinov2-base.onnx — только путём,
///    superpoint_lightglue_pipeline.onnx — путь или скачивание.
///
/// Поток: Scan → эмбеддинги (кэш) → кластеризация (≤10k agglomerative,
/// 10k–50k union-find, &gt;50k PQ) → распускание кластеров &gt; MaxClusterSize →
/// пары (опционально верификация LightGlue с кэшем вердиктов) → группы
/// (верифицированные: максимальные клики Bron–Kerbosch с pivot; остальные: все пары кластера)
/// → плоский список пар.
/// </summary>
public class DeepSearchService : IDisposable
{
    private const int PQThreshold = 50_000;

    private readonly IDeepCandidateSource _candidateSource;
    private readonly IFeatureCache _cache;
    private readonly Func<string, IEmbeddingExtractor> _extractorFactory;
    private readonly GeometricVerifierFactory _verifierFactory;
    private readonly IDeepImageSource _imageSource;
    private readonly string _modelsDirectory;
    private readonly IDeepLogger _logger;

    private IEmbeddingExtractor? _extractor;
    private string _extractorModelPath = "";
    private IGeometricVerifier? _currentVerifier;
    private string _verifierModelPath = "";
    // Порог средней уверенности совпадений (DeepSettings.MinMatchScore) — параметр запуска:
    // верификатор — синглтон без состояния; порог передаётся в каждый вызов Verify.
    private float _minMatchScore = 0.5f;

    public string LastVerificationBackend { get; private set; } = "Off";

    public DeepSearchService(
        IDeepCandidateSource candidateSource,
        IFeatureCache cache,
        Func<string, IEmbeddingExtractor> extractorFactory,
        GeometricVerifierFactory verifierFactory,
        IDeepImageSource imageSource,
        string modelsDirectory,
        IDeepLogger logger)
    {
        _candidateSource = candidateSource ?? throw new ArgumentNullException(nameof(candidateSource));
        _cache = cache ?? throw new ArgumentNullException(nameof(cache));
        _extractorFactory = extractorFactory ?? throw new ArgumentNullException(nameof(extractorFactory));
        _verifierFactory = verifierFactory ?? throw new ArgumentNullException(nameof(verifierFactory));
        _imageSource = imageSource ?? throw new ArgumentNullException(nameof(imageSource));
        _modelsDirectory = modelsDirectory ?? throw new ArgumentNullException(nameof(modelsDirectory));
        _logger = logger ?? throw new ArgumentNullException(nameof(logger));
    }

    /// <summary>
    /// Полный deep-поиск. Блокирующий вызов — выполнять из фонового потока.
    /// </summary>
    public DeepPair[] Search(DeepSettings settings, IProgress<DeepProgress>? progress = null, CancellationToken ct = default)
    {
        Debug.Assert(settings is not null, nameof(settings));
        _currentVerifier = settings.GeometricCheckEnabled ? CreateVerifier(settings) : null;
        LastVerificationBackend = _currentVerifier?.Name ?? "Off";
        _minMatchScore = settings.MinMatchScore;
        ct.ThrowIfCancellationRequested();
        progress?.Report(new DeepProgress(0, "Scanning candidates (core filters)..."));
        // Кандидаты — через порт: драйвер (ядро AntiDupl.dll) прогоняет фильтры хоста.
        // Use case не знает о конкретном драйвере.
        var candidates = _candidateSource.Scan();
        if (candidates.Count == 0)
        {
            progress?.Report(new DeepProgress(100, "No images found"));
            return Array.Empty<DeepPair>();
        }

        // Размер каждого кандидата ядро уже знает после скана (adScanCandidatesW)
        // — берём для заполнения результатов, без повторного обращения к диску.
        var candidateSizes = new Dictionary<string, long>(candidates.Count, StringComparer.OrdinalIgnoreCase);
        var candidateTimes = new Dictionary<string, long>(candidates.Count, StringComparer.OrdinalIgnoreCase);
        foreach (var c in candidates)
        {
            candidateSizes[c.Path] = c.Size;
            candidateTimes[c.Path] = c.Time;
        }

        // Статистика декодирования (нативные размеры, метрики) — на каждый поиск свежая;
        // если декоды попадают в feature-кэш (ядро не вызывается), восстанавливаем
        // её из кэша (deep_filestats.dat), записанного предыдущими прогонками.
        var fileStats = _imageSource as IDeepImageFileStats;
        if (fileStats != null)
        {
            fileStats.ClearFileStats();
            if (_cache.TryLoadFileStats(out var persistedStats))
                fileStats.SeedFileStats(persistedStats);
        }

        var paths = candidates.Select(c => c.Path).ToList();
        progress?.Report(new DeepProgress(2, $"Found {paths.Count} images. Loading model..."));

        var extractor = GetExtractor(settings);
        extractor.SetProgress(progress);
        extractor.BatchSize = settings.BatchSize;
        extractor.PrefetchCount = settings.PrefetchCount;

        progress?.Report(new DeepProgress(5, "Computing embeddings..."));
        var embs = extractor.EmbedImagesBatch(paths, ct);
        Debug.Assert(embs.Length == paths.Count, "extractor must return one embedding per file");

        ct.ThrowIfCancellationRequested();
        progress?.Report(new DeepProgress(40, "Clustering images..."));
        int[] labels;
        if (embs.Length > PQThreshold)
        {
            progress?.Report(new DeepProgress(40, $"Large dataset ({embs.Length} images), using Product Quantization..."));
            labels = PqClusterer.Cluster(embs, settings.DistanceThreshold, settings.TransitivityRatio, _modelsDirectory, progress, ct);
        }
        else
        {
            labels = AgglomerativeClustering.FitPredict(embs, settings.DistanceThreshold, progress, settings.TransitivityRatio);
        }

        // Одна метка на файл — иначе clusters соберутся по чужим индексам.
        Debug.Assert(labels.Length == paths.Count,
            $"clustering must return one label per image ({labels.Length} != {paths.Count})");

        // Кластеры больше MaxClusterSize распускаются на одиночные метки (как в референсе).
        int maxClusterSize = settings.MaxClusterSize;
        var labelCounts = new Dictionary<int, int>();
        for (int i = 0; i < labels.Length; i++)
        {
            if (!labelCounts.ContainsKey(labels[i]))
                labelCounts[labels[i]] = 0;
            labelCounts[labels[i]]++;
        }
        int nextLabel = labels.Length > 0 ? labels.Max() + 1 : 0;
        for (int i = 0; i < labels.Length; i++)
        {
            if (labelCounts[labels[i]] > maxClusterSize)
                labels[i] = nextLabel++;
        }

        var clusters = new Dictionary<int, List<string>>();
        for (int i = 0; i < paths.Count; i++)
        {
            if (!clusters.ContainsKey(labels[i]))
                clusters[labels[i]] = new List<string>();
            clusters[labels[i]].Add(paths[i]);
        }

        ct.ThrowIfCancellationRequested();
        progress?.Report(new DeepProgress(45, "Verifying geometric consistency..."));
        int totalPairs = clusters.Values.Sum(items => items.Count * (items.Count - 1) / 2);
        var bigClusters = clusters.Values.Where(items => items.Count > 100).OrderByDescending(items => items.Count).Take(5);
        foreach (var items in bigClusters)
            _logger.Write($"[WGC] Big cluster: {items.Count} images, {items.Count * (items.Count - 1) / 2} pairs");
        _logger.Write($"[WGC] Total clusters: {clusters.Count}, total pairs: {totalPairs}");
        _logger.Write($"[CLUSTER] Images: {paths.Count}, Threshold: {settings.DistanceThreshold}, MinSimilarity: {settings.MinSimilarityForPair}");

        // Log cluster size distribution
        var sizeHist = clusters.Values.GroupBy(c => c.Count).OrderBy(g => g.Key);
        foreach (var g in sizeHist)
            _logger.Write($"[CLUSTER] {g.Count()} clusters with {g.Key} images");

        // Log top similarity pairs for debugging
        var topPairs = new List<(int i, int j, float sim)>();
        for (int i = 0; i < Math.Min(paths.Count, 100); i++)
            for (int j = i + 1; j < Math.Min(paths.Count, 100); j++)
            {
                float sim = DotProduct(embs[i], embs[j]);
                if (sim > 0.3f) topPairs.Add((i, j, sim));
            }
        topPairs.Sort((a, b) => b.sim.CompareTo(a.sim));
        foreach (var (i, j, sim) in topPairs.Take(10))
            _logger.Write($"[CLUSTER] Top pair: {Path.GetFileName(paths[i])} <-> {Path.GetFileName(paths[j])} sim={sim:F4}");

        var clusterPairs = new ConcurrentDictionary<int, List<DeepPair>>();
        var clusterWgcGraph = new ConcurrentDictionary<int, Dictionary<string, HashSet<string>>>();
        int pairCount = 0;

        var pathToIndex = new Dictionary<string, int>(paths.Count);
        for (int i = 0; i < paths.Count; i++)
            pathToIndex[paths[i]] = i;

        var clusterData = new List<(int clusterId, List<string> items, float[][] clusterEmbs)>();
        foreach (var clusterKvp in clusters)
        {
            if (clusterKvp.Value.Count <= 1) continue;
            var clusterEmbs = new float[clusterKvp.Value.Count][];
            for (int i = 0; i < clusterKvp.Value.Count; i++)
                clusterEmbs[i] = embs[pathToIndex[clusterKvp.Value[i]]];
            clusterData.Add((clusterKvp.Key, clusterKvp.Value, clusterEmbs));
        }

        Parallel.ForEach(clusterData, new ParallelOptions { CancellationToken = ct, MaxDegreeOfParallelism = settings.WgcParallelism }, data =>
        {
            var (clusterId, items, clusterEmbs) = data;
            var pairs = new List<DeepPair>();
            var wgcGraph = new Dictionary<string, HashSet<string>>();
            foreach (var item in items) wgcGraph[item] = new HashSet<string>();

            for (int i = 0; i < items.Count; i++)
            {
                if (ct.IsCancellationRequested)
                    break;
                for (int j = i + 1; j < items.Count; j++)
                {
                    var sim = DotProduct(clusterEmbs[i], clusterEmbs[j]);

                    if (settings.GeometricCheckEnabled)
                    {
                        var (ok, angle, scale, angleVotes, scaleVotes) = VerifyGeometric(items[i], items[j]);

                        int done = Interlocked.Increment(ref pairCount);
                        var b1 = Path.GetFileName(items[i]);
                        var b2 = Path.GetFileName(items[j]);
                        var status = ok ? "PASS" : "FAIL";
                        _logger.Write($"[PAIR] {b1} <-> {b2} sim={sim:F4} geo={status} votes={angleVotes}/{scaleVotes} backend={LastVerificationBackend}");

                        if (done % Math.Max(1, totalPairs / 100) == 0)
                        {
                            progress?.Report(new DeepProgress(45.0 + 35.0 * done / totalPairs,
                                $"{_currentVerifier?.Name}: {done}/{totalPairs} {b1} vs {b2}", $"[{status}] sim={sim:F3}"));
                        }

                        if (ok && sim >= settings.MinSimilarityForPair)
                        {
                            var pair = new DeepPair
                            {
                                Path1 = items[i],
                                Path2 = items[j],
                                Similarity = sim,
                                GeometricVerified = true,
                                GeometricAngle = angle,
                                GeometricAngleVotes = angleVotes,
                                GeometricScale = scale,
                                GeometricScaleVotes = scaleVotes
                            };
                            pairs.Add(pair);
                            lock (wgcGraph[items[i]]) wgcGraph[items[i]].Add(items[j]);
                            lock (wgcGraph[items[j]]) wgcGraph[items[j]].Add(items[i]);
                        }
                    }
                    else
                    {
                        var pair = new DeepPair
                        {
                            Path1 = items[i],
                            Path2 = items[j],
                            Similarity = sim
                        };
                        pairs.Add(pair);
                        lock (wgcGraph[items[i]]) wgcGraph[items[i]].Add(items[j]);
                        lock (wgcGraph[items[j]]) wgcGraph[items[j]].Add(items[i]);
                    }
                }
            }
            clusterPairs[clusterId] = pairs;
            clusterWgcGraph[clusterId] = wgcGraph;
        });

        ct.ThrowIfCancellationRequested();
        progress?.Report(new DeepProgress(80, "Building duplicate groups..."));
        var allPairs = new List<DeepPair>();

        if (settings.GeometricCheckEnabled)
        {
            // Верифицированные группы: максимальные клики в графе WGC кластера.
            // Семантика референса: каждый клик — отдельная группа с последовательным номером;
            // файл может входить в несколько групп; пара, содержащаяся в двух кликах,
            // идёт в обе — отдельными записями с разными GroupId.
            int groupId = 0;
            var stamped = new HashSet<(string, string)>();
            foreach (var clusterKvp in clusters)
            {
                var clusterId = clusterKvp.Key;
                if (!clusterWgcGraph.ContainsKey(clusterId)) continue;

                var cliques = CliqueExtractor.FindCliques(clusterWgcGraph[clusterId]);
                foreach (var clique in cliques)
                {
                    if (clique.Count < 2) continue;

                    var cliqueSet = new HashSet<string>(clique, StringComparer.OrdinalIgnoreCase);
                    var seen = new HashSet<(string, string)>();
                    var groupEntries = new List<DeepPair>();
                    foreach (var pair in clusterPairs[clusterId])
                    {
                        if (!pair.GeometricVerified) continue;
                        if (!cliqueSet.Contains(pair.Path1) || !cliqueSet.Contains(pair.Path2)) continue;

                        var key = OrderKey(pair);
                        if (!seen.Add(key)) continue;

                        // Если пара уже добавлена в другую группу — копия (первоначальная запись сохраняет свой GroupId).
                        var entry = stamped.Contains(key) ? ClonePair(pair) : pair;
                        entry.GroupId = groupId;
                        groupEntries.Add(entry);
                    }

                    if (groupEntries.Count > 0)
                    {
                        foreach (var entry in groupEntries) stamped.Add(OrderKey(entry));
                        allPairs.AddRange(groupEntries);
                        _logger.Write($"[GROUP {groupId}] from cluster={clusterId} ({clique.Count} images, {groupEntries.Count} pairs): {string.Join(", ", clique.Select(Path.GetFileName))}");
                        groupId++;
                    }
                }
            }
        }
        else
        {
            // Без верификации: каждый кластер целиком — одна «группа» (номер = кластер), все его пары.
            foreach (var clusterKvp in clusters)
            {
                var clusterId = clusterKvp.Key;
                if (!clusterPairs.ContainsKey(clusterId)) continue;
                foreach (var pair in clusterPairs[clusterId])
                {
                    pair.GroupId = clusterId;
                    allPairs.Add(pair);
                }
            }
        }

        try { _cache.Save(); } catch { }

        FillFileStats(allPairs, candidateSizes, candidateTimes, fileStats);

        // Персист пер-файловой статистики: при повторном поиске (декоды из кэша)
        // результаты всё равно будут нести разрешение/метрики.
        if (fileStats != null)
        {
            try { _cache.SaveFileStats(fileStats.CollectFileStats()); } catch { }
        }

        progress?.Report(new DeepProgress(100, $"Found {allPairs.Count} duplicate pairs"));
        return allPairs.ToArray();
    }

    /// <summary>
    /// Заполняет у пар статистикой файлов: размер (из скана ядра), нативные размеры и
    /// метрики blurring/blockiness — всё это побочный продукт уже сделанных ядром
    /// декодирований (adScanCandidatesW / adLoadBitmapExW / adLoadBitmapLetterboxMetricsW),
    /// без какого-либо дополнительного чтения файлов.
    /// </summary>
    private void FillFileStats(List<DeepPair> pairs, IReadOnlyDictionary<string, long> candidateSizes,
        IReadOnlyDictionary<string, long> candidateTimes, IDeepImageFileStats? stats)
    {
        foreach (var p in pairs)
        {
            // ВАЖНО: локальные переменные — в C# свойство (даже auto) нельзя
            // передавать как out/ref-аргумент (CS0206), только переменные/поля.
            if (candidateSizes.TryGetValue(p.Path1, out long size1)) { p.Size1 = size1; }
            if (candidateSizes.TryGetValue(p.Path2, out long size2)) { p.Size2 = size2; }
            if (candidateTimes.TryGetValue(p.Path1, out long time1)) { p.Time1 = time1; }
            if (candidateTimes.TryGetValue(p.Path2, out long time2)) { p.Time2 = time2; }
            if (stats != null)
            {
                if (stats.TryGetNativeSize(p.Path1, out int w1, out int h1)) { p.Width1 = w1; p.Height1 = h1; }
                if (stats.TryGetNativeSize(p.Path2, out int w2, out int h2)) { p.Width2 = w2; p.Height2 = h2; }
                if (stats.TryGetFileMetrics(p.Path1, out double b1, out double k1)) { p.Bluring1 = b1; p.Blockiness1 = k1; }
                if (stats.TryGetFileMetrics(p.Path2, out double b2, out double k2)) { p.Bluring2 = b2; p.Blockiness2 = k2; }
            }
        }
    }

    /// <summary>Очистка кэша эмбеддингов/вердиктов. Возвращает освобождённый размер (байт).</summary>
    public long ClearCache() => _cache.ClearAll();

    private IGeometricVerifier CreateVerifier(DeepSettings settings)
    {
        // Путь к модели SuperPoint+LightGlue задаёт хост (WPF: Configuration + DeepModelManager,
        // скачивание при отсутствии — DD7). Здесь только проверяем готовность.
        string modelPath = settings.LightGlueModelPath ?? "";
        if (string.IsNullOrWhiteSpace(modelPath) || !File.Exists(modelPath))
            throw new InvalidOperationException(
                $"LightGlue verification is enabled, but the model is missing: {modelPath}. " +
                "Download the model in Configuration or turn verification off.");

        if (_currentVerifier != null && _verifierModelPath == modelPath)
            return _currentVerifier;

        DisposeVerifier();
        _currentVerifier = _verifierFactory.Create(modelPath);
        if (_currentVerifier == null)
            throw new InvalidOperationException("Failed to create the LightGlue verifier (model file: " + modelPath + ").");
        _verifierModelPath = modelPath;
        return _currentVerifier;
    }

    private IEmbeddingExtractor GetExtractor(DeepSettings settings)
    {
        string modelPath = settings.Dinov2ModelPath ?? "";
        if (string.IsNullOrWhiteSpace(modelPath) || !File.Exists(modelPath))
            throw new InvalidOperationException(
                $"DINOv2 model not found: {modelPath}. Place dinov2-base.onnx into the models folder or set the path in Configuration.");

        if (_extractor != null && _extractorModelPath == modelPath)
            return _extractor;

        _extractor?.Dispose();
        _extractor = _extractorFactory(modelPath);
        _extractorModelPath = modelPath;
        return _extractor;
    }









    private (bool ok, float angle, float scale, int angleVotes, int scaleVotes) VerifyGeometric(string path1, string path2)
    {
        var mtime1 = GetMtime(path1);
        var mtime2 = GetMtime(path2);

        var cached = _cache.GetWgc(path1, path2, mtime1, mtime2, _minMatchScore);
        if (cached.HasValue)
        {
            _logger.Write($"[WGC] Cache hit: {path1} {path2} {mtime1} {mtime2}");
            return (cached.Value.result, cached.Value.angle, cached.Value.scale, cached.Value.angleVotes, cached.Value.scaleVotes);
        }

        // Декодирование grayscale 800×600 через ядро (adLoadBitmapW) вместо OpenCV.
        float[,]? gray1 = _imageSource.DecodeLightGlueInput(path1);
        float[,]? gray2 = _imageSource.DecodeLightGlueInput(path2);

        if (gray1 == null || gray2 == null)
            return (false, 0, 0, 0, 0);

        var (ok, angle, scale, angleVotes, scaleVotes) = _currentVerifier!.Verify(gray1, gray2, _minMatchScore);
        _logger.Write($"[PAIR] {_currentVerifier.Name}: votes={angleVotes}/{scaleVotes} backend={LastVerificationBackend}");

        try { _cache.SetWgc(path1, path2, mtime1, mtime2, _minMatchScore, ok, angle, scale, angleVotes, scaleVotes); }
        catch (Exception ex) { _logger.Write($"[WGC] Cache write error: {ex.Message}"); }

        return (ok, angle, scale, angleVotes, scaleVotes);
    }

    private static (string, string) OrderKey(DeepPair p) =>
        string.Compare(p.Path1, p.Path2) < 0 ? (p.Path1, p.Path2) : (p.Path2, p.Path1);

    private static DeepPair ClonePair(DeepPair p) => new DeepPair
    {
        Path1 = p.Path1,
        Path2 = p.Path2,
        Similarity = p.Similarity,
        GeometricVerified = p.GeometricVerified,
        GeometricAngle = p.GeometricAngle,
        GeometricAngleVotes = p.GeometricAngleVotes,
        GeometricScale = p.GeometricScale,
        GeometricScaleVotes = p.GeometricScaleVotes,
        Size1 = p.Size1,
        Size2 = p.Size2,
        Time1 = p.Time1,
        Time2 = p.Time2,
        Width1 = p.Width1,
        Height1 = p.Height1,
        Width2 = p.Width2,
        Height2 = p.Height2,
        Bluring1 = p.Bluring1,
        Blockiness1 = p.Blockiness1,
        Bluring2 = p.Bluring2,
        Blockiness2 = p.Blockiness2
    };

    private static double GetMtime(string path)
    {
        try { return File.GetLastWriteTimeUtc(path).Ticks / (double)TimeSpan.TicksPerSecond; }
        catch { return 0.0; }
    }

    private static float DotProduct(float[] a, float[] b)
    {
        float sum = 0;
        for (int i = 0; i < a.Length; i++) sum += a[i] * b[i];
        return sum;
    }



    public void Dispose()
    {
        _extractor?.Dispose();
        _extractor = null;
        DisposeVerifier();
        _cache?.Dispose();
    }

    private void DisposeVerifier()
    {
        if (_currentVerifier is IDisposable disposable)
            disposable.Dispose();
        _currentVerifier = null;
        _verifierModelPath = "";
    }
}