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
using System.Drawing;
using System.Windows.Forms;

namespace AntiDupl.NET.WinForms
{
    /// <summary>
    /// How a pair's difference is shown and copied: 0.00 only for identical
    /// images, so a real difference never looks like none. Sorting uses the
    /// core's exact values and is not affected.
    /// </summary>
    static public class DifferenceFormat
    {
        // The core reports 0 for files with the same bytes, and adds 1e-6 to
        // the difference of files whose bytes differ: below this bound the
        // images themselves are identical.
        private const double IDENTICAL_MAX = 0.00001;

        // The largest difference shown with two decimals as 0.01.
        private const double SMALLEST_SHOWN = 0.01;

        static public bool IsByteIdentical(double difference)
        {
            return difference == 0;
        }

        static public bool IsIdentical(double difference)
        {
            return difference < IDENTICAL_MAX;
        }

        /// <summary>
        /// 0.00 for identical images, 0.01 for any difference up to 0.01,
        /// otherwise two decimals.
        /// </summary>
        static public string ToString(double difference)
        {
            if (IsIdentical(difference))
                return 0.0.ToString("F2");
            if (difference <= SMALLEST_SHOWN)
                return SMALLEST_SHOWN.ToString("F2");
            return difference.ToString("F2");
        }

        /// <summary>The value with up to six decimals, for the tooltip.</summary>
        static public string ToExactString(double difference)
        {
            return difference.ToString("0.######");
        }

        /// <summary>
        /// The text colour for a difference: green for the same file bytes,
        /// orange for the same image in different files, otherwise the given
        /// colour. Dark shades on the light theme and light ones on the dark
        /// theme, each at least 4.5:1 against the grid's background.
        /// </summary>
        static public Color GetColor(double difference, Color other)
        {
            bool dark = Application.IsDarkModeEnabled;
            if (IsByteIdentical(difference))
                return dark ? Color.LightGreen : Color.Green;
            if (IsIdentical(difference))
                return dark ? Color.FromArgb(0xFF, 0xB3, 0x66) : Color.FromArgb(0xB3, 0x59, 0x00);
            return other;
        }
    }
}
