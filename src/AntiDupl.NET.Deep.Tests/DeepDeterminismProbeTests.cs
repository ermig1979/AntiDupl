using AntiDupl.NET.Deep.Tests;
using AntiDupl.NET.Deep.Tests;
using Xunit;
using Xunit.Abstractions;

namespace AntiDupl.NET.Deep.Tests;

/// <summary>
/// Временная диагностика (WGC-флейк): 3 последовательных запуска в одном процессе.
/// Флип внутри процесса → недетерминизм ORT-инференса (CPU-ядра/треды).
/// Стабильно внутри процесса, но флипает между процессами → процесс-уровень (запуск ORT).
/// </summary>
[Collection(nameof(DeepCollection))]
public class DeepDeterminismProbeTests
{
    private readonly DeepTestFixture _fixture;
    private readonly ITestOutputHelper _output;

    public DeepDeterminismProbeTests(DeepTestFixture fixture, ITestOutputHelper output)
    {
        _fixture = fixture;
        _output = output;
    }

    [Fact]
    public void ThreeSequentialRuns_sameProcess()
    {
        for (int i = 1; i <= 3; i++)
        {
            var pairs = _fixture.SearchOnce(0.45f);
            var names = pairs.Select(p => DeepTestFixture.NamesKey(p.Path1, p.Path2)).OrderBy(x => x);
            bool has2658717 = DeepTestFixture.ContainsPair(pairs, "Sicario-2658717.jpg", "Sicario-2658717_2.jpg");
            bool hasKaren = DeepTestFixture.ContainsPair(pairs, "Карен Фукухара_2.jpg", "Карен Фукухара_3.jpg");
            _output.WriteLine($"--- run {i}: {pairs.Length} пар; 2658717={has2658717}; Карен={hasKaren} ---");
            foreach (var n in names) _output.WriteLine("   " + n);
        }
    }
}