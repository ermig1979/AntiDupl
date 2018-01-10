using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.DragDrop;
using AntiDuplWPF.Model;

namespace AntiDuplWPF.ViewModel
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
                        LocationsModel.AddPath(path);
                    }
                }
                LocationsModel.CopyToDll();
            }
        }

        #endregion
    }
}
