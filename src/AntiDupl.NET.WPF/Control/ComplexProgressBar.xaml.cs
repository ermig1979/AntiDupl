using System.Windows;
using System.Windows.Controls;

namespace AntiDupl.NET.WPF.Control;

/// <summary>
///     Логика взаимодействия для ComplexProgressBar.xaml
/// </summary>
public partial class ComplexProgressBar : UserControl
{
    public static readonly DependencyProperty MaximumProperty =
        DependencyProperty.Register(nameof(Maximum), typeof(uint), typeof(ComplexProgressBar));


    public static readonly DependencyProperty CurrentFirstProperty =
        DependencyProperty.Register(nameof(CurrentFirst), typeof(uint), typeof(ComplexProgressBar),
            new PropertyMetadata(OnCurrentFirstChanged));

    public static readonly DependencyProperty CurrentSecondProperty =
        DependencyProperty.Register(nameof(CurrentSecond), typeof(uint), typeof(ComplexProgressBar),
            new PropertyMetadata(OnCurrentSecondChanged));

    public ComplexProgressBar()
    {
        InitializeComponent();
        ProgressText.Text = "Loading...";
    }

    public uint Maximum
    {
        get => (uint)GetValue(MaximumProperty);
        set => SetValue(MaximumProperty, value);
    }

    public uint CurrentFirst
    {
        get => (uint)GetValue(CurrentFirstProperty);
        set => SetValue(CurrentFirstProperty, value);
    }

    public uint CurrentSecond
    {
        get => (uint)GetValue(CurrentSecondProperty);
        set => SetValue(CurrentSecondProperty, value);
    }

    private static void OnCurrentFirstChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
    {
        if (sender is not ComplexProgressBar myControl) return;

        if (myControl.ActualWidth > 0)
            myControl.ProgressBarForeground.Width =
                (myControl.ActualWidth - 2) * myControl.CurrentFirst / myControl.Maximum;

        if (myControl.Maximum > 0)
            myControl.ProgressText.Text =
                (myControl.CurrentFirst / (double)myControl.Maximum * 100).ToString("F2") + "%";
        else
            myControl.ProgressText.Text = "";
    }

    private static void OnCurrentSecondChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
    {
        if (sender is not ComplexProgressBar myControl) return;

        if (myControl.ActualWidth > 0)
            myControl.ProgressBarForeground2.Width =
                (myControl.ActualWidth - 2) * myControl.CurrentSecond / myControl.Maximum;
    }
}