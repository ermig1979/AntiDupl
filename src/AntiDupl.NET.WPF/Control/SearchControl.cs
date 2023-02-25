using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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
    public class SearchControl : ContentControl
    {
        #region Поля
        //private TextBox _filterNameTextBox;
        //private ComboBox _filterTypeComboBox;
        //private CheckBox _isSearchByType;
        //private Button _selectType;
        //private TextBox _filterReplacementTextBox;
        #endregion

        #region Конструкторы

        static SearchControl()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(SearchControl), new FrameworkPropertyMetadata(typeof(SearchControl)));
        }

        #endregion

        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();

            //_filterNameTextBox = Template.FindName("PART_FilterName", this) as TextBox;
            //_filterTypeComboBox = Template.FindName("PART_FilterType", this) as ComboBox;
            //_isSearchByType = Template.FindName("PART_IsSearchByType", this) as CheckBox;
            //_selectType = Template.FindName("PART_SelectType", this) as Button;
            //_filterReplacementTextBox = Template.FindName("PART_FilterReplacement", this) as TextBox;
            //_filterTypeComboBox.ItemsSource = MainViewModel.AvailableTypeOfTool;
        }

        #region Свойства зависимостей

        //public string FilterName
        //{
        //    get { return (string)GetValue(FilterNameProperty); }
        //    set { SetValue(FilterNameProperty, value); }
        //}

        //public static readonly DependencyProperty FilterNameProperty = DependencyProperty.Register("FilterName",
        //    typeof(string), typeof(SearchControl), new PropertyMetadata(default(string)));

        //public string FilterType
        //{
        //    get { return (string)GetValue(FilterTypeProperty); }
        //    set { SetValue(FilterTypeProperty, value); }
        //}

        //public static readonly DependencyProperty FilterTypeProperty = DependencyProperty.Register("FilterType",
        //    typeof(string), typeof(FiltrationControl), new PropertyMetadata(default(string)));

        //public bool IsSearchByType
        //{
        //    get { return (bool)GetValue(IsSearchByTypeProperty); }
        //    set { SetValue(IsSearchByTypeProperty, value); }
        //}

        public ICommand SearchCommand
        {
            get { return (ICommand)GetValue(SearchCommandProperty); }
            set { SetValue(SearchCommandProperty, value); }
        }

        public static readonly DependencyProperty SearchCommandProperty = DependencyProperty.Register("SearchCommand",
            typeof(ICommand), typeof(SearchControl), new PropertyMetadata(null));

        public ICommand ClearCommand
        {
            get { return (ICommand)GetValue(ClearCommandProperty); }
            set { SetValue(ClearCommandProperty, value); }
        }

        public static readonly DependencyProperty ClearCommandProperty = DependencyProperty.Register("ClearCommand",
            typeof(ICommand), typeof(SearchControl), new PropertyMetadata(null));


        public string SearchText
        {
            get { return (string)GetValue(SearchTextProperty); }
            set { SetValue(SearchTextProperty, value); }
        }

        public static readonly DependencyProperty SearchTextProperty = DependencyProperty.Register("SearchText",
            typeof(string), typeof(SearchControl), new PropertyMetadata("", OnSearchTextChanged));

        static void OnSearchTextChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            SearchControl control = d as SearchControl;
            if (String.IsNullOrEmpty((string)e.NewValue) && !String.IsNullOrEmpty((string)e.OldValue)
                && d != null && control.ClearCommand != null)
                control.ClearCommand.Execute(null);
        }

        public string Watermark
        {
            get { return (string)GetValue(WatermarkProperty); }
            set { SetValue(WatermarkProperty, value); }
        }

        public static readonly DependencyProperty WatermarkProperty = DependencyProperty.Register("Watermark",
            typeof(string), typeof(SearchControl), new PropertyMetadata("Поиск"));

        #endregion

    }
}
