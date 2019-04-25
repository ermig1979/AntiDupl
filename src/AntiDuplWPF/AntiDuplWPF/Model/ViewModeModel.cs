using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.Model
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
