using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.Helper;

namespace AntiDuplWPF.UndoRedo
{
    class Replace2Command : IUCommand
    {
        private ObjectModel.ImageInfoClass _replaced;
        private ObjectModel.ImageInfoClass _forReplace;
        string _deletedFilePathWithTempExtension;
        //string _movedFileOriginalPath;

        public Replace2Command(ObjectModel.ImageInfoClass replaced, ObjectModel.ImageInfoClass forReplace)
        {
            this._replaced = replaced;
            this._forReplace = forReplace;
        }

        public bool Execute()
        {
            _deletedFilePathWithTempExtension = _replaced.Path + Const.ForDeleteEXTENSION;
            File.Move(_replaced.Path, _deletedFilePathWithTempExtension);
            //_movedFileOriginalPath = _forReplace.Path;
            File.Move(_forReplace.Path, _replaced.Path);
            _description = String.Format(@"Заменили {0} на {1}", _replaced.Path, _forReplace.Path);

            return true;
        }

        public bool UnExecute()
        {
            File.Move(_replaced.Path, _forReplace.Path);
            File.Move(_deletedFilePathWithTempExtension, _replaced.Path);

            return true;
        }

        string _description;
        public string Description
        {
            get { return _description; }
        }

        public void Dispose()
        {
            WinAPIMethods.DeleteToRecycleBin(_deletedFilePathWithTempExtension);
        }
    }
}
