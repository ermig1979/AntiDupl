using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.Service
{
    public interface ILanguageService
    {
        event EventHandler LanguageChanged;

        CultureInfo Language { get; set; }
        ObservableCollection<LanguageViewModel> LanguageCollection { get; }
        ICommand LanguageSelectedCommand { get; }
    }
}
