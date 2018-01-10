using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.View
{
    public interface IWindowService
    {
        ProgressDialogViewModel OpenProgressWindow(ProgressDialogViewModel vm);
        void ShowDialogWindow<T>(object dataContext) where T : System.Windows.Window, new();
        void CloseProgressWindow();
    }
}
