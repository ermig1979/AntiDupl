using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Threading;
using System.Threading.Tasks;

namespace AntiDupl.NET.Deep.Ml;

/// <summary>
/// Product Quantization: сжатие векторов и быстрый поиск приближённых ближайших соседей
/// (перенос ProductQuantizer из DinoDuplicateSearchWPF, без примера использования).
/// Используется для набора больше 50 000 изображений (PQThreshold в DeepSearchService):
/// 96 подвекторов × 256 центроидов, обучение k-means (50 итераций, seed 42),
/// ADC-таблица + topK=6, кэш центроидов/соседей в models-каталоге.
/// </summary>
public class ProductQuantizer
{
    private readonly int _dimension;           // D – исходная размерность
    private readonly int _subvectorCount;      // M – количество подвекторов
    private readonly int _centroidsPerSubspace;// K – число центроидов в каждом подпространстве
    private readonly int _subvectorDim;        // d = D / M – размерность одного подпространства

    // centroids[m][k][component] – центроиды: M подпространств, K центроидов, d компонент
    private float[][][]? _centroids;
    private bool _trained;

    public ProductQuantizer(int dimension, int subvectorCount, int centroidsPerSubspace = 256)
    {
        if (dimension % subvectorCount != 0)
            throw new ArgumentException("Размерность D должна нацело делиться на число подпространств M.");

        _dimension = dimension;
        _subvectorCount = subvectorCount;
        _centroidsPerSubspace = centroidsPerSubspace;
        _subvectorDim = dimension / subvectorCount;
        _centroids = null;
        _trained = false;
    }

    public void Train(float[][] vectors, int maxIterations = 100, float tolerance = 1e-4f, int? seed = null, IProgress<string>? progress = null)
    {
        if (vectors == null || vectors.Length == 0)
            throw new ArgumentException("Обучающая выборка не может быть пустой.");
        if (vectors.Any(v => v.Length != _dimension))
            throw new ArgumentException($"Все векторы должны иметь размерность {_dimension}.");

        _centroids = new float[_subvectorCount][][];
        Random rng = seed.HasValue ? new Random(seed.Value) : new Random();
        int completed = 0;

        Parallel.For(0, _subvectorCount, m =>
        {
            float[][] subvectors = ExtractSubvectors(vectors, m);
            _centroids[m] = KMeans(subvectors, _centroidsPerSubspace, maxIterations, tolerance, rng.Next());
            int done = Interlocked.Increment(ref completed);
            progress?.Report($"k-means: {done} из {_subvectorCount} подпространств");
        });

        _trained = true;
    }

    public bool TryLoadCentroids(string path, float[][] sampleVectors)
    {
        if (!File.Exists(path)) return false;

        try
        {
            using var fs = new FileStream(path, FileMode.Open, FileAccess.Read);
            using var br = new BinaryReader(fs);

            int dim = br.ReadInt32();
            int m = br.ReadInt32();
            int k = br.ReadInt32();
            long storedHash = br.ReadInt64();

            if (dim != _dimension || m != _subvectorCount || k != _centroidsPerSubspace)
                return false;

            long currentHash = ComputeHash(sampleVectors);
            if (storedHash != currentHash) return false;

            _centroids = new float[m][][];
            for (int i = 0; i < m; i++)
            {
                _centroids[i] = new float[k][];
                for (int j = 0; j < k; j++)
                {
                    _centroids[i][j] = new float[_subvectorDim];
                    for (int d = 0; d < _subvectorDim; d++)
                        _centroids[i][j][d] = br.ReadSingle();
                }
            }

            _trained = true;
            return true;
        }
        catch
        {
            return false;
        }
    }

    public void SaveCentroids(string path, float[][] sampleVectors)
    {
        EnsureTrained();

        using var fs = new FileStream(path, FileMode.Create, FileAccess.Write);
        using var bw = new BinaryWriter(fs);

        bw.Write(_dimension);
        bw.Write(_subvectorCount);
        bw.Write(_centroidsPerSubspace);
        bw.Write(ComputeHash(sampleVectors));

        for (int m = 0; m < _subvectorCount; m++)
            for (int k = 0; k < _centroidsPerSubspace; k++)
                for (int d = 0; d < _subvectorDim; d++)
                    bw.Write(_centroids[m][k][d]);
    }

