using System;
using System.Globalization;
using System.Windows;
using System.Windows.Data;
using AntiDupl.NET.Core.Original;

namespace AntiDupl.NET.WPF.Convertor
{
    /// <summary>
    /// Локализованный tooltip для иконки трансформации (зеркало WF:
    /// ResultRowSetter_*_Icon_ToolTip_Text). Текст берётся из активного
    /// словаря приложения в момент конвертации.
    /// </summary>
    public class TransformTypeToToolTipConverter : IValueConverter
    {
        static string ResourceKey(CoreDll.TransformType transform) => transform switch
        {
            CoreDll.TransformType.Turn_0 => "mainwindow_TransformTurn0ToolTip",
            CoreDll.TransformType.Turn_90 => "mainwindow_TransformTurn90ToolTip",
            CoreDll.TransformType.Turn_180 => "mainwindow_TransformTurn180ToolTip",
            CoreDll.TransformType.Turn_270 => "mainwindow_TransformTurn270ToolTip",
            CoreDll.TransformType.MirrorTurn_0 => "mainwindow_TransformMirrorTurn0ToolTip",
            CoreDll.TransformType.MirrorTurn_90 => "mainwindow_TransformMirrorTurn90ToolTip",
            CoreDll.TransformType.MirrorTurn_180 => "mainwindow_TransformMirrorTurn180ToolTip",
            CoreDll.TransformType.MirrorTurn_270 => "mainwindow_TransformMirrorTurn270ToolTip",
            _ => null
        };

        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is not CoreDll.TransformType transform)
                return string.Empty;

            string key = ResourceKey(transform);
            if (key == null)
                return string.Empty;

            return Application.Current != null &&
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