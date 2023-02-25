using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDupl.NET.WPF.ObjectModel.DirectoryFilter
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
