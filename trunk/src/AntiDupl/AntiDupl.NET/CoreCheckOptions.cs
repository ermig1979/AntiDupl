/*
* AntiDupl.NET Program.
*
* Copyright (c) 2002-2013 Yermalayeu Ihar.
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
using System.Text;

namespace AntiDupl.NET
{
    public class CoreCheckOptions
    {
        public bool checkOnDefect;
        public bool checkOnEquality;
        public bool transformedImage;
        public bool sizeControl;
        public bool typeControl;
        public bool ratioControl;
        public int thresholdDifference;
        public int minimalImageSize;
        public int maximalImageSize;
        public bool compareInsideOneFolder;

        public CoreCheckOptions()
        {
        }

        public CoreCheckOptions(CoreCheckOptions checkOptions)
        {
            checkOnDefect = checkOptions.checkOnDefect;
            checkOnEquality = checkOptions.checkOnEquality;
            transformedImage = checkOptions.transformedImage;
            sizeControl = checkOptions.sizeControl;
            typeControl = checkOptions.typeControl;
            ratioControl = checkOptions.ratioControl;
            thresholdDifference = checkOptions.thresholdDifference;
            minimalImageSize = checkOptions.minimalImageSize;
            maximalImageSize = checkOptions.maximalImageSize;
            compareInsideOneFolder = checkOptions.compareInsideOneFolder;
        }

        public CoreCheckOptions(ref CoreDll.adCheckOptions checkOptions)
        {
            checkOnDefect = checkOptions.checkOnDefect != CoreDll.FALSE;
            checkOnEquality = checkOptions.checkOnEquality != CoreDll.FALSE;
            transformedImage = checkOptions.transformedImage != CoreDll.FALSE;
            sizeControl = checkOptions.sizeControl != CoreDll.FALSE;
            typeControl = checkOptions.typeControl != CoreDll.FALSE;
            ratioControl = checkOptions.ratioControl != CoreDll.FALSE;
            thresholdDifference = checkOptions.thresholdDifference;
            minimalImageSize = checkOptions.minimalImageSize;
            maximalImageSize = checkOptions.maximalImageSize;
            compareInsideOneFolder = checkOptions.compareInsideOneFolder != CoreDll.FALSE;
        }

        public void ConvertTo(ref CoreDll.adCheckOptions checkOptions)
        {
            checkOptions.checkOnDefect = checkOnDefect ? CoreDll.TRUE : CoreDll.FALSE;
            checkOptions.checkOnEquality = checkOnEquality ? CoreDll.TRUE : CoreDll.FALSE;
            checkOptions.transformedImage = transformedImage ? CoreDll.TRUE : CoreDll.FALSE;
            checkOptions.sizeControl = sizeControl ? CoreDll.TRUE : CoreDll.FALSE;
            checkOptions.typeControl = typeControl ? CoreDll.TRUE : CoreDll.FALSE;
            checkOptions.ratioControl = ratioControl ? CoreDll.TRUE : CoreDll.FALSE;
            checkOptions.thresholdDifference = thresholdDifference;
            checkOptions.minimalImageSize = minimalImageSize;
            checkOptions.maximalImageSize = maximalImageSize;
            checkOptions.compareInsideOneFolder = compareInsideOneFolder ? CoreDll.TRUE : CoreDll.FALSE;
        }

        public CoreCheckOptions Clone()
        {
            return new CoreCheckOptions(this);
        }

        public bool Equals(CoreCheckOptions checkOptions)
        {
            return
                checkOnDefect == checkOptions.checkOnDefect &&
                checkOnEquality == checkOptions.checkOnEquality &&
                transformedImage == checkOptions.transformedImage &&
                sizeControl == checkOptions.sizeControl &&
                typeControl == checkOptions.typeControl &&
                ratioControl == checkOptions.ratioControl &&
                thresholdDifference == checkOptions.thresholdDifference &&
                minimalImageSize == checkOptions.minimalImageSize &&
                maximalImageSize == checkOptions.maximalImageSize &&
                compareInsideOneFolder == checkOptions.compareInsideOneFolder;
        }
    }
}
