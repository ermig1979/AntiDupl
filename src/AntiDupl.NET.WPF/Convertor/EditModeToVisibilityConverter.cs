using System;
using System.Globalization;
using System.Windows;
using System.Windows.Data;
using AntiDupl.NET.WPF.Control;

namespace AntiDupl.NET.WPF.Convertor
{
    /// <summary>
    /// Видимость по режиму RenameTextBox:
    /// parameter "box" — сам TextBox: Edit → Visible, иначе Hidden (ложится под diff-оверлей);
    /// иначе (оверлей DiffTextBlock): Edit → Collapsed, иначе Visible.
    /// </summary>
    public class EditModeToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            bool edit = value is TextBoxMode mode && mode == TextBoxMode.Edit;
            if ("box".Equals(parameter as string, StringComparison.OrdinalIgnoreCase))
                return edit ? Visibility.Visible : Visibility.Hidden;

            return edit ? Visibility.Collapsed : Visibility.Visible;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotSupportedException();
        }
    }
}