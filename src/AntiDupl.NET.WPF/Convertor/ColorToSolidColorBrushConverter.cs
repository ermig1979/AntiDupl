using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media;

namespace AntiDupl.NET.WPF.Convertor
{
    public sealed class ColorToSolidColorBrushConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (!(value is Color))
                return value;

            var brush = new SolidColorBrush((Color) value);
            brush.Freeze();

            return brush;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var brush = value as SolidColorBrush;
            if (brush == null)
                throw new Exception("Provided value is not brush");

            return brush.Color;
        }
    }
}