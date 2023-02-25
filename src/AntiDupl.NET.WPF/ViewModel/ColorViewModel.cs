using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace AntiDupl.NET.WPF.ViewModel
{
    public class ColorViewModel : PropertyChangedBase
    {
        public ColorViewModel(Color color)
        {
            Color = color;
        }

        Color _color;
        public Color Color
        {
            get { return _color; }
            set
            {
                if (_color != value)
                {
                    _color = value;
                    RaisePropertyChangedEvent("Color");

                    RedChannel = (int)_color.R;
                    GreenChannel = (int)_color.G;
                    BlueChannel = (int)_color.B;
                    AlphaChannel = (int)_color.A;
                    ColourValue = _color.ToString();
                }
            }
        }

        int _redChannel;
        public int RedChannel
        {
            get { return _redChannel; }
            set
            {
                if (_redChannel != value)
                {
                    _redChannel = value;
                    RaisePropertyChangedEvent("RedChannel");
                    DoColorChange((int)value, c => c.R, () => Color.FromArgb(Color.A, ((byte)(int)value), Color.G, Color.B));
                }
            }
        }

        int _greenChannel;
        public int GreenChannel
        {
            get { return _greenChannel; }
            set
            {
                if (_greenChannel != value)
                {
                    _greenChannel = value;
                    RaisePropertyChangedEvent("RedChannel");
                    DoColorChange((int)value, c => c.G, () => Color.FromArgb(Color.A, Color.R, ((byte)(int)value), Color.B));
                }
            }
        }

        int _blueChannel;
        public int BlueChannel
        {
            get { return _blueChannel; }
            set
            {
                if (_blueChannel != value)
                {
                    _blueChannel = value;
                    RaisePropertyChangedEvent("BlueChannel");
                    DoColorChange((int)value, c => c.B, () => Color.FromArgb(Color.A, Color.R, Color.G, (byte)(int)value));
                }
            }
        }

        int _alphaChannel;
        public int AlphaChannel
        {
            get { return _alphaChannel; }
            set
            {
                if (_alphaChannel != value)
                {
                    _alphaChannel = value;
                    RaisePropertyChangedEvent("AlphaChannel");
                    DoColorChange((int)value, c => c.A, () => Color.FromArgb((byte)(int)value, Color.R, Color.G, Color.B));
                }
            }
        }

        string _colourValue;
        public string ColourValue
        {
            get { return _colourValue; }
            set
            {
                if (_colourValue != value)
                {
                    _colourValue = value;
                    RaisePropertyChangedEvent("ColourValue");

                    Color? newColour = null;
                    try
                    {
                        newColour = (Color)ColorConverter.ConvertFromString(value);
                    }
                    catch { }
                    if (newColour != null && newColour.HasValue)
                    {
                        Color = newColour.Value;
                    }
                }
            }
        }

        private void DoColorChange(int newValue, Func<Color, int> colorCompare, Func<Color> getColor)
        {
            if (colorCompare(Color) == newValue)
                return;
            Color = getColor();
            ColourValue = Color.ToString();
        }
    }
}
