using System;
using System.Drawing;

using AntiDupl.NET.Core;
using AntiDupl.NET.Core.Enums;
using AntiDupl.NET.Core.Original;

namespace AntiDupl.NET.WinForms
{
    public static class BitmapWorker
    {
        public static Bitmap LoadBitmap(CoreLib coreLib, int width, int height, string path)
        {
            if (height * width == 0)
                return null;

            Bitmap bitmap = null;
            try
            {
                bitmap = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            }
            catch (Exception)
            {
                GC.Collect();
                try
                {
                    bitmap = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
                }
                catch (Exception)
                {
                    return null;
                }
            }
            System.Drawing.Imaging.BitmapData bitmapData = new System.Drawing.Imaging.BitmapData();
            bitmapData = bitmap.LockBits(
                new Rectangle(0, 0, width, height),
                System.Drawing.Imaging.ImageLockMode.WriteOnly,
                System.Drawing.Imaging.PixelFormat.Format32bppArgb,
                bitmapData);
            AdBitmap[] pBitmap = new AdBitmap[1];
            pBitmap[0].width = (uint)bitmapData.Width;
            pBitmap[0].height = (uint)bitmapData.Height;
            pBitmap[0].stride = bitmapData.Stride;
            pBitmap[0].format = PixelFormatType.Argb32;
            pBitmap[0].data = bitmapData.Scan0;
            Error error = coreLib.LoadBitmap(path, pBitmap);
            bitmap.UnlockBits(bitmapData);
            return error == Error.Ok ? bitmap : null;
        }

        public static Bitmap LoadBitmap(CoreLib coreLib, Size size, string path)
        {
            return LoadBitmap(coreLib, size.Width, size.Height, path);
        }

        public static Bitmap LoadBitmap(CoreLib coreLib, CoreImageInfo imageInfo)
        {
            return LoadBitmap(coreLib, (int)imageInfo.width, (int)imageInfo.height, imageInfo.path);
        }
    }
}
