using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Media;

namespace AntiDupl.NET.WPF.Convertor
{
    public class DifferenceValueConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            double difference = (double)value;
            if (difference != null)
            {
               if (difference == 0.0)
                   return new SolidColorBrush(Colors.LightGreen);
            }

            return new SolidColorBrush();
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
