using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using AntiDuplWPF.Core;
using AntiDuplWPF.Helper;

namespace AntiDuplWPF.ViewModel
{
    public class SearchPathViewModel : PropertyChangedBase
    {
        Location _model;
        public Location Model
        {
            get { return _model; }
        }

        public SearchPathViewModel()
        {
            _model = new Location();
        }

        public SearchPathViewModel(Location sp)
        {
            _model = sp;
            UpdateImage();
        }

        private void UpdateImage()
        {
            Image = IconReader.GetIcon(_model.Path, _model.IsDirectory);
        }

        //Icon _image;
        BitmapSource _image;
        public BitmapSource Image
        {
            get { return _image; }
            set
            {
                _image = value;
                RaisePropertyChangedEvent("Image");
            }
        }

        public bool SearchInSubfolder
        {
            get { return _model.SearchInSubfolder; }
            set
            {
                if (_model.SearchInSubfolder != value)
                {
                    _model.SearchInSubfolder = value;
                    //const string dictionaryKey = "SearchInSubfolder";
                    //if (_searchInSubfolder && !_isDirectory)
                    //    base.AddError(dictionaryKey, "Поиск в поддиректориях можно установить только для директории");
                    //else
                    //    base.RemoveError(dictionaryKey);

                    //ValidateSearchInSubfolder();
                    RaisePropertyChangedEvent("SearchInSubfolder");
                }
            }
        }

        public string Path
        {
            get { return _model.Path; }
            set
            {
                if (value.Length > CoreDll.MAX_PATH_EX)
                    throw new ArgumentOutOfRangeException(String.Format("Длина пути превышает {0} символов", CoreDll.MAX_PATH_EX));

                string error;
                if (!_model.SetPath(value, out error))
                    throw new ApplicationException(error);
                RaisePropertyChangedEvent("Path");
                RaisePropertyChangedEvent("IsDirectory");
                UpdateImage();
            }
        }


        public bool IsDirectory
        {
            get
            {
                if (_model != null)
                    return _model.IsDirectory;
                return true;
            }
        }
    }
}
