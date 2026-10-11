using System;
using System.Windows;

namespace AntiDupl.NET.WPF.Service
{
    /// <summary>
    /// Обёртка над Clipboard.SetText (). Конкретный класс без интерфейса
    /// (по CA-политике WPF-слоя). Ошибки доступа к буферу (буфер занят другим
    /// процессом) не роняют приложение.
    /// </summary>
    public class ClipboardService
    {
        /// <summary>Копирует текст в буфер обмена; пустой текст игнорируется.</summary>
        public void CopyText(string text)
        {
            if (string.IsNullOrEmpty(text))
                return;
            try
            {
                Clipboard.SetText(text, TextDataFormat.Text);
            }
            catch (Exception)
            {
                // Буфер временно занят другим приложением — молча пропускаем.
            }
        }
    }
}