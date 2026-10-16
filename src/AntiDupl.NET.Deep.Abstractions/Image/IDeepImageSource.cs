namespace AntiDupl.NET.Deep.Image;

/// <summary>
/// Источник растров для deep-пайплайна — замена OpenCV-декодирования референс-проекта
/// (Cv2.ImRead / ImageUtils.ReadImageCv2).
///
/// Контракт (сходится с поведением исходника):
/// <see cref="DecodeDinov2Input"/>  — 8-битное RGB-изображение, уменьшенное (с искажением
///   пропорций, как Cv2.Resize) до 224×224, каналы в порядке R,G,B, значения 0..1,
///   раскладка CHW [3,224,224] — прямой вход модели DINOv2-base (pixel_values).
/// <see cref="DecodeLightGlueInput"/> — 8-bit grayscale content fitted without upscaling
///   into an 800×600 black canvas while preserving aspect ratio; fitted dimensions are
///   multiples of 8 and content is placed at the top left. Values are 0..1 and layout is
///   [600,800] (H,W), the direct input of the SuperPoint+LightGlue ONNX model.
///
/// null = файл не декодировался (в референсе такие кадры дают нулевой 768-D эмбеддинг).
/// </summary>
public interface IDeepImageSource
{
    float[,,]? DecodeDinov2Input(string path);

    float[,]? DecodeLightGlueInput(string path);
}