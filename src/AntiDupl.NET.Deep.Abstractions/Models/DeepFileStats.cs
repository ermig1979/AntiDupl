namespace AntiDupl.NET.Deep.Models;

/// <summary>
/// Пер-файловая статистика, полученная при декодировании в ходе deep-поиска:
/// нативное разрешение (стадия DINOv2, adLoadBitmapExW) и метрики blurring/blockiness
/// (стадия WGC, adLoadBitmapLetterboxMetricsW). Декоды — побочный продукт уже сделанных
/// вызовов ядра, дополнительного чтения файлов нет.
///
/// Персистится в feature-кэше (deep_filestats.dat), чтобы при повторном поиске
/// (декоды идут из кэша, ядро не вызывается) поля результатов всё равно заполнялись.
/// Нули означают «данные этой части не собраны» (например, файл проходил только
/// DINOv2-стадию и не имел разрешения в WGC-канвасе — метрики отсутствуют).
/// </summary>
public sealed class DeepFileStats
{
    public int Width;
    public int Height;
    public double Blurring;
    public double Blockiness;
}