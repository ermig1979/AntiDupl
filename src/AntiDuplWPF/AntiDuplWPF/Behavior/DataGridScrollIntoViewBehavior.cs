using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using Microsoft.Xaml.Behaviors;

namespace AntiDuplWPF.Behavior
{
    public class DataGridScrollIntoViewBehavior : Behavior<DataGrid>
    {
        protected override void OnAttached()
        {
            AssociatedObject.Loaded += OnLoaded;
            AssociatedObject.Unloaded += OnUnLoaded;
        }

        protected override void OnDetaching()
        {
            AssociatedObject.Loaded -= OnLoaded;
            AssociatedObject.Unloaded -= OnUnLoaded;
        }

        private void OnLoaded(object sender, RoutedEventArgs e)
        {
            AssociatedObject.SelectionChanged += AssociatedObject_SelectionChanged;
        }

        void AssociatedObject_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (AssociatedObject is DataGrid)
            {
                DataGrid grid = (DataGrid)AssociatedObject;
                grid.Dispatcher.BeginInvoke((Action)(() =>
                {
                    grid.UpdateLayout();
                    if (grid.SelectedItem != null)
                        grid.ScrollIntoView(grid.SelectedItem);
                }));
            }
        }

        private void OnUnLoaded(object sender, RoutedEventArgs e)
        {
            AssociatedObject.SelectionChanged -= AssociatedObject_SelectionChanged;
        }


    }
}
