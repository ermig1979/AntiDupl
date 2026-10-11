using System;
using System.Globalization;
using System.Windows;
using System.Windows.Data;
using AntiDupl.NET.Core.Original;

namespace AntiDupl.NET.WPF.Convertor
{
    /// <summary>
    /// Локализованный tooltip для иконки колонки Type
    /// (зеркало WF: ResultRowSetter_DuplPairIcon/DefectIcon_ToolTip_Text).
    /// </summary>
    public class ResultTypeToToolTipConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            string key = value switch
            {
                CoreDll.ResultType.DuplImagePair => "mainwindow_DuplPairIconToolTip",
                CoreDll.ResultType.DefectImage => "mainwindow_DefectIconToolTip",
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