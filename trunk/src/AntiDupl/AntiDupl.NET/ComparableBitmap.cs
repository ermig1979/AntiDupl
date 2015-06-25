// From DupliFinder

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