    private static long ComputeHash(float[][] vectors)
    {
        int sampleSize = Math.Min(vectors.Length, 1000);
        int step = Math.Max(1, vectors.Length / sampleSize);
        long hash = 17;
        for (int i = 0; i < vectors.Length; i += step)
        {
            for (int j = 0; j < vectors[i].Length; j += 8)
                hash = hash * 31 + BitConverter.SingleToInt32Bits(vectors[i][j]);
        }
        return hash;
    }

    /// <summary>Кодирует один вектор в коды PQ (индексы ближайших центроидов в каждом подпространстве).</summary>
    public int[] Encode(float[] vector)
    {
        EnsureTrained();
        if (vector.Length != _dimension)
            throw new ArgumentException($"Вектор должен иметь размерность {_dimension}.");

        int[] codes = new int[_subvectorCount];
        Parallel.For(0, _subvectorCount, m =>
        {
            float[] subvec = GetSubvector(vector, m);
            codes[m] = FindNearestCentroidIndex(subvec, _centroids[m]);
        });
        return codes;
    }

    /// <summary>Кодирует коллекцию векторов.</summary>
    public int[][] Encode(float[][] vectors)
    {
        return vectors.Select(Encode).ToArray();
    }

    /// <summary>Восстанавливает приближённый вектор по кодам PQ.</summary>
    public float[] Decode(int[] codes)
    {
        EnsureTrained();
        if (codes.Length != _subvectorCount)
            throw new ArgumentException($"Код должен содержать {_subvectorCount} индексов.");

        float[] reconstructed = new float[_dimension];
        for (int m = 0; m < _subvectorCount; m++)
        {
            int code = codes[m];
            if (code < 0 || code >= _centroidsPerSubspace)
                throw new ArgumentException($"Некорректный код {code} в подпространстве {m}.");
            float[] centroid = _centroids[m][code];
            int offset = m * _subvectorDim;
            Array.Copy(centroid, 0, reconstructed, offset, _subvectorDim);
        }
        return reconstructed;
    }

    /// <summary>
    /// Строит таблицу асимметричного расстояния (ADC) для вектора запроса.
    /// adcTable[m][k] = расстояние от подвектора запроса до k-го центроида в подпространстве m.
    /// </summary>
    public float[][] ComputeADCTable(float[] query)
    {
        EnsureTrained();
        if (query.Length != _dimension)
            throw new ArgumentException($"Запрос должен иметь размерность {_dimension}.");

        float[][] table = new float[_subvectorCount][];
        for (int m = 0; m < _subvectorCount; m++)
        {
            float[] querySub = GetSubvector(query, m);
            table[m] = new float[_centroidsPerSubspace];
            for (int k = 0; k < _centroidsPerSubspace; k++)
                table[m][k] = EuclideanDistanceSquared(querySub, _centroids[m][k]);
        }
        return table;
    }

    /// <summary>
    /// Вычисляет приближённое расстояние (квадрат L2) между запросом и вектором, заданным кодами PQ.
    /// </summary>
    public float ComputeDistance(float[] query, int[] codes, float[][]? adcTable = null)
    {
        EnsureTrained();
        if (adcTable == null)
            adcTable = ComputeADCTable(query);

        float distance = 0f;
        for (int m = 0; m < _subvectorCount; m++)
            distance += adcTable[m][codes[m]];
        return distance;
    }

    /// <summary>Поиск top-k ближайших соседей среди набора закодированных векторов.</summary>
    public List<(int index, float distance)> Search(int[][] encodedDatabase, float[] query, int topK)
    {
        EnsureTrained();
        float[][] adcTable = ComputeADCTable(query);
        // Очередь с приоритетом для top-k (храним пары: расстояние, индекс)
        var candidates = new SortedSet<(float distance, int index)>(Comparer<(float distance, int index)>.Create((a, b) =>
        {
            int cmp = a.distance.CompareTo(b.distance);
            return cmp != 0 ? cmp : a.index.CompareTo(b.index);
        }));

        for (int i = 0; i < encodedDatabase.Length; i++)
        {
            float dist = ComputeDistance(query, encodedDatabase[i], adcTable);
            candidates.Add((dist, i));
            if (candidates.Count > topK)
                candidates.Remove(candidates.Max);
        }

        return candidates.Select(p => (p.index, p.distance)).ToList();
    }

    // ====================== Приватные вспомогательные методы ======================

