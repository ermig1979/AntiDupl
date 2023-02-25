using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;
using System.Windows.Media;
using AntiDupl.NET.WPF.ObjectModel;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.Convertor
{
    public class DuplResultMultiValueConverter : IMultiValueConverter
    {
        public object Convert(object[] values, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            if (values[2] != DependencyProperty.UnsetValue && values[3] != DependencyProperty.UnsetValue)
            {
                DuplPairViewModel result = values[0] as DuplPairViewModel;
                PositionInDuplPair position = (PositionInDuplPair)values[1];
                Color goodColor = (Color)values[2];
                Color badColor = (Color)values[3];
                MaxProperty property = (MaxProperty)parameter;
                if (result != null && position != null && goodColor != null && badColor != null && property != null)
                {
                    switch (property)
                    {
                        case MaxProperty.FileSize:
                            if (result.FirstFile.FileSize == result.SecondFile.FileSize)
                                return new SolidColorBrush();
                            if (result.FirstFile.FileSize < result.SecondFile.FileSize)
                                return position == PositionInDuplPair.First ? new SolidColorBrush(badColor) : new SolidColorBrush(goodColor);
                            else
                                return position == PositionInDuplPair.Second ? new SolidColorBrush(badColor) : new SolidColorBrush(goodColor);
                        //чем ниже Blockiness тем лучше изображение
                        case MaxProperty.Blockiness:
                            if (result.FirstFile.Blockiness == result.SecondFile.Blockiness)
                                return new SolidColorBrush();
                            if (result.FirstFile.Blockiness > result.SecondFile.Blockiness)
                                return position == PositionInDuplPair.First ? new SolidColorBrush(badColor) : new SolidColorBrush(goodColor);
                            else
                                return position == PositionInDuplPair.Second ? new SolidColorBrush(badColor) : new SolidColorBrush(goodColor);
                            //чем ниже Bluring тем лучше изображение
                        case MaxProperty.Bluring:
                            if (result.FirstFile.Bluring == result.SecondFile.Bluring)
                                return new SolidColorBrush();
                            if (result.FirstFile.Bluring > result.SecondFile.Bluring)
                                return position == PositionInDuplPair.First ? new SolidColorBrush(badColor) : new SolidColorBrush(goodColor);
                            else
                                return position == PositionInDuplPair.Second ? new SolidColorBrush(badColor) : new SolidColorBrush(goodColor);
                        //чем меньше JpegPeaks, тем лучше изображение
                        case MaxProperty.JpegPeaks:
                            if (result.FirstFile.JpegPeaks == result.SecondFile.JpegPeaks)
                                return new SolidColorBrush();
                            if (result.FirstFile.JpegPeaks > result.SecondFile.JpegPeaks)
                                return position == PositionInDuplPair.First ? new SolidColorBrush(badColor) : new SolidColorBrush(goodColor);
                            else
                                return position == PositionInDuplPair.Second ? new SolidColorBrush(badColor) : new SolidColorBrush(goodColor);
                        case MaxProperty.Resolution:
                            if (result.FirstFile.Width * result.FirstFile.Height == result.SecondFile.Width * result.SecondFile.Height)
                                return new SolidColorBrush();
                            if (result.FirstFile.Width * result.FirstFile.Height < result.SecondFile.Width * result.SecondFile.Height)
                                return position == PositionInDuplPair.First ? new SolidColorBrush(badColor) : new SolidColorBrush(goodColor);
                            else
                                return position == PositionInDuplPair.Second ? new SolidColorBrush(badColor) : new SolidColorBrush(goodColor);
                        case MaxProperty.UtilityIndex:
                            if (result.FirstFile.UtilityIndex == result.SecondFile.UtilityIndex)
                                return new SolidColorBrush();
                            if (result.FirstFile.UtilityIndex < result.SecondFile.UtilityIndex)
                                return position == PositionInDuplPair.First ? new SolidColorBrush(badColor) : new SolidColorBrush(goodColor);
                            else
                                return position == PositionInDuplPair.Second ? new SolidColorBrush(badColor) : new SolidColorBrush(goodColor);
                    }
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
