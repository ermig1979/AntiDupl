namespace AntiDupl.NET.Deep.Image;

/// <summary>
/// Источник растров для deep-пайплайна — замена OpenCV-декодирования референс-проекта
/// (Cv2.ImRead / ImageUtils.ReadImageCv2).
///
/// Контракт (сходится с поведением исходника):
/// <see cref="DecodeDinov2Input"/>  — 8-битное RGB-изображение, уменьшенное (с искажением
///   пропорций, как Cv2.Resize) до 224×224, каналы в порядке R,G,B, значения 0..1,
///   раскладка CHW [3,224,224] — прямой вход модели DINOv2-base (pixel_values).
/// <see cref="DecodeLightGlueInput"/> — 8-битный grayscale-кадр, уменьшенный (с искажением
///   пропорций) до 800×600, значения 0..1, раскладка [600,800] (H,W) — прямой вход
///   сжатого SuperPoint+LightGlue ONNX-моделя (оба кадра одного размера → батч 2×1×600×800,
///   padding не нужен; 800 и 600 кратны 8).
///
/// null = файл не декодировался (в референсе такие кадры дают нулевой 768-D эмбеддинг).
/// </summary>
public interface IDeepImageSource
{
    float[,,]? DecodeDinov2Input(string path);

    float[,]? DecodeLightGlueInput(string path);
}