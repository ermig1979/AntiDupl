using System.ComponentModel;
using System.Windows;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.View;

/// <summary>
///     Логика взаимодействия для ProgressDialog.xaml
/// </summary>
public partial class ProgressDialog : Window
{
    public ProgressDialog()
    {
        InitializeComponent();
    }

    protected override void OnClosing(CancelEventArgs e)
    {
        base.OnClosing(e);
        var vm = (ProgressDialogViewModel)DataContext;
        if (vm.IsCancelled) return;
        
        //CancelCommand will close the window
        e.Cancel = true;
        vm.CancelCommand.Execute(null);
    }
}