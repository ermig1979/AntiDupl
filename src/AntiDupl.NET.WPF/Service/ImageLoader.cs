using System;
using System.Collections.Generic;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using AntiDupl.NET.WPF.Core;

using AntiDupl.NET.Core;
using AntiDupl.NET.Core.Original;
using AntiDupl.NET.Core.Enums;

namespace AntiDupl.NET.WPF.Service
{
    class ImageLoader : IImageLoader
    {
        CoreLib _core;

        public ImageLoader(CoreLib core)
        {
            _core = core; 
        }

        public System.Windows.Media.Imaging.BitmapSource LoadImage(string path, uint width, uint height, CoreDll.ImageType type)
        {
             if (File.Exists(path))
             {
                 //может считать net Framework
                if (type <= CoreDll.ImageType.Icon)
                {
                    using (var fs = new FileStream(path, FileMode.Open, FileAccess.Read))
                    {
                        return BitmapFrame.Create(
                            fs, BitmapCreateOptions.None, BitmapCacheOption.OnLoad);
                    }
                }
                else
                {
                    if (height * width == 0)
                        return null;

                    // Define parameters used to create the BitmapSource.
                    System.Windows.Media.PixelFormat pf = System.Windows.Media.PixelFormats.Bgra32;
                    //int width = 200;
                    //int height = 200;
                    //int rawStride = (width * pf.BitsPerPixel + 7) / 8;
                    //with WPF it's just PixelWidth*BitsPerPixel 
                    int rawStride = (int)width * pf.BitsPerPixel;
                    byte[] rawImage = new byte[rawStride * height];

                    AdBitmap[] pBitmap = new AdBitmap[1];
                    pBitmap[0].width = width;
                    pBitmap[0].height = height;
                    pBitmap[0].stride = rawStride;
                    pBitmap[0].format = PixelFormatType.Argb32;
                    pBitmap[0].data = Marshal.UnsafeAddrOfPinnedArrayElement(rawImage, 0);
                    //CoreDll.Error error = m_dll.adLoadBitmapW(path, Marshal.UnsafeAddrOfPinnedArrayElement(pBitmap, 0));
                    //_core.LoadBitmap(path, Marshal.UnsafeAddrOfPinnedArrayElement(pBitmap, 0));

                    Error error = _core.LoadBitmap(path, pBitmap);

                    //Marshal.FreeHGlobal(pBitmap[0].data);

                    // Create a BitmapSource.
                    BitmapSource bitmap = BitmapSource.Create((int)width, (int)height,
                        96, 96, pf, null,
                        rawImage, rawStride);
                    rawImage = null;
                    return bitmap;

                    //BitmapSource bs = System.Windows.Interop.Imaging.CreateBitmapSourceFromHBitmap(
                    //  bmp.GetHbitmap(),
                    //  IntPtr.Zero,
                    //  System.Windows.Int32Rect.Empty,
                    //  BitmapSizeOptions.FromWidthAndHeight(bmp.Width, bmp.Height));
                    


                    /*
                    System.Drawing.Bitmap bitmap = null;
                    try
                    {
                        bitmap = new System.Drawing.Bitmap((int)width, (int)height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
                    }
                    catch (System.Exception)
                    {
                        GC.Collect();
                        try
                        {
                            bitmap = new System.Drawing.Bitmap((int)width, (int)height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
                        }
                        catch (System.Exception)
                        {
                            return null;
                        }
                    }
                    System.Drawing.Imaging.BitmapData bitmapData = new System.Drawing.Imaging.BitmapData();
                    bitmapData = bitmap.LockBits(
                        new System.Drawing.Rectangle(0, 0, (int)width, (int)height),
                        System.Drawing.Imaging.ImageLockMode.WriteOnly,
                        System.Drawing.Imaging.PixelFormat.Format32bppArgb,
                        bitmapData);

                    Core.CoreDll.adBitmap[] pBitmap = new Core.CoreDll.adBitmap[1];
                    pBitmap[0].width = (uint)bitmapData.Width;
                    pBitmap[0].height = (uint)bitmapData.Height;
                    pBitmap[0].stride = bitmapData.Stride;
                    pBitmap[0].format = CoreDll.PixelFormatType.Argb32;
                    pBitmap[0].data = bitmapData.Scan0;
                    bool result = _core.LoadBitmap(path, Marshal.UnsafeAddrOfPinnedArrayElement(pBitmap, 0));
                    bitmap.UnlockBits(bitmapData);

                    if (result)
                    {
                        using (MemoryStream str = new MemoryStream())
                        {
                            //bitmap.Save(Path.GetFileNameWithoutExtension(path));
                            bitmap.Save(str, ImageFormat.Bmp);
                            str.Seek(0, SeekOrigin.Begin);
                            BitmapDecoder bdc = new BmpBitmapDecoder(str, BitmapCreateOptions.PreservePixelFormat, BitmapCacheOption.OnLoad);
                            // Retrieve the BitmapSource
                            //image1.Source = bdc.Frames[0];
                            return bdc.Frames[0];
                        }
                    }*/
                }
             }
             return null;
        }


