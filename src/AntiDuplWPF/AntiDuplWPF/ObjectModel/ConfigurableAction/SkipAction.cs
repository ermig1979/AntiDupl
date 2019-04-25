using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.ObjectModel.ConfigurableAction
{
    public class SkipAction : IConfigurableAction
    {
        private string _message;

        public SkipAction(string message)
        {
            _message = message;
        }

        public string Name
        {
            get { return String.Format("Skip. {0}", _message); }
        }

        public override string ToString()
        {
            return Name;
        }

        public bool Excute(UndoRedo.IUndoRedoEngine undoRedoEngine, 
            System.Collections.ObjectModel.ObservableCollection<ViewModel.DuplPairViewModel> resultList, 
            System.ComponentModel.IEditableCollectionView resultCollectionView)
        {
            return true;
        }
    }
}
