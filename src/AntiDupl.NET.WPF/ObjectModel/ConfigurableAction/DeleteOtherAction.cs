using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDupl.NET.WPF.UndoRedo;

namespace AntiDupl.NET.WPF.ObjectModel.ConfigurableAction
{
    class DeleteOtherAction : IConfigurableAction
    {
        private ImageInfoClass _bestsByPath;
        IEnumerable<ImageInfoClass> _forDelete;
        IUndoRedoEngine _undoRedoEngine;

        public DeleteOtherAction(ImageInfoClass bestsByPath, DuplicateGroup group, IUndoRedoEngine undoRedoEngine)
        {
            // TODO: Complete member initialization
            this._bestsByPath = bestsByPath;
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

            IUCommand deleteImageUndoCommand = new DeleteOtherCommand(_bestsByPath, _forDelete);

            _undoRedoEngine.ExecuteCommand(deleteImageUndoCommand);

            List<ImageInfoClass> removed = new List<ImageInfoClass>(_forDelete);
            removed.Add(_bestsByPath);
            var forDeletePuplPair = resultList.Where(d => removed.Any(r => r.Path == d.FirstFile.Path)
                && removed.Any(r => r.Path == d.SecondFile.Path)).ToList();
            foreach (var item in forDeletePuplPair)
            {
                resultList.Remove(item);
            }
            //((ICollectionView)resultCollectionView).Refresh();

            return true;
        }


        public string BestImage
        {
            get { return _bestsByPath.FileName; }
        }

        public string BestByPath
        {
            get { return _bestsByPath.FileName; }
        }
    }
}