        public BitmapSource LoadResizedBitmapImage(string path, uint width, uint height, CoreDll.ImageType imageType, int resizedWidth)
        {
            if (File.Exists(path))
            {
                BitmapImage image;
                //может считать net Framework
                if (imageType <= CoreDll.ImageType.Heif)
                {
                    using (Stream stream = File.OpenRead(path))
                    {
                        image = new BitmapImage();
                        image.BeginInit();
                        image.CreateOptions = BitmapCreateOptions.IgnoreColorProfile;
                        image.CacheOption = BitmapCacheOption.OnLoad;
                        //image.DecodePixelWidth = Const.IMAGE_WIDTH;
                        //image.DecodePixelWidth = (int)(resizedWidth * 1.5);
                        image.DecodePixelWidth = resizedWidth;
                        //image.DecodePixelHeight = Const.IMAGE_HEIGHT;
                        image.StreamSource = stream;
                        image.EndInit();
                        image.Freeze();

                        GC.Collect(GC.MaxGeneration);
                    }
                }
                else
                {
                    if (height * width == 0)
                        return null;

                    // Define parameters used to create the BitmapSource.
                    System.Windows.Media.PixelFormat pf = System.Windows.Media.PixelFormats.Bgra32;
                    //int rawStride = (width * pf.BitsPerPixel + 7) / 8;
                    //with WPF it's just PixelWidth*BitsPerPixel 
                    int rawStride = (int)width * pf.BitsPerPixel;
                    byte[] rawImage = new byte[rawStride * height];

                    AdBitmap[] pBitmap = new AdBitmap[1];
                    pBitmap[0].width = width;
                    pBitmap[0].height = height;
                    pBitmap[0].stride = rawStride;
                    pBitmap[0].format = PixelFormatType.Argb32;
                    pBitmap[0].data = Marshal.UnsafeAddrOfPinnedArrayElement(rawImage, 0);
                    Error error = _core.LoadBitmap(path, pBitmap);

                    BitmapSource bitmapSource = BitmapSource.Create((int)width, (int)height,
                                        96, 96, pf, null,
                                        rawImage, rawStride);

                    //bitmapSource.Freeze();
                    //return bitmapSource;

                    //int resizedHeight = (int)(height / (int)(width / resizedWidth));
                    //var resizedImage = CreateResizedImage(bitmapSource, resizedWidth, resizedHeight, 0);
                    //resizedImage.Freeze();
                    //return resizedImage;

                    int scale = (int)(width / resizedWidth);
                    var resizedBitmap = new TransformedBitmap(bitmapSource, new ScaleTransform(scale, scale));
                    //bitmap.BeginInit();
                    //bitmap.CreateOptions = BitmapCreateOptions.IgnoreColorProfile;
                    //bitmap.CacheOption = BitmapCacheOption.OnLoad;
                    //bitmap.DecodePixelWidth = resizedWidth;
                    //bitmap.EndInit();
                    resizedBitmap.Freeze();
                    return resizedBitmap;

                    using (MemoryStream stream = new MemoryStream(rawImage))
                    {
                        image = new BitmapImage();
                        image.BeginInit();
                        image.CreateOptions = BitmapCreateOptions.IgnoreColorProfile;
                        image.CacheOption = BitmapCacheOption.OnLoad;
                        //image.DecodePixelWidth = Const.IMAGE_WIDTH;
                        image.DecodePixelWidth = resizedWidth;
                        //image.DecodePixelHeight = Const.IMAGE_HEIGHT;
                        image.StreamSource = stream;
                        image.EndInit();
                        image.Freeze();

                        GC.Collect(GC.MaxGeneration);
                    }
                    rawImage = null;
                }

                return image;
            }
            return null;
        }

        private static BitmapFrame CreateResizedImage(ImageSource source, int width, int height, int margin)
        {
            var rect = new System.Windows.Rect(margin, margin, width - margin * 2, height - margin * 2);

            var group = new DrawingGroup();
            RenderOptions.SetBitmapScalingMode(group, BitmapScalingMode.HighQuality);
            group.Children.Add(new ImageDrawing(source, rect));

            var drawingVisual = new DrawingVisual();
            using (var drawingContext = drawingVisual.RenderOpen())
                drawingContext.DrawDrawing(group);

            var resizedImage = new RenderTargetBitmap(
                width, height,         // Resized dimensions
                96, 96,                // Default DPI values
                PixelFormats.Default); // Default pixel format
            resizedImage.Render(drawingVisual);

            return BitmapFrame.Create(resizedImage);
        }
    }
}
