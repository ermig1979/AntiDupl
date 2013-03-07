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
    public class CoreStatistic
    {
        public uint scanedFolderNumber;
        public uint searchedImageNumber;
        public uint collectedImageNumber;
        public uint comparedImageNumber;
        public uint collectThreadCount;
        public uint compareThreadCount;
        public uint defectImageNumber;
        public uint duplImagePairNumber;
        public uint renamedImageNumber;
        public uint deletedImageNumber;
        public UInt64 deletedImageSize;
        
        public CoreStatistic(ref CoreDll.adStatistic statistic)
        {
            scanedFolderNumber = statistic.scanedFolderNumber.ToUInt32();
            searchedImageNumber = statistic.searchedImageNumber.ToUInt32();
            collectedImageNumber = statistic.collectedImageNumber.ToUInt32();
            comparedImageNumber = statistic.comparedImageNumber.ToUInt32();
            collectThreadCount = statistic.collectThreadCount.ToUInt32();
            compareThreadCount = statistic.compareThreadCount.ToUInt32();
            defectImageNumber = statistic.defectImageNumber.ToUInt32();
            duplImagePairNumber = statistic.duplImagePairNumber.ToUInt32();
            renamedImageNumber = statistic.renamedImageNumber.ToUInt32();
            deletedImageNumber = statistic.deletedImageNumber.ToUInt32();
            deletedImageSize = statistic.deletedImageSize;
        }
    }
}
