using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using AntiDuplWPF.Helper;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.UndoRedo
{
    class DeleteOtherFromPairCommand : IUCommand
    {
        private ImageInfoClass _forSaveInfo;
        private DuplPairViewModel _result;
        private ObservableCollection<DuplPairViewModel> _resultList;
        private IEditableCollectionView _collectionView;

        private int _index;
        string _description;
        private ImageInfoClass _forDeleteInfo;
        string _pathWithTempExtension;

        public DeleteOtherFromPairCommand(ImageInfoClass currentInfo, DuplPairViewModel result,
            ObservableCollection<DuplPairViewModel> resultList, IEditableCollectionView collectionView, int index)
        {
            _forSaveInfo = currentInfo;
            _result = result;
            _resultList = resultList;
            _collectionView = collectionView;
            _index = index;
        }

        public bool Execute()
        {
            //_index = ((ICollectionView)_collectionView).CurrentPosition;

            if (_result.FirstFile.Equals(_forSaveInfo))
                _forDeleteInfo = _result.SecondFile;
            else if (_result.SecondFile.Equals(_forSaveInfo))
                _forDeleteInfo = _result.FirstFile;
            else
                throw new ArgumentException("Не знаем что удалять!");

            _pathWithTempExtension = _forDeleteInfo.Path + Const.ForDeleteEXTENSION;
            File.Move(_forDeleteInfo.Path, _pathWithTempExtension);

            _resultList.RemoveAt(_index);

            string res = Application.Current.Resources["desc_deleteOtherFromPair"] as string;
            _description = String.Format(res, _forSaveInfo.Path);

            return true;
        }

        public bool UnExecute()
        {
            File.Move(_pathWithTempExtension, _forDeleteInfo.Path);
            _resultList.Insert(_index, _result);
            return true;
        }

        public string Description
        {
            get { return _description; }
        }

        public void Dispose()
        {
            File.Move(_pathWithTempExtension, _forDeleteInfo.Path);
            WinAPIMethods.DeleteToRecycleBin(_forDeleteInfo.Path);
        }
    }
}
