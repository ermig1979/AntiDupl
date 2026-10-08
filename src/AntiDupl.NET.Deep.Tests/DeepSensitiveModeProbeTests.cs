using AntiDupl.NET.Deep.Models;
using Xunit;
using Xunit.Abstractions;

namespace AntiDupl.NET.Deep.Tests;

/// <summary>
/// «Чувствительный режим»: порог 0.80 + MinSimilarityForPair 0.25 (WGC ON).
/// Проверка: слабые пары (Sicario-2676063 sim=0.266, sportishka sim=0.397) находятся,
/// и цена — сколько ложных пар появляется на TestCropHard.
/// </summary>
[Collection(nameof(DeepCollection))]
public sealed class DeepSensitiveModeProbeTests
{
    private readonly DeepTestFixture _fixture;
    private readonly ITestOutputHelper _output;

    public DeepSensitiveModeProbeTests(DeepTestFixture fixture, ITestOutputHelper output)
    {
        _fixture = fixture;
        _output = output;
    }

    [Fact]
    public void SensitiveMode_080_minSim025_probe()
    {
        var settings = _fixture.MakeSettings(0.80f);
        settings.MinSimilarityForPair = 0.25f;
        var pairs = _fixture.Service.Search(settings, null, CancellationToken.None);

        _output.WriteLine($"--- sensitive (threshold=0.80, minSim=0.25, WGC=ON): {pairs.Length} пар ---");
        foreach (var p in pairs)
            _output.WriteLine($"  {p.Similarity,6:0.00}  {Path.GetFileName(p.Path1)}  <->  {Path.GetFileName(p.Path2)}");

        var expectedKeys = DeepTestFixture.ExpectedPairs
            .Select(x => DeepTestFixture.NamesKey(x.A, x.B)).ToHashSet();
        var gt = DeepTestFixture.PairNames(pairs).Where(k => expectedKeys.Contains(k)).ToList();
        var nonGt = DeepTestFixture.PairNames(pairs).Where(k => !expectedKeys.Contains(k)).ToList();
        _output.WriteLine($"GT-пары: {gt.Count}/13; не-GT: {nonGt.Count}");
        foreach (var k in nonGt) _output.WriteLine($"  + {k}");

        Assert.True(DeepTestFixture.ContainsPair(pairs, "Sicario-2676063.jpg", "Sicario-2676063_2.jpg"),
            "Sicario-2676063 не найден в чувствительном режиме");
        Assert.True(DeepTestFixture.ContainsPair(
            pairs,
            "1667438424_15-sportishka-com-p-boi-gladiatorov-v-drevnem-rime-oboi-16.jpg",
            "1667438424_15-sportishka-com-p-boi-gladiatorov-v-drevnem-rime-oboi-16_.jpg"),
            "sportishka не найдена в чувствительном режиме");
    }
}