using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.ObjectModel.ByPath
{
    public interface IBestSelectingByPath
    {
        ImageInfoClass GetBest(DuplicateGroup group, GettingNumber.IGettingNumber algorithmOfGettingNumberFromFileName);

        bool ExistBest(DuplicateGroup group, GettingNumber.IGettingNumber algorithmOfGettingNumberFromFileName);

        string Name { get; }
    }
}
