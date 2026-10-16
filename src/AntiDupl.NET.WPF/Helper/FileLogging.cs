using System;
using System.Diagnostics;
using System.IO;
using Microsoft.Extensions.Logging;

namespace AntiDupl.NET.WPF.Helper
{
    /// <summary>
    /// Провайдер стандартного ILogger, пишущий в файл
    /// logs\AntiDupl_WPF_&lt;дата&gt;.log рядом с исполняемым файлом.
    /// </summary>
    public sealed class FileLoggerProvider : ILoggerProvider
    {
        private readonly object _lock = new object();
        private readonly string _logFilePath;
        private LogLevel _minimumLevel = LogLevel.Information;

        public FileLoggerProvider(string logDirectory)
        {
            if (string.IsNullOrWhiteSpace(logDirectory))
                logDirectory = AppDomain.CurrentDomain.BaseDirectory;
            try
            {
                Directory.CreateDirectory(logDirectory);
            }
            catch
            {
                logDirectory = AppDomain.CurrentDomain.BaseDirectory;
            }
            _logFilePath = Path.Combine(logDirectory,
                string.Format("AntiDupl_WPF_{0:yyyy-MM-dd}.log", DateTime.Now));
            Write(string.Format("===== {0:yyyy-MM-dd HH:mm:ss} AntiDupl.NET.WPF started, PID={1} =====",
                DateTime.Now, Process.GetCurrentProcess().Id));
        }

        public string LogFilePath
        {
            get { return _logFilePath; }
        }

        public ILogger CreateLogger(string categoryName)
        {
            return new FileLogger(this, categoryName);
        }

        public void SetMinimumLevel(LogLevel minimumLevel)
        {
            _minimumLevel = minimumLevel;
        }

        public LogLevel MinimumLevel
        {
            get { return _minimumLevel; }
        }

        internal void Write(string line)
        {
            lock (_lock)
            {
                try
                {
                    File.AppendAllText(_logFilePath, line + Environment.NewLine);
                }
                catch
                {
                    // Ошибка записи лога не должна ронять приложение.
                }
            }
        }

        public void Dispose()
        {
        }
    }

    public sealed class FileLogger : ILogger
    {
        private readonly FileLoggerProvider _provider;
        private readonly string _categoryName;

        public FileLogger(FileLoggerProvider provider, string categoryName)
        {
            _provider = provider;
            _categoryName = categoryName;
        }

        public IDisposable BeginScope<TState>(TState state)
        {
            return NullScope.Instance;
        }

        public bool IsEnabled(LogLevel logLevel)
        {
            return logLevel != LogLevel.None && logLevel >= _provider.MinimumLevel;
        }

        public void Log<TState>(LogLevel logLevel, EventId eventId, TState state,
            Exception exception, Func<TState, Exception, string> formatter)
        {
            if (!IsEnabled(logLevel))
                return;

            string line = string.Format("{0:yyyy-MM-dd HH:mm:ss.fff} [{1}] [{2}] [{3}] {4}",
                DateTime.Now,
                logLevel,
                ThreadHelper.CurrentThreadName(),
                _categoryName,
                formatter(state, exception));
            if (exception != null)
                line += Environment.NewLine + exception;
            _provider.Write(line);
        }

        private sealed class NullScope : IDisposable
        {
            public static readonly NullScope Instance = new NullScope();
            public void Dispose()
            {
            }
        }
    }

    /// <summary>
    /// Имя текущего потока для логов (UI / фоновые потоки).
    /// </summary>
    public static class ThreadHelper
    {
        public static string CurrentThreadName()
        {
            string name = System.Threading.Thread.CurrentThread.Name;
            if (!string.IsNullOrEmpty(name))
                return name;
            try
            {
                if (System.Windows.Application.Current != null &&
                    System.Windows.Application.Current.Dispatcher.Thread == System.Threading.Thread.CurrentThread)
                    return "UI";
            }
            catch
            {
            }
            return "bg";
        }
    }

    public static class FileLoggerExtensions
    {
        public static ILoggingBuilder AddFileLog(this ILoggingBuilder builder, string logDirectory)
        {
            builder.AddProvider(new FileLoggerProvider(logDirectory));
            return builder;
        }
    }
}