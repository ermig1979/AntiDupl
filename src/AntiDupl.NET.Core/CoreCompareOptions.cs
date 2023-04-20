/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy 
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
* copies of the Software, and to permit persons to whom the Software is 
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in 
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using AntiDupl.NET.Core.Original;
using AntiDupl.NET.Core.Enums;
using System.Runtime.InteropServices;

namespace AntiDupl.NET.Core
{
    public class CoreCompareOptions : INotifyPropertyChanged
    {
        private bool _checkOnEquality;
        private bool _transformedImage;
        private bool _sizeControl;
        private bool _typeControl;
        private bool _ratioControl;
        private int _thresholdDifference;
        private int _minimalImageSize;
        private int _maximalImageSize;
        private bool _compareInsideOneFolder;
        private bool _compareInsideOneSearchPath;
        private AlgorithmComparing _algorithmComparing;

        public CoreCompareOptions()
        {
        }

        public CoreCompareOptions(CoreCompareOptions compareOptions)
        {
            _checkOnEquality = compareOptions._checkOnEquality;
            _transformedImage = compareOptions._transformedImage;
            _sizeControl = compareOptions._sizeControl;
            _typeControl = compareOptions._typeControl;
            _ratioControl = compareOptions._ratioControl;
			_algorithmComparing = compareOptions._algorithmComparing;
            _thresholdDifference = compareOptions._thresholdDifference;
            _minimalImageSize = compareOptions._minimalImageSize;
            _maximalImageSize = compareOptions._maximalImageSize;
            _compareInsideOneFolder = compareOptions._compareInsideOneFolder;
            _compareInsideOneSearchPath = compareOptions._compareInsideOneSearchPath;
        }

        public CoreCompareOptions(ref CoreDll.adCompareOptions compareOptions)
        {
            _checkOnEquality = compareOptions.checkOnEquality != CoreDll.FALSE;
            _transformedImage = compareOptions.transformedImage != CoreDll.FALSE;
            _sizeControl = compareOptions.sizeControl != CoreDll.FALSE;
            _typeControl = compareOptions.typeControl != CoreDll.FALSE;
            _ratioControl = compareOptions.ratioControl != CoreDll.FALSE;
			_algorithmComparing = compareOptions.algorithmComparing;
            _thresholdDifference = compareOptions.thresholdDifference;
            _minimalImageSize = compareOptions.minimalImageSize;
            _maximalImageSize = compareOptions.maximalImageSize;
            _compareInsideOneFolder = compareOptions.compareInsideOneFolder != CoreDll.FALSE;
            _compareInsideOneSearchPath = compareOptions.compareInsideOneSearchPath != CoreDll.FALSE;
        }

        public void ConvertTo(ref CoreDll.adCompareOptions compareOptions)
        {
            compareOptions.checkOnEquality = _checkOnEquality ? CoreDll.TRUE : CoreDll.FALSE;
            compareOptions.transformedImage = _transformedImage ? CoreDll.TRUE : CoreDll.FALSE;
            compareOptions.sizeControl = _sizeControl ? CoreDll.TRUE : CoreDll.FALSE;
            compareOptions.typeControl = _typeControl ? CoreDll.TRUE : CoreDll.FALSE;
            compareOptions.ratioControl = _ratioControl ? CoreDll.TRUE : CoreDll.FALSE;
			compareOptions.algorithmComparing = _algorithmComparing;
            compareOptions.thresholdDifference = _thresholdDifference;
            compareOptions.minimalImageSize = _minimalImageSize;
            compareOptions.maximalImageSize = _maximalImageSize;
            compareOptions.compareInsideOneFolder = _compareInsideOneFolder ? CoreDll.TRUE : CoreDll.FALSE;
            compareOptions.compareInsideOneSearchPath = _compareInsideOneSearchPath ? CoreDll.TRUE : CoreDll.FALSE;
        }

        public CoreCompareOptions Clone()
        {
            return new CoreCompareOptions(this);
        }

        public bool Equals(CoreCompareOptions compareOptions)
        {
            return
                _checkOnEquality == compareOptions._checkOnEquality &&
                _transformedImage == compareOptions._transformedImage &&
                _sizeControl == compareOptions._sizeControl &&
                _typeControl == compareOptions._typeControl &&
                _ratioControl == compareOptions._ratioControl &&
				_algorithmComparing == compareOptions._algorithmComparing &&
                _thresholdDifference == compareOptions._thresholdDifference &&
                _minimalImageSize == compareOptions._minimalImageSize &&
                _maximalImageSize == compareOptions._maximalImageSize &&
                _compareInsideOneFolder == compareOptions._compareInsideOneFolder &&
                _compareInsideOneSearchPath == compareOptions._compareInsideOneSearchPath;
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

        public bool CompareInsideOneFolder
        {
            get { return _compareInsideOneFolder; }
            set
            {
                _compareInsideOneFolder = value;
                NotifyPropertyChanged("CompareInsideOneFolder");
            }
        }

        public bool CompareInsideOneSearchPath
        {
            get { return _compareInsideOneSearchPath; }
            set
            {
                _compareInsideOneSearchPath = value;
                NotifyPropertyChanged("CompareInsideOneSearchPath");
            }
        }

        public AlgorithmComparing AlgorithmComparing
        {
            get { return _algorithmComparing; }
            set
            {
                _algorithmComparing = value;
                NotifyPropertyChanged("AlgorithmComparing");
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