    [System.Diagnostics.CodeAnalysis.MemberNotNull(nameof(_centroids))]
    private void EnsureTrained()
    {
        if (!_trained || _centroids == null)
            throw new InvalidOperationException("Квантователь не обучен. Сначала вызовите Train().");
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private float[] GetSubvector(float[] vector, int m)
    {
        int start = m * _subvectorDim;
        float[] sub = new float[_subvectorDim];
        Array.Copy(vector, start, sub, 0, _subvectorDim);
        return sub;
    }

    private float[][] ExtractSubvectors(float[][] vectors, int m)
    {
        int start = m * _subvectorDim;
        float[][] subs = new float[vectors.Length][];
        for (int i = 0; i < vectors.Length; i++)
        {
            subs[i] = new float[_subvectorDim];
            Array.Copy(vectors[i], start, subs[i], 0, _subvectorDim);
        }
        return subs;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private int FindNearestCentroidIndex(float[] subvec, float[][] centroids)
    {
        float minDist = float.MaxValue;
        int best = 0;
        for (int k = 0; k < centroids.Length; k++)
        {
            float dist = EuclideanDistanceSquared(subvec, centroids[k]);
            if (dist < minDist)
            {
                minDist = dist;
                best = k;
            }
        }
        return best;
    }
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private static float EuclideanDistanceSquared(float[] a, float[] b)
    {
        int i = 0;
        float sum = 0f;
        int vecSize = Vector<float>.Count;

        if (vecSize > 0 && a.Length >= vecSize)
        {
            var sumVec = Vector<float>.Zero;
            for (; i <= a.Length - vecSize; i += vecSize)
            {
                var diff = new Vector<float>(a, i) - new Vector<float>(b, i);
                sumVec += diff * diff;
            }
            for (int j = 0; j < vecSize; j++)
                sum += sumVec[j];
        }

        for (; i < a.Length; i++)
        {
            float diff = a[i] - b[i];
            sum += diff * diff;
        }
        return sum;
    }

    // Стандартный алгоритм k‑средних (Lloyd)
    private static float[][] KMeans(float[][] data, int k, int maxIterations, float tolerance, int seed)
    {
        if (data.Length < k)
            throw new ArgumentException("Недостаточно данных для формирования k кластеров.");

        int dim = data[0].Length;
        Random rng = new Random(seed);

        // Инициализация: случайный выбор k точек из данных
        float[][] centroids = new float[k][];
        int[] indices = Enumerable.Range(0, data.Length).OrderBy(_ => rng.Next()).Take(k).ToArray();
        for (int i = 0; i < k; i++)
        {
            centroids[i] = new float[dim];
            Array.Copy(data[indices[i]], centroids[i], dim);
        }

        int[] assignments = new int[data.Length];
        float[][] newCentroids = new float[k][];
        bool changed = true;
        int iter = 0;

        while (changed && iter < maxIterations)
        {
            changed = false;
            iter++;

            // Шаг 1: назначение точек ближайшим центроидам (параллельно)
            bool anyChanged = false;
            Parallel.For(0, data.Length, () => false, (i, state, localChanged) =>
            {
                int nearest = 0;
                float minDist = float.MaxValue;
                for (int j = 0; j < k; j++)
                {
                    float dist = EuclideanDistanceSquared(data[i], centroids[j]);
                    if (dist < minDist)
                    {
                        minDist = dist;
                        nearest = j;
                    }
                }
                if (assignments[i] != nearest)
                {
                    assignments[i] = nearest;
                    localChanged = true;
                }
                return localChanged;
            }, localChanged => { if (localChanged) anyChanged = true; });
            changed = anyChanged;

            // Шаг 2: пересчёт центроидов
            float[][] sum = new float[k][];
            int[] count = new int[k];
            for (int j = 0; j < k; j++)
                sum[j] = new float[dim];

            for (int i = 0; i < data.Length; i++)
            {
                int cluster = assignments[i];
                count[cluster]++;
                for (int d = 0; d < dim; d++)
                    sum[cluster][d] += data[i][d];
            }

            float maxShift = 0f;
            for (int j = 0; j < k; j++)
            {
                float[] newCent = new float[dim];
                if (count[j] > 0)
                {
                    for (int d = 0; d < dim; d++)
                        newCent[d] = sum[j][d] / count[j];
                }
                else
                {
                    // Если кластер пуст, оставляем старый центроид
                    Array.Copy(centroids[j], newCent, dim);
                }
                float shift = EuclideanDistanceSquared(centroids[j], newCent);
                if (shift > maxShift)
                    maxShift = shift;
                newCentroids[j] = newCent;
            }

            centroids = newCentroids;

            if (maxShift <= tolerance * tolerance) // т.к. сравниваем квадрат расстояния
                break;
        }

        return centroids;
    }
}