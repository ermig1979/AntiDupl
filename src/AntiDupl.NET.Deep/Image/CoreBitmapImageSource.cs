using System.Collections.Concurrent;
using System.Runtime.InteropServices;
using AntiDupl.NET.Core;
using AntiDupl.NET.Core.Enums;
using AntiDupl.NET.Core.Original;
using AntiDupl.NET.Deep.Models;

namespace AntiDupl.NET.Deep.Image;

/// <summary>
/// Декодирование изображений через ядро AntiDupl.dll: <c>adLoadBitmapW</c>.
/// Ядро само делает декодирование (TImage::Load) и SIMD-уменьшение (Simd::Resize)
/// до размера, указанного в <see cref="AdBitmap"/>, в формат BGRA (AD_PIXEL_FORMAT_ARGB32).
/// Это полная замена Cv2.ImRead + Cv2.Resize референс-проекта.
///
/// Вход LightGlue: при наличии экспорта <c>adLoadBitmapLetterboxW</c> (ядро ≥ 2.4.0)
/// кадр вписывается в канвас 800×600 без искажения аспекта (без апскейла, кратно 8),
/// остальное — чёрное; на старом ядре — фолбэк на stretch в 800×600.
///
/// Побочная статистика (IDeepImageFileStats): при наличии Ex-экспортов ядра
/// (<c>adLoadBitmapExW</c>, <c>adLoadBitmapLetterboxMetricsW</c>) источник запоминает
/// нативные размеры (стадия DINOv2) и метрики blurring/blockiness (стадия WGC) —
/// без какого-либо дополнительного чтения файлов.
///
/// Вызовы сериализованы через _gate: буферы переиспользуются между вызовами,
/// а Parallel.For в EmbeddingExtractor декодирует несколько файлов одновременно.
/// </summary>
public sealed class CoreBitmapImageSource : IDeepImageSource, IDeepImageFileStats
{
    public const int Dinov2InputSize = 224;
    public const int LightGlueInputWidth = 800;
    public const int LightGlueInputHeight = 600;

    private readonly CoreLib _core;
    private readonly object _gate = new();
    private readonly byte[] _dinov2Buffer = new byte[Dinov2InputSize * Dinov2InputSize * 4];
    private readonly byte[] _glueBuffer = new byte[LightGlueInputWidth * LightGlueInputHeight * 4];

    // Статистика декодирования (побочный продукт вызовов ядра, без доп. чтения файлов).
    private readonly ConcurrentDictionary<string, (int Width, int Height)> _nativeSizes = new(StringComparer.OrdinalIgnoreCase);
    private readonly ConcurrentDictionary<string, (double Blurring, double Blockiness)> _fileMetrics = new(StringComparer.OrdinalIgnoreCase);

    public CoreBitmapImageSource(CoreLib core)
    {
        _core = core ?? throw new ArgumentNullException(nameof(core));
    }

    /// <summary>
    /// Порядок байтов в буфере ядра ЗАВИСИТ от ветки декодера (legacy-особенности ядра):
    ///  • JPEG JFIF (FF D8 FF E0) — libjpeg-turbo (TJPF_RGBA) → байты R,G,B,A;
    ///  • JPEG с EXIF (FF D8 FF E1) и прочие форматы — GDI+ → настоящие B,G,R,A.
    /// (useLibJpegTurbo по умолчанию TRUE; при FALSE все JPEG идут через GDI+ → BGRA.)
    /// Определяем по заголовку файла.
    /// </summary>
    private static bool IsRgbaBuffer(string path)
    {
        try
        {
            using var fs = File.OpenRead(path);
            var buf = new byte[4];
            int n = fs.Read(buf, 0, 4);
            return n == 4 && buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE0;
        }
        catch
        {
            return false;
        }
    }

