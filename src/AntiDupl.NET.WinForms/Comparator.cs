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

namespace AntiDupl.NET
{
    class Comparator
    {
        public static float Similarity(byte[] first, byte[] second)
        {
            float distance = Distance(first, second);

            int length = first.Length > second.Length ? second.Length : first.Length;

            // Need to convert this to a scale from 0 to 100
            int maxunsimilarity = (int)Math.Pow(255, 2) * length / length;
            return 100 * (1 - (distance / maxunsimilarity));
        }

        private static float Distance(byte[] first, byte[] second)
        {
            int sum = 0;

            // We'll use which ever array is shorter.
            int length = first.Length > second.Length ? second.Length : first.Length;
            for (int x = 0; x < length; x++)
            {
                sum = sum + (int)Math.Pow((first[x] - second[x]), 2);
            }

            return sum / length;
        }
    }
}
