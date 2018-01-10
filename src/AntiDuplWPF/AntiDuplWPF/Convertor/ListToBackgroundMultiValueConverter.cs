using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;
using System.Windows.Media;
using AntiDuplWPF.ObjectModel;

namespace AntiDuplWPF.Convertor
{
    public class ListToBackgroundMultiValueConverter : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            if (values[0] is IEnumerable
                //&& values[1] is uint
                && values[2] != DependencyProperty.UnsetValue
                && values[3] != DependencyProperty.UnsetValue
                && parameter != null)
            {
                System.Windows.Media.Color goodColor = (System.Windows.Media.Color)values[2];
                System.Windows.Media.Color badColor = (System.Windows.Media.Color)values[3];
                MaxProperty property = (MaxProperty)parameter;

                ulong maxUint = 0;
                double maxDouble = 0.0;
                decimal maxDecimal = 0;
                //uint min = uint.MaxValue;

                foreach (ImageInfoClass info in (IEnumerable)values[0])
                {
                    switch (property)
                    {
                        case MaxProperty.FileSize:
                            if (info.FileSize > maxUint)
                                maxUint = info.FileSize;
                            break;
                        case MaxProperty.JpegQuality:
                            if (info.JpegQuality > maxDouble)
                                maxDouble = info.JpegQuality;
                            break;
                        case MaxProperty.Sharpness:
                            if (info.Sharpness > maxDouble)
                                maxDouble = info.Sharpness;
                            break;
                        case MaxProperty.Resolution:
                            if (info.Width * info.Height > maxUint)
                                maxUint = info.Width * info.Height;
                            break;
                        case MaxProperty.UtilityIndex:
                            if (info.UtilityIndex > maxDecimal)
                                maxDecimal = info.UtilityIndex;
                            break;
                    }
                }

                switch (property)
                {
                    case MaxProperty.FileSize:
                        if ((uint)values[1] == maxUint
                            && maxUint != 0)
                            return new SolidColorBrush(goodColor);
                        break;
                    case MaxProperty.Resolution:
                        if ((uint)values[1] * (uint)values[4] == maxUint
                            && maxUint != 0)
                            return new SolidColorBrush(goodColor);
                        break;
                    case MaxProperty.JpegQuality:
                    case MaxProperty.Sharpness:
                        if ((double)values[1] == maxDouble
                            && maxDouble != 0.0)
                            return new SolidColorBrush(goodColor);
                        break;
                    case MaxProperty.UtilityIndex:
                        if ((decimal)values[1] == maxDecimal
                            && maxDecimal != 0)
                            return new SolidColorBrush(goodColor);
                        break;
                }

                return new SolidColorBrush(badColor);
            }
            return new SolidColorBrush();
        }


        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
