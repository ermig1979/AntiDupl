using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDupl.NET.WPF.UndoRedo
{
    public interface IUCommand : IDisposable
    {
        bool Execute();
        bool UnExecute();
        string Description { get; }
    }
}
