using AntiDupl.NET.Deep.Cache;
using AntiDupl.NET.Deep.Ml;
using AntiDupl.NET.Deep.Models;
using Xunit;

namespace AntiDupl.NET.Deep.Tests;

/// <summary>
/// Чистые юнит-тесты без ядра и моделей: кэш, кластеризация, union-find, PQ.
/// Проходят на любой машине (не зависят от AntiDupl.dll и TestCropHard).
/// </summary>
public sealed class DeepUnitTests
{
    private static double MtimeUtcSeconds(string path)
        => File.GetLastWriteTimeUtc(path).Ticks / (double)TimeSpan.TicksPerSecond;

    private static string WriteTempImage(string dir, string name, byte[] content)
    {
        Directory.CreateDirectory(dir);
        var path = Path.Combine(dir, name);
        File.WriteAllBytes(path, content);
        return path;
    }

    /// <summary>Каталог тестовых файлов кэша — рядом с программой (не в %TEMP%).</summary>
    private static string TestCacheDir()
        => Path.Combine(AppContext.BaseDirectory, "deeptest", "cache_" + Guid.NewGuid().ToString("N"));

    // ---------------- FeatureCache ----------------

    [Fact]
    public void FeatureCache_roundTrip_persistsAcrossInstances()
    {
        var dir = TestCacheDir();
        try
        {
            var path = WriteTempImage(dir, "a.jpg", new byte[] { 1, 2, 3, 4 });
            var vec = new float[] { 0.5f, -0.25f, 1f };

            using (var cache = new FeatureCache(dir))
            {
                cache.SetEmbedding(path, MtimeUtcSeconds(path), vec);
                cache.SetWgc(path, Path.Combine(dir, "b.jpg"), MtimeUtcSeconds(path), 123.0, 0.5f,
                    true, 45f, 1.2f, 9, 4);
            } // dispose => save

            using (var reopened = new FeatureCache(dir))
            {
                var emb = reopened.GetEmbedding(path);
                Assert.NotNull(emb);
                Assert.Equal(vec.Length, emb!.Value.embedding.Length);
                for (int i = 0; i < vec.Length; i++)
                    Assert.True(Math.Abs(vec[i] - emb.Value.embedding[i]) < 1e-4f,
                        $"Эмбеддинг изменился после round-trip: [{i}] {vec[i]} != {emb.Value.embedding[i]}");
                Assert.Equal(1L, reopened.GetEmbeddingCount());
            }
        }
        finally
        {
            try { Directory.Delete(dir, recursive: true); } catch { }
        }
    }

    [Fact]
    public void FeatureCache_wgcVerdictIsKeyedByMinMatchScore()
    {
        // Регрессия: вердикт WGC зависит от порога avgScore ≥ minMatchScore, поэтому
        // порог входит в ключ кэша — смена порога в настройках не возвращает старый вердикт.
        var dir = TestCacheDir();
        try
        {
            var path = WriteTempImage(dir, "a.jpg", new byte[] { 1, 2, 3 });
            var b = Path.Combine(dir, "b.jpg");
            var t1 = MtimeUtcSeconds(path);
            var t2 = 123.0;

            using var cache = new FeatureCache(dir);
            cache.SetWgc(path, b, t1, t2, 0.5f, true, 45f, 1.0f, 59, 55);

            // Тот же порог — hit; другой порог — miss (пересчёт с актуальным критерием PASS).
            Assert.NotNull(cache.GetWgc(path, b, t1, t2, 0.5f));
            Assert.Null(cache.GetWgc(path, b, t1, t2, 0.6f));

            // Round-trip: свойство порога переживает перезапись/чтение файла кэша.
            cache.Save();
            using var reopened = new FeatureCache(dir);
            Assert.NotNull(reopened.GetWgc(path, b, t1, t2, 0.5f));
            Assert.Null(reopened.GetWgc(path, b, t1, t2, 0.6f));
        }
        finally
        {
            try { Directory.Delete(dir, recursive: true); } catch { }
        }
    }

    [Fact]
    public void FeatureCache_invaliatesOnMtimeChange()
    {
        var dir = TestCacheDir();
        try
        {
            var path = WriteTempImage(dir, "a.jpg", new byte[] { 1, 2, 3, 4 });
            var vec = new float[] { 0.5f, -0.25f, 1f };

            using var cache = new FeatureCache(dir);
            cache.SetEmbedding(path, MtimeUtcSeconds(path), vec);
            Assert.NotNull(cache.GetEmbedding(path));

            // «Изменили» файл — mtime прыгает дальше допустимых 0.01 с.
            File.SetLastWriteTimeUtc(path, DateTime.UtcNow.AddMinutes(5));
            Assert.Null(cache.GetEmbedding(path));
        }
        finally
        {
            try { Directory.Delete(dir, recursive: true); } catch { }
        }
    }

