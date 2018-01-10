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
    public class CompareOption : INotifyPropertyChanged
    {
        [MarshalAs(UnmanagedType.U1)]
        bool _checkOnEquality;
        [MarshalAs(UnmanagedType.U1)]
        bool _transformedImage;
        [MarshalAs(UnmanagedType.U1)]
        bool _sizeControl;
        [MarshalAs(UnmanagedType.U1)]
        bool _typeControl;
        [MarshalAs(UnmanagedType.U1)]
        bool _ratioControl;
        [MarshalAs(UnmanagedType.I4)]
        int _thresholdDifference;
        [MarshalAs(UnmanagedType.I4)]
        int _minimalImageSize;
        [MarshalAs(UnmanagedType.I4)]
        int _maximalImageSize;
        [MarshalAs(UnmanagedType.U1)]
        bool _compareInsideOneFolder;
        [MarshalAs(UnmanagedType.U1)]
        bool _compareInsideOneSearchPath;
        [MarshalAs(UnmanagedType.I4)]
        CoreDll.AlgorithmComparing _algorithmComparing;

        public CompareOption()
        {
            _checkOnEquality = true;
        }

        public bool CheckOnEquality
        {
            get { return _checkOnEquality; }
            set
            {
                _checkOnEquality = value;
                NotifyPropertyChanged("CheckOnEquality");
            }
        }

        public bool TransformedImage
        {
            get { return _transformedImage; }
            set
            {
                _transformedImage = value;
                NotifyPropertyChanged("TransformedImage");
            }
        }

        public bool SizeControl
        {
            get { return _sizeControl; }
            set
            {
                _sizeControl = value;
                NotifyPropertyChanged("SizeControl");
            }
        }

        public bool TypeControl
        {
            get { return _typeControl; }
            set
            {
                _typeControl = value;
                NotifyPropertyChanged("TypeControl");
            }
        }

        public bool RatioControl
        {
            get { return _ratioControl; }
            set
            {
                _ratioControl = value;
                NotifyPropertyChanged("RatioControl");
            }
        }

        public int ThresholdDifference
        {
            get { return _thresholdDifference; }
            set
            {
                _thresholdDifference = value;
                NotifyPropertyChanged("ThresholdDifference");
            }
        }

        public int MinimalImageSize
        {
            get { return _minimalImageSize; }
            set
            {
                _minimalImageSize = value;
                NotifyPropertyChanged("MinimalImageSize");
            }
        }

        public int MaximalImageSize
        {
            get { return _maximalImageSize; }
            set
            {
                _maximalImageSize = value;
                NotifyPropertyChanged("MaximalImageSize");
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
