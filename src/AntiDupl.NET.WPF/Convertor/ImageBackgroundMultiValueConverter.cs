using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;
using System.Windows.Media;
using AntiDupl.NET.WPF.ObjectModel;

namespace AntiDupl.NET.WPF.Convertor
{
    public class ImageBackgroundMultiValueConverter : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            if (values[0] != null && values[0] is ImageInfoClass
                && values[1] != null && values[1] is ImageInfoClass
                && values[2] != DependencyProperty.UnsetValue
                && values[3] != DependencyProperty.UnsetValue)
            {
                ImageInfoClass mainImage = values[0] as ImageInfoClass;
                ImageInfoClass additionalImage = values[1] as ImageInfoClass;
                System.Windows.Media.Color goodColor = (System.Windows.Media.Color)values[2];
                System.Windows.Media.Color badColor = (System.Windows.Media.Color)values[3];

                switch ((MaxProperty)parameter)
                {
                    //чем ниже Blockiness тем лучше изображение
                    case MaxProperty.Blockiness:
                        if (mainImage.Blockiness == additionalImage.Blockiness)
                            return new SolidColorBrush();
                        if (mainImage.Blockiness < additionalImage.Blockiness)
                            return new SolidColorBrush(badColor);
                        break;
                    case MaxProperty.Bluring:
                        if (mainImage.Bluring == additionalImage.Bluring)
                            return new SolidColorBrush();
                        if (mainImage.Bluring < additionalImage.Bluring)
                            return new SolidColorBrush(badColor);
                        break;
                    case MaxProperty.FileSize:
                        if (mainImage.FileSize == additionalImage.FileSize)
                            return new SolidColorBrush();
                        if (mainImage.FileSize < additionalImage.FileSize)
                            return new SolidColorBrush(badColor);
                        break;
                    case MaxProperty.Resolution:
                        if (mainImage.Width * mainImage.Height == additionalImage.Width * additionalImage.Height)
                            return new SolidColorBrush();
                        if (mainImage.Width * mainImage.Height < additionalImage.Width * additionalImage.Height)
                            return new SolidColorBrush(badColor);
                        break;
                    //case MaxProperty.EXIF:
                    //    if (mainImage.EXIF.Equals(additionalImage.EXIF))
                    //        return new SolidColorBrush();
                    //    else
                    //        return new SolidColorBrush(badColor);
                    case MaxProperty.UtilityIndex:
                        if (mainImage.UtilityIndex.Equals(additionalImage.UtilityIndex))
                            return new SolidColorBrush();
                        if (mainImage.UtilityIndex < additionalImage.UtilityIndex)
                            return new SolidColorBrush(badColor);
                        break;
                }
                return new SolidColorBrush(goodColor);
            }
            return new SolidColorBrush();
        }


        public object[] ConvertBack(object value, Type[] targetTypes, object parameter, System.Globalization.CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
