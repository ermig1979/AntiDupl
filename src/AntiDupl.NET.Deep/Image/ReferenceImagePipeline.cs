using System.Windows.Media.Imaging;

namespace AntiDupl.NET.Deep.Image;

/// <summary>
/// «Eталонный» вариант препроцессинга (A/B-замер): воспроизводит входы референс-проекта
/// DinoDuplicateSearchWPF 1-в-1 (кроме декодера JPEG):
/// <list type="bullet">
/// <item><b>Декод</b>: WPF <see cref="BitmapImage"/> (WIC, OnLoad + IgnoreColorProfile + Freeze + CopyPixels) —
///   как <c>ImageUtils.BitmapImageToMat</c> оригинала. Оригинал в DINOv2-ветке использовал
///   <c>Cv2.ImRead</c> (libjpeg) для ВСЕХ путей; WIC — ближайший доступный декодер
///   («core»-вариант использует libjpeg-turbo через ядро).</item>
/// <item><b>DINOv2</b>: BGR2RGB → <c>Cv2.Resize(224×224, INTER_LINEAR)</c> (<see cref="OpenCvResize"/>) → /255, CHW RGB.</item>
/// <item><b>LightGlue</b>: BGR2GRAY (фикс-точка OpenCV) на ПОЛНОМ размере →
///   <c>scale = min(1, 800/w, 600/h)</c> (оригинал НЕ апскейлит мелкие кадры) →
///   размеры кратно 8 (мин. 8) → INTER_LINEAR → копирование в левый верх 800×600-холста (остальное — 0) → /255.
///   (Оригинал пэдил пару до (maxH, maxW); для SuperPoint/LightGlue эквивалентно —
///   чёрная рамка не порождает ключевых точек, содержимое остаётся в нативном масштабе.)</item>
/// </list>
/// </summary>
public sealed class ReferenceImagePipeline : IDeepImageSource
{
    public const int Dinov2InputSize = 224;
    public const int LightGlueInputWidth = 800;
    public const int LightGlueInputHeight = 600;

    public float[,,]? DecodeDinov2Input(string path)
    {
        var decoded = DecodeBgra(path);
        if (decoded == null)
            return null;
        var (bgra, w, h) = decoded.Value;

        // BGR2RGB (packed)
        var rgb = new byte[w * h * 3];
        for (int i = 0; i < w * h; i++)
        {
            rgb[i * 3 + 0] = bgra[i * 4 + 2]; // R
            rgb[i * 3 + 1] = bgra[i * 4 + 1]; // G
            rgb[i * 3 + 2] = bgra[i * 4 + 0]; // B
        }

        var dst = new byte[Dinov2InputSize * Dinov2InputSize * 3];
        OpenCvResize.Resize(rgb, w, h, 3, dst, Dinov2InputSize, Dinov2InputSize);

        var tensor = new float[3, Dinov2InputSize, Dinov2InputSize];
        for (int i = 0; i < Dinov2InputSize * Dinov2InputSize; i++)
        {
            int y = i / Dinov2InputSize;
            int x = i % Dinov2InputSize;
            tensor[0, y, x] = dst[i * 3 + 0] / 255f;
            tensor[1, y, x] = dst[i * 3 + 1] / 255f;
            tensor[2, y, x] = dst[i * 3 + 2] / 255f;
        }
        return tensor;
    }

    public float[,]? DecodeLightGlueInput(string path)
    {
        var decoded = DecodeBgra(path);
        if (decoded == null)
            return null;
        var (bgra, w, h) = decoded.Value;

        // BGR2GRAY (OpenCV-коэффициенты, фикс-точка) — как Cv2.CvtColor(BGR2GRAY), ДО resize.
        var gray = new byte[w * h];
        for (int i = 0; i < w * h; i++)
        {
            int b = bgra[i * 4];
            int g = bgra[i * 4 + 1];
            int r = bgra[i * 4 + 2];
            gray[i] = (byte)((1868 * b + 9617 * g + 4899 * r + 8192) >> 14);
        }

        // scale = min(1, 800/w, 600/h) — апскейла нет (оригинал SuperPointLightGluePipeline.Match).
        float scale = Math.Min(1.0f, Math.Min((float)LightGlueInputWidth / w, (float)LightGlueInputHeight / h));
        int tw = (int)(w * scale / 8) * 8;
        int th = (int)(h * scale / 8) * 8;
        if (tw < 8) tw = 8;
        if (th < 8) th = 8;

        var resized = new byte[tw * th];
        OpenCvResize.Resize(gray, w, h, 1, resized, tw, th);

        // Холст 800×600: содержимое в левом верху, остальное — 0 (вместо (maxH,maxW)-пэдинга пары).
        var canvas = new float[LightGlueInputHeight, LightGlueInputWidth];
        for (int y = 0; y < th; y++)
            for (int x = 0; x < tw; x++)
                canvas[y, x] = resized[y * tw + x] / 255f;
        return canvas;
    }

    /// <summary>WIC-декод в BGRA (аналог ImageUtils.ReadImageCv2-ветки оригинала).</summary>
    private static (byte[] bgra, int w, int h)? DecodeBgra(string path)
    {
        try
        {
            using var stream = File.OpenRead(path);
            var bitmap = new BitmapImage();
            bitmap.BeginInit();
            bitmap.CacheOption = BitmapCacheOption.OnLoad;
            bitmap.StreamSource = stream;
            bitmap.CreateOptions = BitmapCreateOptions.IgnoreColorProfile;
            bitmap.EndInit();
            bitmap.Freeze();

            int stride = bitmap.PixelWidth * 4;
            var pixels = new byte[stride * bitmap.PixelHeight];
            bitmap.CopyPixels(pixels, stride, 0);
            return (pixels, bitmap.PixelWidth, bitmap.PixelHeight);
        }
        catch
        {
            return null;
        }
    }
}