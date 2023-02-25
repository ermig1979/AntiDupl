using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;
using System.Windows.Media;
using AntiDupl.NET.WPF.ObjectModel;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.Convertor
{
    public class DeleteBackgroundValueConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            DuplPairViewModel result = value as DuplPairViewModel;
            PositionInDuplPair position = (PositionInDuplPair)parameter;
            if (result != null && position != null)
            {
                if (result.FirstFile.UtilityIndex > result.SecondFile.UtilityIndex)
                {
                    if (position == PositionInDuplPair.Second)
                        return new SolidColorBrush(Colors.LightCoral);
                }
                else if (result.FirstFile.UtilityIndex < result.SecondFile.UtilityIndex)
                {
                    if (position == PositionInDuplPair.First)
                        return new SolidColorBrush(Colors.LightCoral);
                }
            }

            return new SolidColorBrush();
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
