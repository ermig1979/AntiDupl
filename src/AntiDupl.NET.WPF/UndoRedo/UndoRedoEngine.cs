using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.Model;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.UndoRedo
{
    public class UndoRedoEngine : PropertyChangedBase, INotifyPropertyChanged, IUndoRedoEngine
    {
        private Stack<IUCommand> _undoCommandStack = new Stack<IUCommand>();
        private Stack<IUCommand> _redoCommandStack = new Stack<IUCommand>();
        IConfigurationModel _configuration;

        public UndoRedoEngine(IConfigurationModel configuration)
        {
            _configuration = configuration;
        }

        public bool Undo()
        {
            //if (_UndoStack.Count > 0)
            //{
            //    ListDuplicates = _UndoStack.Pop();
            //    OnActoinAppledEvent();
            //    return true;
            //}
            //return false;
            if (_undoCommandStack.Count > 0)
            {
                IUCommand command = _undoCommandStack.Pop();
                command.UnExecute();
                //ListDuplicates = command.
                _redoCommandStack.Push(command);
                //if (OnActoinAppledEvent != null)
                //    OnActoinAppledEvent();
                RaisePropertyChangedEvent("UndoEnable");
                RaisePropertyChangedEvent("RedoEnable");
                return true;
            }
            return false;
        }

        public bool Redo()
        {
            if (_redoCommandStack.Count > 0)
            {
                IUCommand command = _redoCommandStack.Pop();
                command.Execute();
                _undoCommandStack.Push(command);
                // if (OnActoinAppledEvent != null)
                //    OnActoinAppledEvent();
                RaisePropertyChangedEvent("UndoEnable");
                RaisePropertyChangedEvent("RedoEnable");
                return true;
            }
            return false;
        }

        public string UndoTooltip
        {
            get
            {
                if (_undoCommandStack.Any())
                {
                    IUCommand command = _undoCommandStack.Peek();
                    return command.Description;
                }
                return "";
            }
        }

        public bool UndoEnable
        {
            get
            {
                //return (_UndoStack.Count > 0);
                return (_undoCommandStack.Count > 0);
            }
        }

        public bool RedoEnable
        {
            get
            {
                return (_redoCommandStack.Count > 0);
            }
        }

        public void ExecuteCommand(IUCommand command)
        {
            if (_undoCommandStack.Count + 1 > _configuration.UndoRedoStackSize)
            {
                IUCommand commandForDispose = _undoCommandStack.Pop();
                commandForDispose.Dispose();
            }

            _undoCommandStack.Push(command);
            command.Execute();
            //if (OnActoinAppledEvent != null)
            //   OnActoinAppledEvent();
            RaisePropertyChangedEvent("UndoEnable");
            RaisePropertyChangedEvent("UndoTooltip");
            if (_redoCommandStack.Any())
            {
                _redoCommandStack.Clear();
                RaisePropertyChangedEvent("RedoEnable");
            }
        }

        public void Clear()
        {
            foreach (var item in _undoCommandStack)
            {
                item.Dispose();
            }
        }
    }
}
