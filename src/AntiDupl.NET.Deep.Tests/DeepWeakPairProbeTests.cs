using AntiDupl.NET.Deep.Cv;
using AntiDupl.NET.Deep.Image;
using AntiDupl.NET.Deep.Models;
using AntiDupl.NET.Onnx.Cv;
using AntiDupl.NET.Onnx.Ml;
using Xunit;
using Xunit.Abstractions;

namespace AntiDupl.NET.Deep.Tests;

/// <summary>
/// Замер «слабых» пар, которые не находит ни один пайплайн (в т.ч. оригинальный —
/// headless-прогон: их нет в 8/10 парах на порогах 0.45/0.60):
///  • Sicario-2676063 <-> _2
///  • sportishka-обои 16 <-> _
/// Выводит фактическую DINOv2-схожесть (косинус) и вердикт WGC (SuperPoint+LightGlue),
/// чтобы обосновать: ограничение модели, а не баг порта.
/// </summary>
[Collection(nameof(DeepCollection))]
public sealed class DeepWeakPairProbeTests
{
    private readonly DeepTestFixture _fixture;
    private readonly ITestOutputHelper _output;

    public DeepWeakPairProbeTests(DeepTestFixture fixture, ITestOutputHelper output)
    {
        _fixture = fixture;
        _output = output;
    }

    [Fact]
    public void WeakPairs_similarity_and_wgc_probe()
    {
        var pairs = new (string A, string B)[]
        {
            ("Sicario-2676063.jpg", "Sicario-2676063_2.jpg"),
            ("1667438424_15-sportishka-com-p-boi-gladiatorov-v-drevnem-rime-oboi-16.jpg",
             "1667438424_15-sportishka-com-p-boi-gladiatorov-v-drevnem-rime-oboi-16_.jpg"),
        };

        // Отдельный лёгкий кэш, чтобы не трогать кэш фикстуры.
        var cache = new AntiDupl.NET.Deep.Cache.FeatureCache(Path.Combine(_fixture.UserPath, "deep_weakprobe"));
        cache.ClearAll();
        var extractor = new EmbeddingExtractor(
            _fixture.Dinov2ModelPath, cache, _fixture.ImageSource, _fixture.Logger);
        extractor.BatchSize = 2;
        extractor.PrefetchCount = 0;
        LightGlueGeometricVerifier? verifier = null;
        try
        {
            verifier = new LightGlueGeometricVerifier(
                new SuperPointLightGluePipeline(_fixture.PipelineModelPath, _fixture.Logger));

            foreach (var (a, b) in pairs)
            {
                var pathA = Path.Combine(_fixture.TestFolder, a);
                var pathB = Path.Combine(_fixture.TestFolder, b);

                _output.WriteLine($"== {a} <-> {b} ==");

                var embs = extractor.EmbedImagesBatch(new List<string> { pathA, pathB });
                if (embs.Length != 2 || embs[0] == null || embs[1] == null)
                {
                    _output.WriteLine("   DINOv2: дёкод/инференс не удался");
                    continue;
                }

                float sim = Cosine(embs[0], embs[1]);
                _output.WriteLine($"   DINOv2 sim = {sim:0.000} (dist = {1 - sim:0.000}; порог-соседство 0.45 ⇔ dist<0.45 ⇔ sim>0.55; MinSimilarityForPair=0.5)");

                var grayA = _fixture.ImageSource.DecodeLightGlueInput(pathA);
                var grayB = _fixture.ImageSource.DecodeLightGlueInput(pathB);
                if (grayA != null && grayB != null)
                {
                    var (ok, angle, scale, angleVotes, scaleVotes) = verifier.Verify(grayA, grayB, 0.5f);
                    _output.WriteLine($"   WGC: {(ok ? "PASS" : "FAIL")} (minMatchScore=0.5, angleVotes={angleVotes}, scaleVotes={scaleVotes})");
                }
                else
                {
                    _output.WriteLine("   WGC: gray-дёкод не удался");
                }
            }
        }
        finally
        {
            extractor.Dispose();
            verifier?.Dispose();
            cache.Dispose();
        }
    }

    private static float Cosine(float[] a, float[] b)
    {
        double dot = 0, na = 0, nb = 0;
        for (int i = 0; i < a.Length; i++)
        {
            dot += a[i] * b[i];
            na += a[i] * a[i];
            nb += b[i] * b[i];
        }
        return (float)(dot / (Math.Sqrt(na) * Math.Sqrt(nb)));
    }
}