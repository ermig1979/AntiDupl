using System;
using System.Globalization;
using System.Windows;
using System.Windows.Data;
using AntiDupl.NET.Core.Original;

namespace AntiDupl.NET.WPF.Convertor
{
    /// <summary>
    /// Локализованный tooltip для иконки колонки Defect
    /// (зеркало WF: ResultRowSetter_*Icon_ToolTip_Text).
    /// </summary>
    public class DefectTypeToToolTipConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            string key = value switch
            {
                CoreDll.DefectType.Unknown => "mainwindow_UnknownDefectIconToolTip",
                CoreDll.DefectType.JpegEndMarkerIsAbsent => "mainwindow_JpegEndMarkerIsAbsentIconToolTip",
                CoreDll.DefectType.Blockiness => "mainwindow_BlockinessIconToolTip",
                CoreDll.DefectType.Blurring => "mainwindow_BlurringIconToolTip",
                _ => null
            };

            return key != null && Application.Current != null &&
                   Application.Current.TryFindResource(key) is string text
                ? text
                : string.Empty;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}