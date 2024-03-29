﻿using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.UndoRedo
{
    class IgnorePairCommand : IUCommand
    {
        List<DuplPairViewModel> _ignoreList;
        private ObservableCollection<DuplPairViewModel> _resultList;
        IEditableCollectionView _collectionView;

        DuplPairViewModel _ignoredResult;
        //int _index;
        int _currentPosition;
        string _description;

        public IgnorePairCommand(ObservableCollection<DuplPairViewModel> resultList, IEditableCollectionView collectionView,
            List<DuplPairViewModel> ignoreList)
        {
            _resultList = resultList;
            _collectionView = collectionView;
            _ignoreList = ignoreList;
            //_index = index;
            //_dispatcher = dispatcher;
        }

        public bool Execute()
        {
            // _index = ((ICollectionView)_collectionView).CurrentPosition;
            //_collectionView.EditItem(_resultList[_index]);
            _currentPosition = ((ICollectionView)_collectionView).CurrentPosition;
            _ignoredResult = _resultList[_currentPosition];
            //_dispatcher.Invoke(() =>
            //{
            _resultList.RemoveAt(_currentPosition);
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
            _resultList.Insert(_currentPosition, _ignoredResult);
            _ignoreList.Remove(_ignoredResult);
            ((ICollectionView)_collectionView).MoveCurrentToPosition(_currentPosition);
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
