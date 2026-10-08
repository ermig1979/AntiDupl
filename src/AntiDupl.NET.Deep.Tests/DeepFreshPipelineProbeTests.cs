using AntiDupl.NET.Deep.Cv;
using AntiDupl.NET.Deep.Logging;
using AntiDupl.NET.Onnx.Cv;
using Xunit;
using Xunit.Abstractions;

namespace AntiDupl.NET.Deep.Tests;

/// <summary>
/// Дефит «0 matches» на СВЕЖИХ инстансах пайплайна: в одном процессе держим
/// 5 живых инстансов (каждый — своя ORT-сессия), один и тот же gray-вход
/// (задекодирован один раз). Если свежие сессии дают 0 matches → проблема
/// в создании/работе ORT CPU-сессии при множестве живых сессий (память/инициализация).
/// </summary>
[Collection(nameof(DeepCollection))]
public class DeepFreshPipelineProbeTests : IDisposable
{
    private readonly DeepTestFixture _fixture;
    private readonly ITestOutputHelper _output;
    private readonly List<SuperPointLightGluePipeline> _alive = new();

    public DeepFreshPipelineProbeTests(DeepTestFixture fixture, ITestOutputHelper output)
    {
        _fixture = fixture;
        _output = output;
    }

    public void Dispose()
    {
        foreach (var p in _alive) p.Dispose();
    }

    [Fact]
    public void FiveFreshPipelines_sameGray()
    {
        var pathA = Path.Combine(_fixture.TestFolder, "Sicario-2658717.jpg");
        var pathB = Path.Combine(_fixture.TestFolder, "Sicario-2658717_2.jpg");

        var gA = _fixture.ImageSource.DecodeLightGlueInput(pathA)!;
        var gB = _fixture.ImageSource.DecodeLightGlueInput(pathB)!;

        var logger = new FileDeepLogger(Path.Combine(_fixture.UserPath, "fresh_pipeline_logs"));
        logger.Clear();

        var alive = _alive;
        for (int i = 1; i <= 5; i++)
        {
            var p = new SuperPointLightGluePipeline(_fixture.PipelineModelPath, logger);
            var (ms, sc, _, _) = p.Match(gA, gB);
            double avg = sc.Length > 0 ? sc.Average() : 0;
            _output.WriteLine($"pipeline{i} (живо: {alive.Count + 1}): matches={ms.Length} avg={avg:0.000}");
            alive.Add(p);
        }
    }
}