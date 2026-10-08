using AntiDupl.NET.Deep.Models;
using Xunit;
using Xunit.Abstractions;

namespace AntiDupl.NET.Deep.Tests;

/// <summary>
/// Диагностический тест: таблица «пара → симилярити DINOv2 → найдена?» для порогов
/// 0.45 / 0.60 / 0.90 (WGC включён, MinSimilarityForPair=0 — чтобы видеть все пары
/// из кластеров, включая просеянные порогом сходства). Ассертов нет — таблица для
/// ревью качества порога.
/// </summary>
[Collection(nameof(DeepCollection))]
public sealed class DeepRecallDiagnostics
{
    private readonly DeepTestFixture _fixture;
    private readonly ITestOutputHelper _output;

    public DeepRecallDiagnostics(DeepTestFixture fixture, ITestOutputHelper output)
    {
        _fixture = fixture;
        _output = output;
    }

    [Fact]
    public void RecallTable_byThreshold()
    {
        float[] thresholds = { 0.45f, 0.60f, 0.90f };
        var runs = thresholds
            .Select(t => (t, pairs: _fixture.Service.Search(new DeepSettings
            {
                DistanceThreshold = t,
                GeometricCheckEnabled = true,
                MinSimilarityForPair = 0.0f,
                MinMatchScore = 0.5f,
                Dinov2ModelPath = _fixture.Dinov2ModelPath,
                LightGlueModelPath = _fixture.PipelineModelPath,
            }, null, CancellationToken.None)))
            .ToList();

        _output.WriteLine($"{"Пара", -46} | {"0.45", 7} | {"0.60", 7} | {"0.90", 7}");
        _output.WriteLine(new string('-', 78));
        foreach (var (a, b) in DeepTestFixture.ExpectedPairs)
        {
            var label = (Short(a) + " <-> " + Short(b)).PadRight(46);
            var row = new System.Text.StringBuilder(label);
            foreach (var (t, pairs) in runs)
            {
                var sim = SimilarityOf(pairs, a, b);
                row.Append($" | {sim:7:0.00}");
            }
            _output.WriteLine(row.ToString());
        }
        _output.WriteLine("(пусто = пара не вошла ни в один кластер на этом пороге)");
    }

    private static float? SimilarityOf(DeepPair[] pairs, string a, string b)
    {
        var p = pairs.FirstOrDefault(p =>
            (Path.GetFileName(p.Path1) == a && Path.GetFileName(p.Path2) == b) ||
            (Path.GetFileName(p.Path1) == b && Path.GetFileName(p.Path2) == a));
        return p?.Similarity;
    }

    private static string Short(string name)
        => name.Length > 24 ? name[..21] + "..." : name;
}