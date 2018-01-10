using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace AntiDuplWPF.Control
{
    /// <summary>
    /// Логика взаимодействия для CgsImageViewer.xaml
    /// </summary>
    public partial class CgsImageViewer : UserControl
    {
        public CgsImageViewer()
        {
            InitializeComponent();
        }

        #region Properties & Members

        public static readonly DependencyProperty CurrentImageProperty =
            DependencyProperty.Register("CurrentImage", typeof(BitmapImage), typeof(CgsImageViewer),
            new PropertyMetadata(null, OnCurrentImageChanged));

        private static void OnCurrentImageChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
        {
            CgsImageViewer myControl = sender as CgsImageViewer;
            // Gestion des synchonisations des colonnes
            //double width = (double)sender;

            //e.Property = (double)e.NewValue + 5;
            myControl.imgCurrent.Source = (BitmapImage)e.NewValue;
        }

        public BitmapImage CurrentImage
        {
            get
            {
                return (BitmapImage)GetValue(CurrentImageProperty);
            }
            set
            {
                SetValue(CurrentImageProperty, value);
                this.imgCurrent.Source = value;
            }
        }


        /*private BitmapImage _currentImage;
        public BitmapImage CurrentImage
        {
            get
            {
                return _currentImage;
            }
            set
            {
                if (_currentImage != value)
                {
                    _currentImage = value;
                    this.imgCurrent.Source = _currentImage;
                }
            }
        }*/

        private string _currentImagePath;
        public string CurrentImagePath
        {
            get
            {
                return _currentImagePath;
            }
            set
            {
                if (_currentImagePath != value)
                {
                    _currentImagePath = value;
                    CurrentImage = new BitmapImage(new Uri(_currentImagePath));
                }
            }
        }

        private bool IsFit = true;
        private double _zoomLevel = 0;
        private double _rotationAngle = 0;

        public double ZoomLevel
        {
            get
            {
                return _zoomLevel;
            }
            set
            {
                _zoomLevel = value;
                IsFit = (_zoomLevel == 1);

                if (panZoomViewer.IsLoaded)
                {
                    var centerPoint = new Point(this.ActualWidth / 2, this.ActualHeight / 2);
                    panZoomViewer.DoZoomPercent(_zoomLevel, centerPoint, centerPoint);
                }
            }
        }

        public double ViewableWidth
        {
            get
            {
                double width = (IsHeightWidthSwitched) ?
                    this.ActualWidth - (MainBorder.BorderThickness.Left + MainBorder.BorderThickness.Right)
                    : this.ActualHeight - (spnlControlBar.ActualHeight + MainBorder.BorderThickness.Top);

                return (width >= 0) ? width : 0;
            }
        }
        public double ViewableHeight
        {
            get
            {
                double height = (IsHeightWidthSwitched) ? this.ActualHeight - (spnlControlBar.ActualHeight + MainBorder.BorderThickness.Top) : this.ActualWidth - (MainBorder.BorderThickness.Left + MainBorder.BorderThickness.Right);

                return (height >= 0) ? height : 0;
            }
        }

        private double ImageWidth
        {
            get
            {
                if (CurrentImage != null)
                {
                    return (IsHeightWidthSwitched) ? CurrentImage.Width : CurrentImage.Height;
                }

                return 0;
            }
        }
        private double ImageHeight
        {
            get
            {
                if (CurrentImage != null)
                {
                    return (IsHeightWidthSwitched) ? CurrentImage.Height : CurrentImage.Width;
                }

                return 0;
            }
        }

        private bool IsHeightWidthSwitched
        {
            get
            {
                if (_rotationAngle != 0)
                {
                    return ((Math.Abs(_rotationAngle) / 90) % 2) != 1;
                }

                return true;
            }
        }
        #endregion

        #region UI Events
        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            ScaleToFit();
        }

        private void UserControl_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (IsFit)
            {
                ScaleToFit();
            }
        }

        private void btnFitSize_Click(object sender, RoutedEventArgs e)
        {
            sldZoomLevel.Value = 1;
            ScaleToFit();
        }
        private void btnRotateLeft_Click(object sender, RoutedEventArgs e)
        {
            RotateLeft();
        }
        private void btnRotateRight_Click(object sender, RoutedEventArgs e)
        {
            RotateRight();
        }

        private void btnSelectZoom_Click(object sender, RoutedEventArgs e)
        {
            popupZoomLevel.VerticalOffset = -15;
            popupZoomLevel.HorizontalOffset = -8;

            popupZoomLevel.StaysOpen = false;
            popupZoomLevel.IsOpen = true;
        }

        private void sldZoomLevel_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            ZoomLevel = sldZoomLevel.Value;
        }
        #endregion

        #region Image Manipulation

        public void ScaleToFit()
        {
            if (panZoomViewer.IsLoaded)
            {
                panZoomViewer.Reset();
            }

            imgCurrent.ClearValue(WidthProperty);
            imgCurrent.ClearValue(HeightProperty);

            if (ImageHeight > ViewableHeight)
            {
                imgCurrent.Height = ViewableHeight;
            }
            if (ImageWidth > ViewableWidth)
            {
                imgCurrent.Width = ViewableWidth;
            }
        }

        public void RotateLeft()
        {
            _rotationAngle -= 90;
            imgCurrent.LayoutTransform = new RotateTransform(_rotationAngle);


            ZoomLevel = 0;
            ScaleToFit();
        }
        public void RotateRight()
        {
            _rotationAngle += 90;
            imgCurrent.LayoutTransform = new RotateTransform(_rotationAngle);

            ZoomLevel = 0;
            ScaleToFit();
        }

        #endregion
    }
}
