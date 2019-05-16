using System;
using System.Linq;
using System.Windows;
using System.Windows.Input;
using AntiDuplWPF.Command;

namespace AntiDuplWPF.Service
{
    public enum IconSet
    {
        WindowsMetro,
        Windows10,
        Color,
    }

    public interface IIconSetThemeService
    {
        ICommand ChangeIconSetCmd { get; }
    }

    public sealed class IconSetThemeService : IIconSetThemeService
    {
        public ICommand ChangeIconSetCmd { get; private set; }

        public IconSetThemeService()
        {
            ChangeIconSetCmd = new RelayCommand(ChangeIconSet_Execute);
        }

        private void ChangeIconSet_Execute(object obj)
        {
            var newSet = (IconSet) obj;

            var dict = new ResourceDictionary {Source = new Uri(string.Format("Themes/{0}.xaml", newSet.ToString()), UriKind.Relative)};
            var position = 0;

            if (Application.Current.Resources.MergedDictionaries.Count > 0)
            {
                var oldDict = Application.Current.Resources.MergedDictionaries
                    .FirstOrDefault(d => d.Source != null && d.Source.OriginalString.Contains("Themes/"));

                if (oldDict != null)
                {
                    position = Application.Current.Resources.MergedDictionaries.IndexOf(oldDict);
                    Application.Current.Resources.MergedDictionaries.Remove(oldDict);
                }
            }

            Application.Current.Resources.MergedDictionaries.Insert(position, dict);
        }
    }
}