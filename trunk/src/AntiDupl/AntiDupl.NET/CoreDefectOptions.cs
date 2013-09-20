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
    public class CoreDefectOptions
    {
        public bool checkOnDefect;
        public bool checkOnBlockiness;
        public int blockinessThreshold;

        public CoreDefectOptions()
        {
        }

        public CoreDefectOptions(CoreDefectOptions defectOptions)
        {
            checkOnDefect = defectOptions.checkOnDefect;
            checkOnBlockiness = defectOptions.checkOnBlockiness;
            blockinessThreshold = defectOptions.blockinessThreshold;
        }

        public CoreDefectOptions(ref CoreDll.adDefectOptions defectOptions)
        {
            checkOnDefect = defectOptions.checkOnDefect != CoreDll.FALSE;
            checkOnBlockiness = defectOptions.checkOnBlockiness != CoreDll.FALSE;
            blockinessThreshold = defectOptions.blockinessThreshold;
        }

        public void ConvertTo(ref CoreDll.adDefectOptions defectOptions)
        {
            defectOptions.checkOnDefect = checkOnDefect ? CoreDll.TRUE : CoreDll.FALSE;
            defectOptions.checkOnBlockiness = checkOnBlockiness ? CoreDll.TRUE : CoreDll.FALSE;
            defectOptions.blockinessThreshold = blockinessThreshold;
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
                blockinessThreshold == defectOptions.blockinessThreshold;
        }
    }
}
