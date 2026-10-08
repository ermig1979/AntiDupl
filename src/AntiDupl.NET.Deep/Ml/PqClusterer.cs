using System.Collections.Concurrent;
using System.IO;
using AntiDupl.NET.Deep.Models;

namespace AntiDupl.NET.Deep.Ml;

/// <summary>
/// Кластеризация больших выборок (10k–50k+) через Product Quantization.
/// Вынесен из DeepSearchService: k-means-центроиды + поиск соседей (kNN) +
/// union-find с транзитивностью; кэш центроидов/соседей — бинарные файлы в
/// models-каталоге (хэш эмбеддингов — ключ валидности).
/// </summary>
public static class PqClusterer
{
    private const int SubvectorCount = 96;
    private const int CentroidsPerSubspace = 256;
    private const int TopK = 5;

    /// <param name="pqDirectory">Каталог для кэша центроидов/соседей (models-каталог хоста).</param>
    public static int[] Cluster(
        float[][] embeddings,
        float distanceThreshold,
        float transitivityRatio,
        string pqDirectory,
        IProgress<DeepProgress>? progress,
        CancellationToken ct)
    {
        int n = embeddings.Length;
        int dim = embeddings[0].Length;

        var pq = new ProductQuantizer(dim, SubvectorCount, CentroidsPerSubspace);
        string centroidsPath = Path.Combine(pqDirectory, "pq_centroids.bin");

        progress?.Report(new DeepProgress(40, "PQ: Loading centroids..."));
        if (pq.TryLoadCentroids(centroidsPath, embeddings))
        {
            progress?.Report(new DeepProgress(42, "PQ: Centroids loaded from cache"));
        }
        else
        {
            progress?.Report(new DeepProgress(40, "PQ: Training centroids..."));
            var pqProgress = new Progress<string>(msg => progress?.Report(new DeepProgress(41, $"PQ: {msg}")));
            pq.Train(embeddings, maxIterations: 50, seed: 42, progress: pqProgress);
            pq.SaveCentroids(centroidsPath, embeddings);
        }

        ct.ThrowIfCancellationRequested();
        progress?.Report(new DeepProgress(42, "PQ: Encoding database..."));
        var encodedDb = pq.Encode(embeddings);

        ct.ThrowIfCancellationRequested();
        string neighborsPath = Path.Combine(pqDirectory, "pq_neighbors.bin");
        var neighbors = new ConcurrentBag<(int, int)>();
        float pqThreshold = 2 * distanceThreshold;

        if (!TryLoadNeighbors(neighborsPath, embeddings, neighbors))
        {
            progress?.Report(new DeepProgress(43, "PQ: Searching nearest neighbors..."));
            int completed = 0;
            int reportInterval = Math.Max(1, n / 100);

            Parallel.For(0, n, i =>
            {
                ct.ThrowIfCancellationRequested();
                var results = pq.Search(encodedDb, embeddings[i], TopK + 1);
                foreach (var result in results)
                {
                    if (result.index != i && result.distance < pqThreshold)
                        neighbors.Add((i, result.index));
                }

                int done = Interlocked.Increment(ref completed);
                if (done % reportInterval == 0)
                    progress?.Report(new DeepProgress(43.0 + 2.0 * done / n, $"PQ: {done}/{n}"));
            });

            SaveNeighbors(neighborsPath, embeddings, neighbors.ToArray());
        }
        else
        {
            progress?.Report(new DeepProgress(45, "PQ: Neighbors loaded from cache"));
        }

        progress?.Report(new DeepProgress(45, "PQ: Union-Find clustering..."));
        var edgeList = new List<(int i, int j, float dist)>();
        foreach (var pair in neighbors)
        {
            float dist = 1 - DotProduct(embeddings[pair.Item1], embeddings[pair.Item2]);
            edgeList.Add((pair.Item1, pair.Item2, dist));
        }
        edgeList.Sort((a, b) => a.dist.CompareTo(b.dist));

        float pqTransitivityThreshold = distanceThreshold * transitivityRatio;
        var uf = new UnionFind(n);
        foreach (var (i, j, dist) in edgeList)
        {
            if (uf.Find(i) == uf.Find(j)) continue;
            if (dist < pqTransitivityThreshold)
            {
                uf.Union(i, j);
            }
            else if (uf.Size(uf.Find(i)) + uf.Size(uf.Find(j)) <= 50)
            {
                uf.Union(i, j);
            }
        }

        return uf.GetLabels();
    }

    private static bool TryLoadNeighbors(string path, float[][] embeddings, ConcurrentBag<(int, int)> neighbors)
    {
        if (!File.Exists(path)) return false;

        try
        {
            using var fs = new FileStream(path, FileMode.Open, FileAccess.Read);
            using var br = new BinaryReader(fs);

            long storedHash = br.ReadInt64();
            int count = br.ReadInt32();

            long currentHash = ComputeEmbeddingsHash(embeddings);
            if (storedHash != currentHash) return false;

            for (int i = 0; i < count; i++)
            {
                int a = br.ReadInt32();
                int b = br.ReadInt32();
                neighbors.Add((a, b));
            }
            return true;
        }
        catch
        {
            return false;
        }
    }

    private static void SaveNeighbors(string path, float[][] embeddings, (int, int)[] neighbors)
    {
        using var fs = new FileStream(path, FileMode.Create, FileAccess.Write);
        using var bw = new BinaryWriter(fs);

        bw.Write(ComputeEmbeddingsHash(embeddings));
        bw.Write(neighbors.Length);

        foreach (var (a, b) in neighbors)
        {
            bw.Write(a);
            bw.Write(b);
        }
    }

    private static long ComputeEmbeddingsHash(float[][] embeddings)
    {
        long hash = embeddings.Length;
        int step = Math.Max(1, embeddings.Length / 1000);
        for (int i = 0; i < embeddings.Length; i += step)
        {
            for (int j = 0; j < embeddings[i].Length; j += 8)
                hash = hash * 31 + BitConverter.SingleToInt32Bits(embeddings[i][j]);
        }
        return hash;
    }

    private static float DotProduct(float[] a, float[] b)
    {
        float sum = 0;
        for (int i = 0; i < a.Length; i++) sum += a[i] * b[i];
        return sum;
    }
}