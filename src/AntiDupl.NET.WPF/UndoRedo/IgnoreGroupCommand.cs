using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Windows;
using AntiDupl.NET.WPF.ObjectModel;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.UndoRedo
{
    /// <summary>
    /// Команда «Игнорировать группу»: убирает группу из таблицы результатов
    /// и добавляет все её пары в список игнорируемых. Откат возвращает группу
    /// на прежнее место и убирает пары из списка игнорируемых.
    /// </summary>
    class IgnoreGroupCommand : IUCommand
    {
        private readonly ObservableCollection<DuplicateGroup> _groups;
        private readonly ICollectionView _groupsView;
        private readonly IList<DuplPairViewModel> _resultList;
        private readonly List<DuplPairViewModel> _ignoreList;
        private readonly DuplicateGroup _group;
        private int _position = -1;
        private string _description;

        public IgnoreGroupCommand(ObservableCollection<DuplicateGroup> groups, ICollectionView groupsView,
            IList<DuplPairViewModel> resultList, List<DuplPairViewModel> ignoreList, DuplicateGroup group)
        {
            _groups = groups;
            _groupsView = groupsView;
            _resultList = resultList;
            _ignoreList = ignoreList;
            _group = group;
        }

        public string Description
        {
            get { return _description; }
        }

        public bool Execute()
        {
            _position = _groups.IndexOf(_group);

            foreach (var pair in _group.ResultList)
            {
                _ignoreList.Add(pair);
                if (_resultList.Contains(pair))
                    _resultList.Remove(pair);
            }

            _groups.Remove(_group);
            _groupsView.Refresh();

            string format = Application.Current.Resources["desc_ignoreGroup"] as string
                ?? "Added to ignored group ({0} files)";
            _description = String.Format(format, _group.FileList.Count);
            return true;
        }

        public bool UnExecute()
        {
            if (_position < 0 || _position > _groups.Count)
                _position = _groups.Count;
            _groups.Insert(_position, _group);

            foreach (var pair in _group.ResultList)
            {
                _ignoreList.Remove(pair);
                if (!_resultList.Contains(pair))
                    _resultList.Add(pair);
            }

            _groupsView.Refresh();
            return true;
        }

        public void Dispose()
        {
        }
    }
}