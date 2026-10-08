using AntiDupl.NET.Deep.Cache;
using AntiDupl.NET.Deep.Core;
using AntiDupl.NET.Deep.Cv;
using AntiDupl.NET.Deep.Image;
using AntiDupl.NET.Deep.Infra;
using AntiDupl.NET.Deep.Models;
using AntiDupl.NET.Onnx.Cv;
using AntiDupl.NET.Onnx.Ml;
using Xunit;
using Xunit.Abstractions;

namespace AntiDupl.NET.Deep.Tests;

/// <summary>
/// A/B-замер препроцессинга: «core» (ядро AntiDupl.dll: adLoadBitmapW/adLoadBitmapLetterboxW
/// + Simd::Resize) против «ref» (WIC + OpenCV-совместимый INTER_LINEAR + нативный gray).
/// Оба варианта — латербокс в канвас 800×600. Модели, WGC-логика и параметры поиска одинаковы;
/// отличие — ТОЛЬКО входные пиксели (декодер и ресайзер).
///
/// Таблица: для каждой GT-пары сходство DINOv2 на порогах 0.45/0.50/0.60 в обоих вариантах;
/// плюс список «лишних» (не-GT) пар — индикатор ложных срабатываний.
/// Ассертов нет — данные для выбора дефолтного варианта.
/// </summary>
[Collection(nameof(DeepCollection))]
public sealed class DeepAbPreprocessingTests
{
    private readonly DeepTestFixture _fixture;
    private readonly ITestOutputHelper _output;
    private readonly DeepSearchService _coreService;
    private readonly DeepSearchService _referenceService;
    private readonly FeatureCache _coreCache;
    private readonly FeatureCache _referenceCache;

    public DeepAbPreprocessingTests(DeepTestFixture fixture, ITestOutputHelper output)
    {
        _fixture = fixture;
        _output = output;

        // Отдельный кэш на каждый вариант: эмбеддинги различаются и не должны мешать друг другу.
        // «core»-сторона собирается явно (фикстура теперь несёт дефолтный «ref»-пайплайн).
        _coreCache = new FeatureCache(Path.Combine(_fixture.UserPath, "deep_core"));
        _coreCache.ClearAll();
        var coreSource = new CoreBitmapImageSource(_fixture.Core);
        _coreService = new DeepSearchService(
            new CoreLibCandidateSource(_fixture.Core),
            _coreCache,
            p => new EmbeddingExtractor(p, _coreCache, coreSource, _fixture.Logger),
            new GeometricVerifierFactory(p => new LightGlueGeometricVerifier(new SuperPointLightGluePipeline(p, _fixture.Logger))),
            coreSource,
            Path.Combine(_fixture.UserPath, "models"),
            _fixture.Logger);

        _referenceCache = new FeatureCache(Path.Combine(_fixture.UserPath, "deep_ref"));
        _referenceCache.ClearAll();
        var referenceSource = new ReferenceImagePipeline();
        _referenceService = new DeepSearchService(
            new CoreLibCandidateSource(_fixture.Core),
            _referenceCache,
            p => new EmbeddingExtractor(p, _referenceCache, referenceSource, _fixture.Logger),
            new GeometricVerifierFactory(p => new LightGlueGeometricVerifier(new SuperPointLightGluePipeline(p, _fixture.Logger))),
            referenceSource,
            Path.Combine(_fixture.UserPath, "models"),
            _fixture.Logger);
    }

    [Fact]
    public void AbTable_byThreshold()
    {
        float[] thresholds = { 0.45f, 0.50f, 0.60f };

        var coreRuns = thresholds.Select(t => (t, pairs: _coreService.Search(_fixture.MakeSettings(t), null, CancellationToken.None))).ToList();
        _output.WriteLine("Запускаем «ref»-вариант (WIC + OpenCV-совместимый препроцессинг)...");
        var refRuns = thresholds
            .Select(t => (t, pairs: _referenceService.Search(_fixture.MakeSettings(t), null, CancellationToken.None)))
            .ToList();

        _output.WriteLine($"{"Пара", -46} | {"core045", 8} | {"core050", 8} | {"core060", 8} | {"ref045", 8} | {"ref050", 8} | {"ref060", 8}");
        _output.WriteLine(new string('-', 104));
        foreach (var (a, b) in DeepTestFixture.ExpectedPairs)
        {
            var label = (Short(a) + " <-> " + Short(b)).PadRight(46);
            var row = new System.Text.StringBuilder(label);
            foreach (var (t, pairs) in coreRuns)
                row.Append($" | {Cell(pairs, a, b):8}");
            row.Append(" |");
            foreach (var (t, pairs) in refRuns)
                row.Append($" {Cell(pairs, a, b):8}");
            _output.WriteLine(row.ToString());
        }
        _output.WriteLine("(пусто = пара не вошла ни в один кластер на этом пороге)");

        // Индикатор ложных срабатываний: пары, не входящие в GT-набор.
        var gtKeys = DeepTestFixture.ExpectedPairs.Select(p => DeepTestFixture.NamesKey(
            Path.Combine("x", p.A), Path.Combine("x", p.B))).ToHashSet();
        _output.WriteLine("");
        _output.WriteLine("== Всего пар и «лишних» (не-GT) — контроль ложных срабатываний ==");
        foreach (var (t, pairs) in coreRuns)
            ReportExtra("core", t, pairs, gtKeys);
        foreach (var (t, pairs) in refRuns)
            ReportExtra("ref ", t, pairs, gtKeys);
    }

    private void ReportExtra(string variant, float threshold, DeepPair[] pairs, HashSet<string> gtKeys)
    {
        var extra = pairs.Where(p => !gtKeys.Contains(DeepTestFixture.NamesKey(p.Path1, p.Path2))).ToList();
        _output.WriteLine($"{variant} {threshold:0.00}: всего пар = {pairs.Length}, не-GT = {extra.Count}");
        foreach (var p in extra)
            _output.WriteLine($"   + {Path.GetFileName(p.Path1)}  <->  {Path.GetFileName(p.Path2)}  (sim={p.Similarity:0.00})");
    }

    private string Cell(DeepPair[] pairs, string a, string b)
    {
        var p = pairs.FirstOrDefault(p =>
            (Path.GetFileName(p.Path1) == a && Path.GetFileName(p.Path2) == b) ||
            (Path.GetFileName(p.Path1) == b && Path.GetFileName(p.Path2) == a));
        return p?.Similarity.ToString("0.00") ?? "";
    }

    private static string Short(string name)
        => name.Length > 22 ? name[..19] + "..." : name;
}