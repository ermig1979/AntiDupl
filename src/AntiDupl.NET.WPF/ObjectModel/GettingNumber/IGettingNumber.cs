using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.ObjectModel.GettingNumber
{
    public interface IGettingNumber
    {
        string Name { get; }

        bool ContainNumber(ViewModel.DuplPairViewModel result);

        bool ContainNumber(DuplicateGroup group);

        uint? GetNumber(string fileName);
    }
}
