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
    /// Логика взаимодействия для ComplexProgressBar.xaml
    /// </summary>
    public partial class ComplexProgressBar : UserControl
    {
        public ComplexProgressBar()
        {
            InitializeComponent();
        }

        public static readonly DependencyProperty MaximumProperty =
           DependencyProperty.Register("Maximum", typeof(uint), typeof(ComplexProgressBar));

        public uint Maximum
        {
            get
            {
                return (uint)GetValue(MaximumProperty);
            }
            set
            {
                SetValue(MaximumProperty, value);
            }
        }


        public static readonly DependencyProperty CurrentFirstProperty =
            DependencyProperty.Register("CurrentFirst", typeof(uint), typeof(ComplexProgressBar),
            new PropertyMetadata(OnCurrentFirstChanged));

        public uint CurrentFirst
        {
            get
            {
                return (uint)GetValue(CurrentFirstProperty);
            }
            set
            {
                SetValue(CurrentFirstProperty, value);
            }
        }

        private static void OnCurrentFirstChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
        {
            ComplexProgressBar myControl = sender as ComplexProgressBar;
            // Gestion des synchonisations des colonnes
            //double width = (double)sender;

            //e.Property = (double)e.NewValue + 5;
            //myControl.imgCurrent.Source = ((ImageInfoClass)e.NewValue).Image;

            myControl.ProgressBarForeground.Width = (myControl.ActualWidth - 2) * myControl.CurrentFirst / myControl.Maximum;
           // myControl.ProgressBarForeground2.Width = (myControl.ActualWidth - 2) * myControl. / myControl.Maximum;
        }

        public static readonly DependencyProperty CurrentSecondProperty =
          DependencyProperty.Register("CurrentSecond", typeof(uint), typeof(ComplexProgressBar),
          new PropertyMetadata(OnCurrentSecondChanged));

        public uint CurrentSecond
        {
            get
            {
                return (uint)GetValue(CurrentSecondProperty);
            }
            set
            {
                SetValue(CurrentSecondProperty, value);
            }
        }

        private static void OnCurrentSecondChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
        {
            ComplexProgressBar myControl = sender as ComplexProgressBar;
            myControl.ProgressBarForeground2.Width = (myControl.ActualWidth - 2) * myControl.CurrentSecond / myControl.Maximum;
        }

    }
}
