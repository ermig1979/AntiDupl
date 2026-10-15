using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using AntiDupl.NET.Deep.Cache;
using AntiDupl.NET.Deep.Image;
using AntiDupl.NET.Deep.Logging;
using AntiDupl.NET.Deep.Models;
using AntiDupl.NET.Deep.Ml;
using AntiDupl.NET.Onnx;
using Microsoft.ML.OnnxRuntime;
using Microsoft.ML.OnnxRuntime.Tensors;

namespace AntiDupl.NET.Onnx.Ml;

/// <summary>
/// Экстрактор эмбеддингов DINOv2-base (перенос EmbeddingExtractor из DinoDuplicateSearch.ML).
///
/// Контракт идентичен исходнику: вход pixel_values (N,3,224,224) float без mean/std,
/// вывод last_hidden_state (N, L, 768), CLS-токен = [b, 0, :] (берём напрямую
/// из тензора — хрупкий flat-offset b*stride из исходника брал dims[2]=768 (feature-dim)
/// вместо L=197/201, из-за чего в батчах &gt;1 все слоты кроме 0-го получали обрывки
/// первого файла, а не CLS своей картинки). Ручная L2-нормализация.
///
/// Отличие: вместо Cv2.ImRead/CvtColor/Resize — <see cref="IDeepImageSource"/>
/// (ядро AntiDupl.dll, adLoadBitmapW: decode + SIMD-resize до 224×224 в BGRA).
/// </summary>
public class EmbeddingExtractor : IEmbeddingExtractor, IDisposable
{
    private readonly string _modelPath;
    private OnnxSession? _session;
    private readonly IFeatureCache _cache;
    private readonly IDeepImageSource _imageSource;
    private readonly IDeepLogger _logger;
    private IProgress<DeepProgress>? _progress;
    private bool _useGpu;
    private readonly Action<string>? _gpuFallbackNotification;
    private int _batchSize = 32;
    public int BatchSize
    {
        get => _batchSize;
        set => _batchSize = Math.Max(1, value);
    }

    private int _prefetchCount = 2;
    public int PrefetchCount
    {
        get => _prefetchCount;
        set => _prefetchCount = Math.Max(0, value);
    }

    public EmbeddingExtractor(string modelPath, IFeatureCache cache, IDeepImageSource imageSource,
        IDeepLogger logger, Action<string>? gpuFallbackNotification = null)
    {
        _modelPath = modelPath ?? throw new ArgumentNullException(nameof(modelPath));
        _cache = cache ?? throw new ArgumentNullException(nameof(cache));
        _imageSource = imageSource ?? throw new ArgumentNullException(nameof(imageSource));
        _logger = logger ?? throw new ArgumentNullException(nameof(logger));
        _gpuFallbackNotification = gpuFallbackNotification;
    }

    public void SetProgress(IProgress<DeepProgress>? progress)
    {
        _progress = progress;
    }

    private void LoadModel()
    {
        if (_session != null) return;
        _progress?.Report(new DeepProgress(0, "Loading ONNX model..."));
        // Сессия создаётся единой фабрикой: надёжный GPU→CPU fallback (в ORT 1.21
        // отсутствие CUDA бросает на CreateSession, а не на Append) + уведомление хоста.
        var session = OnnxSessionFactory.CreateSession(_modelPath, _logger, _gpuFallbackNotification);
        _session = session;
        _useGpu = session.IsGpu;
        if (_useGpu)
        {
            _progress?.Report(new DeepProgress(5, "Using GPU (CUDA)..."));
            _logger.Write("[MODEL] GPU (CUDA) available, using GPU");
        }
        else
        {
            _progress?.Report(new DeepProgress(5, "GPU unavailable (install cuDNN for CUDA), using CPU..."));
            _logger.Write("[MODEL] GPU unavailable, using CPU");
        }

        // Фиксируем реальное число токенов (dims[1]; 197 без / 201 с register tokens)
        // только для диагностики — сам CLS берём из тензора как [b, 0, :], что не зависит
        // от stride вообще. (Старый код исходника брал dims[2] = 768 = feature-dim как
        // "seq_len" — при батче >1 это смещало CLS-обрезки во все последующие слоты.)
        var outputMeta = _session.Session.OutputMetadata["last_hidden_state"];
        var dims = outputMeta.Dimensions;
        _logger.Write($"[MODEL] Outputs: {string.Join(", ", _session.Session.OutputMetadata.Keys)}");
        _logger.Write($"[MODEL] Output last_hidden_state dims: {string.Join("x", dims.Select(d => d >= 0 ? d.ToString() : "?"))}");

        _progress?.Report(new DeepProgress(100, "Model loaded"));
    }

    public float[] EmbedImage(string path, CancellationToken ct = default)
    {
        var results = EmbedImagesBatch(new List<string> { path }, ct);
        return results[0];
    }

    private readonly record struct PreparedBatch(
        int BatchStart,
        int TotalToEmbed,
        List<(int Index, string Path, double Mtime)> Meta,
        DenseTensor<float>? Tensor);

