using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.ObjectModel
{
    public class Criterion : PropertyChangedBase
    {
        /*string _name;
        public string Name
        {
            get { return _name; }
            set
            {
                _name = value;
                RaisePropertyChangedEvent("Name");
            }
        }*/

        public CriterionType Type { get; set; }

        public string Name
        {
            get
            {
                return Application.Current.Resources["criterion_" + Type.ToString()] as string;
            }
        }

        decimal _weight;
        public decimal Weight
        {
            get { return _weight; }
            set
            {
                _weight = value;
                RaisePropertyChangedEvent("Weight");
            }
        }

        bool _isUse;
        public bool IsUse
        {
            get { return _isUse; }
            set
            {
                _isUse = value;
                RaisePropertyChangedEvent("IsUse");
            }
        }
    }
}
