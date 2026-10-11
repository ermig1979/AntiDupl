using System;
using System.Diagnostics;

namespace AntiDupl.NET.WPF.Service
{
    /// <summary>
    /// Запуск внешней diff-утилиты для пары изображений (зеркало WF ImageDiffOpener).
    /// Конкретный класс без интерфейса — политика команды (см. план).
    /// </summary>
    public class ExternalToolLauncher
    {
        /// <summary>
        /// Запускает утилиту с плейсхолдерами %1 (первый файл) / %2 (второй файл).
        /// </summary>
        /// <returns>null при успехе; текст ошибки при неудаче.</returns>
        public string OpenFiles(string filePathFirstImage, string filePathSecondImage,
            string executablePath, string executableArguments)
        {
            if (String.IsNullOrEmpty(executablePath))
                return "External tool path is not set";

            try
            {
                string arguments = executableArguments ?? "";
                arguments = arguments.Replace("%1", filePathFirstImage)
                                    .Replace("%2", filePathSecondImage);

                ProcessStartInfo startInfo = new ProcessStartInfo();
                startInfo.FileName = executablePath;
                startInfo.Arguments = arguments;
                startInfo.WindowStyle = ProcessWindowStyle.Maximized;
                startInfo.UseShellExecute = false;

                Process.Start(startInfo);
                return null;
            }
            catch (Exception ex)
            {
                return ex.Message;
            }
        }
    }
}