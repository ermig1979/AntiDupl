using System.Numerics;
using System.Threading;
using System.Threading.Tasks;
using AntiDupl.NET.Deep.Models;

namespace AntiDupl.NET.Deep.Ml;

/// <summary>
/// Кластеризация эмбеддингов (перенос AgglomerativeClustering из DinoDuplicateSearch.ML).
/// До 10 000 изображений — average-linkage agglomerative на полной матрице расстояний;
/// выше — all-pairs union-find: «сильная» связь (dist &lt; порог × транзитивность) всегда
/// объединяет компоненты, «слабая» — только если объединяемый размер ≤ 50.
/// </summary>
public static class AgglomerativeClustering
{
    private const int SmallDatasetThreshold = 10_000;

    public static int[] FitPredict(float[][] embeddings, float distanceThreshold, IProgress<DeepProgress>? progress = null, float transitivityRatio = 0.7f)
    {
        var n = embeddings.Length;
        if (n == 0) return Array.Empty<int>();

        return n <= SmallDatasetThreshold
            ? FitPredictAgglomerative(embeddings, distanceThreshold, progress)
            : FitPredictUnionFind(embeddings, distanceThreshold, progress, transitivityRatio);
    }

    private static int[] FitPredictUnionFind(float[][] embeddings, float distanceThreshold, IProgress<DeepProgress>? progress, float transitivityRatio = 0.7f)
    {
        var n = embeddings.Length;
        var dim = embeddings[0].Length;
        float transitivityThreshold = distanceThreshold * transitivityRatio;

        var edges = new System.Collections.Concurrent.ConcurrentBag<(int i, int j, float dist)>();
        int completed = 0;
        int reportInterval = Math.Max(1, n / 100);

        Parallel.For(0, n, i =>
        {
            for (int j = i + 1; j < n; j++)
            {
                float dist = 1 - DotProduct(embeddings[i], embeddings[j], dim);
                if (dist < distanceThreshold)
                    edges.Add((i, j, dist));
            }

            int done = Interlocked.Increment(ref completed);
            if (done % reportInterval == 0)
                progress?.Report(new DeepProgress(40.0 + 5.0 * done / n, $"Clustering: {done}/{n}"));
        });

        progress?.Report(new DeepProgress(43, "Graph clustering..."));
        var sortedEdges = edges.OrderBy(e => e.dist).ToArray();

        var uf = new UnionFind(n);
        foreach (var (i, j, dist) in sortedEdges)
        {
            if (uf.Find(i) == uf.Find(j)) continue;
            if (dist < transitivityThreshold)
            {
                uf.Union(i, j);
            }
            else
            {
                var rootI = uf.Find(i);
                var rootJ = uf.Find(j);
                if (uf.Size(rootI) + uf.Size(rootJ) <= 50)
                    uf.Union(i, j);
            }
        }

        progress?.Report(new DeepProgress(45, "Clustering complete"));
        return uf.GetLabels();
    }

    private static int[] FitPredictAgglomerative(float[][] embeddings, float distanceThreshold, IProgress<DeepProgress>? progress)
    {
        var n = embeddings.Length;
        var dim = embeddings[0].Length;

        var dist = new float[n][];
        for (int i = 0; i < n; i++)
            dist[i] = new float[n];

        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                float dot = DotProduct(embeddings[i], embeddings[j], dim);
                dist[i][j] = 1 - dot;
                dist[j][i] = dist[i][j];
            }
            if (i % Math.Max(1, n / 100) == 0)
                progress?.Report(new DeepProgress(40.0 + 2.0 * i / n, $"Building distance matrix: {i}/{n}"));
        }

        var labels = Enumerable.Range(0, n).ToArray();
        var clusterSize = Enumerable.Repeat(1, n).ToArray();
        var active = Enumerable.Repeat(true, n).ToArray();

        int activeCount = n;
        int mergeStep = 0;

        while (true)
        {
            float minDist = float.MaxValue;
            int minI = -1, minJ = -1;

            for (int i = 0; i < n; i++)
            {
                if (!active[i]) continue;
                for (int j = i + 1; j < n; j++)
                {
                    if (!active[j]) continue;
                    if (dist[i][j] < minDist)
                    {
                        minDist = dist[i][j];
                        minI = i;
                        minJ = j;
                    }
                }
            }

            if (minI == -1 || minDist >= distanceThreshold)
                break;

            for (int k = 0; k < n; k++)
            {
                if (!active[k] || k == minI || k == minJ) continue;
                float newDist = (dist[minI][k] * clusterSize[minI] + dist[minJ][k] * clusterSize[minJ])
                               / (clusterSize[minI] + clusterSize[minJ]);
                dist[minI][k] = newDist;
                dist[k][minI] = newDist;
            }

            var oldLabel = labels[minJ];
            var newLabel = labels[minI];
            for (int k = 0; k < n; k++)
            {
                if (labels[k] == oldLabel)
                    labels[k] = newLabel;
            }

            clusterSize[minI] += clusterSize[minJ];
            active[minJ] = false;
            activeCount--;
            mergeStep++;

            if (mergeStep % Math.Max(1, n / 100) == 0)
                progress?.Report(new DeepProgress(42.0 + 3.0 * (n - activeCount) / n, $"Merging clusters: {mergeStep} merges, {activeCount} active"));
        }

        progress?.Report(new DeepProgress(45, "Clustering complete"));

        var uniqueLabels = labels.Distinct().OrderBy(x => x).ToArray();
        var labelMap = new Dictionary<int, int>();
        for (int i = 0; i < uniqueLabels.Length; i++)
            labelMap[uniqueLabels[i]] = i;

        for (int i = 0; i < n; i++)
            labels[i] = labelMap[labels[i]];

        return labels;
    }

    private static float DotProduct(float[] a, float[] b, int dim)
    {
        int i = 0;
        float sum = 0;

        int vecSize = Vector<float>.Count;
        if (vecSize > 0 && dim >= vecSize)
        {
            var sumVec = Vector<float>.Zero;
            for (; i <= dim - vecSize; i += vecSize)
                sumVec += new Vector<float>(a, i) * new Vector<float>(b, i);
            for (int j = 0; j < vecSize; j++)
                sum += sumVec[j];
        }

        for (; i < dim; i++)
            sum += a[i] * b[i];

        return sum;
    }
}