    /// <summary>
    /// Дёкод в 224×224 RGB (CHW, 0..1). Порядок каналов буфера — по IsRgbaBuffer(path).
    /// При наличии adLoadBitmapExW нативные размеры запоминаются в _nativeSizes.
    /// </summary>
    public float[,,]? DecodeDinov2Input(string path)
    {
        lock (_gate)
        {
            if (!TryDecode(path, _dinov2Buffer, Dinov2InputSize, Dinov2InputSize))
                return null;

            bool rgba = IsRgbaBuffer(path);
            const int size = Dinov2InputSize;
            var tensor = new float[3, size, size];
            for (int i = 0; i < size * size; i++)
            {
                int o = i * 4;
                int y = i / size;
                int x = i % size;
                int r = rgba ? _dinov2Buffer[o] : _dinov2Buffer[o + 2];
                int g = _dinov2Buffer[o + 1];
                int b = rgba ? _dinov2Buffer[o + 2] : _dinov2Buffer[o];
                tensor[0, y, x] = r / 255f;
                tensor[1, y, x] = g / 255f;
                tensor[2, y, x] = b / 255f;
            }
            return tensor;
        }
    }

    /// <summary>
    /// Дёкод в 800×600 grayscale (0..1). Коеффициенты OpenCV BGR2GRAY (фикс-точка, как в Cv2).
    /// При наличии adLoadBitmapLetterboxW ядро вписывает кадр в канвас без искажения
    /// аспекта (letterbox, чёрная рамка); иначе — stretch в 800×600 (старое ядро).
    /// При наличии adLoadBitmapLetterboxMetricsW дополнительно запоминаются нативные
    /// размеры и метрики blurring/blockiness (без доп. чтения файла).
    /// </summary>
    public float[,]? DecodeLightGlueInput(string path)
    {
        lock (_gate)
        {
            bool ok;
            if (_core.SupportsLoadBitmapLetterboxMetrics)
                ok = TryDecodeLetterboxMetrics(path);
            else if (_core.SupportsLoadBitmapLetterbox)
                ok = TryDecodeLetterbox(path);
            else
                ok = TryDecode(path, _glueBuffer, LightGlueInputWidth, LightGlueInputHeight);
            if (!ok)
                return null;

            bool rgba = IsRgbaBuffer(path);
            const int w = LightGlueInputWidth;
            const int h = LightGlueInputHeight;
            var gray = new float[h, w];
            for (int i = 0; i < w * h; i++)
            {
                int o = i * 4;
                int r = rgba ? _glueBuffer[o] : _glueBuffer[o + 2];
                int g = _glueBuffer[o + 1];
                int b = rgba ? _glueBuffer[o + 2] : _glueBuffer[o];
                // OpenCV BGR2GRAY: (1868*B + 9617*G + 4899*R + 8192) >> 14
                int gray8 = (1868 * b + 9617 * g + 4899 * r + 8192) >> 14;
                gray[i / w, i % w] = gray8 / 255f;
            }
            return gray;
        }
    }

    private bool TryDecode(string path, byte[] buffer, int width, int height)
    {
        GCHandle bufferH = GCHandle.Alloc(buffer, GCHandleType.Pinned);
        AdBitmap[] pBitmap = new AdBitmap[1];
        try
        {
            pBitmap[0] = new AdBitmap
            {
                width = (uint)width,
                height = (uint)height,
                stride = width * 4,
                format = PixelFormatType.Argb32,
                data = bufferH.AddrOfPinnedObject()
            };
            if (_core.SupportsLoadBitmapEx)
            {
                if (_core.LoadBitmapEx(path, pBitmap, out int nw, out int nh) != Error.Ok)
                    return false;
                _nativeSizes[path] = (nw, nh);
                return true;
            }
            return _core.LoadBitmap(path, pBitmap) == Error.Ok;
        }
        catch
        {
            return false;
        }
        finally
        {
            bufferH.Free();
        }
    }

