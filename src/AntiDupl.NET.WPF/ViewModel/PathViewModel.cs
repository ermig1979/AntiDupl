using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
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

        public PathViewModel(Model.LocationsModel LocationsModel)
        {
            // TODO: Complete member initialization
            this._locationsModel = LocationsModel;
        }

        #region IDropable Members

        void IDropable.Drop(object dropData)
        {
            var filepaths = dropData as string[];
            if (filepaths != null)
            {
                foreach (string path in filepaths)
                {
                    if (!String.IsNullOrEmpty(path))
                    {
                        LocationsModel.AddSearchPath(path);
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
                        foreach (var item in dlg.FileNames)
                        {
                            LocationsModel.AddSearchPath(item);
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
                        foreach (var item in openFileDialog.FileNames)
                        {
                            LocationsModel.AddSearchPath(item);
                        }
                        LocationsModel.CopyToDll();
                    }
                }));
            }
        }
    }
}
