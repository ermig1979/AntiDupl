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
using System.Text;

namespace AntiDupl.NET
{
    public static class ProgressUtils
    {
        public static string GetProgressString(double progress, DateTime startDateTime)
        {
            Strings s = Resources.Strings.Current;
            StringBuilder builder = new StringBuilder();
            builder.AppendFormat(s.ProgressUtils_Completed, Convert.ToInt32(Math.Ceiling(progress * 100)).ToString());
            if (progress > 0.0001)
            {
                builder.Append("; ");
                TimeSpan used = DateTime.Now - startDateTime;
                double usedMilliseconds = used.TotalMilliseconds;
                double remainedMilliseconds = usedMilliseconds * (1 - progress) / progress;
                TimeSpan remained = TimeSpan.FromMilliseconds(remainedMilliseconds);
                if (remained.Hours >= 4 || remained.Days > 0)
                {
                    builder.AppendFormat(s.ProgressUtils_5HoursRemaining, remained.Days*24 + remained.Hours + 1);
                }
                else if (remained.Hours >= 1)
                {
                    builder.AppendFormat(s.ProgressUtils_2HoursRemaining, remained.Hours + 1);
                }
                else if (remained.Minutes >= 4)
                {
                    int minuts = (remained.Minutes / 5 + 1) * 5;
                    builder.AppendFormat(s.ProgressUtils_5MinutesRemaining, minuts);
                }
                else if (remained.Minutes >= 1)
                {
                    builder.AppendFormat(s.ProgressUtils_2MinutesRemaining, remained.Minutes + 1);
                }
                else
                {
                    int seconds = (remained.Seconds / 5 + 1) * 5;
                    builder.AppendFormat(s.ProgressUtils_5SecondsRemaining, seconds);
                }
            }
            return builder.ToString();
        }
    }
}
