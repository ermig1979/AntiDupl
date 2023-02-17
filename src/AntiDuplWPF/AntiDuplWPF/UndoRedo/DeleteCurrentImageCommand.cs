using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.Helper;
using AntiDuplWPF.ObjectModel;

namespace AntiDuplWPF.UndoRedo
{
    class DeleteCurrentImageCommand : IUCommand
    {
        private ViewModel.DuplPairViewModel _result;
        private ObjectModel.PositionInDuplPair _positionInDuplPair;
        private System.Collections.ObjectModel.ObservableCollection<ViewModel.DuplPairViewModel> _resultList;
        private System.ComponentModel.IEditableCollectionView _resultCollectionView;

        string _deletedFileWithTempExtension;
        string _description;
        int _currentPosition;

        public DeleteCurrentImageCommand(ViewModel.DuplPairViewModel result, PositionInDuplPair positionInDuplPair, 
            System.Collections.ObjectModel.ObservableCollection<ViewModel.DuplPairViewModel> resultList,
            System.ComponentModel.IEditableCollectionView resultCollectionView)
        {
            this._result = result;
            this._positionInDuplPair = positionInDuplPair;
            this._resultList = resultList;
            this._resultCollectionView = resultCollectionView;
        }

        public bool Execute()
        {
            //удаляем первую картинку
            if (_positionInDuplPair == PositionInDuplPair.First)
            {
                _deletedFileWithTempExtension = _result.FirstFile.Path + Const.ForDeleteEXTENSION;
                File.Move(_result.FirstFile.Path, _deletedFileWithTempExtension);
                _description = String.Format("Удалили {0}", _result.FirstFile.Path);
            }  //удаляем вторую картинку
            else if (_positionInDuplPair == PositionInDuplPair.Second)
            {
                _deletedFileWithTempExtension = _result.SecondFile.Path + Const.ForDeleteEXTENSION;
                File.Move(_result.SecondFile.Path, _deletedFileWithTempExtension);
                _description = String.Format("Удалили {0}", _result.SecondFile.Path, _result.FirstFile.Path);
            }
            else
                throw new ArgumentException("Неизвестный тип PositionInDuplPair " + _positionInDuplPair);


            _currentPosition = ((ICollectionView)_resultCollectionView).CurrentPosition;
            _resultList.RemoveAt(_currentPosition);

            return true;
        }

        public bool UnExecute()
        {
            if (_positionInDuplPair == PositionInDuplPair.First)
            {
                File.Move(_deletedFileWithTempExtension, _result.FirstFile.Path);
            }
            else if (_positionInDuplPair == PositionInDuplPair.Second)
            {
                File.Move(_deletedFileWithTempExtension, _result.SecondFile.Path);
            }
            else
                throw new ArgumentException("Неизвестный тип PositionInDuplPair " + _positionInDuplPair);
            _resultList.Insert(_currentPosition, _result);

            ((ICollectionView)_resultCollectionView).MoveCurrentToPosition(_currentPosition);
            return true;
        }

        public string Description
        {
            get { return _description; }
        }

        public void Dispose()
        {
            WinAPIMethods.DeleteToRecycleBin(_deletedFileWithTempExtension);
        }
    }
}
