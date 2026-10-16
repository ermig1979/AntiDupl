namespace AntiDupl.NET.Deep.Models;

/// <summary>
/// Пара дубликатов, найденная deep-поиском (аналог DuplicatePair референс-проекта).
/// </summary>
public class DeepPair
{
    public string Path1 { get; set; } = "";
    public string Path2 { get; set; } = "";

    /// <summary>Косинусное сходство эмбеддингов DINOv2 (0..1).</summary>
    public float Similarity { get; set; }

    /// <summary>Прошла пара геометрическую верификацию (SuperPoint + LightGlue).</summary>
    public bool GeometricVerified { get; set; }

    public float GeometricAngle { get; set; }
    public int GeometricAngleVotes { get; set; }
    public float GeometricScale { get; set; }
    /// <summary>Средняя уверенность совпадений LightGlue, % (5-й элемент кортежа).</summary>
    public int GeometricScaleVotes { get; set; }

    /// <summary>
    /// Номер группы (максимального клика WGC-графа), к которой отнесена пара при поиске.
    /// Одна пара может входить в несколько групп — тогда она представлена несколькими
    /// записями с разными номерами (как в референсе: файл может быть в нескольких группах).
    /// Без верификации — номер кластера. -1 = номер не присвоен.
    /// </summary>
    public int GroupId { get; set; } = -1;

    // ---------- Статистика файлов (побочный продукт декодирования ядра, без доп. чтения) ---

    /// <summary>Размер файла 1, байт (из скана ядра adScanCandidatesW).</summary>
    public long Size1 { get; set; }
    /// <summary>Размер файла 2, байт (из скана ядра adScanCandidatesW).</summary>
    public long Size2 { get; set; }

    /// <summary>Время изменения файла 1 — сырой FILETIME (из скана ядра adScanCandidatesW); 0 = неизвестно.</summary>
    public long Time1 { get; set; }
    /// <summary>Время изменения файла 2 — сырой FILETIME (из скана ядра adScanCandidatesW); 0 = неизвестно.</summary>
    public long Time2 { get; set; }

    /// <summary>Нативные размеры файла 1 (до ресайза), из adLoadBitmapExW; 0 = неизвестно.</summary>
    public int Width1 { get; set; }
    public int Height1 { get; set; }
    /// <summary>Нативные размеры файла 2 (до ресайза), из adLoadBitmapExW; 0 = неизвестно.</summary>
    public int Width2 { get; set; }
    public int Height2 { get; set; }

    /// <summary>Метрики качества файла 1 (blurring/blockiness, алгоритмы классического пайплайна,
    /// adLoadBitmapLetterboxMetricsW); 0 = недоступно (WGC отключён/старое ядро/файл не декодировался).</summary>
    public double Bluring1 { get; set; }
    public double Blockiness1 { get; set; }
    /// <summary>Метрики качества файла 2 (см. <see cref="Bluring1"/>).</summary>
    public double Bluring2 { get; set; }
    public double Blockiness2 { get; set; }
}