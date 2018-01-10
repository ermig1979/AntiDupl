using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.UndoRedo
{
    class IgnorePairCommand : IUCommand
    {
        List<DuplPairViewModel> _ignoreList;
        private ObservableCollection<DuplPairViewModel> _resultList;
        IEditableCollectionView _collectionView;

        DuplPairViewModel _ignoredResult;
        int _index;
        string _description;

        public IgnorePairCommand(ObservableCollection<DuplPairViewModel> resultList, IEditableCollectionView collectionView,
            List<DuplPairViewModel> ignoreList, int index)
        {
            _resultList = resultList;
            _collectionView = collectionView;
            _ignoreList = ignoreList;
            _index = index;
            //_dispatcher = dispatcher;
        }

        public bool Execute()
        {
            // _index = ((ICollectionView)_collectionView).CurrentPosition;
            //_collectionView.EditItem(_resultList[_index]);
            _ignoredResult = _resultList[_index];
            //_dispatcher.Invoke(() =>
            //{
            _resultList.RemoveAt(_index);
            //});
            _ignoreList.Add(_ignoredResult);
            //_collectionView.Refresh();
            //http://drwpf.com/blog/2008/10/20/itemscontrol-e-is-for-editable-collection/
            //_collectionView.CommitEdit();
            //_collectionView.RemoveAt(_index);
            string res = Application.Current.Resources["desc_ignorePair"] as string;
            _description = String.Format(res, _ignoredResult.FirstFile.Path, _ignoredResult.SecondFile.Path);
            return true;
        }

        public bool UnExecute()
        {
            //_collectionView
            _resultList.Insert(_index, _ignoredResult);
            _ignoreList.Remove(_ignoredResult);
            ((ICollectionView)_collectionView).MoveCurrentToPosition(_index);
            //_collectionView.CommitEdit();
            //_collectionView.CommitNew();
            //_collectionView.Refresh();
            return true;
        }

        public void Dispose()
        {
        }

        public string Description
        {
            get { return _description; }
        }
    }
}
