using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.UndoRedo
{
    class IgnoreMultiPairCommand : IUCommand
    {
        private IList _resultsForIgnore;
        private List<DuplPairViewModel> _resultsForIgnoreCopy;
        private ObservableCollection<DuplPairViewModel> _sourceResultList;
        private List<DuplPairViewModel> _ignoreList;
        string _description;


        public IgnoreMultiPairCommand(IList resultsForIgnore,
            ObservableCollection<DuplPairViewModel> sourceResultList,
            List<DuplPairViewModel> ignoreList)
        {
            _resultsForIgnore = resultsForIgnore;
            _sourceResultList = sourceResultList;
            _ignoreList = ignoreList;
            _resultsForIgnoreCopy = resultsForIgnore.Cast<DuplPairViewModel>().ToList();
        }

        public bool Execute()
        {
            foreach (DuplPairViewModel item in _resultsForIgnoreCopy)
            {
                _sourceResultList.Remove(item);
                _ignoreList.Add(item);
            }
            string res = Application.Current.Resources["desc_ignorePairs"] as string;
            _description = String.Format(res, _resultsForIgnoreCopy.Count);
            return true;
        }

        public bool UnExecute()
        {
            foreach (DuplPairViewModel item in _resultsForIgnoreCopy)
            {
                _sourceResultList.Add(item);
                _ignoreList.Remove(item);
            }
            return true;
        }

        public string Description
        {
            get { return _description; }
        }

        public void Dispose()
        {
        }
    }
}
