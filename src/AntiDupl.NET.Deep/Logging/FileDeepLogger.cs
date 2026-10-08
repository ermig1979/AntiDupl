using System.IO;

namespace AntiDupl.NET.Deep.Logging;

/// <summary>
/// Реализация <see cref="IDeepLogger"/>: файл <c>deep.log</c> в каталоге, который
/// задаёт хост (WPF — общий <c>logs</c>-каталог рядом с программой; тесты — свой каталог).
/// Формат строк и потокобезопасность — 1:1 как у legacy-статического <c>DeepLog</c>,
/// только экземплярный и вне «entities»-слоя.
/// </summary>
public sealed class FileDeepLogger : IDeepLogger
{
    private readonly object _lock = new();
    private string _directory;

    public FileDeepLogger(string directory = "")
    {
        _directory = directory ?? "";
        if (_directory.Length > 0)
        {
            try { Directory.CreateDirectory(_directory); } catch { }
        }
    }

    public string LogDirectory
    {
        get { lock (_lock) { return _directory; } }
    }

    public string LogFilePath
    {
        get { lock (_lock) { return _directory.Length > 0 ? Path.Combine(_directory, "deep.log") : ""; } }
    }

    public void Write(string line)
    {
        lock (_lock)
        {
            System.Diagnostics.Debug.WriteLine(line);
            if (_directory.Length == 0) return;
            try { File.AppendAllText(LogFilePath, $"[{DateTime.Now:HH:mm:ss.fff}] {line}\n"); }
            catch { }
        }
    }

    public void Clear()
    {
        lock (_lock)
        {
            if (_directory.Length == 0) return;
            try { File.WriteAllText(LogFilePath, ""); } catch { }
        }
    }
}