    /// <summary>Letterbox-декод в канвас 800×600 (adLoadBitmapLetterboxW): аспект сохраняется.</summary>
    private bool TryDecodeLetterbox(string path)
    {
        GCHandle bufferH = GCHandle.Alloc(_glueBuffer, GCHandleType.Pinned);
        AdBitmap[] pBitmap = new AdBitmap[1];
        try
        {
            pBitmap[0] = new AdBitmap
            {
                width = LightGlueInputWidth,
                height = LightGlueInputHeight,
                stride = LightGlueInputWidth * 4,
                format = PixelFormatType.Argb32,
                data = bufferH.AddrOfPinnedObject()
            };
            return _core.LoadBitmapLetterbox(path, LightGlueInputWidth, LightGlueInputHeight, pBitmap) == Error.Ok;
        }
        catch
        {
            return false;
        }
        finally
        {
            bufferH.Free();
        }
    }

    /// <summary>
    /// Letterbox-декод с метриками (adLoadBitmapLetterboxMetricsW): канвас 800×600 +
    /// нативные размеры + blurring/blockiness на нативном сером растре (одно чтение файла).
    /// </summary>
    private bool TryDecodeLetterboxMetrics(string path)
    {
        GCHandle bufferH = GCHandle.Alloc(_glueBuffer, GCHandleType.Pinned);
        AdBitmap[] pBitmap = new AdBitmap[1];
        try
        {
            pBitmap[0] = new AdBitmap
            {
                width = LightGlueInputWidth,
                height = LightGlueInputHeight,
                stride = LightGlueInputWidth * 4,
                format = PixelFormatType.Argb32,
                data = bufferH.AddrOfPinnedObject()
            };
            if (_core.LoadBitmapLetterboxMetrics(path, LightGlueInputWidth, LightGlueInputHeight, pBitmap,
                out int nw, out int nh, out double blurring, out double blockiness) != Error.Ok)
                return false;
            _nativeSizes[path] = (nw, nh);
            _fileMetrics[path] = (blurring, blockiness);
            return true;
        }
        catch
        {
            return false;
        }
        finally
        {
            bufferH.Free();
        }
    }

    //---------- IDeepImageFileStats --------------------------------------------

    public void ClearFileStats()
    {
        _nativeSizes.Clear();
        _fileMetrics.Clear();
    }

    public void SeedFileStats(IReadOnlyDictionary<string, DeepFileStats> stats)
    {
        foreach (var kv in stats)
        {
            if (kv.Value.Width > 0 || kv.Value.Height > 0)
                _nativeSizes[kv.Key] = (kv.Value.Width, kv.Value.Height);
            if (kv.Value.Blurring != 0.0 || kv.Value.Blockiness != 0.0)
                _fileMetrics[kv.Key] = (kv.Value.Blurring, kv.Value.Blockiness);
        }
    }

    public IReadOnlyDictionary<string, DeepFileStats> CollectFileStats()
    {
        var result = new Dictionary<string, DeepFileStats>(StringComparer.OrdinalIgnoreCase);
        foreach (var kv in _nativeSizes)
        {
            var s = result.TryGetValue(kv.Key, out var existing) ? existing : new DeepFileStats();
            s.Width = kv.Value.Width;
            s.Height = kv.Value.Height;
            result[kv.Key] = s;
        }
        foreach (var kv in _fileMetrics)
        {
            var s = result.TryGetValue(kv.Key, out var existing) ? existing : new DeepFileStats();
            s.Blurring = kv.Value.Blurring;
            s.Blockiness = kv.Value.Blockiness;
            result[kv.Key] = s;
        }
        return result;
    }

    public bool TryGetNativeSize(string path, out int width, out int height)
    {
        if (_nativeSizes.TryGetValue(path, out var v))
        {
            width = v.Width;
            height = v.Height;
            return true;
        }
        width = 0;
        height = 0;
        return false;
    }

    public bool TryGetFileMetrics(string path, out double blurring, out double blockiness)
    {
        if (_fileMetrics.TryGetValue(path, out var v))
        {
            blurring = v.Blurring;
            blockiness = v.Blockiness;
            return true;
        }
        blurring = 0;
        blockiness = 0;
        return false;
    }
}