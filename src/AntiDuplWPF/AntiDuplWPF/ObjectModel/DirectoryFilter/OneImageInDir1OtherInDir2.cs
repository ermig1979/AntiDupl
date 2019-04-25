using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.ObjectModel.DirectoryFilter
{
    class OneImageInDir1OtherInDir2 : IDirectoryFilter
    {
        public string Name
        {
            get { return "OneImageInDir1OtherInDir2"; }
        }

        public bool IsSelect(ViewModel.DuplPairViewModel pair)
        {
            throw new NotImplementedException();
        }


        public bool IsSelect(DuplicateGroup group)
        {
            throw new NotImplementedException();
        }
    }
}
