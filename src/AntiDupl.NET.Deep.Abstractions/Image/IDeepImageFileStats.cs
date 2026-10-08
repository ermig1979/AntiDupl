namespace AntiDupl.NET.Deep.Image;

using AntiDupl.NET.Deep.Models;

/// <summary>
/// Побочная статистика декодирования, собираемая источником растров во время deep-поиска.
///
/// Данные НЕ требуют повторного чтения файлов: ядро AntiDupl.dll и так декодирует
/// изображения (adLoadBitmapExW — стадия DINOv2, adLoadBitmapLetterboxMetricsW — стадия
/// WGC) и сообщает нативные размеры/метрики в том же вызове.
///
/// Доступна у реализации, которая знает ядро (CoreBitmapImageSource); сервис проверяет
/// <c>imageSource is IDeepImageFileStats</c>. Старое ядро без Ex-экспортов → источник
/// статистики не имеет → все Get* возвращают false.
///
/// Статистика не переживает запуск процесса, поэтому сервис персистит её в feature-кэше
/// (deep_filestats.dat): в начале поиска <see cref="SeedFileStats"/>, в конце —
/// <see cref="CollectFileStats"/>.
/// </summary>
public interface IDeepImageFileStats
{
    /// <summary>Очистка накопленной статистики (вызывается в начале поиска).</summary>
    void ClearFileStats();

    /// <summary>Предзаполняет статистику из кэша (после <see cref="ClearFileStats"/>,
    /// до декодирований; свежие декоды перезаписывают значения).</summary>
    void SeedFileStats(IReadOnlyDictionary<string, DeepFileStats> stats);

    /// <summary>Собирает накопленную статистику для персиста в кэш (в конце поиска).</summary>
    IReadOnlyDictionary<string, DeepFileStats> CollectFileStats();

    /// <summary>Нативные (до ресайза) размеры файла, если файл декодировался в ходе поиска.</summary>
    bool TryGetNativeSize(string path, out int width, out int height);

    /// <summary>Метрики качества файла (blurring/blockiness, алгоритмы классического пайплайна),
    /// если файл проходил WGC-декод с метриками в ходе поиска.</summary>
    bool TryGetFileMetrics(string path, out double blurring, out double blockiness);
}