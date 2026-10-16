namespace AntiDupl.NET.Deep.Cv;

/// <summary>
/// Верификатор геометрической согласованности пары изображений.
/// Перенос IGeometricVerifier референс-проекта (без OpenCV Mat на входе).
/// </summary>
public interface IGeometricVerifier
{
    string Name { get; }

    /// <param name="gray1">Grayscale-кадр первого изображения, 0..1, [600,800].</param>
    /// <param name="gray2">Grayscale-кадр второго изображения, 0..1, [600,800].</param>
    /// <param name="minMatchScore">
    /// Минимальная средняя уверенность совпадений LightGlue (параметр запуска,
    /// DeepSettings.MinMatchScore; в оригинале задавался на верификаторе при создании фабрики).
    /// </param>
    /// <returns>(ok, angle, scale, matchCount, avgScorePercent) — см. LightGlueGeometricVerifier.</returns>
    (bool ok, float angle, float scale, int angleVotes, int scaleVotes) Verify(float[,] gray1, float[,] gray2, float minMatchScore);
}