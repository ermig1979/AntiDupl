using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.ObjectModel.DirectoryFilter
{
    class AnywhereDirectoryFilter : IDirectoryFilter
    {
        public string Name
        {
            get { return "Anywhere"; }
        }

        public bool IsSelect(ViewModel.DuplPairViewModel pair)
        {
            return true;
        }

        public bool IsSelect(DuplicateGroup group)
        {
            return true;
        }
    }
}