    [Fact]
    public void FeatureCache_clearAll_removesRecords()
    {
        var dir = TestCacheDir();
        try
        {
            var path = WriteTempImage(dir, "a.jpg", new byte[] { 9 });
            using var cache = new FeatureCache(dir);
            cache.SetEmbedding(path, MtimeUtcSeconds(path), new float[] { 1f });
            cache.ClearAll();
            Assert.Equal(0L, cache.GetEmbeddingCount());
            Assert.Null(cache.GetEmbedding(path));
        }
        finally
        {
            try { Directory.Delete(dir, recursive: true); } catch { }
        }
    }

    [Fact]
    public void FeatureCache_fileStats_roundTrip_andClearAll()
    {
        var dir = TestCacheDir();
        try
        {
            using (var cache = new FeatureCache(dir))
            {
                // Пустого кэша — файла статистики нет.
                Assert.False(cache.TryLoadFileStats(out _));

                var stats = new Dictionary<string, DeepFileStats>
                {
                    ["a.jpg"] = new DeepFileStats { Width = 640, Height = 480, Blurring = 0.5, Blockiness = 1.25 },
                    ["b.jpg"] = new DeepFileStats { Width = 800, Height = 600 },
                };
                cache.SaveFileStats(stats);
            }

            // Новый экземпляр считывает файл с диска.
            using (var cache = new FeatureCache(dir))
            {
                Assert.True(cache.TryLoadFileStats(out var loaded));
                Assert.Equal(2, loaded.Count);
                Assert.Equal((640, 480), (loaded["a.jpg"].Width, loaded["a.jpg"].Height));
                Assert.Equal(0.5, loaded["a.jpg"].Blurring, 10);
                Assert.Equal(1.25, loaded["a.jpg"].Blockiness, 10);
                Assert.Equal((800, 600), (loaded["b.jpg"].Width, loaded["b.jpg"].Height));
                Assert.Equal(0.0, loaded["b.jpg"].Blurring, 10);

                // Регистр пути не имеет значения.
                Assert.Equal(640, loaded["A.JPG"].Width);
            }

            // ClearAll стирает и статистику.
            using (var cache = new FeatureCache(dir))
            {
                cache.ClearAll();
                Assert.False(cache.TryLoadFileStats(out _));
            }
        }
        finally
        {
            try { Directory.Delete(dir, recursive: true); } catch { }
        }
    }

    // ---------------- Clustering ----------------

    [Fact]
    public void Clustering_groupsCloseVectors_separatesFarOnes()
    {
        // a и b почти одинаковы (угол ~4°), c — перпендикулярен.
        float Near(float deg) => (float)Math.Tan(deg * Math.PI / 180.0);
        var a = new float[] { 1f, 0f, 0f, 0f };
        var b = Normalize(new float[] { 1f, Near(4f), 0f, 0f });
        var c = new float[] { 0f, 1f, 0f, 0f };

        var labels = AgglomerativeClustering.FitPredict(new[] { a, b, c }, distanceThreshold: 0.2f);

        Assert.Equal(labels[0], labels[1]);
        Assert.NotEqual(labels[0], labels[2]);
    }

    private static float[] Normalize(float[] v)
    {
        double n = 0;
        foreach (var x in v) n += x * x;
        n = Math.Sqrt(n);
        return v.Select(x => (float)(x / n)).ToArray();
    }

    // ---------------- UnionFind ----------------

    [Fact]
    public void UnionFind_mergesSets_andReportsSizes()
    {
        var uf = new UnionFind(6);
        uf.Union(0, 1);
        uf.Union(1, 2);
        uf.Union(3, 4);

        Assert.Equal(uf.Find(0), uf.Find(2));
        Assert.NotEqual(uf.Find(0), uf.Find(3));
        Assert.Equal(3, uf.Size(2));
        Assert.Equal(2, uf.Size(4));
        Assert.Equal(1, uf.Size(5));

        var labels = uf.GetLabels();
        Assert.Equal(labels[0], labels[2]);
        Assert.NotEqual(labels[0], labels[3]);
    }

    // ---------------- ProductQuantizer ----------------

    [Fact]
    public void ProductQuantizer_topK_recoversQueryItself()
    {
        const int dim = 16;
        var rnd = new Random(42);
        float[] RandomVec()
        {
            var v = new float[dim];
            for (int i = 0; i < dim; i++) v[i] = (float)rnd.NextDouble();
            return v;
        }

        var db = new List<float[]>();
        for (int i = 0; i < 300; i++) db.Add(RandomVec());

        var pq = new ProductQuantizer(dim, subvectorCount: 4, centroidsPerSubspace: 256);
        pq.Train(db.ToArray(), maxIterations: 50, seed: 42);

        var query = db[7]; // ищем вектор, который уже в БД
        var encoded = pq.Encode(db.ToArray());
        var results = pq.Search(encoded, query, topK: 5);

        Assert.Equal(5, results.Count);
        Assert.True(results.Any(r => r.index == 7),
            $"Сам запрос не попал в top-K: [{string.Join(", ", results.Select(r => r.index))}]");
        var self = results.First(r => r.index == 7);
        Assert.True(self.distance < 0.05f,
            $"Расстояние до себя должно быть ~0 (мелкое квантование), получено {self.distance}");
        Assert.True(results.All(r => r.distance >= self.distance - 1e-6f),
            "Сам запрос должен быть ближайшим из top-K");
    }
}