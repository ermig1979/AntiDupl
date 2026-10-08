using AntiDupl.NET.Core;
using AntiDupl.NET.Deep.Cache;
using AntiDupl.NET.Deep.Core;
using AntiDupl.NET.Deep.Cv;
using AntiDupl.NET.Deep.Image;
using AntiDupl.NET.Deep.Infra;
using AntiDupl.NET.Deep.Logging;
using AntiDupl.NET.Deep.Ml;
using AntiDupl.NET.Deep.Models;
using AntiDupl.NET.Onnx.Cv;
using AntiDupl.NET.Onnx.Ml;
using Xunit;

namespace AntiDupl.NET.Deep.Tests;

[CollectionDefinition(nameof(DeepCollection))]
public sealed class DeepCollection : ICollectionFixture<DeepTestFixture>
{
    public DeepCollection()
    {
    }
}

/// <summary>
/// Общий фикстура-хост: одна AntiDupl.dll, одна папка с тестовыми изображениями,
/// один FeatureCache, один DeepSearchService (эксстрактор/верификатор кэшируются
/// внутри сервиса — повторные запуски ходят в кэш).
///
/// Пути переопределяются переменными окружения:
///   ANTIDUPL_DLL_DIR   — каталог с AntiDupl.dll (по умолчанию bin\Debug репо)
///   DEEP_TEST_FOLDER   — папка с тестовыми картинками (по умолчанию TestCropHard)
///   DEEP_DINOV2_MODEL  — путь к dinov2-base.onnx
///   DEEP_PIPELINE_MODEL— путь к superpoint_lightglue_pipeline.onnx
/// </summary>
public sealed class DeepTestFixture : IDisposable
{
    private static bool _dllPathConfigured;
    private static readonly object Gate = new();

    private readonly Lazy<CoreLib> _core;
    private readonly Lazy<IDeepImageSource> _imageSource;
    private readonly Lazy<DeepSearchService> _service;
    private readonly Dictionary<float, DeepPair[]> _memoizedRuns = new();

    public DeepTestFixture()
    {
        // Детерминизация тестов: все ORT-сессии (DINOv2 + SuperPoint/LightGlue) — на CPU.
        // GPU-инференс ORT недетерминирован (CUDA ScatterND-атомарные, порядок редукций):
        // WGC-голоса «плавают» от рана к рану и recall-тесты превращаются во флейки.
        // CPU медленнее, но стабилен (см. OnnxSessionFactory: DEEP_FORCE_CPU).
        Environment.SetEnvironmentVariable("DEEP_FORCE_CPU", "1");
        // Каталоги тестовых артефактов — рядом с программой (не в %TEMP%):
        // бинарный кэш — в "deeptest\deep", логи ядра/Deep — в "deeptest\logs".
        UserPath = Path.Combine(AppContext.BaseDirectory, "deeptest");
        Directory.CreateDirectory(UserPath);
        // Логгер deep-пайплайна (замена статического DeepLog): файл в deeptest\logs.
        Logger = new FileDeepLogger(Path.Combine(UserPath, "logs"));
        Logger.Clear();
        Cache = new FeatureCache(Path.Combine(UserPath, "deep"));
        Cache.ClearAll(); // каждый прогон тестов — с холодным кэшем (deterministic)

        _core = new Lazy<CoreLib>(() =>
        {
            EnsureDllPath();
            var core = new CoreLib(UserPath);

            var so = new CoreSearchOptions();
            so.subFolders = false;
            so.system = false;
            so.hidden = false;
            so.JPEG = true;
            so.PNG = true;
            so.BMP = true;
            so.GIF = true;
            so.TIFF = true;
            so.WEBP = true;
            core.searchOptions = so;

            var sp = new CorePathWithSubFolder();
            sp.path = TestFolder;
            sp.enableSubFolder = false;
            core.searchPath = new[] { sp };
            return core;
        });

        // Дефолтный пайплайн — «core» (ядро: 224×224 + латербокс-канвас 800×600 через
        // adLoadBitmapW/adLoadBitmapLetterboxW): A/B-замер на TestCropHard (core 2.4.0) показал
        // эквивалентность «core» и «ref» (WIC + OpenCV-совместимый ресайз, фиксированный канвас 800×600) —
        // 13/13 GT-пар и 1 мягкое кросс-сценовное FP на всех порогах. Letterbox (сохранение пропорций)
        // улучшил WGC-геометрию «core»: пары, пропущенные при stretch, теперь находятся.
        _imageSource = new Lazy<IDeepImageSource>(() => new CoreBitmapImageSource(_core.Value));

        _service = new Lazy<DeepSearchService>(() => new DeepSearchService(
            new CoreLibCandidateSource(_core.Value),
            Cache,
            p => new EmbeddingExtractor(p, Cache, _imageSource.Value, Logger),
            new GeometricVerifierFactory(p => new LightGlueGeometricVerifier(new SuperPointLightGluePipeline(p, Logger))),
            _imageSource.Value,
            Path.Combine(UserPath, "models"),
            Logger));
    }

    public string CoreDllDirectory { get; } = EnvOr("ANTIDUPL_DLL_DIR", @"F:\E\SourceAntiDupl\AntiDupl\bin\Debug");
    public string TestFolder { get; } = EnvOr("DEEP_TEST_FOLDER", @"F:\E\SourceAntiDupl\TestCropHard");
    public string Dinov2ModelPath { get; } = EnvOr("DEEP_DINOV2_MODEL",
        @"F:\E\SourceDuplicateImages\DinoDuplicateSearchWPF\src\DinoDuplicateSearch.WPF\Models\dinov2-base.onnx");
    public string PipelineModelPath { get; } = EnvOr("DEEP_PIPELINE_MODEL",
        @"F:\E\SourceDuplicateImages\DinoDuplicateSearchWPF\Models\superpoint_lightglue_pipeline.onnx");
    public string UserPath { get; }
    public IFeatureCache Cache { get; }
    public IDeepLogger Logger { get; private set; }

