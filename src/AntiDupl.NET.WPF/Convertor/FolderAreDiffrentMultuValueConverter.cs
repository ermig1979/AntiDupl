using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;
using System.Windows.Media;


namespace AntiDuplWPF.Convertor
{
    public class FolderAreDiffrentMultuValueConverter : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            if (values[0] != DependencyProperty.UnsetValue && values[1] != DependencyProperty.UnsetValue)
            {
                bool folderAreDiffrent = (bool)values[0];
                Color color = (Color)values[1];
                if (folderAreDiffrent && color != null)
                {
                    return new SolidColorBrush(color);
                }
            }
            return new SolidColorBrush();
        }

        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
