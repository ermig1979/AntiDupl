using System;
using System.Collections.Concurrent;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using AntiDupl.NET.Core.Original;

namespace AntiDupl.NET.WPF.Convertor
{
    /// <summary>
    /// Иконка колонки Type (зеркало WF ResultRowSetter):
    /// пара дублей — DuplPairHorizontalIcon, дефектный кадр — DefectIcon.
    /// </summary>
    public class ResultTypeToIconConverter : IValueConverter
    {
        static readonly ConcurrentDictionary<string, ImageSource> ms_Cache
            = new ConcurrentDictionary<string, ImageSource>();

        static ImageSource Get(string name) => ms_Cache.GetOrAdd(name, n =>
        {
            try
            {
                return new BitmapImage(new Uri(
                    $"pack://application:,,,/AntiDupl.NET.WPF;component/Image/wf/{n}.img"));
            }
            catch
            {
                return null;
            }
        });

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is not CoreDll.ResultType type)
                return null;

            return type switch
            {
                CoreDll.ResultType.DuplImagePair => Get("DuplPairHorizontalIcon"),
                CoreDll.ResultType.DefectImage => Get("DefectIcon"),
                _ => null
            };
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}