using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.Core
{
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
    public class SearchOption : INotifyPropertyChanged
    {
        [MarshalAs(UnmanagedType.U1)]
        bool _system;
        [MarshalAs(UnmanagedType.U1)]
        bool _hidden;

        [MarshalAs(UnmanagedType.U1)]
        bool JPEG;
        [MarshalAs(UnmanagedType.U1)]
        bool BMP;
        [MarshalAs(UnmanagedType.U1)]
        bool GIF;
        [MarshalAs(UnmanagedType.U1)]
        bool PNG;
        [MarshalAs(UnmanagedType.U1)]
        bool TIFF;
        [MarshalAs(UnmanagedType.U1)]
        bool EMF;
        [MarshalAs(UnmanagedType.U1)]
        bool WMF;
        [MarshalAs(UnmanagedType.U1)]
        bool EXIF;
        [MarshalAs(UnmanagedType.U1)]
        bool ICON;
        [MarshalAs(UnmanagedType.U1)]
        bool JP2;
        [MarshalAs(UnmanagedType.U1)]
        bool PSD;
        [MarshalAs(UnmanagedType.U1)]
        bool DDS;
        [MarshalAs(UnmanagedType.U1)]
        bool TGA;

        public SearchOption()
        {
            JPEG = BMP = GIF = PNG = TIFF = EMF = WMF = EXIF = ICON = JP2 = PSD = DDS = TGA = true;
        }


        public bool System
        {
            get { return _system; }
            set
            {
                _system = value;
                NotifyPropertyChanged("System");
            }
        }

        public bool Hidden
        {
            get { return _hidden; }
            set
            {
                _hidden = value;
                NotifyPropertyChanged("Hidden");
            }
        }

        #region Члены INotifyPropertyChanged

        public event PropertyChangedEventHandler PropertyChanged;

        // This method is called by the Set accessor of each property.
        // The CallerMemberName attribute that is applied to the optional propertyName
        // parameter causes the property name of the caller to be substituted as an argument.
        private void NotifyPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }

        #endregion

    }
}
