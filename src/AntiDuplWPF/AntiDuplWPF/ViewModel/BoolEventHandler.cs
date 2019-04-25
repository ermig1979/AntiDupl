using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.ViewModel
{
    public class BoolEventHandler : EventArgs
    {
        private bool _dialogResult;

        public BoolEventHandler(bool dialogResult)
        {
            _dialogResult = dialogResult;
        }

        public bool DialogResult
        {
            get { return _dialogResult; }
        }
    }
}
