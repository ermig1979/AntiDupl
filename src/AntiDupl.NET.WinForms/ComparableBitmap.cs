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

using System.Drawing;
using System.Drawing.Imaging;

namespace AntiDupl.NET
{
    public class ComparableBitmap
    {
        public Rectangle Rect { get; set; }
        private byte[] _grayScaleData = null;
        public byte[] GrayscaleData
        {
            get { return _grayScaleData; }
        }

        /// <summary>
        /// A colormatric we use to convert the image to grayscale.
        /// </summary>
        private static ColorMatrix colorMatrix = new ColorMatrix(
               new float[][]
              {
                 new float[] {.3f, .3f, .3f, 0, 0},
                 new float[] {.59f, .59f, .59f, 0, 0},
                 new float[] {.11f, .11f, .11f, 0, 0},
                 new float[] {0, 0, 0, 1, 0},
                 new float[] {0, 0, 0, 0, 1}
              });

        public ComparableBitmap(Bitmap bitmapSource, Rectangle section)
        {
            this.Rect = section;

            // Create the new bitmap and associated graphics object
            using (Bitmap sectionBmp = new Bitmap(section.Width, section.Height))
            {
                using (Graphics g = Graphics.FromImage(sectionBmp))
                {
                    // Draw the specified section of the source bitmap to the new one
                    g.DrawImage(bitmapSource, 0, 0, section, GraphicsUnit.Pixel);
                }

                this._grayScaleData = GetBmpBytes(ToGrayScale(sectionBmp));
            }
        }

        /// <summary>
        /// During this process, we chuck the RGBA channels.  The RGB channels are all equal, and the A channels is completely
        /// irrelevant.  The remaining byte[] is simply a greyscale luma channel.
        /// </summary>
        /// <param name="sectionBmp"></param>
        /// <returns></returns>
        byte[] GetBmpBytes(Bitmap bmp)
        {
            BitmapData bData = bmp.LockBits(new Rectangle(new Point(), bmp.Size), ImageLockMode.ReadOnly, bmp.PixelFormat);
            int byteCount = (bData.Stride * bmp.Height) / 4;
            int bytesPerPixel = (bData.Stride * bmp.Height) / (bmp.Height * bmp.Width);
            byte[] bmpBytes = new byte[byteCount];

            unsafe
            {
                byte* p = (byte*)(void*)bData.Scan0;
                for (int x = 0; x < bmpBytes.Length; ++x)
                {
                    bmpBytes[x] = *p;
                    p += bytesPerPixel;
                }
            }

            bmp.UnlockBits(bData);
            return bmpBytes;
        }

        public Bitmap ToGrayScale(System.Drawing.Bitmap b)
        {
            //create a blank bitmap the same size as original
            Bitmap newBitmap = new Bitmap(b.Width, b.Height);

            //get a graphics object from the new image
            using (Graphics g = Graphics.FromImage(newBitmap))
            {

                //create some image attributes
                ImageAttributes attributes = new ImageAttributes();

                //set the color matrix attribute
                attributes.SetColorMatrix(colorMatrix);

                //draw the original image on the new image
                //using the grayscale color matrix
                g.DrawImage(b, new Rectangle(0, 0, b.Width, b.Height),
                   0, 0, b.Width, b.Height, GraphicsUnit.Pixel, attributes);

            }

            return newBitmap;
        }

    }
}
