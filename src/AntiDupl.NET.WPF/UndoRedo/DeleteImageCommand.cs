using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.Helper;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.UndoRedo
{
    class DeleteImageCommand : IUCommand
    {
        private DuplPairViewModel _result;
        ImageInfoClass _forDelete;
        private ObservableCollection<ViewModel.DuplPairViewModel> _resultList;
        private IEditableCollectionView _resultCollectionView;

        string _deletedFileWithTempExtension;
        string _description;


        public DeleteImageCommand(ViewModel.DuplPairViewModel result, ImageInfoClass forDelete,
            ObservableCollection<DuplPairViewModel> resultList,
            System.ComponentModel.IEditableCollectionView resultCollectionView)
        {
            this._result = result;
            _forDelete = forDelete;
            this._resultList = resultList;
            this._resultCollectionView = resultCollectionView;
        }

        public bool Execute()
        {
            _deletedFileWithTempExtension = _forDelete.Path + Const.ForDeleteEXTENSION;
            File.Move(_forDelete.Path, _deletedFileWithTempExtension);
            _description = String.Format("Удалили {0}", _forDelete.Path);

            _resultList.Remove(_result);

            return true;
        }

        public bool UnExecute()
        {
            File.Move(_deletedFileWithTempExtension, _forDelete.Path);

            _resultList.Add(_result);

            //((ICollectionView)_resultCollectionView).MoveCurrentToPosition(_currentPosition);
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
