/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar, 2013-2018 Borisov Dmitry.
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

namespace AntiDupl.NET.Core
{
    public class CoreDefectOptions : INotifyPropertyChanged
    {
        public bool checkOnDefect;
        public bool checkOnBlockiness;
        public int blockinessThreshold;
        public bool checkOnBlockinessOnlyNotJpeg;
        public bool checkOnBlurring;
        public int blurringThreshold;

        public CoreDefectOptions()
        {
        }

        public CoreDefectOptions(CoreDefectOptions defectOptions)
        {
            checkOnDefect = defectOptions.checkOnDefect;
            checkOnBlockiness = defectOptions.checkOnBlockiness;
            blockinessThreshold = defectOptions.blockinessThreshold;
			checkOnBlockinessOnlyNotJpeg = defectOptions.checkOnBlockinessOnlyNotJpeg;
            checkOnBlurring = defectOptions.checkOnBlurring;
            blurringThreshold = defectOptions.blurringThreshold;
        }

        public CoreDefectOptions(ref CoreDll.adDefectOptions defectOptions)
        {
            checkOnDefect = defectOptions.checkOnDefect != CoreDll.FALSE;
            checkOnBlockiness = defectOptions.checkOnBlockiness != CoreDll.FALSE;
            blockinessThreshold = defectOptions.blockinessThreshold;
			checkOnBlockinessOnlyNotJpeg = defectOptions.checkOnBlockinessOnlyNotJpeg != CoreDll.FALSE;
            checkOnBlurring = defectOptions.checkOnBlurring != CoreDll.FALSE;
            blurringThreshold = defectOptions.blurringThreshold;
        }

        public void ConvertTo(ref CoreDll.adDefectOptions defectOptions)
        {
            defectOptions.checkOnDefect = checkOnDefect ? CoreDll.TRUE : CoreDll.FALSE;
            defectOptions.checkOnBlockiness = checkOnBlockiness ? CoreDll.TRUE : CoreDll.FALSE;
            defectOptions.blockinessThreshold = blockinessThreshold;
			defectOptions.checkOnBlockinessOnlyNotJpeg = checkOnBlockinessOnlyNotJpeg ? CoreDll.TRUE : CoreDll.FALSE;
            defectOptions.checkOnBlurring = checkOnBlurring ? CoreDll.TRUE : CoreDll.FALSE;
            defectOptions.blurringThreshold = blurringThreshold;
        }

        public CoreDefectOptions Clone()
        {
            return new CoreDefectOptions(this);
        }

        public bool Equals(CoreDefectOptions defectOptions)
        {
            return
                checkOnDefect == defectOptions.checkOnDefect &&
                checkOnBlockiness == defectOptions.checkOnBlockiness &&
                blockinessThreshold == defectOptions.blockinessThreshold &&
 				checkOnBlockinessOnlyNotJpeg == defectOptions.checkOnBlockinessOnlyNotJpeg &&
                checkOnBlurring == defectOptions.checkOnBlurring &&
                blurringThreshold == defectOptions.blurringThreshold;
        }

        public bool CheckOnDefect
        {
            get { return checkOnDefect; }
            set
            {
                checkOnDefect = value;
                NotifyPropertyChanged("CheckOnDefect");
            }
        }

        public bool CheckOnBlockiness
        {
            get { return checkOnBlockiness; }
            set
            {
                checkOnBlockiness = value;
                NotifyPropertyChanged("CheckOnBlockiness");
            }
        }

        public int BlockinessThreshold
        {
            get { return blockinessThreshold; }
            set
            {
                blockinessThreshold = value;
                NotifyPropertyChanged("BlockinessThreshold");
            }
        }

        public bool CheckOnBlockinessOnlyNotJpeg
        {
            get { return checkOnBlockinessOnlyNotJpeg; }
            set
            {
                checkOnBlockinessOnlyNotJpeg = value;
                NotifyPropertyChanged("CheckOnBlockinessOnlyNotJpeg");
            }
        }

        public bool CheckOnBlurring
        {
            get { return checkOnBlurring; }
            set
            {
                checkOnBlurring = value;
                NotifyPropertyChanged("CheckOnBlurring");
            }
        }

        public int BlurringThreshold
        {
            get { return blurringThreshold; }
            set
            {
                blurringThreshold = value;
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
