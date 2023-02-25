using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDupl.NET.WPF.ObjectModel;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.Model
{
    public class ViewModeModel : PropertyChangedBase, IViewModeModel
    {
        ViewMode _viewMode;
        public ViewMode ViewMode
        {
            get { return _viewMode; }
            set
            {
                _viewMode = value;
                RaisePropertyChangedEvent("ViewMode");
            }
        }
    }
}
