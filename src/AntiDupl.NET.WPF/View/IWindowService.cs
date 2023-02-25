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
        //System.Windows.Window MainWindow { get; set; }
        ProgressDialogViewModel OpenProgressWindow(ProgressDialogViewModel vm);
        void ShowDialogWindow<T>(object dataContext) where T : System.Windows.Window, new();
        bool? ShowDialogWindow<T>(CloseableViewModel dataContext) where T : System.Windows.Window, new();
        void CloseProgressWindow();
        void OpenComparatorWindow(ComparatorViewModel vm);
        void CloseComparatorWindow();

        
        void ShowPleaseWait();
        void ClosePleaseWait();

        void ShowMessage(string message);
    }
}
