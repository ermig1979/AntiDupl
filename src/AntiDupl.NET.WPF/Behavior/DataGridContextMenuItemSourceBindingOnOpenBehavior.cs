using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using Microsoft.Xaml.Behaviors;
using AntiDupl.NET.WPF.Helper;
using AntiDupl.NET.WPF.ObjectModel;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.Behavior
{
    public class DataGridContextMenuItemSourceBindingOnOpenBehavior : Behavior<DataGrid>
    {
        // Using a DependencyProperty as the backing store for theFilter.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty MenuGeneratorDuplPairProperty =
            DependencyProperty.Register("MenuGeneratorDuplPair",
            typeof(Func<DuplPairViewModel, BindableMenuItem[]>),
            typeof(DataGridContextMenuItemSourceBindingOnOpenBehavior),
            new UIPropertyMetadata(null));

        public Func<DuplPairViewModel, BindableMenuItem[]> MenuGeneratorDuplPair
        {
            get { return (Func<DuplPairViewModel, BindableMenuItem[]>)GetValue(MenuGeneratorDuplPairProperty); }
            set { SetValue(MenuGeneratorDuplPairProperty, value); }
        }

        public static readonly DependencyProperty MenuGeneratorDuplGroupProperty =
             DependencyProperty.Register("MenuGeneratorDuplGroup",
             typeof(Func<DuplicateGroup, BindableMenuItem[]>),
             typeof(DataGridContextMenuItemSourceBindingOnOpenBehavior),
             new UIPropertyMetadata(null));

        public Func<DuplicateGroup, BindableMenuItem[]> MenuGeneratorDuplGroup
        {
            get { return (Func<DuplicateGroup, BindableMenuItem[]>)GetValue(MenuGeneratorDuplGroupProperty); }
            set { SetValue(MenuGeneratorDuplGroupProperty, value); }
        }

        public static readonly DependencyProperty MenuGeneratorMultiDuplPairProperty =
                DependencyProperty.Register("MenuGeneratorMultiDuplPair",
                typeof(Func<IList, BindableMenuItem[]>),
                typeof(DataGridContextMenuItemSourceBindingOnOpenBehavior),
                new UIPropertyMetadata(null));

        public Func<IList, BindableMenuItem[]> MenuGeneratorMultiDuplPair
        {
            get { return (Func<IList, BindableMenuItem[]>)GetValue(MenuGeneratorMultiDuplPairProperty); }
            set { SetValue(MenuGeneratorMultiDuplPairProperty, value); }
        }

        public static readonly DependencyProperty MenuGeneratorMultiDuplGroupProperty =
                DependencyProperty.Register("MenuGeneratorMultiDuplGroup",
                typeof(Func<IList, BindableMenuItem[]>),
                typeof(DataGridContextMenuItemSourceBindingOnOpenBehavior),
                new UIPropertyMetadata(null));

        public Func<IList, BindableMenuItem[]> MenuGeneratorMultiDuplGroup
        {
            get { return (Func<IList, BindableMenuItem[]>)GetValue(MenuGeneratorMultiDuplGroupProperty); }
            set { SetValue(MenuGeneratorMultiDuplGroupProperty, value); }
        }

        protected override void OnAttached()
        {
            base.OnAttached();

            AssociatedObject.ContextMenuOpening += AssociatedObject_ContextMenuOpening;
        }

        void AssociatedObject_ContextMenuOpening(object sender, ContextMenuEventArgs e)
        {
            var grid = sender as DataGrid;
            if (grid == null)
                return;

            if (grid.ContextMenu == null)
                return;

            grid.ContextMenu.ItemsSource = null;
            grid.ContextMenu.Items.Clear();

            var fe = e.OriginalSource as FrameworkElement;
            if (fe != null)
            {
                if (HandleColumnHeadersMenuOpen(grid, grid.ContextMenu))
                    return;
            }

            if (grid.SelectedItems.Count > 1)
            {
                if (grid.SelectedItem is DuplPairViewModel)
                {
                    IList viewResultsOfDupl = (IList)grid.SelectedItems;
                    if (MenuGeneratorMultiDuplPair != null)
                        grid.ContextMenu.ItemsSource = MenuGeneratorMultiDuplPair(viewResultsOfDupl);
                }
                else if (grid.SelectedItem is DuplicateGroup)
                {
                    IList viewResults = (IList)grid.SelectedItems;
                    if (MenuGeneratorMultiDuplGroup != null)
                        grid.ContextMenu.ItemsSource = MenuGeneratorMultiDuplGroup(viewResults);
                }
            }
            else
            {
                if (grid.SelectedItem is DuplPairViewModel)
                {
                    DuplPairViewModel viewResultOfDupl = (DuplPairViewModel)grid.SelectedItem;
                    if (MenuGeneratorDuplPair != null)
                        grid.ContextMenu.ItemsSource = MenuGeneratorDuplPair(viewResultOfDupl);
                }
                else if (grid.SelectedItem is DuplicateGroup)
                {
                    DuplicateGroup duplicateGroup = (DuplicateGroup)grid.SelectedItem;
                    if (MenuGeneratorDuplGroup != null)
                        grid.ContextMenu.ItemsSource = MenuGeneratorDuplGroup(duplicateGroup);
                }
            }
        }

        private static bool HandleColumnHeadersMenuOpen(DataGrid grid, ContextMenu menu)
        {
            var headersPresenter = VisualTreeHelperEx.FindVisualChildByType<DataGridColumnHeadersPresenter>(grid);
            if (headersPresenter == null)
                return false;

            var currentPosition = Mouse.GetPosition(headersPresenter);
            if (currentPosition.X < 0 || currentPosition.X > headersPresenter.ActualWidth ||
                currentPosition.Y < 0 || currentPosition.Y > headersPresenter.ActualHeight)
                return false;

            var enabled = DataGridColumnChooserHelper.GetIsEnabled(grid);
            if (!enabled)
                return true;

            foreach (var col in grid.Columns.OrderBy(col => col.DisplayIndex))
            {
                string title;
                if (col.Header is TextBlock)
                {
                    var tblock = (TextBlock) col.Header;
                    title = !string.IsNullOrEmpty(tblock.Text)
                        ? tblock.Text
                        : string.Join(string.Empty, tblock.Inlines.OfType<Run>().Select(i => i.Text))
                            .Replace(Environment.NewLine, string.Empty);
                }
                else
                    title = (string) col.Header;

                var menuItem = new MenuItem {Header = title, IsCheckable = true};
                var binding = new Binding
                {
                    Source = col,
                    Path = new PropertyPath(DataGridColumn.VisibilityProperty.Name),
                    Converter = new VisibilityToBoolConverter()
                };
                BindingOperations.SetBinding(menuItem, MenuItem.IsCheckedProperty, binding);
                menuItem.StaysOpenOnClick = true;
                menu.Items.Add(menuItem);
            }

            return true;
        }

        protected override void OnDetaching()
        {
            base.OnDetaching();

            AssociatedObject.ContextMenuOpening -= AssociatedObject_ContextMenuOpening;
        }
    }
}
