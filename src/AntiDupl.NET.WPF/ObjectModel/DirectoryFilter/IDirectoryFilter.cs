using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDupl.NET.WPF.ObjectModel.DirectoryFilter
{
    public interface IDirectoryFilter
    {
        string Name { get; }

        bool IsSelect(ViewModel.DuplPairViewModel pair);

        bool IsSelect(DuplicateGroup group);
    }
}
