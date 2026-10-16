using System.Windows.Media.Imaging;
using AntiDupl.NET.Deep.Image;
using AntiDupl.NET.Deep.Models;
using Xunit;
using Xunit.Abstractions;

namespace AntiDupl.NET.Deep.Tests;

/// <summary>
/// Пиксельный пробник (без инференса): для файлов пары Карен
/// — нативные размеры, средние каналов R/G/B у «core» и «ref» вариантов
/// (правильность порядка каналов), gray-средние и попутельная MAE входов.
/// Если бы порядок каналов был перепутан у одного из вариантов,
/// средние R и B были бы замето поменяны местами относительно другого.
/// </summary>
[Collection(nameof(DeepCollection))]
public sealed class DeepChannelProbeTests
{
    private readonly DeepTestFixture _fixture;
    private readonly ITestOutputHelper _output;

    public DeepChannelProbeTests(DeepTestFixture fixture, ITestOutputHelper output)
    {
        _fixture = fixture;
        _output = output;
    }

    [Fact]
    public void ChannelOrder_andInputDiff_probe()
    {
        var names = new[]
        {
            "Карен Фукухара_2.jpg",
            "Карен Фукухара_3.jpg",
            "Sicario-2658717.jpg",
            "Sicario-2667418.jpg",
        };

        var reference = new ReferenceImagePipeline();
        var core = new CoreBitmapImageSource(_fixture.Core);

        foreach (var name in names)
        {
            var file = Path.Combine(_fixture.TestFolder, name);
            if (!File.Exists(file))
            {
                _output.WriteLine($"== {name}: НЕТ ФАЙЛА ==");
                continue;
            }

            int nw = 0, nh = 0;
            try
            {
                using var stream = File.OpenRead(file);
                var bmp = new BitmapImage { CacheOption = BitmapCacheOption.OnLoad, StreamSource = stream };
                bmp.Freeze();
                nw = bmp.PixelWidth;
                nh = bmp.PixelHeight;
            }
            catch { }

            _output.WriteLine($"== {name}: native {nw}x{nh} ==");

            var dinov2Ref = reference.DecodeDinov2Input(file);
            var dinov2Core = core.DecodeDinov2Input(file);
            var grayRef = reference.DecodeLightGlueInput(file);
            var grayCore = core.DecodeLightGlueInput(file);

            if (dinov2Ref != null)
            {
                var (r, g, b) = Means224(dinov2Ref);
                _output.WriteLine($"   ref  dinov2 224: R={r:0.000} G={g:0.000} B={b:0.000}");
            }
            if (dinov2Core != null)
            {
                var (r, g, b) = Means224(dinov2Core);
                _output.WriteLine($"   core dinov2 224: R={r:0.000} G={g:0.000} B={b:0.000}");
            }
            if (dinov2Ref != null && dinov2Core != null)
            {
                float mr = 0, mg = 0, mb = 0;
                for (int y = 0; y < 224; y++)
                    for (int x = 0; x < 224; x++)
                    {
                        mr += Math.Abs(dinov2Ref[0, y, x] - dinov2Core[0, y, x]);
                        mg += Math.Abs(dinov2Ref[1, y, x] - dinov2Core[1, y, x]);
                        mb += Math.Abs(dinov2Ref[2, y, x] - dinov2Core[2, y, x]);
                    }
                int n = 224 * 224;
                _output.WriteLine($"   MAE dinov2 (ref vs core): R={mr / n:0.0000} G={mg / n:0.0000} B={mb / n:0.0000}");
            }
            _output.WriteLine($"   gray means: ref={Mean2D(grayRef):0.000} core={Mean2D(grayCore):0.000}" +
                             (grayRef != null && grayCore != null ? $" grayMAE={GrayMae(grayRef, grayCore):0.0000}" : ""));

            var cRef = dinov2Ref == null ? null : new[] { dinov2Ref[0, 112, 112], dinov2Ref[1, 112, 112], dinov2Ref[2, 112, 112] };
            var cCore = dinov2Core == null ? null : new[] { dinov2Core[0, 112, 112], dinov2Core[1, 112, 112], dinov2Core[2, 112, 112] };
            if (cRef != null && cCore != null)
                _output.WriteLine($"   center px: ref  R={cRef[0]:0.00} G={cRef[1]:0.00} B={cRef[2]:0.00} | core R={cCore[0]:0.00} G={cCore[1]:0.00} B={cCore[2]:0.00}");
        }
    }

    private static (float r, float g, float b) Means224(float[,,] t)
    {
        float r = 0, g = 0, b = 0;
        for (int y = 0; y < 224; y++)
            for (int x = 0; x < 224; x++)
            {
                r += t[0, y, x];
                g += t[1, y, x];
                b += t[2, y, x];
            }
        int n = 224 * 224;
        return (r / n, g / n, b / n);
    }

    private static float Mean2D(float[,]? g)
    {
        if (g == null) return 0;
        float s = 0;
        for (int y = 0; y < g.GetLength(0); y++)
            for (int x = 0; x < g.GetLength(1); x++)
                s += g[y, x];
        return s / (float)(g.GetLength(0) * g.GetLength(1));
    }

    private static float GrayMae(float[,] a, float[,] b)
    {
        float s = 0;
        for (int y = 0; y < a.GetLength(0); y++)
            for (int x = 0; x < a.GetLength(1); x++)
                s += Math.Abs(a[y, x] - b[y, x]);
        return s / (float)(a.GetLength(0) * a.GetLength(1));
    }
}