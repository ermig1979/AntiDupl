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
        public bool checkOnEquality;
        public bool transformedImage;
        public bool sizeControl;
        public bool typeControl;
        public bool ratioControl;
        public int thresholdDifference;
        public int minimalImageSize;
        public int maximalImageSize;
        public bool compareInsideOneFolder;
        public bool compareInsideOneSearchPath;
        public AlgorithmComparing algorithmComparing;

        public CoreCompareOptions()
        {
        }

        public CoreCompareOptions(CoreCompareOptions compareOptions)
        {
            checkOnEquality = compareOptions.checkOnEquality;
            transformedImage = compareOptions.transformedImage;
            sizeControl = compareOptions.sizeControl;
            typeControl = compareOptions.typeControl;
            ratioControl = compareOptions.ratioControl;
			algorithmComparing = compareOptions.algorithmComparing;
            thresholdDifference = compareOptions.thresholdDifference;
            minimalImageSize = compareOptions.minimalImageSize;
            maximalImageSize = compareOptions.maximalImageSize;
            compareInsideOneFolder = compareOptions.compareInsideOneFolder;
            compareInsideOneSearchPath = compareOptions.compareInsideOneSearchPath;
        }

        public CoreCompareOptions(ref CoreDll.adCompareOptions compareOptions)
        {
            checkOnEquality = compareOptions.checkOnEquality != CoreDll.FALSE;
            transformedImage = compareOptions.transformedImage != CoreDll.FALSE;
            sizeControl = compareOptions.sizeControl != CoreDll.FALSE;
            typeControl = compareOptions.typeControl != CoreDll.FALSE;
            ratioControl = compareOptions.ratioControl != CoreDll.FALSE;
			algorithmComparing = compareOptions.algorithmComparing;
            thresholdDifference = compareOptions.thresholdDifference;
            minimalImageSize = compareOptions.minimalImageSize;
            maximalImageSize = compareOptions.maximalImageSize;
            compareInsideOneFolder = compareOptions.compareInsideOneFolder != CoreDll.FALSE;
            compareInsideOneSearchPath = compareOptions.compareInsideOneSearchPath != CoreDll.FALSE;
        }

        public void ConvertTo(ref CoreDll.adCompareOptions compareOptions)
        {
            compareOptions.checkOnEquality = checkOnEquality ? CoreDll.TRUE : CoreDll.FALSE;
            compareOptions.transformedImage = transformedImage ? CoreDll.TRUE : CoreDll.FALSE;
            compareOptions.sizeControl = sizeControl ? CoreDll.TRUE : CoreDll.FALSE;
            compareOptions.typeControl = typeControl ? CoreDll.TRUE : CoreDll.FALSE;
            compareOptions.ratioControl = ratioControl ? CoreDll.TRUE : CoreDll.FALSE;
			compareOptions.algorithmComparing = algorithmComparing;
            compareOptions.thresholdDifference = thresholdDifference;
            compareOptions.minimalImageSize = minimalImageSize;
            compareOptions.maximalImageSize = maximalImageSize;
            compareOptions.compareInsideOneFolder = compareInsideOneFolder ? CoreDll.TRUE : CoreDll.FALSE;
            compareOptions.compareInsideOneSearchPath = compareInsideOneSearchPath ? CoreDll.TRUE : CoreDll.FALSE;
        }

        public CoreCompareOptions Clone()
        {
            return new CoreCompareOptions(this);
        }

        public bool Equals(CoreCompareOptions compareOptions)
        {
            return
                checkOnEquality == compareOptions.checkOnEquality &&
                transformedImage == compareOptions.transformedImage &&
                sizeControl == compareOptions.sizeControl &&
                typeControl == compareOptions.typeControl &&
                ratioControl == compareOptions.ratioControl &&
				algorithmComparing == compareOptions.algorithmComparing &&
                thresholdDifference == compareOptions.thresholdDifference &&
                minimalImageSize == compareOptions.minimalImageSize &&
                maximalImageSize == compareOptions.maximalImageSize &&
                compareInsideOneFolder == compareOptions.compareInsideOneFolder &&
                compareInsideOneSearchPath == compareOptions.compareInsideOneSearchPath;
        }

        public bool CheckOnEquality
        {
            get { return checkOnEquality; }
            set
            {
                checkOnEquality = value;
                NotifyPropertyChanged("CheckOnEquality");
            }
        }

        public bool TransformedImage
        {
            get { return transformedImage; }
            set
            {
                transformedImage = value;
                NotifyPropertyChanged("TransformedImage");
            }
        }

        public bool SizeControl
        {
            get { return sizeControl; }
            set
            {
                sizeControl = value;
                NotifyPropertyChanged("SizeControl");
            }
        }

        public bool TypeControl
        {
            get { return typeControl; }
            set
            {
                typeControl = value;
                NotifyPropertyChanged("TypeControl");
            }
        }

        public bool RatioControl
        {
            get { return ratioControl; }
            set
            {
                ratioControl = value;
                NotifyPropertyChanged("RatioControl");
            }
        }

        public int ThresholdDifference
        {
            get { return thresholdDifference; }
            set
            {
                thresholdDifference = value;
                NotifyPropertyChanged("ThresholdDifference");
            }
        }

        public int MinimalImageSize
        {
            get { return minimalImageSize; }
            set
            {
                minimalImageSize = value;
                NotifyPropertyChanged("MinimalImageSize");
            }
        }

        public int MaximalImageSize
        {
            get { return maximalImageSize; }
            set
            {
                maximalImageSize = value;
                NotifyPropertyChanged("MaximalImageSize");
            }
        }

        public bool CompareInsideOneFolder
        {
            get { return compareInsideOneFolder; }
            set
            {
                compareInsideOneFolder = value;
                NotifyPropertyChanged("CompareInsideOneFolder");
            }
        }

        public bool CompareInsideOneSearchPath
        {
            get { return compareInsideOneSearchPath; }
            set
            {
                compareInsideOneSearchPath = value;
                NotifyPropertyChanged("CompareInsideOneSearchPath");
            }
        }

        public AlgorithmComparing AlgorithmComparing
        {
            get { return algorithmComparing; }
            set
            {
                algorithmComparing = value;
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
