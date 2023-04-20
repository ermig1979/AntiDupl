using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDupl.NET.Core;
using AntiDupl.NET.WPF.Core;

namespace AntiDupl.NET.WPF.ViewModel
{
    public class CompareOptionViewModel : PropertyChangedBase
    {
        private readonly CoreOptions _option;

        public CompareOptionViewModel(CoreOptions option)
        {
            _option = option;
        }

        public int ThresholdDifference
        {
            get { return _option.CompareOptions.ThresholdDifference; }
            set
            {
                _option.CompareOptions.ThresholdDifference = value;
                RaisePropertyChangedEvent();
            }
        }
    }
}
