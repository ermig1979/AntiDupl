using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.ObjectModel;

namespace AntiDuplWPF.Model
{
    public interface IViewModeModel : INotifyPropertyChanged
    {
        ViewMode ViewMode { get; set; }
    }
}
