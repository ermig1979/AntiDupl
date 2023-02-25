using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDupl.NET.WPF.ObjectModel.DirectoryFilter
{
    class SameDirectory : IDirectoryFilter
    {
        public string Name
        {
            get { return "SameDirectory"; }
        }


        public bool IsSelect(ViewModel.DuplPairViewModel pair)
        {
            return pair.FirstFile.Directory == pair.SecondFile.Directory;
        }


        public bool IsSelect(DuplicateGroup group)
        {
            var firstMatch = group.FileList.FirstOrDefault();

            if (firstMatch != null)
            {
                return group.FileList.All(e => e.Directory == firstMatch.Directory);
            }

            return false;
        }
    }
}
