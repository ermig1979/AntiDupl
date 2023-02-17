using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.UndoRedo;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.ObjectModel.ConfigurableAction
{
   /* class DeleteBadAction : IConfigurableAction
    {
        private ImageInfoClass _bad;
        DuplPairViewModel _item;

        public DeleteBadAction(ImageInfoClass bad, DuplPairViewModel item)
        {
            this._bad = bad;
            _item = item;
        }

        public string Name 
        {
            get { return String.Format("Delete {0}", _bad.FileName); }
        }

        public override string ToString()
        {
            return Name;
        }

        public void Excute(UndoRedo.IUndoRedoEngine undoRedoEngine, ObservableCollection<DuplPairViewModel> resultList, 
            IEditableCollectionView resultCollectionView)
        {
            IUCommand deleteImageUndoCommand = new DeleteImageCommand(_item, _bad, resultList, resultCollectionView);

            undoRedoEngine.ExecuteCommand(deleteImageUndoCommand);
        }


    }*/
}
