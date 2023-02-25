using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using Microsoft.Xaml.Behaviors;
using AntiDupl.NET.WPF.Model;
using AntiDupl.NET.WPF.ObjectModel;
using TinyIoC;

namespace AntiDupl.NET.WPF.Behavior
{
    public class MenuItemButtonGroupBehavior : Behavior<MenuItem>
    {
        private IViewModeModel _viewModeModel;
        IEnumerable<MenuItem> _menus;

        public MenuItemButtonGroupBehavior()
        {
            if (_viewModeModel == null)
            {
                _viewModeModel = TinyIoCContainer.Current.Resolve<IViewModeModel>();
            }

            if (_viewModeModel == null)
            {
                throw new ArgumentNullException("No IViewModeModel is registered");
            }

            _viewModeModel.PropertyChanged += _viewModeModel_PropertyChanged;
        }

        void _viewModeModel_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            _menus.ToList().ForEach(item => item.IsChecked = false);
            SetCheck(_menus, _viewModeModel.ViewMode);
        }

        protected override void OnAttached()
        {
            base.OnAttached();

            _menus = GetCheckableSubMenuItems(AssociatedObject);
            _menus.ToList()
                  .ForEach(item => item.Click += OnClick);

            SetCheck(_menus, _viewModeModel.ViewMode);
        }

        protected override void OnDetaching()
        {
            base.OnDetaching();

            //GetCheckableSubMenuItems(AssociatedObject)
            _menus.ToList()
                .ForEach(item => item.Click -= OnClick);
            _viewModeModel.PropertyChanged -= _viewModeModel_PropertyChanged;
        }

        private static IEnumerable<MenuItem> GetCheckableSubMenuItems(ItemsControl menuItem)
        {
            var itemCollection = menuItem.Items;
            return itemCollection.OfType<MenuItem>().Where(menuItemCandidate => menuItemCandidate.IsCheckable);
        }

        private void OnClick(object sender, RoutedEventArgs routedEventArgs)
        {
            var menuItem = (MenuItem)sender;

            if (!menuItem.IsChecked)
            {
                menuItem.IsChecked = true;
                return;
            }

            SetViewMode(menuItem);

            //GetCheckableSubMenuItems(AssociatedObject)
            _menus.Where(item => item != menuItem)
                .ToList()
                .ForEach(item => item.IsChecked = false);
        }

        private void SetCheck(IEnumerable<MenuItem> menus, ViewMode viewMode)
        {
            if (viewMode == ObjectModel.ViewMode.Group)
            {
                var item = menus.First(m => m.Name == "toGroup");
                if (item != null)
                    item.IsChecked = true;
            }
            else if (viewMode == ObjectModel.ViewMode.Pair)
            {
                var item = menus.First(m => m.Name == "toDuplPair");
                if (item != null)
                    item.IsChecked = true;
            }
        }

        private void SetViewMode(MenuItem menuItem)
        {
            if (menuItem.Name == "toGroup")
            {
                _viewModeModel.PropertyChanged -= _viewModeModel_PropertyChanged;
                _viewModeModel.ViewMode = ViewMode.Group;
                _viewModeModel.PropertyChanged += _viewModeModel_PropertyChanged;
            }
            else if (menuItem.Name == "toDuplPair")
            {
                _viewModeModel.PropertyChanged -= _viewModeModel_PropertyChanged;
                _viewModeModel.ViewMode = ViewMode.Pair;
                _viewModeModel.PropertyChanged += _viewModeModel_PropertyChanged;
            }
        }
    }
}
