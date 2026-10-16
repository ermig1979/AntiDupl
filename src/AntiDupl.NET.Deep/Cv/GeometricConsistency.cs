using System.Collections.Generic;

namespace AntiDupl.NET.Deep.Cv;

/// <summary>
/// Вспомогательные функции геометрической согласованности.
/// Перенос полезной части GeometricConsistency из DinoDuplicateSearch.CV:
/// только гистограмма углов (используется LightGlueGeometricVerifier).
/// SIFT/LightGlue-ветви и Mat-конверсии референса не переносились.
/// </summary>
public static class GeometricConsistency
{
    public static int[] ComputeHistogramPublic(List<float> values, int bins, float min, float max)
        => ComputeHistogram(values, bins, min, max);

    private static int[] ComputeHistogram(List<float> values, int bins, float min, float max)
    {
        var hist = new int[bins];
        var binWidth = (max - min) / bins;
        foreach (var v in values)
        {
            var idx = (int)((v - min) / binWidth);
            idx = Math.Clamp(idx, 0, bins - 1);
            hist[idx]++;
        }
        return hist;
    }
}