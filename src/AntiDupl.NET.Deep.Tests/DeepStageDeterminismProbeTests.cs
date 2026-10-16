using AntiDupl.NET.Deep.Cache;
using AntiDupl.NET.Deep.Cv;
using AntiDupl.NET.Deep.Models;
using AntiDupl.NET.Onnx.Cv;
using AntiDupl.NET.Onnx.Ml;
using Xunit;
using Xunit.Abstractions;

namespace AntiDupl.NET.Deep.Tests;

/// <summary>
/// Декомпозиция WGC-флейка на паре Sicario-2658717 (CPU-режим, DEEP_FORCE_CPU=1):
/// 3 свежих пайплайна/экстрактора в одном процессе.
///  (1) gray-декод (C++ ядро) — детерминирован? (хэш 800×600 float);
///  (2) DINOv2-эмбеддинг (ORT CPU) — детерминирован? (хэш 768 float);
///  (3) SuperPoint+LightGlue Match (ORT CPU) — детерминирован? (matches/avgScore).
/// </summary>
[Collection(nameof(DeepCollection))]
public class DeepStageDeterminismProbeTests
{
    private readonly DeepTestFixture _fixture;
    private readonly ITestOutputHelper _output;

    public DeepStageDeterminismProbeTests(DeepTestFixture fixture, ITestOutputHelper output)
    {
        _fixture = fixture;
        _output = output;
    }

    private static string FloatHash(float[] v)
    {
        unchecked
        {
            int h = 17;
            foreach (var x in v) h = h * 31 + x.GetHashCode();
            return h.ToString("x8");
        }
    }

    [Fact]
    public void StageDeterminism_2658717()
    {
        var pathA = Path.Combine(_fixture.TestFolder, "Sicario-2658717.jpg");
        var pathB = Path.Combine(_fixture.TestFolder, "Sicario-2658717_2.jpg");

        for (int i = 1; i <= 3; i++)
        {
            // (1) gray
            var gA = _fixture.ImageSource.DecodeLightGlueInput(pathA)!;
            var gB = _fixture.ImageSource.DecodeLightGlueInput(pathB)!;
            string ha = FloatHash(ToLinear(gA)), hb = FloatHash(ToLinear(gB));

            // (2) DINOv2 (свежий экстрактор, свежий кэш)
            var cache = new FeatureCache(Path.Combine(_fixture.UserPath, "stage_det_" + i));
            cache.ClearAll();
            var extractor = new EmbeddingExtractor(
                _fixture.Dinov2ModelPath, cache, _fixture.ImageSource, _fixture.Logger)
            { BatchSize = 2, PrefetchCount = 0 };
            float[]? ea = null, eb = null;
            try
            {
                var embs = extractor.EmbedImagesBatch(new List<string> { pathA, pathB });
                if (embs.Length == 2 && embs[0] != null && embs[1] != null)
                {
                    ea = (float[])embs[0];
                    eb = (float[])embs[1];
                }
            }
            finally { extractor.Dispose(); }

            // (3) WGC Match (свежий пайплайн)
            float matches = 0, avg = 0;
            using var pipeline = new SuperPointLightGluePipeline(_fixture.PipelineModelPath, _fixture.Logger);
            var (ms, sc, _, _) = pipeline.Match(gA, gB);
            matches = ms.Length;
            avg = sc.Length > 0 ? sc.Average() : 0;

            _output.WriteLine($"run{i}: grayA={ha} grayB={hb} embA={FloatHash(ea!)} embB={FloatHash(eb!)} matches={matches} avgScore={avg:0.000}");
        }
    }

    private static float[] ToLinear(float[,] m)
    {
        var r = new float[m.GetLength(0) * m.GetLength(1)];
        for (int y = 0; y < m.GetLength(0); y++)
            for (int x = 0; x < m.GetLength(1); x++)
                r[y * m.GetLength(1) + x] = m[y, x];
        return r;
    }
}