namespace AntiDupl.NET.Deep.Image;

/// <summary>
/// Managed-реализация <c>Cv2.Resize(..., INTER_LINEAR)</c> OpenCV для 8-битных packed-изображений.
/// Точная формула OpenCV (Resize.cpp, без fx/fy):
/// <code>
/// srcX = (x + 0.5) * srcW / dstW - 0.5;
/// x0 = (int)srcX;
/// if (srcX &lt; 0) { x0 = 0; ax = 0; }
/// else { ax = srcX - x0; if (x0 >= srcW - 1) { x0 = srcW - 1; ax = 0; } }
/// x1 = min(x0 + 1, srcW - 1);  (вклад x1 нулевой, когда ax == 0)
/// v = (1-ay)*(1-ax)*s[y0,x0] + (1-ay)*ax*s[y0,x1] + ay*(1-ax)*s[y1,x0] + ay*ax*s[y1,x1];
/// dst = cvRound(v)  — для v в [0..255] эквивалентно (int)(v + 0.5)
/// </code>
/// Никакой антиалиасинг/low-pass — как у OpenCV INTER_LINEAR (точечный би-линейный сэмплинг).
/// Используется эталонным вариантом препроцессинга (<see cref="ReferenceImagePipeline"/>, A/B-замер),
/// чтобы воспроизвести пиксельные входы референс-проекта.
/// </summary>
public static class OpenCvResize
{
    /// <param name="src">packed-источник, stride = srcW * channels</param>
    /// <param name="dst">packed-результат, stride = dstW * channels</param>
    public static void Resize(byte[] src, int srcW, int srcH, int channels, byte[] dst, int dstW, int dstH)
    {
        float invScaleX = (float)srcW / dstW;
        float invScaleY = (float)srcH / dstH;

        for (int y = 0; y < dstH; y++)
        {
            double srcY = (y + 0.5) * invScaleY - 0.5;
            int y0 = (int)srcY;
            float ay;
            if (srcY < 0) { y0 = 0; ay = 0f; }
            else { ay = (float)(srcY - y0); if (y0 >= srcH - 1) { y0 = srcH - 1; ay = 0f; } }
            int y1 = Math.Min(y0 + 1, srcH - 1);

            for (int x = 0; x < dstW; x++)
            {
                double srcX = (x + 0.5) * invScaleX - 0.5;
                int x0 = (int)srcX;
                float ax;
                if (srcX < 0) { x0 = 0; ax = 0f; }
                else { ax = (float)(srcX - x0); if (x0 >= srcW - 1) { x0 = srcW - 1; ax = 0f; } }
                int x1 = Math.Min(x0 + 1, srcW - 1);

                int d00 = (y0 * srcW + x0) * channels;
                int d01 = (y0 * srcW + x1) * channels;
                int d10 = (y1 * srcW + x0) * channels;
                int d11 = (y1 * srcW + x1) * channels;
                int di = (y * dstW + x) * channels;

                for (int c = 0; c < channels; c++)
                {
                    float v00 = src[d00 + c];
                    float v01 = src[d01 + c];
                    float v10 = src[d10 + c];
                    float v11 = src[d11 + c];
                    float row0 = v00 + (v01 - v00) * ax;
                    float row1 = v10 + (v11 - v10) * ax;
                    float v = row0 + (row1 - row0) * ay;
                    dst[di + c] = (byte)(v + 0.5f);
                }
            }
        }
    }
}