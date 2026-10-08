namespace AntiDupl.NET.Deep.Models;

/// <summary>
/// Прогресс deep-поиска (аналог ProgressData референс-проекта).
/// </summary>
/// <param name="Percent">Процент выполнения 0..100.</param>
/// <param name="Status">Человекочитаемый статус.</param>
/// <param name="FilePath">Файл/счётчик текущей операции (необязательно).</param>
public record DeepProgress(double Percent, string Status, string? FilePath = null);