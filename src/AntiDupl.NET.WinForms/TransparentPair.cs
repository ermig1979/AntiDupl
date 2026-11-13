/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2023 Yermalayeu Ihar.
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
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

namespace AntiDupl.NET.WinForms
{
    /// <summary>
    /// Two pictures of a pair as the search compares them with the
    /// transparency option on (CoreCompareOptions.transparentAsShown, the
    /// core's TImageComparer::IsDuplPairShown), for the highlighted
    /// differences: the colour a file stores under transparent pixels is
    /// never seen, so a picture with transparency is laid over a background.
    /// </summary>
    /// <remarks>
    /// Pictures are laid over a background at their own size, before they
    /// are resized: the resize fades a picture's border into transparency,
    /// and resizing loses the colour of fully transparent pixels.
    /// </remarks>
    public static class TransparentPair
    {
        // The core's limits for a copy whose transparency was filled in with
        // one colour (UNIFORM_FILL_* in adConfig.h, which explains them).
        private const int COVERAGE_PERCENT = 5;
        private const int AREA_PERCENT = 2;
        private const double DEVIATION_MAX = 12;
        private const double CONTRAST_MIN = 16;
        private const double FAST_DIFFERENCE_MAX = 48;
        // The sizes of the core's thumbnails the decision is made on.
        private const int COLOR_SIZE = 64;
        private const int FAST_SIZE = 4;

        /// <summary>
        /// The pairs of pictures to compare: a fragment differs as much as
        /// in the pair where it differs most. Each picture comes back
        /// through output, which may resize it (and may give null).
        /// <para>Without transparency: the two pictures. Two pictures with
        /// transparency: over white and over black. One with transparency
        /// and an opaque one: over the colour the opaque one is filled with
        /// where the other is transparent, if it is a filled-in copy,
        /// otherwise over white and over black.</para>
        /// </summary>
        public static List<Bitmap[]> Shown(Bitmap first, Bitmap second, Func<Bitmap, Bitmap> output)
        {
            Kind a = KindOf(first), b = KindOf(second);
            List<Bitmap[]> pairs = new List<Bitmap[]>();
            if (a != Kind.Transparent && b != Kind.Transparent)
            {
                pairs.Add(new Bitmap[] { output(Opaque(first, a)), output(Opaque(second, b)) });
                return pairs;
            }
            if (a == Kind.Transparent && b == Kind.Transparent)
            {
                pairs.Add(new Bitmap[] { output(Over(first, Color.White)), output(Over(second, Color.White)) });
                pairs.Add(new Bitmap[] { output(Over(first, Color.Black)), output(Over(second, Color.Black)) });
                return pairs;
            }
            bool firstClear = a == Kind.Transparent;
            Bitmap alpha = firstClear ? first : second;
            Bitmap opaque = firstClear ? Opaque(second, b) : Opaque(first, a);
            Bitmap overWhite = Over(alpha, Color.White), overBlack = Over(alpha, Color.Black);
            byte[] fill = UniformFill(overWhite, overBlack, opaque);
            Bitmap shownWhite = output(overWhite), shownBlack = output(overBlack), shownOpaque = output(opaque);
            List<Bitmap> shown = fill == null || shownWhite == null || shownBlack == null ?
                new List<Bitmap> { shownWhite, shownBlack } : new List<Bitmap> { Filled(shownWhite, shownBlack, fill) };
            foreach (Bitmap picture in shown)
                pairs.Add(firstClear ? new Bitmap[] { picture, shownOpaque } : new Bitmap[] { shownOpaque, picture });
            return pairs;
        }

        private enum Kind
        {
            Opaque,
            Transparent,
            // Alpha is 0 everywhere: an unused fourth byte, as in some 32-bit
            // BMPs. The core counts such a picture as opaque.
            AllClear,
        }

        private static unsafe Kind KindOf(Bitmap bitmap)
        {
            PixelFormat format = bitmap.PixelFormat;
            if (!Image.IsAlphaPixelFormat(format) && (format & PixelFormat.Indexed) == 0)
                return Kind.Opaque;
            BitmapData data = bitmap.LockBits(new Rectangle(0, 0, bitmap.Width, bitmap.Height),
                ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);
            bool anyClear = false, anyCovered = false;
            try
            {
                for (int y = 0; y < data.Height && !(anyClear && anyCovered); ++y)
                {
                    byte* row = (byte*)data.Scan0 + y * data.Stride;
                    for (int x = 0; x < data.Width; ++x)
                    {
                        byte alpha = row[4 * x + 3];
                        anyClear |= alpha < 255;
                        anyCovered |= alpha > 0;
                    }
                }
            }
            finally
            {
                bitmap.UnlockBits(data);
            }
            return !anyCovered ? Kind.AllClear : anyClear ? Kind.Transparent : Kind.Opaque;
        }

