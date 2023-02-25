using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDupl.NET.WPF.UndoRedo;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.ObjectModel.ConfigurableAction
{
    class ReplaceAndDeleteAction : IConfigurableAction
    {
        private ImageInfoClass _bestsByPath;
        private IEnumerable<ImageInfoClass> _bests;
        private DuplicateGroup _group;

        ImageInfoClass _forReplace;
        IEnumerable<ImageInfoClass> _forDelete;
        IUndoRedoEngine _undoRedoEngine;

        public ReplaceAndDeleteAction(ImageInfoClass bestsByPath, IEnumerable<ImageInfoClass> bests, DuplicateGroup group,
            IUndoRedoEngine undoRedoEngine)
        {
            // TODO: Complete member initialization
            this._bestsByPath = bestsByPath;
            this._bests = bests;
            this._group = group;
            _undoRedoEngine = undoRedoEngine;

            _forReplace = bests.First();
            _forDelete = group.FileList.Where(f => f.FileName != _bestsByPath.FileName 
                && f.FileName != _forReplace.FileName);
        }

        public string Name
        {
            get { return String.Format("Replace {0} by {1}{2}", _bestsByPath.FileName, _forReplace.FileName,
                _forDelete.Any() ? String.Format(", delete {0}", String.Join(",", _forDelete.Select(f => f.FileName).ToArray())) : ""
                );
            }
        }

        public bool Excute(UndoRedo.IUndoRedoEngine undoRedoEngine, 
            ObservableCollection<DuplPairViewModel> resultList, 
            IEditableCollectionView resultCollectionView)
        {
            if (!File.Exists(_bestsByPath.Path)
                || !File.Exists(_forReplace.Path)
                || _forDelete.Any(i => !File.Exists(i.Path)))
                return false;

            IUCommand replace = new Replace2Command(_bestsByPath, _forReplace);
            _undoRedoEngine.ExecuteCommand(replace);

            IUCommand deleteMulti = new DeleteMultiCommand(_forDelete);
            _undoRedoEngine.ExecuteCommand(deleteMulti);

            List<ImageInfoClass> removed = new List<ImageInfoClass>(_forDelete);
            removed.Add(_bestsByPath);
            removed.Add(_forReplace);
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
            get { return _forReplace.FileName; }
        }

        public string BestByPath
        {
            get { return _bestsByPath.FileName; }
        }
    }
}
