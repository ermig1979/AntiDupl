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
    /// Иконка колонки Defect (зеркало WF ResultRowSetter): иконка по типу дефекта
    /// (UnknownDefect/JpegEndMarkerIsAbsent/Blockiness/Blurring);
    /// None — пусто (как m_nullIcon в WF).
    /// </summary>
    public class DefectTypeToIconConverter : IValueConverter
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
            if (value is not CoreDll.DefectType defect)
                return null;

            return defect switch
            {
                CoreDll.DefectType.Unknown => Get("UnknownDefectIcon"),
                CoreDll.DefectType.JpegEndMarkerIsAbsent => Get("JpegEndMarkerIsAbsentIcon"),
                CoreDll.DefectType.Blockiness => Get("BlockinessIcon"),
                CoreDll.DefectType.Blurring => Get("BlurringIcon"),
                _ => null
            };
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}