    public CoreLib Core => _core.Value;
    public IDeepImageSource ImageSource => _imageSource.Value;
    public DeepSearchService Service => _service.Value;

    /// <summary>Ожидаемые пары (TestCropHard): 11 пар + троица Avengers = 13 пар.</summary>
    public static readonly (string A, string B)[] ExpectedPairs =
    {
        ("9955dd6dffcd25bdc98b16b7d7041a75.jpg", "9955dd6dffcd25bdc98b16b7d7041a75_2.jpg"),
        ("20120613-tom-cruise-syuting-film-science-fiction-01.jpg", "20120613-tom-cruise-syuting-film-science-fiction-02.jpg"),
        ("1667438424_15-sportishka-com-p-boi-gladiatorov-v-drevnem-rime-oboi-16.jpg",
            "1667438424_15-sportishka-com-p-boi-gladiatorov-v-drevnem-rime-oboi-16_.jpg"),
        ("emma stone.jpg", "emma stone katharine mcphee kat dennings anna faris rumer willis the house bunny.jpg"),
        ("gladiatrisy-1.jpg", "gladiatrisy-1_.jpg"),
        ("Sicario-2658717.jpg", "Sicario-2658717_2.jpg"),
        ("Sicario-2667418.jpg", "Sicario-2667418_2.jpg"),
        ("Sicario-2667419.jpg", "Sicario-2667419_2.jpg"),
        ("Sicario-2676063.jpg", "Sicario-2676063_2.jpg"),
        ("The-Avengers-On-Set-148 (1).jpg", "The-Avengers-On-Set-148_2.jpg"),
        ("The-Avengers-On-Set-148 (1).jpg", "The-Avengers-On-Set-148_3.jpg"),
        ("The-Avengers-On-Set-148_2.jpg", "The-Avengers-On-Set-148_3.jpg"),
        ("Карен Фукухара_2.jpg", "Карен Фукухара_3.jpg"),
    };

    public DeepSettings MakeSettings(float threshold) => new()
    {
        DistanceThreshold = threshold,
        GeometricCheckEnabled = true,
        MinSimilarityForPair = 0.5f,
        MinMatchScore = 0.5f,
        Dinov2ModelPath = Dinov2ModelPath,
        LightGlueModelPath = PipelineModelPath,
    };

    /// <summary>Запуск поиска (memoized по порогу — один прогон на порог).</summary>
    public DeepPair[] Run(float threshold)
    {
        if (_memoizedRuns.TryGetValue(threshold, out var cached))
            return cached;
        var result = Service.Search(MakeSettings(threshold), null, CancellationToken.None);
        lock (_memoizedRuns) _memoizedRuns[threshold] = result;
        return result;
    }

    /// <summary>Запуск поиска без мемоизации (для замеров кэша).</summary>
    public DeepPair[] SearchOnce(float threshold)
        => Service.Search(MakeSettings(threshold), null, CancellationToken.None);

    /// <summary>Ищет в результатах пару (имена файлов, порядок не важен, без учёта регистра).</summary>
    public static bool ContainsPair(DeepPair[] pairs, string fileA, string fileB)
        => pairs.Any(p =>
            (Eq(p.Path1, fileA) && Eq(p.Path2, fileB)) ||
            (Eq(p.Path1, fileB) && Eq(p.Path2, fileA)));

    private static bool Eq(string path, string fileName)
        => string.Equals(Path.GetFileName(path), fileName, StringComparison.OrdinalIgnoreCase);

    public static HashSet<string> PairNames(DeepPair[] pairs)
        => pairs.Select(p => NamesKey(p.Path1, p.Path2)).ToHashSet();

    public static string NamesKey(string p1, string p2)
    {
        var n1 = Path.GetFileName(p1).ToLowerInvariant();
        var n2 = Path.GetFileName(p2).ToLowerInvariant();
        return string.CompareOrdinal(n1, n2) < 0 ? $"{n1}|{n2}" : $"{n2}|{n1}";
    }

    private static string EnvOr(string name, string fallback)
    {
        var v = Environment.GetEnvironmentVariable(name);
        return string.IsNullOrWhiteSpace(v) ? fallback : v!;
    }

    private static void EnsureDllPath()
    {
        lock (Gate)
        {
            if (_dllPathConfigured) return;
            SetDllDirectory(CoreDllDirectorySafe());
            var path = Environment.GetEnvironmentVariable("PATH") ?? "";
            Environment.SetEnvironmentVariable("PATH", CoreDllDirectorySafe() + ";" + path);
            _dllPathConfigured = true;
        }
    }

    private static string CoreDllDirectorySafe()
    {
        var v = Environment.GetEnvironmentVariable("ANTIDUPL_DLL_DIR");
        return string.IsNullOrWhiteSpace(v) ? @"F:\E\SourceAntiDupl\AntiDupl\bin\Debug" : v!;
    }

    public void Dispose()
    {
        if (_core.IsValueCreated)
        {
            try { _core.Value.Release(); } catch { }
        }
        try { Cache.Dispose(); } catch { }
    }

    [System.Runtime.InteropServices.DllImport("kernel32", CharSet = System.Runtime.InteropServices.CharSet.Unicode)]
    private static extern bool SetDllDirectory(string lpPathName);
}