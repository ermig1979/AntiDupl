using System;

namespace AntiDupl.NET.Deep.Models;

/// <summary>
/// Параметры deep-поиска (перенос SearchSettings референс-проекта).
/// Каталог/фильтры задаёт ядро AntiDupl.dll (ScanCandidates), здесь — только параметры ML-пайплайна.
/// </summary>
public class DeepSettings
{
    /// <summary>
    /// Порог расстояния (1 − косинусное сходство) для объявления изображений дубликатами.
    /// WPF передаёт пороговую разность в процентах: 45% → 0.45 (дефолт референс-проекта).
    /// </summary>
    public float DistanceThreshold { get; set; } = 0.45f;

    /// <summary>Включена ли геометрическая верификация пар (SuperPoint + LightGlue).</summary>
    /// <summary>Верификация пар SuperPoint+LightGlue. Дефолт true — как в оригинале (SearchSettings).</summary>
    public bool GeometricCheckEnabled { get; set; } = true;

    /// <summary>Минимальное сходство пары для включения в верифицированную группу (аналог MinSimilarityForPair).</summary>
    public float MinSimilarityForPair { get; set; } = 0.5f;

    /// <summary>Минимальная средняя уверенность совпадений LightGlue (аналог MinMatchScore).</summary>
    public float MinMatchScore { get; set; } = 0.6f;

    /// <summary>Размер батча DINOv2-инференса.</summary>
    public int BatchSize { get; set; } = 32;

    /// <summary>Глубина очереди предзагрузки батчей (0 — последовательно).</summary>
    public int PrefetchCount { get; set; } = 2;

    /// <summary>Максимальный размер кластера; большие кластеры распускаются на одиночные метки.</summary>
    public int MaxClusterSize { get; set; } = 50;

    /// <summary>Доля порога, ниже которой связь считается «сильной» (проходит транзитивно).</summary>
    public float TransitivityRatio { get; set; } = 0.7f;

    /// <summary>Параллелизм верификации пар.</summary>
    /// <summary>Параллелизм WGC-верификации (0 = авто: половина процессоров).</summary>
    public int WgcParallelism { get; set; } = Math.Max(1, Environment.ProcessorCount / 2);

    /// <summary>
    /// Полный путь к модели DINOv2-base (resolves хостом: DeepModelManager.
    /// ResolveDinov2Path — кастомный путь из Configuration или дефолтный models-каталог).
    /// Если файла нет — поиск не запускается (см. DeepSearchService.GetExtractor).
    /// </summary>
    public string? Dinov2ModelPath { get; set; }

    /// <summary>
    /// Полный путь к сжатой модели SuperPoint+LightGlue (resolves хостом).
    /// Пусто/нет файла — верификация пропускается (режим «без верификации»). Задаётся
    /// только при включённой верификации; скачивание при отсутствии — обязанность хоста.
    /// </summary>
    public string? LightGlueModelPath { get; set; }
}