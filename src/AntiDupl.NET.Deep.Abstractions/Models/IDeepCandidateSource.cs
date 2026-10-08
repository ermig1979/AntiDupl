namespace AntiDupl.NET.Deep.Models;

/// <summary>
/// Кандидатный файл для deep-поиска — минимальный POCO на границе use case/драйвера.
/// Путь используется пайплайном как основной ключ; <see cref="Size"/> (если драйвер его
/// сообщает — ядро AntiDupl.dll знает размер после скана adScanCandidatesW) передаётся
/// в результаты без повторного обращения к файловой системе. 0 = неизвестно.
/// </summary>
public sealed class DeepCandidate
{
    public DeepCandidate(string path)
        : this(path, 0)
    {
    }

    public DeepCandidate(string path, long size)
    {
        Path = path;
        Size = size;
    }

    public string Path { get; }

    /// <summary>Размер файла, байт (из скана ядра); 0 = драйвер не сообщает.</summary>
    public long Size { get; }
}

/// <summary>
/// Порт источника кандидатов: драйвер (ядро AntiDupl.dll) прогоняет фильтры хоста
/// (пути/подпапки, скрытые/системные, ignore/valid, форматы, размер) и возвращает список.
/// Use case (DeepSearchService) не знает о конкретном драйвере — только о порте.
/// </summary>
public interface IDeepCandidateSource
{
    /// <summary>Блокирующий скан — выполнять из фонового потока.</summary>
    IReadOnlyList<DeepCandidate> Scan();
}