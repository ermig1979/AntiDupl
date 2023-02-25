using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.UndoRedo
{
    public interface IUndoRedoEngine : INotifyPropertyChanged
    {
        void ExecuteCommand(IUCommand command);
        bool Redo();
        bool RedoEnable { get; }
        bool Undo();
        bool UndoEnable { get; }

        void Clear();

        string UndoTooltip { get; }
    }
}
