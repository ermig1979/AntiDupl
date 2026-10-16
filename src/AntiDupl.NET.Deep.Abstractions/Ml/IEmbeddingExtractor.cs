using AntiDupl.NET.Deep.Models;

namespace AntiDupl.NET.Deep.Ml;

/// <summary>
/// Порт экстрактора эмбеддингов (DINOv2): use case работает с интерфейсом,
/// конкретный ONNX-раннер — в AntiDupl.NET.Onnx (юнит-тесты возможны с фейком).
/// </summary>
public interface IEmbeddingExtractor : IDisposable
{
    /// <summary>Размер батча инференса (≥1).</summary>
    int BatchSize { get; set; }

    /// <summary>Глубина предзагрузки батчей (декодирование параллельно инференсу); 0 — последовательно.</summary>
    int PrefetchCount { get; set; }

    /// <summary>Задаёт источник прогресса (может вызываться из фоновых потоков).</summary>
    void SetProgress(IProgress<DeepProgress>? progress);

    /// <summary>
    /// Эмбеддинги для списка путей (кэш + mtime-валидация внутри). Позиция i результата
    /// соответствует пути paths[i]; недочищаемый файл — нулевой вектор.
    /// </summary>
    float[][] EmbedImagesBatch(List<string> paths, CancellationToken ct = default);
}