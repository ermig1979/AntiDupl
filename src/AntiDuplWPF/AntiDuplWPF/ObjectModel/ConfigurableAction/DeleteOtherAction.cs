using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.UndoRedo;

namespace AntiDuplWPF.ObjectModel.ConfigurableAction
{
    class DeleteOtherAction : IConfigurableAction
    {
        private ImageInfoClass _bestsByPath;
        private DuplicateGroup _group;
        IEnumerable<ImageInfoClass> _forDelete;
        IUndoRedoEngine _undoRedoEngine;

        public DeleteOtherAction(ImageInfoClass bestsByPath, DuplicateGroup group, IUndoRedoEngine undoRedoEngine)
        {
            // TODO: Complete member initialization
            this._bestsByPath = bestsByPath;
            this._group = group;
            _undoRedoEngine = undoRedoEngine;

            _forDelete = group.FileList.Where(f => f.FileName != _bestsByPath.FileName);
        }

        public string Name
        {
            get { return String.Format("Keep {0}, delete {1}", _bestsByPath.FileName, 
                String.Join(",", _forDelete.Select(f => f.FileName).ToArray())); }
        }

        public bool Excute(UndoRedo.IUndoRedoEngine undoRedoEngine, 
            ObservableCollection<ViewModel.DuplPairViewModel> resultList, 
            System.ComponentModel.IEditableCollectionView resultCollectionView)
        {
            if (!File.Exists(_bestsByPath.Path)
                || _forDelete.Any(i => !File.Exists(i.Path)))
                        return false;

            IUCommand deleteImageUndoCommand = new DeleteOtherFromGroup2Command(_bestsByPath, _forDelete, _group);

            _undoRedoEngine.ExecuteCommand(deleteImageUndoCommand);

            return true;
        }
    }
}
