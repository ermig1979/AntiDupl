using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace AntiDupl.NET.WPF.Control
{
    public static class BrushExtender
    {
        public readonly static DependencyProperty BrushProperty = DependencyProperty.RegisterAttached("Brush", typeof(Brush), typeof(BrushExtender), new PropertyMetadata(Brushes.Black, DoBrushChanged));

        public readonly static DependencyProperty RedChannelProperty = DependencyProperty.RegisterAttached("RedChannel", typeof(int), typeof(BrushExtender), new PropertyMetadata(DoColorChangedRed));

        public readonly static DependencyProperty GreenChannelProperty = DependencyProperty.RegisterAttached("GreenChannel", typeof(int), typeof(BrushExtender), new PropertyMetadata(DoColorChangedGreen));

        public readonly static DependencyProperty BlueChannelProperty = DependencyProperty.RegisterAttached("BlueChannel", typeof(int), typeof(BrushExtender), new PropertyMetadata(DoColorChangedBlue));

        public readonly static DependencyProperty AlphaChannelProperty = DependencyProperty.RegisterAttached("AlphaChannel", typeof(int), typeof(BrushExtender), new PropertyMetadata(DoColorChangedAlpha));

        public readonly static DependencyProperty ColourValueProperty = DependencyProperty.RegisterAttached("ColourValue", typeof(string), typeof(BrushExtender), new PropertyMetadata(DoValueChanged));

        public static void SetRedChannel(DependencyObject o, int value)
        {
            o.SetValue(RedChannelProperty, value);
        }

        public static void SetGreenChannel(DependencyObject o, int value)
        {
            o.SetValue(GreenChannelProperty, value);
        }

        public static void SetBlueChannel(DependencyObject o, int value)
        {
            o.SetValue(BlueChannelProperty, value);
        }

        public static void SetAlphaChannel(DependencyObject o, int value)
        {
            o.SetValue(AlphaChannelProperty, value);
        }

        public static void SetBrush(DependencyObject o, SolidColorBrush brush)
        {
            o.SetValue(BrushProperty, brush);
        }

        public static void SetColourValue(DependencyObject o, string value)
        {
            o.SetValue(ColourValueProperty, value);
        }

        private static void DoColorChangedRed(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var color = ((SolidColorBrush)d.GetValue(BrushProperty)).Color;
            DoColorChange(d, (int)e.NewValue, c => c.R, () => Color.FromArgb(color.A, ((byte)(int)e.NewValue), color.G, color.B));
        }

        private static void DoColorChangedGreen(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var color = ((SolidColorBrush)d.GetValue(BrushProperty)).Color;
            DoColorChange(d, (int)e.NewValue, c => c.G, () => Color.FromArgb(color.A, color.R, ((byte)(int)e.NewValue), color.B));
        }

        private static void DoColorChangedBlue(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var color = ((SolidColorBrush)d.GetValue(BrushProperty)).Color;
            DoColorChange(d, (int)e.NewValue, c => c.B, () => Color.FromArgb(color.A, color.R, color.G, (byte)(int)e.NewValue));
        }

        private static void DoColorChangedAlpha(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var color = ((SolidColorBrush)d.GetValue(BrushProperty)).Color;
            DoColorChange(d, (int)e.NewValue, c => c.A, () => Color.FromArgb((byte)(int)e.NewValue, color.R, color.G, color.B));
        }

        private static void DoColorChange(DependencyObject d, int newValue, Func<Color, int> colorCompare, Func<Color> getColor)
        {
            var color = ((SolidColorBrush)d.GetValue(BrushProperty)).Color;
            if (colorCompare(color) == newValue)
                return;
            var newBrush = new SolidColorBrush(getColor());
            d.SetValue(BrushProperty, newBrush);
            d.SetValue(ColourValueProperty, newBrush.Color.ToString());
        }

        private static void DoValueChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var color = ((SolidColorBrush)d.GetValue(BrushProperty)).Color;
            if (color.ToString() == (string)e.NewValue)
                return;
            Color? newColour = null;
            try
            {
                newColour = (Color)ColorConverter.ConvertFromString((string)e.NewValue);
            }
            catch { }
            if (newColour == null)
                return;
            var newBrush = new SolidColorBrush(newColour.Value);
            d.SetValue(BrushProperty, newBrush);
        }


        private static void DoBrushChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (e.NewValue == e.OldValue)
                return;
            var colour = ((SolidColorBrush)e.NewValue).Color;
            d.SetValue(RedChannelProperty, (int)colour.R);
            d.SetValue(GreenChannelProperty, (int)colour.G);
            d.SetValue(BlueChannelProperty, (int)colour.B);
            d.SetValue(AlphaChannelProperty, (int)colour.A);
            d.SetValue(ColourValueProperty, colour.ToString());
        }
    }
}
