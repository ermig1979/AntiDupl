using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using AntiDupl.NET.Core.Original;

namespace AntiDupl.NET.WPF.Convertor
{
    /// <summary>
    /// Иконка трансформации пары (зеркало WF: DataGridViewImageCell в колонке Transform,
    /// ResultRowSetter) — иконки Turn/MirrorTurn из набора WF.
    /// </summary>
    public class TransformTypeToImageSourceConverter : IValueConverter
    {
        static readonly System.Collections.Concurrent.ConcurrentDictionary<CoreDll.TransformType, ImageSource> ms_Cache
            = new System.Collections.Concurrent.ConcurrentDictionary<CoreDll.TransformType, ImageSource>();

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is not CoreDll.TransformType transform)
                return null;

            return ms_Cache.GetOrAdd(transform, t =>
            {
                try
                {
                    return new BitmapImage(new Uri(
                        $"pack://application:,,,/AntiDupl.NET.WPF;component/Image/wf/{t}_Icon.img"));
                }
                catch
                {
                    return null;
                }
            });
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}