using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDupl.NET.WPF.Helper;

namespace AntiDupl.NET.WPF.UndoRedo
{
    class DeleteMultiCommand : IUCommand
    {
        private IEnumerable<ObjectModel.ImageInfoClass> _forDelete;
        List<RenamedImage> _renamedList = new List<RenamedImage>();

        public DeleteMultiCommand(IEnumerable<ObjectModel.ImageInfoClass> _forDelete)
        {
            // TODO: Complete member initialization
            this._forDelete = _forDelete;
        }

        public bool Execute()
        {
            foreach (var item in _forDelete)
            {
                var renamed = new RenamedImage(item);
                _renamedList.Add(renamed);
                File.Move(renamed.ImageInfo.Path, renamed.PathWithTempExtension);
            }

            _description = String.Format("Deleted {0} files", _renamedList.Count);

            return true;
        }

        public bool UnExecute()
        {
            foreach (var image in _renamedList)
            {
                File.Move(image.PathWithTempExtension, image.ImageInfo.Path);
            }

            return true;
        }

        string _description;
        public string Description
        {
            get { return _description; }
        }

        public void Dispose()
        {
            foreach (var image in _renamedList)
            {
                File.Move(image.PathWithTempExtension, image.ImageInfo.Path);
                WinAPIMethods.DeleteToRecycleBin(image.ImageInfo.Path);
            }
        }
    }
}
