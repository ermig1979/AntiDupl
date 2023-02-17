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
    public class AdvancedOption : INotifyPropertyChanged
    {
        [MarshalAs(UnmanagedType.I4)]
        int _ratioResolution;
        [MarshalAs(UnmanagedType.I4)]
        int _compareThreadCount;
        [MarshalAs(UnmanagedType.I4)]
        int _collectThreadCount;
        [MarshalAs(UnmanagedType.I4)]
        int _reducedImageSize;
        [MarshalAs(UnmanagedType.I4)]
        int _resultCountMax;
        [MarshalAs(UnmanagedType.I4)]
        int _ignoreFrameWidth;
        [MarshalAs(UnmanagedType.U1)]
        bool _useImageDataBase;

        public AdvancedOption()
        {
            _useImageDataBase = true;
            _ratioResolution = 32;
            _reducedImageSize = 32;
            _resultCountMax = 100000;
            _ignoreFrameWidth = 0;
            _collectThreadCount = _compareThreadCount = Environment.ProcessorCount;
        }

        public int RatioResolution
        {
            get { return _ratioResolution; }
            set
            {
                _ratioResolution = value;
                NotifyPropertyChanged("RatioResolution");
            }
        }

        public int CompareThreadCount
        {
            get { return _compareThreadCount; }
            set
            {
                _compareThreadCount = value;
                NotifyPropertyChanged("CompareThreadCount");
            }
        }

        public int CollectThreadCount
        {
            get { return _collectThreadCount; }
            set
            {
                _collectThreadCount = value;
                NotifyPropertyChanged("CollectThreadCount");
            }
        }

        public int ReducedImageSize
        {
            get { return _reducedImageSize; }
            set
            {
                _reducedImageSize = value;
                NotifyPropertyChanged("ReducedImageSize");
            }
        }

        public int ResultCountMax
        {
            get { return _resultCountMax; }
            set
            {
                _resultCountMax = value;
                NotifyPropertyChanged("ResultCountMax");
            }
        }

        public int IgnoreFrameWidth
        {
            get { return _ignoreFrameWidth; }
            set
            {
                _ignoreFrameWidth = value;
                NotifyPropertyChanged("IgnoreFrameWidth");
            }
        }

        public bool UseImageDataBase
        {
            get { return _useImageDataBase; }
            set
            {
                _useImageDataBase = value;
                NotifyPropertyChanged("UseImageDataBase");
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
