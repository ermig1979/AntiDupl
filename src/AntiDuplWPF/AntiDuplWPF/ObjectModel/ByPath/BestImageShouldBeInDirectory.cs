using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.ObjectModel.ByPath
{
    class BestImageShouldBeInDirectory : IBestSelectingByPath
    {
        public ImageInfoClass GetBest(DuplicateGroup group, GettingNumber.IGettingNumber algorithmOfGettingNumberFromFileName)
        {
            throw new NotImplementedException();
        }


        public bool ExistBest(DuplicateGroup group, GettingNumber.IGettingNumber algorithmOfGettingNumberFromFileName)
        {
            throw new NotImplementedException();
        }


        public string Name
        {
            get { return "BestImageShouldBeInDirectory"; }
        }
    }
}
