using System.Windows;

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
            string template = (Application.Current != null &&
                               Application.Current.TryFindResource(key) is string s)
                ? s
                : key;

            return args != null && args.Length > 0 ? string.Format(template, args) : template;
        }
    }
}