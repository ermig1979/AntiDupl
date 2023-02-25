using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.UndoRedo
{
    public interface IUCommand : IDisposable
    {
        bool Execute();
        bool UnExecute();
        string Description { get; }
    }
}