    public float[][] EmbedImagesBatch(List<string> paths, CancellationToken ct = default)
    {
        LoadModel();

        var result = new float[paths.Count][];
        var toEmbed = new List<(int index, string path)>();

        for (int i = 0; i < paths.Count; i++)
        {
            var currentMtime = 0.0;
            try { currentMtime = File.GetLastWriteTimeUtc(paths[i]).Ticks / (double)TimeSpan.TicksPerSecond; }
            catch { }

            var cached = _cache.GetEmbedding(paths[i]);
            if (cached.HasValue && Math.Abs(cached.Value.mtime - currentMtime) < 0.01)
            {
                result[i] = cached.Value.embedding;
            }
            else
            {
                toEmbed.Add((i, paths[i]));
            }
        }

        if (toEmbed.Count == 0)
            return result;

        if (PrefetchCount == 0)
        {
            ProcessBatchesSequential(toEmbed, result, ct);
            return result;
        }

        var queue = new BlockingCollection<PreparedBatch>(PrefetchCount);
        var totalBatches = (toEmbed.Count + BatchSize - 1) / BatchSize;

        var producer = Task.Run(() =>
        {
            try
            {
                for (int batchStart = 0; batchStart < toEmbed.Count; batchStart += BatchSize)
                {
                    ct.ThrowIfCancellationRequested();
                    var batch = toEmbed.Skip(batchStart).Take(BatchSize).ToList();
                    var prepared = new (int index, string path, double mtime, float[,,]? tensor)[batch.Count];

                    Parallel.For(0, batch.Count, new ParallelOptions { CancellationToken = ct }, b =>
                    {
                        var (index, path) = batch[b];
                        var mtime = 0.0;
                        try { mtime = File.GetLastWriteTimeUtc(path).Ticks / (double)TimeSpan.TicksPerSecond; }
                        catch { }

                        // Декодирование через ядро (adLoadBitmapW): 224×224 RGB CHW, 0..1.
                        // null — файл не декодировался (в референсе: mat.Empty()).
                        var tensor = _imageSource.DecodeDinov2Input(path);
                        if (tensor == null)
                        {
                            prepared[b] = (index, path, mtime, null);
                            return;
                        }

                        prepared[b] = (index, path, mtime, tensor);
                    });

                    int validCount = prepared.Count(p => p.tensor != null);
                    if (validCount == 0)
                    {
                        foreach (var (index, _, _, _) in prepared)
                            result[index] = new float[768];
                        continue;
                    }

                    var batchTensor = new DenseTensor<float>(new[] { validCount, 3, 224, 224 });
                    var batchMeta = new List<(int index, string path, double mtime)>();
                    int slot = 0;

                    foreach (var (index, path, mtime, tensor) in prepared)
                    {
                        if (tensor == null)
                        {
                            result[index] = new float[768];
                            continue;
                        }

                        for (int c = 0; c < 3; c++)
                            for (int y = 0; y < 224; y++)
                                for (int x = 0; x < 224; x++)
                                    batchTensor[slot, c, y, x] = tensor[c, y, x];

                    batchMeta.Add((index, path, mtime));
                        slot++;
                    }

                    // Слот тензора и метка файла должны быть в строгом соответствии,
                    // иначе эмбеддинг запишется чужому файлу.
                    Debug.Assert(slot == validCount,
                        $"slot ({slot}) must equal valid tensor count ({validCount})");

                    queue.Add(new PreparedBatch(batchStart, toEmbed.Count, batchMeta, batchTensor));
                }
            }
            finally
            {
                queue.CompleteAdding();
            }
        }, ct);

        foreach (var batch in queue.GetConsumingEnumerable(ct))
        {
            var pct = 5 + (int)((double)batch.BatchStart / batch.TotalToEmbed * 35);
            var batchNum = batch.BatchStart / BatchSize + 1;
            _progress?.Report(new DeepProgress(pct, _useGpu
                ? $"Batch {batchNum}/{totalBatches} on GPU ({batch.Meta.Count})..."
                : $"Batch {batchNum}/{totalBatches} on CPU ({batch.Meta.Count})..."));

            var inputs = new List<NamedOnnxValue>
            {
                NamedOnnxValue.CreateFromTensor("pixel_values", batch.Tensor!)
            };

            using var gpuResults = _session!.Session.Run(inputs);
            // Модель может иметь несколько выходов (last_hidden_state + pooler_output и т.п.) —
            // берём по имени, а не по порядку/количеству.
            Debug.Assert(gpuResults.Any(r => r.Name == "last_hidden_state"),
                "DINOv2 model must output last_hidden_state");
            var outTensor = gpuResults.First(r => r.Name == "last_hidden_state").AsTensor<float>();
            Debug.Assert(outTensor.Dimensions.Length == 3 && outTensor.Dimensions[2] == 768,
                "last_hidden_state must be (N, L, 768) — check dinov2-base.onnx is not replaced");

            for (int b = 0; b < batch.Meta.Count; b++)
            {
                var embedding = new float[768];
                // CLS-токен = [b, 0, :] — напрямую из тензора (flat-offset уязвим к stride-багу).
                for (int i = 0; i < 768; i++) embedding[i] = outTensor[b, 0, i];

                float norm = 0;
                for (int i = 0; i < embedding.Length; i++) norm += embedding[i] * embedding[i];
                norm = MathF.Sqrt(norm);
                if (norm > 0) for (int i = 0; i < embedding.Length; i++) embedding[i] /= norm;

                var (index, path, mtime) = batch.Meta[b];
                result[index] = embedding;

                try { _cache.SetEmbedding(path, mtime, embedding); }
                catch { }
            }
        }

        // Каждый файл обязан получить результат: из кэша, от модели или нулевой
        // вектор при сбое декода — иначе ниже NRE/мусорные пары.
        Debug.Assert(result.All(r => r is not null),
            "every file must have an embedding result (cache, model output or zero vector)");

        producer.Wait();
        return result;
    }

