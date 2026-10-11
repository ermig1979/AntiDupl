using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;

namespace AntiDupl.NET.WPF.Helper
{
    public sealed class ReferenceEqualityComparer<T> : IEqualityComparer<T>
    {
        public static readonly IEqualityComparer<T> DefaultInstance = new ReferenceEqualityComparer<T>();

        public bool Equals(T x, T y)
        {
            return ReferenceEquals(x, y);
        }

        public int GetHashCode(T obj)
        {
            return 0;
        }
    }

    public static class DataGridColumnChooserHelper
    {
        private static readonly Dictionary<string, DataGrid> ms_Grids = new Dictionary<string, DataGrid>();
        private static readonly Dictionary<Button, string> ms_Buttons = new Dictionary<Button, string>(ReferenceEqualityComparer<Button>.DefaultInstance);
        private static readonly Dictionary<MenuItem, string> ms_MenuItems = new Dictionary<MenuItem, string>(ReferenceEqualityComparer<MenuItem>.DefaultInstance);

        public static readonly DependencyProperty IsEnabledProperty = DependencyProperty.RegisterAttached(
            "IsEnabled",
            typeof(bool),
            typeof(DataGridColumnChooserHelper),
            new PropertyMetadata(default(bool)));

        public static void SetIsEnabled(DependencyObject element, bool value)
        {
            element.SetValue(IsEnabledProperty, value);
        }

        public static bool GetIsEnabled(DependencyObject element)
        {
            return (bool) element.GetValue(IsEnabledProperty);
        }

        public static readonly DependencyProperty ChooserIdProperty =
            DependencyProperty.RegisterAttached(
                "ChooserId",
                typeof(string),
                typeof(DataGridColumnChooserHelper),
                new PropertyMetadata(default(string), PropertyChangedCallback));

        private static void PropertyChangedCallback(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var grid = d as DataGrid;
            var key = (string)e.NewValue;
            if (grid != null)
            {
                ms_Grids[key] = grid;
                // Перерегистрация при каждом возвращении в визуальное дерево:
                // TabControl выгружает контент невывиденных вкладок (Unloaded),
                // иначе кнопка-chooser после переключения вкладок молча переставает работать.
                grid.Loaded += (s, e2) => { ms_Grids[key] = grid; };
                grid.Unloaded += GridOnUnloaded;
                return;
            }

            var btn = d as Button;
            if (btn != null)
            {
                ms_Buttons[btn] = key;
                btn.Click += BtnOnClick;
                btn.Unloaded += BtnOnUnloaded;
                return;
            }

            // Зеркало WF: «Select columns» — пункт View-меню (m_view_selectColumnsMenuItem),
            // не кнопка тулбара.
            var mnu = d as MenuItem;
            if (mnu != null)
            {
                ms_MenuItems[mnu] = key;
                mnu.Click += MenuItemOnClick;
                mnu.Unloaded += MenuItemOnUnloaded;
            }
        }

        private static void GridOnUnloaded(object sender, RoutedEventArgs e)
        {
            var grid = (DataGrid)sender;
            var grids = ms_Grids.Where(i => ReferenceEquals(i.Value, grid)).ToArray();
            foreach (var gridPair in grids)
                ms_Grids.Remove(gridPair.Key);
        }

        private static void BtnOnUnloaded(object sender, RoutedEventArgs routedEventArgs)
        {
            var btn = (Button)sender;
            btn.Click -= BtnOnClick;
            ms_Buttons.Remove(btn);
        }

        private static void MenuItemOnUnloaded(object sender, RoutedEventArgs routedEventArgs)
        {
            var mnu = (MenuItem)sender;
            mnu.Click -= MenuItemOnClick;
            ms_MenuItems.Remove(mnu);
        }

        private static void MenuItemOnClick(object sender, RoutedEventArgs routedEventArgs)
        {
            var mnu = (MenuItem)sender;
            OpenChooser(mnu, ms_MenuItems[mnu]);
        }

        private static void BtnOnClick(object sender, RoutedEventArgs routedEventArgs)
        {
            var btn = (Button)sender;
            OpenChooser(btn, ms_Buttons[btn]);
        }

        private static void OpenChooser(UIElement anchor, string key)
        {
            DataGrid grid;
            if (!ms_Grids.TryGetValue(key, out grid))
                return;

            var mnu = new ContextMenu();
            foreach (var col in grid.Columns.OrderBy(col => col.DisplayIndex))
            {
                string title;
                if (col.Header is TextBlock)
                {
                    var tblock = (TextBlock)col.Header;
                    title = !string.IsNullOrEmpty(tblock.Text)
                        ? tblock.Text
                        : string.Join(string.Empty, tblock.Inlines.OfType<Run>().Select(i => i.Text)).Replace(Environment.NewLine, string.Empty);
                }
                else
                    title = (string)col.Header;
                var menuItem = new MenuItem {Header = title, IsCheckable = true};
                var binding = new Binding
                {
                    Source = col,
                    Path = new PropertyPath(DataGridColumn.VisibilityProperty.Name),
                    Converter = new VisibilityToBoolConverter()
                };
                BindingOperations.SetBinding(menuItem, MenuItem.IsCheckedProperty, binding);
                menuItem.StaysOpenOnClick = true;
                mnu.Items.Add(menuItem);
            }
            mnu.PlacementTarget = anchor;
            mnu.IsOpen = true;
        }


        public static void SetChooserId(DependencyObject element, string value)
        {
            element.SetValue(ChooserIdProperty, value);
        }

        public static string GetChooserId(DependencyObject element)
        {
            return (string)element.GetValue(ChooserIdProperty);
        }
    }

    public class VisibilityToBoolConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value == null)
                return Visibility.Collapsed;

            return (Visibility)value == Visibility.Visible;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return (bool)(value ?? false) ? Visibility.Visible : Visibility.Collapsed;
        }
    }
}