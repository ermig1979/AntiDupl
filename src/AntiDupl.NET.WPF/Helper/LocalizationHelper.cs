using System.Windows;
using System.Diagnostics;

namespace AntiDupl.NET.WPF.Helper
{
    /// <summary>
    /// Запрос локализованной строки из активного словаря приложения
    /// (Resources/lang*.xaml) с форматированием аргументов.
    /// </summary>
    public static class LocalizationHelper
    {
        public static string S(string key, params object[] args)
        {
            // Пустой ключ — опечатка на вызывающей стороне: fallback к key её маскировал бы.
            Debug.Assert(!string.IsNullOrEmpty(key), nameof(key));
            string template = (Application.Current != null &&
                               Application.Current.TryFindResource(key) is string s)
                ? s
                : key;

            return args != null && args.Length > 0 ? string.Format(template, args) : template;
        }
    }
}