        // A picture with alpha 0 everywhere made opaque with its own colours;
        // any other picture as it is.
        private static unsafe Bitmap Opaque(Bitmap bitmap, Kind kind)
        {
            if (kind != Kind.AllClear)
                return bitmap;
            Rectangle rect = new Rectangle(0, 0, bitmap.Width, bitmap.Height);
            Bitmap opaque = new Bitmap(rect.Width, rect.Height, PixelFormat.Format32bppArgb);
            BitmapData from = bitmap.LockBits(rect, ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);
            BitmapData to = opaque.LockBits(rect, ImageLockMode.WriteOnly, PixelFormat.Format32bppArgb);
            try
            {
                for (int y = 0; y < rect.Height; ++y)
                {
                    uint* src = (uint*)((byte*)from.Scan0 + y * from.Stride);
                    uint* dst = (uint*)((byte*)to.Scan0 + y * to.Stride);
                    for (int x = 0; x < rect.Width; ++x)
                        dst[x] = src[x] | 0xFF000000;
                }
            }
            finally
            {
                bitmap.UnlockBits(from);
                opaque.UnlockBits(to);
            }
            return opaque;
        }

        private static Bitmap Over(Bitmap bitmap, Color background)
        {
            Bitmap shown = new Bitmap(bitmap.Width, bitmap.Height, PixelFormat.Format32bppArgb);
            using (Graphics g = Graphics.FromImage(shown))
            {
                g.Clear(background);
                g.DrawImage(bitmap, new Rectangle(0, 0, bitmap.Width, bitmap.Height),
                    0, 0, bitmap.Width, bitmap.Height, GraphicsUnit.Pixel);
            }
            return shown;
        }

        // Over colour c a pixel is black + (white - black) * c / 255, as in
        // the core's TImageComparer::MakeFilled.
        private static Bitmap Filled(Bitmap overWhite, Bitmap overBlack, byte[] bgr)
        {
            byte[] w = Bytes(overWhite), b = Bytes(overBlack);
            byte[] shown = new byte[w.Length];
            for (int i = 0; i < w.Length; i += 4)
            {
                for (int c = 0; c < 3; ++c)
                    shown[i + c] = (byte)(b[i + c] + ((w[i + c] - b[i + c]) * bgr[c] + 127) / 255);
                shown[i + 3] = w[i + 3];
            }
            Bitmap bitmap = new Bitmap(overWhite.Width, overWhite.Height, PixelFormat.Format32bppArgb);
            Rectangle rect = new Rectangle(0, 0, bitmap.Width, bitmap.Height);
            BitmapData data = bitmap.LockBits(rect, ImageLockMode.WriteOnly, PixelFormat.Format32bppArgb);
            try
            {
                for (int y = 0; y < rect.Height; ++y)
                    Marshal.Copy(shown, y * rect.Width * 4, data.Scan0 + y * data.Stride, rect.Width * 4);
            }
            finally
            {
                bitmap.UnlockBits(data);
            }
            return bitmap;
        }

        // B, G, R, A bytes, top row first, without padding.
        private static byte[] Bytes(Bitmap bitmap)
        {
            Rectangle rect = new Rectangle(0, 0, bitmap.Width, bitmap.Height);
            byte[] bytes = new byte[rect.Width * rect.Height * 4];
            BitmapData data = bitmap.LockBits(rect, ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);
            try
            {
                for (int y = 0; y < rect.Height; ++y)
                    Marshal.Copy(data.Scan0 + y * data.Stride, bytes, y * rect.Width * 4, rect.Width * 4);
            }
            finally
            {
                bitmap.UnlockBits(data);
            }
            return bytes;
        }

