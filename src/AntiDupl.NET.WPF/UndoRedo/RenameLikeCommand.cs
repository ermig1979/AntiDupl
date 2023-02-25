using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.UndoRedo
{
    class RenameLikeCommand : IUCommand
    {
        private string _oldPath;
        private string _newPath;
        DuplPairViewModel _result;
        PositionInDuplPair _place;

        public RenameLikeCommand(DuplPairViewModel result, string newPath, PositionInDuplPair place)
        {
            _newPath = newPath;
            _result = result;
            _place = place;

            //_oldPath = oldPath;
            if (place == PositionInDuplPair.First)
                _oldPath = result.FirstFile.Path;
            else if (place == PositionInDuplPair.Second)
                _oldPath = result.SecondFile.Path;
            else
                throw new ArgumentException("Неизвестно значение PositionInDuplPair " + place);
        }


        public bool Execute()
        {
            if (File.Exists(_oldPath))
            {
                FileInfo file = new FileInfo(_oldPath);
                file.MoveTo(_newPath);
                if (_place == PositionInDuplPair.First)
                    _result.FirstFile.Path = _newPath;
                else if (_place == PositionInDuplPair.Second)
                    _result.SecondFile.Path = _newPath;
                return true;
            }
            return false;
        }

        public bool UnExecute()
        {
            if (File.Exists(_newPath) && !File.Exists(_oldPath))
            {
                FileInfo file = new FileInfo(_newPath);
                file.MoveTo(_oldPath);
                if (_place == PositionInDuplPair.First)
                    _result.FirstFile.Path = _oldPath;
                else if (_place == PositionInDuplPair.Second)
                    _result.SecondFile.Path = _oldPath;
                return true;
            }
            return false;
        }

        public void Dispose()
        {
        }

        public string Description
        {
            get { return "Переименовали как соседа"; }
        }
    }
}
