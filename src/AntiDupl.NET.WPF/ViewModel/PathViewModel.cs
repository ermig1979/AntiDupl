using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using AntiDupl.NET.Core.Original;
using PathType = AntiDupl.NET.Core.Original.CoreDll.PathType;
using AntiDupl.NET.WPF.Command;
using AntiDupl.NET.WPF.DragDrop;
using AntiDupl.NET.WPF.Model;
using Microsoft.Win32;
using Microsoft.WindowsAPICodePack.Dialogs;

namespace AntiDupl.NET.WPF.ViewModel
{
    class PathViewModel : PropertyChangedBase, IDropable
    {
        private LocationsModel _locationsModel;
        public LocationsModel LocationsModel 
        { 
            get { return _locationsModel; } 
        }

        // Связь с TabControl.SelectedIndex в PathWindow.xaml (Mode=TwoWay).
        private int _selectedTabPageIndex;
        public int SelectedTabPageIndex
        {
            get { return _selectedTabPageIndex; }
            set
            {
                if (_selectedTabPageIndex != value)
                {
                    _selectedTabPageIndex = value;
                    this.RaisePropertyChangedEvent();
                }
            }
        }

        public PathViewModel(Model.LocationsModel LocationsModel)
        {
            // TODO: Complete member initialization
            this._locationsModel = LocationsModel;
        }

        /// <summary>
        /// окно путей: вид путей, соответствующий активной вкладке окна
        /// (0=Search, 1=Ignore, 2=Valid, 3=Delete — порядок вкладок в PathWindow.xaml).
        /// </summary>
        public const int PATH_TYPE_TAB_SEARCH = 0;
        public const int PATH_TYPE_TAB_IGNORE = 1;
        public const int PATH_TYPE_TAB_VALID = 2;
        public const int PATH_TYPE_TAB_DELETE = 3;

        PathType CurrentPathType()
        {
            switch (_selectedTabPageIndex)
            {
                case PATH_TYPE_TAB_IGNORE:
                    return PathType.Ignore;
                case PATH_TYPE_TAB_VALID:
                    return PathType.Valid;
                case PATH_TYPE_TAB_DELETE:
                    return PathType.Delete;
                case PATH_TYPE_TAB_SEARCH:
                default:
                    return PathType.Search;
            }
        }

        #region IDropable Members

        void IDropable.Drop(object dropData)
        {
            var filepaths = dropData as string[];
            if (filepaths != null)
            {
                PathType type = CurrentPathType();
                foreach (string path in filepaths)
                {
                    if (!String.IsNullOrEmpty(path))
                    {
                        LocationsModel.AddPath(path, type);
                    }
                }
                LocationsModel.CopyToDll();
            }
        }

        #endregion

        ICommand _selectDirectoryCommand;
        public ICommand SelectDirectoryCommand
        {
            get
            {
                return _selectDirectoryCommand ?? (_selectDirectoryCommand = new RelayCommand(arg =>
                {
                    var dlg = new CommonOpenFileDialog();
                    //dlg.Title = "My Title";
                    dlg.IsFolderPicker = true;

                    dlg.AddToMostRecentlyUsedList = false;
                    dlg.AllowNonFileSystemItems = false;
                    dlg.EnsureFileExists = true;
                    dlg.EnsurePathExists = true;
                    dlg.EnsureReadOnly = false;
                    dlg.EnsureValidNames = true;
                    dlg.Multiselect = true;
                    dlg.ShowPlacesList = true;

                    if (dlg.ShowDialog() == CommonFileDialogResult.Ok)
                    {
                        //var folder = dlg.FileName;
                        PathType type = CurrentPathType();
                        foreach (var item in dlg.FileNames)
                        {
                            LocationsModel.AddPath(item, type);
                        }
                        LocationsModel.CopyToDll();
                    }
                }));
            }
        }

        ICommand _selectImagesCommand;
        public ICommand SelectImagesCommand
        {
            get
            {
                return _selectImagesCommand ?? (_selectImagesCommand = new RelayCommand(arg =>
                {
                    OpenFileDialog openFileDialog = new OpenFileDialog();
                    openFileDialog.Multiselect = true;
                    //openFileDialog.Filter = "Text files (*.txt)|*.txt|All files (*.*)|*.*";
                    //openFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
                    if (openFileDialog.ShowDialog() == true)
                    {
                        PathType type = CurrentPathType();
                        foreach (var item in openFileDialog.FileNames)
                        {
                            LocationsModel.AddPath(item, type);
                        }
                        LocationsModel.CopyToDll();
                    }
                }));
            }
        }
    }
}
