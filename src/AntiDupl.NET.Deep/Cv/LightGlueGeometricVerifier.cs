using System.Collections.Generic;
using AntiDupl.NET.Deep.Image;

using AntiDupl.NET.Onnx.Cv;
namespace AntiDupl.NET.Deep.Cv;

/// <summary>
/// Верификатор на сжатом пайплайне SuperPoint + LightGlue.
/// Перенос LightGlueGeometricVerifier из DinoDuplicateSearch.CV (без OpenCV).
///
/// Критерий PASS (идентичен референсу):
///   ранний выход: matches &lt; 5 → FAIL;
///   иначе PASS ⇔ matches ≥ 10 && maxAngleVotes ≥ 5 (гистограмма углов, 24 бина [0..360))
///                && avgScore ≥ minMatchScore (аргумент Verify; дефолт приложения — 0.6,
///                оригинальный референс использовал 0.5).
/// Возвращаемый кортеж переиспользует поля референса: 4-й элемент = число совпадений,
/// 5-й = (int)(avgScore*100), scale — плейсхолдер 1.0 (LightGlue не даёт масштаб).
/// Верификатор не хранит состояния по minMatchScore: порог передаётся на каждый вызов Verify
/// (данные запуска, а не конфигурация компонента).
/// </summary>
public class LightGlueGeometricVerifier : IGeometricVerifier, IDisposable
{
    private readonly SuperPointLightGluePipeline _pipeline;

    public string Name => "SuperPoint + LightGlue";

    public LightGlueGeometricVerifier(SuperPointLightGluePipeline pipeline)
    {
        _pipeline = pipeline ?? throw new ArgumentNullException(nameof(pipeline));
    }

    public (bool ok, float angle, float scale, int angleVotes, int scaleVotes) Verify(float[,] gray1, float[,] gray2, float minMatchScore)
    {
        var (matches, scores, kp0, kp1) = _pipeline.Match(gray1, gray2);

        if (matches.Length < 5)
            return (false, 0, 0, matches.Length, 0);

        var angles = new List<float>();
        foreach (var m in matches)
        {
            float dx = kp1[m[1], 0] - kp0[m[0], 0];
            float dy = kp1[m[1], 1] - kp0[m[0], 1];
            var matchAngle = (float)(Math.Atan2(dy, dx) * 180.0 / Math.PI + 360) % 360;
            angles.Add(matchAngle);
        }

        var histAngles = GeometricConsistency.ComputeHistogramPublic(angles, 24, 0, 360);
        int maxAngleVotes = histAngles.Max();
        float avgAngle = 0;
        if (maxAngleVotes > 0)
        {
            int bestBin = Array.IndexOf(histAngles, maxAngleVotes);
            float binWidth = 360f / 24;
            avgAngle = (bestBin * binWidth + (bestBin + 1) * binWidth) / 2;
        }

        float avgScore = scores.Length > 0 ? scores.Average() : 0;
        bool passed = matches.Length >= 10 && maxAngleVotes >= 5 && avgScore >= minMatchScore;

        return (passed, avgAngle, 1.0f, matches.Length, (int)(avgScore * 100));
    }

    public void Dispose()
    {
        _pipeline.Dispose();
    }
}