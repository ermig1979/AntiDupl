using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.ObjectModel;

namespace AntiDuplWPF.UndoRedo
{
    class RenameImageCommand : IUCommand
    {
        private ImageInfoClass _imageInfo;
        string _description;
        string _oldPath;
        private string _newPath;

        public RenameImageCommand(ImageInfoClass iic, string newPath)
        {
            _imageInfo = iic;
            _newPath = newPath;
        }

        public bool Execute()
        {
            //var dir = Path.GetDirectoryName(_imageInfo.Path);
            //string target = Path.Combine(dir, _imageInfo.FileName);
            //string target = _imageInfo.Path;
            if (Path.GetExtension(_newPath) != Path.GetExtension(_imageInfo.Path))
                throw new ArgumentException("Вы хотите сменить расширение!");
            if (File.Exists(_newPath))
                throw new ArgumentException(String.Format("Файл с именем {0} уже существует!", _newPath));

            _oldPath = _imageInfo.Path;
            File.Move(_imageInfo.Path, _newPath);
            _description = String.Format("Переименовали {0} в {1}", Path.GetFileName(_oldPath), _newPath);
            _imageInfo.Path = _newPath;
            //_path = target;
            //RaisePropertyChangedEvent("FileName");
            return true;
        }

        public bool UnExecute()
        {
            File.Move(_imageInfo.Path, _oldPath);
            _imageInfo.Path = _oldPath;
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
