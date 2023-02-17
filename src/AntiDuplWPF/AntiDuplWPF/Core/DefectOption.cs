using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;


namespace AntiDuplWPF.Core
{
    [Serializable]
    [StructLayout(LayoutKind.Sequential)]
    public class DefectOption : INotifyPropertyChanged
    {
        [MarshalAs(UnmanagedType.U1)]
        bool _checkOnDefect;
        [MarshalAs(UnmanagedType.U1)]
        bool _checkOnBlockiness;
        [MarshalAs(UnmanagedType.I4)]
        int _blockinessThreshold;
        [MarshalAs(UnmanagedType.U1)]
        bool _checkOnBlockinessOnlyNotJpeg;
        [MarshalAs(UnmanagedType.U1)]
        bool _checkOnBlurring;
        [MarshalAs(UnmanagedType.I4)]
        int _blurringThreshold;

        public DefectOption()
        {
            _checkOnDefect = true;
            _checkOnBlockiness = false;
            _blockinessThreshold = 10;
            _checkOnBlockinessOnlyNotJpeg = false;
            _checkOnBlurring = false;
            _blurringThreshold = 4;
        }

        public bool CheckOnDefect
        {
            get { return _checkOnDefect; }
            set
            {
                _checkOnDefect = value;
                NotifyPropertyChanged("CheckOnDefect");
            }
        }

        public bool CheckOnBlockiness
        {
            get { return _checkOnBlockiness; }
            set
            {
                _checkOnBlockiness = value;
                NotifyPropertyChanged("CheckOnBlockiness");
            }
        }

        public int BlockinessThreshold
        {
            get { return _blockinessThreshold; }
            set
            {
                _blockinessThreshold = value;
                NotifyPropertyChanged("BlockinessThreshold");
            }
        }

        public bool CheckOnBlockinessOnlyNotJpeg
        {
            get { return _checkOnBlockinessOnlyNotJpeg; }
            set
            {
                _checkOnBlockinessOnlyNotJpeg = value;
                NotifyPropertyChanged("CheckOnBlockinessOnlyNotJpeg");
            }
        }

        public bool CheckOnBlurring
        {
            get { return _checkOnBlurring; }
            set
            {
                _checkOnBlurring = value;
                NotifyPropertyChanged("CheckOnBlurring");
            }
        }

        public int BlurringThreshold
        {
            get { return _blurringThreshold; }
            set
            {
                _blurringThreshold = value;
                NotifyPropertyChanged("BlurringThreshold");
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
