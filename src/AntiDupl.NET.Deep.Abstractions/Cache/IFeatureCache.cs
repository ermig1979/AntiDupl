namespace AntiDupl.NET.Deep.Cache;

using AntiDupl.NET.Deep.Models;

/// <summary>
/// Кэш эмбеддингов DINOv2, вердиктов геометрической верификации и пер-файловой
/// статистики (разрешение, blurring/blockiness).
/// Перенос IFeatureCache референс-проекта (без SIFT-части — см. DD11).
///
/// Хранение: на диске, рядом с программой (&lt;BaseDirectory&gt;\deep) — три бинарных
/// файла: <c>deep_embeddings.dat</c>, <c>deep_wgc.dat</c>, <c>deep_filestats.dat</c>.
/// Рабочие данные (эмбеддинги, графы, PQ-таблицы) живут в оперативной памяти и
/// пишутся на диск только при Save()/Dispose.
///
/// Ключ — путь (регистронезависимо), невалидация — по mtime (толеранс 0.01 с,
/// как в референсе) + размеру файла (ключ: (path, size, mtime)).
/// </summary>
public interface IFeatureCache : IDisposable
{
    /// <returns>Кэшированный эмбеддинг, если (path, size, mtime) совпали; иначе null.</returns>
    (double mtime, float[] embedding)? GetEmbedding(string path);

    void SetEmbedding(string path, double mtime, float[] embedding);

    /// <summary>
    /// Кэшированный вердикт WGC, если (path1, path2, mtime) совпали.
    /// <paramref name="minMatchScore"/> входит в ключ: вердикт зависит от порога
    /// (критерий PASS: avgScore ≥ minMatchScore), поэтому один и тот же порог
    /// возвращает записанный вердикт, а другой — промах и пересчёт.
    /// </summary>
    (bool result, float angle, float scale, int angleVotes, int scaleVotes)? GetWgc(
        string path1, string path2, double mtime1, double mtime2, float minMatchScore);

    void SetWgc(string path1, string path2, double mtime1, double mtime2, float minMatchScore,
        bool result, float angle, float scale, int angleVotes, int scaleVotes);

    /// <summary>Очистка всех записей. Возвращает освобождённый размер файлов (байт).</summary>
    long ClearAll();

    /// <summary>Число эмбеддингов в кэше.</summary>
    long GetEmbeddingCount();

    /// <summary>
    /// Сохраняет пер-файловую статистику (deep_filestats.dat) и держит её в памяти.
    /// Служит тому, чтобы при повторном поиске (декоды из кэша) результаты всё равно
    /// несли разрешение/метрики, собранные в предыдущих (холодных) прогонах.
    /// </summary>
    void SaveFileStats(IReadOnlyDictionary<string, DeepFileStats> stats);

    /// <summary>Загружает пер-файловую статистику из кэша; false, если файла нет.</summary>
    bool TryLoadFileStats(out IReadOnlyDictionary<string, DeepFileStats> stats);

    /// <summary>Сразу записывает накопленные изменения на диск.</summary>
    void Save();
}