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
using System.Windows.Input;
using System.Windows.Interactivity;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.Behavior
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

       /* public static readonly DependencyProperty MenuGeneratorDuplGroupProperty =
             DependencyProperty.Register("MenuGeneratorDuplGroup",
             typeof(Func<DuplicateGroup, BindableMenuItem[]>),
             typeof(DataGridContextMenuItemSourceBindingOnOpenBehavior),
             new UIPropertyMetadata(null));

        public Func<DuplicateGroup, BindableMenuItem[]> MenuGeneratorDuplGroup
        {
            get { return (Func<DuplicateGroup, BindableMenuItem[]>)GetValue(MenuGeneratorDuplGroupProperty); }
            set { SetValue(MenuGeneratorDuplGroupProperty, value); }
        }*/

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
            var grid = sender as MultiSelector;
            if (grid != null)
            {
                if (grid.SelectedItems.Count > 1)
                {
                    if (grid.SelectedItem is DuplPairViewModel)
                    {
                        IList viewResultsOfDupl = (IList)grid.SelectedItems;
                        if (MenuGeneratorMultiDuplPair != null)
                            grid.ContextMenu.ItemsSource = MenuGeneratorMultiDuplPair(viewResultsOfDupl);
                    }
                   /* else if (grid.SelectedItem is DuplicateGroup)
                    {
                        IList viewResults = (IList)grid.SelectedItems;
                        if (MenuGeneratorMultiDuplGroup != null)
                            grid.ContextMenu.ItemsSource = MenuGeneratorMultiDuplGroup(viewResults);
                    }*/
                }
                else
                {
                    if (grid.SelectedItem is DuplPairViewModel)
                    {
                        DuplPairViewModel viewResultOfDupl = (DuplPairViewModel)grid.SelectedItem;
                        if (MenuGeneratorDuplPair != null)
                            grid.ContextMenu.ItemsSource = MenuGeneratorDuplPair(viewResultOfDupl);
                    }
                    /*else if (grid.SelectedItem is DuplicateGroup)
                    {
                        DuplicateGroup duplicateGroup = (DuplicateGroup)grid.SelectedItem;
                        if (MenuGeneratorDuplGroup != null)
                            grid.ContextMenu.ItemsSource = MenuGeneratorDuplGroup(duplicateGroup);
                    }*/
                }
            }
        }

        protected override void OnDetaching()
        {
            base.OnDetaching();

            AssociatedObject.ContextMenuOpening -= AssociatedObject_ContextMenuOpening;
        }
    }
}
