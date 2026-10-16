using AntiDupl.NET.Deep.Cv;
using AntiDupl.NET.Onnx.Cv;
using Xunit;
using Xunit.Abstractions;

namespace AntiDupl.NET.Deep.Tests;

/// <summary>
/// Прямые тесты декодера ядра и LightGlue-верификатора (без полного поиска).
/// </summary>
[Collection(nameof(DeepCollection))]
public sealed class DeepDecoderAndVerifierTests
{
    private readonly DeepTestFixture _fixture;
    private readonly ITestOutputHelper _output;

    public DeepDecoderAndVerifierTests(DeepTestFixture fixture, ITestOutputHelper output)
    {
        _fixture = fixture;
        _output = output;
    }

    private string TestFile(string name)
    {
        var path = Path.Combine(_fixture.TestFolder, name);
        Assert.True(File.Exists(path), $"Нет файла: {path}");
        return path;
    }

    [Fact]
    public void ImageSource_decodesDinov2Input_224x224_CHW_unitRange()
    {
        var rgb = _fixture.ImageSource.DecodeDinov2Input(TestFile("Sicario-2658717.jpg"));

        Assert.NotNull(rgb);
        Assert.Equal(3, rgb!.GetLength(0));   // CHW: каналы
        Assert.Equal(224, rgb.GetLength(1));
        Assert.Equal(224, rgb.GetLength(2));

        var (min, max) = (float.PositiveInfinity, float.NegativeInfinity);
        bool allZero = true;
        for (int c = 0; c < 3; c++)
            for (int y = 0; y < 224; y++)
                for (int x = 0; x < 224; x++)
                {
                    float v = rgb[c, y, x];
                    if (v != 0f) allZero = false;
                    if (v < min) min = v;
                    if (v > max) max = v;
                }

        Assert.False(allZero, "Декодер вернул нулевой кадр (неудачное декодирование?)");
        Assert.True(min >= -0.01f, $"Значения ниже 0: min={min}");
        Assert.True(max <= 1.01f, $"Значения выше 1: max={max}");
    }

    [Fact]
    public void ImageSource_decodesLightGlueInput_600x800_unitRange()
    {
        var gray = _fixture.ImageSource.DecodeLightGlueInput(TestFile("Sicario-2658717.jpg"));

        Assert.NotNull(gray);
        Assert.Equal(600, gray!.GetLength(0));  // H
        Assert.Equal(800, gray.GetLength(1));   // W

        var (min, max) = (float.PositiveInfinity, float.NegativeInfinity);
        bool allZero = true;
        for (int y = 0; y < gray!.GetLength(0); y++)
            for (int x = 0; x < gray.GetLength(1); x++)
            {
                float v = gray[y, x];
                if (v != 0f) allZero = false;
                if (v < min) min = v;
                if (v > max) max = v;
            }

        Assert.False(allZero, "Декодер вернул нулевой кадр (неудачное декодирование?)");
        Assert.True(min >= -0.01f, $"Значения ниже 0: min={min}");
        Assert.True(max <= 1.01f, $"Значения выше 1: max={max}");
    }

    [Fact]
    public void Verifier_positiveControl_hardCrop_pairPasses()
    {
        using var pipeline = new SuperPointLightGluePipeline(_fixture.PipelineModelPath, _fixture.Logger);
        using var verifier = new LightGlueGeometricVerifier(pipeline);

        var a = _fixture.ImageSource.DecodeLightGlueInput(TestFile("9955dd6dffcd25bdc98b16b7d7041a75.jpg"))!;
        var b = _fixture.ImageSource.DecodeLightGlueInput(TestFile("9955dd6dffcd25bdc98b16b7d7041a75_2.jpg"))!;
        Assert.NotNull(a);
        Assert.NotNull(b);

        var (ok, angle, scale, angleVotes, scaleVotes) = verifier.Verify(a, b, 0.5f);

        _output.WriteLine($"pair 9955: ok={ok} angle={angle:F1} scale={scale:F2} votes={angleVotes}/{scaleVotes}");
        Assert.True(ok, "Hard-crop дубликат не прошёл LightGlue-верификацию (положительный контроль)");
    }

    [Fact]
    public void Verifier_negativeControl_unrelatedPairFails()
    {
        using var pipeline = new SuperPointLightGluePipeline(_fixture.PipelineModelPath, _fixture.Logger);
        using var verifier = new LightGlueGeometricVerifier(pipeline);

        var a = _fixture.ImageSource.DecodeLightGlueInput(TestFile("Sicario-2658717.jpg"))!;
        var b = _fixture.ImageSource.DecodeLightGlueInput(TestFile("emma stone.jpg"))!;
        Assert.NotNull(a);
        Assert.NotNull(b);

        var (ok, angle, scale, angleVotes, scaleVotes) = verifier.Verify(a, b, 0.5f);

        _output.WriteLine($"unrelated: ok={ok} angle={angle:F1} votes={angleVotes}/{scaleVotes}");
        Assert.False(ok, "Несвязанные изображения прошли LightGlue-верификацию (ложный PASS)");
    }
}