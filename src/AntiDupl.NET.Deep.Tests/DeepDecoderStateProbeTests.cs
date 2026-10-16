using AntiDupl.NET.Deep.Cv;
using AntiDupl.NET.Deep.Image;
using AntiDupl.NET.Deep.Logging;
using AntiDupl.NET.Onnx.Cv;
using Xunit;
using Xunit.Abstractions;

namespace AntiDupl.NET.Deep.Tests;

/// <summary>
/// Изоляция дефита «0 matches»: ОДИН фиксированный пайплайн (создан один раз),
/// а на каждом «этапе» процесса (после 0/3/6/9... поисков) заново декодируем
/// grayscale пары 2658717 через ОБЩИЙ ImageSource и прогоняем Match.
///  • matches стабильны, а gray-хэши/нули «плывут» → дефит в C++-декодере (состояние).
///  • matches=0 при стабильном gray → дефит в ORT-сессии/пайплайне.
/// </summary>
[Collection(nameof(DeepCollection))]
public class DeepDecoderStateProbeTests : IDisposable
{
    private readonly DeepTestFixture _fixture;
    private readonly ITestOutputHelper _output;
    private readonly SuperPointLightGluePipeline _pipeline;

    public DeepDecoderStateProbeTests(DeepTestFixture fixture, ITestOutputHelper output)
    {
        _fixture = fixture;
        _output = output;
        var logger = new FileDeepLogger(Path.Combine(_fixture.UserPath, "decoder_state_logs"));
        logger.Clear();
        _pipeline = new SuperPointLightGluePipeline(_fixture.PipelineModelPath, logger);
    }

    public void Dispose() => _pipeline.Dispose();

    private static string HashGray(float[,] g)
    {
        unchecked
        {
            int h = 17;
            for (int y = 0; y < g.GetLength(0); y++)
                for (int x = 0; x < g.GetLength(1); x++)
                    h = h * 31 + g[y, x].GetHashCode();
            return h.ToString("x8");
        }
    }

    [Fact]
    public void DecoderStateAcrossSearches()
    {
        var pathA = Path.Combine(_fixture.TestFolder, "Sicario-2658717.jpg");
        var pathB = Path.Combine(_fixture.TestFolder, "Sicario-2658717_2.jpg");

        // Один пайплайн на весь тест — исключает «свежие ORT-сессии» как причину.
        for (int stage = 0; stage < 4; stage++)
        {
            // «старение» процесса: один полный поиск фикстурным сервисом.
            _fixture.SearchOnce(0.45f);

            var gA = _fixture.ImageSource.DecodeLightGlueInput(pathA);
            var gB = _fixture.ImageSource.DecodeLightGlueInput(pathB);
            string ha = gA != null ? HashGray(gA) : "NULL";
            string hb = gB != null ? HashGray(gB) : "NULL";

            string match = "n/a (gray null)";
            if (gA != null && gB != null)
            {
                var (ms, sc, _, _) = _pipeline.Match(gA, gB);
                double avg = sc.Length > 0 ? sc.Average() : 0;
                match = $"matches={ms.Length} avg={avg:0.000}";
            }
            _output.WriteLine($"stage{stage}: grayA={ha} grayB={hb} {match}");
        }
    }
}