    private void ProcessBatchesSequential(List<(int index, string path)> toEmbed, float[][] result, CancellationToken ct)
    {
        for (int batchStart = 0; batchStart < toEmbed.Count; batchStart += BatchSize)
        {
            ct.ThrowIfCancellationRequested();
            var batch = toEmbed.Skip(batchStart).Take(BatchSize).ToList();
            var prepared = new (int index, string path, double mtime, float[,,]? tensor)[batch.Count];

            Parallel.For(0, batch.Count, new ParallelOptions { CancellationToken = ct }, b =>
            {
                var (index, path) = batch[b];
                var mtime = 0.0;
                try { mtime = File.GetLastWriteTimeUtc(path).Ticks / (double)TimeSpan.TicksPerSecond; }
                catch { }

                var tensor = _imageSource.DecodeDinov2Input(path);
                prepared[b] = (index, path, mtime, tensor);
            });

            int validCount = prepared.Count(p => p.tensor != null);
            if (validCount == 0)
            {
                foreach (var (index, _, _, _) in prepared)
                    result[index] = new float[768];
                continue;
            }

            var batchTensor = new DenseTensor<float>(new[] { validCount, 3, 224, 224 });
            var batchMeta = new List<(int index, string path, double mtime)>();
            int slot = 0;

            foreach (var (index, path, mtime, tensor) in prepared)
            {
                if (tensor == null)
                {
                    result[index] = new float[768];
                    continue;
                }

                for (int c = 0; c < 3; c++)
                    for (int y = 0; y < 224; y++)
                        for (int x = 0; x < 224; x++)
                            batchTensor[slot, c, y, x] = tensor[c, y, x];

                batchMeta.Add((index, path, mtime));
                slot++;
            }

            // Слот тензора и метка файла должны быть в строгом соответствии,
            // иначе эмбеддинг запишется чужому файлу.
            Debug.Assert(slot == validCount,
                $"slot ({slot}) must equal valid tensor count ({validCount})");

            var inputs = new List<NamedOnnxValue>
            {
                NamedOnnxValue.CreateFromTensor("pixel_values", batchTensor)
            };

            var pct = 5 + (int)((double)batchStart / toEmbed.Count * 35);
            var batchNum = batchStart / BatchSize + 1;
            var totalBatches = (toEmbed.Count + BatchSize - 1) / BatchSize;
            _progress?.Report(new DeepProgress(pct, _useGpu
                ? $"Batch {batchNum}/{totalBatches} on GPU ({validCount})..."
                : $"Batch {batchNum}/{totalBatches} on CPU ({validCount})..."));

            using var gpuResults = _session!.Session.Run(inputs);
            // Модель может иметь несколько выходов (last_hidden_state + pooler_output и т.п.) —
            // берём по имени, а не по порядку/количеству.
            Debug.Assert(gpuResults.Any(r => r.Name == "last_hidden_state"),
                "DINOv2 model must output last_hidden_state");
            var outTensor = gpuResults.First(r => r.Name == "last_hidden_state").AsTensor<float>();
            Debug.Assert(outTensor.Dimensions.Length == 3 && outTensor.Dimensions[2] == 768,
                "last_hidden_state must be (N, L, 768) — check dinov2-base.onnx is not replaced");

            for (int b = 0; b < batchMeta.Count; b++)
            {
                var embedding = new float[768];
                // CLS-токен = [b, 0, :] — напрямую из тензора.
                for (int i = 0; i < 768; i++) embedding[i] = outTensor[b, 0, i];

                float norm = 0;
                for (int i = 0; i < embedding.Length; i++) norm += embedding[i] * embedding[i];
                norm = MathF.Sqrt(norm);
                if (norm > 0) for (int i = 0; i < embedding.Length; i++) embedding[i] /= norm;

                var (index, path, mtime) = batchMeta[b];
                result[index] = embedding;

                try { _cache.SetEmbedding(path, mtime, embedding); }
                catch { }
            }
        }
    }

    public void Dispose()
    {
        _session?.Dispose();
    }
}