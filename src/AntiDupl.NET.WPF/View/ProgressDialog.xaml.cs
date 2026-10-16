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

        var vm = DataContext as ProgressDialogViewModel;

        // Программное закрытие (поиск завершён) или окно уже отменено — просто закрываемся.
        if (vm == null || vm.IsProgrammaticClose || vm.IsCancelled)
            return;

        // Пользователь нажал крестик: останавливаем поиск,
        // окно остаётся открытым (видно статус «Остановлено»);
        // повторный клик по крестику закроет его (см. vm.IsCancelled выше).
        vm.CancelCommand.Execute(null);
        e.Cancel = true;
    }
}