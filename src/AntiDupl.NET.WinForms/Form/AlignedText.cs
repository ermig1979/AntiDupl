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

using AntiDupl.NET.Core.Original;

namespace AntiDupl.NET.WinForms
{
    /// <summary>
    /// The aligned search's level, memory and time as the warning and the
    /// search's progress line say them.
    /// </summary>
    public static class AlignedText
    {
        /// <summary>The smallest crop a level finds, as a share of the picture.</summary>
        public static string Level(Strings s, int level)
        {
            switch (level)
            {
                case 1: return s.AlignedMemoryForm_Level1;
                case 2: return s.AlignedMemoryForm_Level2;
                case 3: return s.AlignedMemoryForm_Level3;
                default: return s.AlignedMemoryForm_Level4;
            }
        }

        public static string Memory(Strings s, ulong bytes)
        {
            return string.Format(s.AlignedMemoryForm_Gigabytes, bytes / (double)(1UL << 30));
        }

        /// <summary>
        /// Minutes up to an hour and a half, hours and minutes up to two
        /// days, then days and hours: as precise as an estimate is.
        /// </summary>
        public static string Duration(Strings s, double seconds)
        {
            long minutes = Math.Max(1, (long)Math.Round(seconds / 60));
            if (minutes < 90)
                return string.Format(s.AlignedSearch_Minutes, minutes);
            if (minutes < 48 * 60)
                return string.Format(s.AlignedSearch_Hours, minutes / 60, minutes % 60);
            long hours = (long)Math.Round(seconds / 3600);
            return string.Format(s.AlignedSearch_Days, hours / 24, hours % 24);
        }

        /// <summary>
        /// The time the aligning has left: done images leave less to do for
        /// the rest, each pair being matched once, so the work done after k
        /// of n images is 1 - (1 - k/n)^2 of it. Before the aligning starts,
        /// the estimate less the time the search has run.
        /// </summary>
        public static double SecondsLeft(CoreDll.adAlignedSearch search, double secondsRun)
        {
            if (search.alignedImages == 0 || search.imageCount == 0)
                return Math.Max(0, search.usedSeconds - secondsRun);
            double left = 1 - Math.Min(1.0, (double)search.alignedImages / search.imageCount);
            double done = 1 - left * left;
            return search.alignedSeconds * (1 - done) / done;
        }
    }
}
