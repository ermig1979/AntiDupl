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
using AntiDuplWPF.ObjectModel;

namespace AntiDuplWPF.Control
{
    /// <summary>
    /// Логика взаимодействия для ImageInfoControl.xaml
    /// </summary>
    public partial class ImageInfoControl : UserControl
    {
        public ImageInfoControl()
        {
            InitializeComponent();
        }

        public static readonly DependencyProperty MainImageProperty =
            DependencyProperty.Register("MainImage", typeof(ImageInfoClass), typeof(ImageInfoControl),
            new PropertyMetadata(null, OnCurrentImageChanged));

        public ImageInfoClass MainImage
        {
            get
            {
                return (ImageInfoClass)GetValue(MainImageProperty);
            }
            set
            {
                SetValue(MainImageProperty, value);
            }
        }


        public static readonly DependencyProperty AdditionalImageProperty =
            DependencyProperty.Register("AdditionalImage", typeof(ImageInfoClass), typeof(ImageInfoControl),
            new PropertyMetadata(null, null));

        public ImageInfoClass AdditionalImage
        {
            get
            {
                return (ImageInfoClass)GetValue(AdditionalImageProperty);
            }
            set
            {
                SetValue(AdditionalImageProperty, value);
            }
        }

        private static void OnCurrentImageChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
        {
            ImageInfoControl myControl = sender as ImageInfoControl;
            // Gestion des synchonisations des colonnes
            //double width = (double)sender;

            //e.Property = (double)e.NewValue + 5;
            //myControl.imgCurrent.Source = ((ImageInfoClass)e.NewValue).Image;
        }


        public static readonly DependencyProperty EnableInfoProperty =
            DependencyProperty.Register("EnableInfo", typeof(bool), typeof(ImageInfoControl),
            new PropertyMetadata(true, null));

        public bool EnableInfo
        {
            get
            {
                return (bool)GetValue(EnableInfoProperty);
            }
            set
            {
                SetValue(EnableInfoProperty, value);
            }
        }

        public static readonly DependencyProperty GoodColorProperty =
            DependencyProperty.Register("GoodColor", typeof(Color), typeof(ImageInfoControl),
            new PropertyMetadata(null));

        public Color GoodColor
        {
            get
            {
                return (Color)GetValue(GoodColorProperty);
            }
            set
            {
                SetValue(GoodColorProperty, value);
            }
        }

        public static readonly DependencyProperty BadColorProperty =
            DependencyProperty.Register("BadColor", typeof(Color), typeof(ImageInfoControl),
            new PropertyMetadata(null));

        public Color BadColor
        {
            get
            {
                return (Color)GetValue(BadColorProperty);
            }
            set
            {
                SetValue(BadColorProperty, value);
            }
        }

        public static readonly DependencyProperty OpenComparatorForDuplPairCommandProperty =
            DependencyProperty.Register("OpenComparatorForDuplPairCommand", typeof(ICommand), typeof(ImageInfoControl),
            new PropertyMetadata(null));

        public ICommand OpenComparatorForDuplPairCommand
        {
            get
            {
                return (ICommand)GetValue(OpenComparatorForDuplPairCommandProperty);
            }
            set
            {
                SetValue(OpenComparatorForDuplPairCommandProperty, value);
            }
        }

        public static readonly DependencyProperty ShowFullImageProperty =
           DependencyProperty.Register("ShowFullImage", typeof(bool), typeof(ImageInfoControl),
           new PropertyMetadata(false));

        public bool ShowFullImage
        {
            get
            {
                return (bool)GetValue(ShowFullImageProperty);
            }
            set
            {
                SetValue(ShowFullImageProperty, value);
            }
        }
    }
}