        /// <summary>
        /// The core's TImageComparer::UniformFill on thumbnails like the
        /// core's: whether the opaque picture could be the other with its
        /// transparency filled in with one colour, and which (B, G, R), or
        /// null. Over white minus over black is 255 times a pixel's
        /// uncovered share.
        /// </summary>
        private static byte[] UniformFill(Bitmap overWhite, Bitmap overBlack, Bitmap opaque)
        {
            const int size = COLOR_SIZE;
            double[] w = Thumbnail(overWhite), b = Thumbnail(overBlack), o = Thumbnail(opaque);

            // A loose first look on 4x4 grey cells: the opaque picture
            // against the other over one grey level k, fitted, leaves no cell
            // far off.
            double[] fw = Fast(w, size), fb = Fast(b, size), fo = Fast(o, size);
            double dd = 0, dr = 0;
            for (int i = 0; i < fw.Length; ++i)
            {
                double d = fw[i] - fb[i];
                dd += d * d;
                dr += d * (fo[i] - fb[i]);
            }
            double k = dd > 0 ? Math.Max(0.0, Math.Min(1.0, dr / dd)) : 0;
            for (int i = 0; i < fw.Length; ++i)
            {
                if (Math.Abs(fb[i] + k * (fw[i] - fb[i]) - fo[i]) > FAST_DIFFERENCE_MAX)
                    return null;
            }

            int pixels = size * size;
            double[] sum = new double[3], square = new double[3];
            int clear = 0, solid = 0;
            for (int p = 0; p < pixels; ++p)
            {
                double uncovered = Uncovered(w, b, p);
                if (uncovered >= 3 * 255.0 * (100 - COVERAGE_PERCENT) / 100)
                {
                    ++clear;
                    for (int c = 0; c < 3; ++c)
                    {
                        sum[c] += o[3 * p + c];
                        square[c] += o[3 * p + c] * o[3 * p + c];
                    }
                }
                else if (uncovered <= 3 * 255.0 * COVERAGE_PERCENT / 100)
                    ++solid;
            }
            if (clear * 100 < pixels * AREA_PERCENT || solid * 100 < pixels * AREA_PERCENT)
                return null;
            byte[] bgr = new byte[3];
            for (int c = 0; c < 3; ++c)
            {
                double mean = sum[c] / clear;
                if (square[c] / clear - mean * mean > DEVIATION_MAX * DEVIATION_MAX)
                    return null;
                bgr[c] = (byte)(mean + 0.5);
            }
            double contrast = 0;
            for (int p = 0; p < pixels; ++p)
            {
                if (Uncovered(w, b, p) <= 3 * 255.0 * COVERAGE_PERCENT / 100)
                {
                    double most = 0;
                    for (int c = 0; c < 3; ++c)
                        most = Math.Max(most, Math.Abs(w[3 * p + c] - bgr[c]));
                    contrast += most;
                }
            }
            return contrast / solid >= CONTRAST_MIN ? bgr : null;
        }

        private static double Uncovered(double[] w, double[] b, int p)
        {
            return w[3 * p] + w[3 * p + 1] + w[3 * p + 2] - b[3 * p] - b[3 * p + 1] - b[3 * p + 2];
        }

        // A picture's B, G, R stretched to COLOR_SIZE square, like the core's
        // colour thumbnail. The edge pixels are mirrored outwards, or the
        // resize would fade the border into transparency.
        private static double[] Thumbnail(Bitmap bitmap)
        {
            using (Bitmap thumbnail = new Bitmap(COLOR_SIZE, COLOR_SIZE, PixelFormat.Format32bppArgb))
            {
                using (Graphics g = Graphics.FromImage(thumbnail))
                using (ImageAttributes attributes = new ImageAttributes())
                {
                    attributes.SetWrapMode(System.Drawing.Drawing2D.WrapMode.TileFlipXY);
                    g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
                    g.DrawImage(bitmap, new Rectangle(0, 0, COLOR_SIZE, COLOR_SIZE),
                        0, 0, bitmap.Width, bitmap.Height, GraphicsUnit.Pixel, attributes);
                }
                byte[] bgra = Bytes(thumbnail);
                double[] bgr = new double[COLOR_SIZE * COLOR_SIZE * 3];
                for (int p = 0; p < COLOR_SIZE * COLOR_SIZE; ++p)
                    for (int c = 0; c < 3; ++c)
                        bgr[3 * p + c] = bgra[4 * p + c];
                return bgr;
            }
        }

        // Grey 4x4 cells of a size x size B, G, R thumbnail, with the core's
        // grey weights.
        private static double[] Fast(double[] bgr, int size)
        {
            double[] cells = new double[FAST_SIZE * FAST_SIZE];
            int[] counts = new int[FAST_SIZE * FAST_SIZE];
            for (int y = 0; y < size; ++y)
            {
                for (int x = 0; x < size; ++x)
                {
                    int p = 3 * (y * size + x);
                    int cell = (y * FAST_SIZE / size) * FAST_SIZE + x * FAST_SIZE / size;
                    cells[cell] += (29 * bgr[p] + 150 * bgr[p + 1] + 77 * bgr[p + 2]) / 256;
                    counts[cell]++;
                }
            }
            for (int i = 0; i < cells.Length; ++i)
                cells[i] /= Math.Max(1, counts[i]);
            return cells;
        }
    }
}
