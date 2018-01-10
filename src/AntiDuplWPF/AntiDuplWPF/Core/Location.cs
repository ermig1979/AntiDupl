using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.Helper;

namespace AntiDuplWPF.Core
{
    [Serializable]
    [ComVisible(false)]
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public class Location
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = CoreDll.MAX_PATH_EX)]
        string _path;
        [MarshalAs(UnmanagedType.U1)]
        bool _isDirectory;
        [MarshalAs(UnmanagedType.U1)]
        bool _searchInSubfolder;

        //Для сериализации
        public Location()
        {
            //_isDirectory = false;
            //_searchInSubfolder = false;
        }

        public Location(string path)
        {
            string error;
            SetPath(path, out error);
            if (_isDirectory)
                _searchInSubfolder = true;
            else
                _searchInSubfolder = false;
        }
     
        public bool SearchInSubfolder
        {
            get { return _searchInSubfolder; }
            set
            {
                if (_searchInSubfolder != value)
                {
                    _searchInSubfolder = value;
                    //const string dictionaryKey = "SearchInSubfolder";
                    //if (_searchInSubfolder && !_isDirectory)
                    //    base.AddError(dictionaryKey, "Поиск в поддиректориях можно установить только для директории");
                    //else
                    //    base.RemoveError(dictionaryKey);

                    //ValidateSearchInSubfolder();
                    //NotifyPropertyChanged("SearchInSubfolder");
                }
            }
        }

        public string Path
        {
            get { return _path; }
            set
            {
                if (value.Length > CoreDll.MAX_PATH_EX)
                    throw new ArgumentOutOfRangeException(String.Format("Длина пути превышает {0} символов", CoreDll.MAX_PATH_EX));
                _path = value;
                //NotifyPropertyChanged("Path");
            }
        }

        [Browsable(false)]
        //[XmlElement("IsDirectory")]
        public bool IsDirectory
        {
            get { return _isDirectory; }
            set { _isDirectory = value; }
        }

        public bool SetPath(string path, out string error)
        {
            error = String.Empty;
            if (IOHelper.IsDirectory(path))
            {
                _path = path;
                _isDirectory = true;
                return true;
            }
            else if (File.Exists(path))
            {
                _path = path;
                _isDirectory = false;
                return true;
            }
            error = "File or directory not exist!";
            return false;
        }
    }
}
