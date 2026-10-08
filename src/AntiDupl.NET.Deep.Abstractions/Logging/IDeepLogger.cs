namespace AntiDupl.NET.Deep.Logging;

/// <summary>
/// Порт логирования deep-пайплайна (заменяет статический <c>DeepLog</c> из legacy-переноса).
/// Реализация хоста — <c>FileDeepLogger</c>
/// (AntiDupl.NET.Deep), регистрируется в DI-цепочке <c>AddDeepSearch</c>.
/// </summary>
public interface IDeepLogger
{
    /// <summary>Каталог, в котором лежит файл лога (пустая строка, если каталог не задан).</summary>
    string LogDirectory { get; }

    /// <summary>Полный путь к файлу лога ("" если каталог не задан).</summary>
    string LogFilePath { get; }

    /// <summary>Записывает строку в лог (формат и поведение — как у legacy <c>DeepLog.Write</c>).</summary>
    void Write(string line);

    /// <summary>Очищает файл лога (новый запуск приложения).</summary>
    void Clear();
}