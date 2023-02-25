using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDupl.NET.WPF.Helper;
using AntiDupl.NET.WPF.ObjectModel;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.UndoRedo
{
    class ReplaceCommand : IUCommand
    {
        private DuplPairViewModel _result;
        //First - заменяем первую второй картинкой
        //Second - заменяем вторую первой картинкой
        private PositionInDuplPair _positionInDuplPair;
        private ObservableCollection<DuplPairViewModel> _resultList;
        private IEditableCollectionView _collectionView;

        string _description;
        //int _index;
        string _deletedFilePathWithTempExtension;
        //string _movedFileOriginalPath;
        int _currentPosition;

        public ReplaceCommand(DuplPairViewModel result, PositionInDuplPair positionInDuplPair,
            ObservableCollection<DuplPairViewModel> resultList, IEditableCollectionView resultView)
        {
            _result = result;
            _positionInDuplPair = positionInDuplPair;
            _resultList = resultList;
            _collectionView = resultView;
        }

        public bool Execute()
        {
            //_index = ((ICollectionView)_collectionView).CurrentPosition;
            //_ignoredResult = _resultList[_index];

            //заменяем первую второй картинкой
            if (_positionInDuplPair == PositionInDuplPair.First)
            {
                _deletedFilePathWithTempExtension = _result.FirstFile.Path + Const.ForDeleteEXTENSION;
                File.Move(_result.FirstFile.Path, _deletedFilePathWithTempExtension);
                //_movedFileOriginalPath = _result.SecondFile.Path;
                File.Move(_result.SecondFile.Path, _result.FirstFile.Path);
                _description = String.Format("Заменили {0} {1}", _result.FirstFile.Path, _result.SecondFile.Path);
            }  //заменяем вторую картинкой первой
            else if (_positionInDuplPair == PositionInDuplPair.Second)
            {
                _deletedFilePathWithTempExtension = _result.SecondFile.Path + Const.ForDeleteEXTENSION;
                File.Move(_result.SecondFile.Path, _deletedFilePathWithTempExtension);
                //_movedFileOriginalPath = _result.FirstFile.Path;
                File.Move(_result.FirstFile.Path, _result.SecondFile.Path);
                _description = String.Format("Заменили {0} {1}", _result.SecondFile.Path, _result.FirstFile.Path);
            }
            else
                throw new ArgumentException("Неизвестный тип PositionInDuplPair " + _positionInDuplPair);

            //_resultList.RemoveAt(_index);
            _currentPosition = ((ICollectionView)_collectionView).CurrentPosition;
            _resultList.RemoveAt(_currentPosition);

            return true;
        }

        public bool UnExecute()
        {
            if (_positionInDuplPair == PositionInDuplPair.First)
            {
                File.Move(_result.FirstFile.Path, _result.SecondFile.Path);
                File.Move(_deletedFilePathWithTempExtension, _result.FirstFile.Path);
            }
            else if (_positionInDuplPair == PositionInDuplPair.Second)
            {
                File.Move(_result.SecondFile.Path, _result.FirstFile.Path);
                File.Move(_deletedFilePathWithTempExtension, _result.SecondFile.Path);
            }
            else
                throw new ArgumentException("Неизвестный тип PositionInDuplPair " + _positionInDuplPair);
            //_resultList.Insert(_index, _result);
            _resultList.Insert(_currentPosition, _result);

            ((ICollectionView)_collectionView).MoveCurrentToPosition(_currentPosition);
            return true;
        }

        public string Description
        {
            get { return _description; }
        }

        public void Dispose()
        {
            WinAPIMethods.DeleteToRecycleBin(_deletedFilePathWithTempExtension);
        }
    }
}
