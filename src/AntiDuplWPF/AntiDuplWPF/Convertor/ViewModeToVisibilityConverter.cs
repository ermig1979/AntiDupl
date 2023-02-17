using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;
using AntiDuplWPF.ObjectModel;

namespace AntiDuplWPF.Convertor
{
    [ValueConversion(typeof(ViewMode), typeof(Visibility))]
    public class ViewModeToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var par = (ViewMode)parameter;
            var viewMode = (ViewMode)value;
            if (viewMode == par)
            {
                return Visibility.Visible;
            }
            return Visibility.Collapsed;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
