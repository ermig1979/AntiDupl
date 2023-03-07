using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
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

        //find out, if the close button was pressed or if the window was closed by win.close()
        if (new StackTrace().GetFrames().FirstOrDefault(x => x.GetMethod() == typeof(Window).GetMethod("Close")) != null) return;

        //if close button was pressed, cancel execution
        var vm = (ProgressDialogViewModel)DataContext;
        vm.CancelCommand.Execute(null);
        e.Cancel = true;
    }
}