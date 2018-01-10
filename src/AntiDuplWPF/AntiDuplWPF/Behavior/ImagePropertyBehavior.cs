using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Interactivity;
using System.Windows.Controls;
using AntiDuplWPF.Helper;
using TinyIoC;
using AntiDuplWPF.Model;

namespace AntiDuplWPF.Behavior
{
    public class ImagePropertyBehavior : Behavior<Image>
    {
        private static IConfigurationModel _iConfigurationModel;

        public ImagePropertyBehavior()
        {
            if (_iConfigurationModel == null)
            {
                _iConfigurationModel = TinyIoCContainer.Current.Resolve<IConfigurationModel>();
            }

            if (_iConfigurationModel == null)
            {
                throw new ArgumentNullException("No IConfigurationModel is registered");
            }
        }

        #region Methods

        void _iConfigurationModel_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "ThumbnailWidth")
                SetPropertyes();
        }

        protected override void OnAttached()
        {
            base.OnAttached();
            _iConfigurationModel.PropertyChanged += _iConfigurationModel_PropertyChanged;
            SetPropertyes();
        }

        protected override void OnDetaching()
        {
            base.OnDetaching();
            _iConfigurationModel.PropertyChanged -= _iConfigurationModel_PropertyChanged;
        }

        private void SetPropertyes()
        {
            var image = AssociatedObject;
            var imageWidth = _iConfigurationModel.ThumbnailWidth;

            if (imageWidth != 0 && (Math.Abs(image.Width - imageWidth) > 0.0001 || double.IsNaN(image.Width)))
            {
                image.Width = imageWidth;
                //var cell = image.FindVisualAncestorByType<DataGridCell>();
                var cell = VisualTreeHelperEx.FindVisualAncestorByType<DataGridCell>(image);
                var column = cell.Column;
                var parentDataGrid = VisualTreeHelperEx.FindVisualAncestorByType<DataGrid>(image);
                if (parentDataGrid != null && column != null)
                {
                    column.Width = 0;
                    parentDataGrid.UpdateLayout();
                    column.Width = new DataGridLength(1, DataGridLengthUnitType.Auto);
                }
            }
        }
        #endregion
    }
}
