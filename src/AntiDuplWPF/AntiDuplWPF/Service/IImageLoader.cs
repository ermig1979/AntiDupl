using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using AntiDupl.NET.Core.Original;

namespace AntiDuplWPF.Service
{
    interface IImageLoader
    {
        System.Windows.Media.Imaging.BitmapSource LoadImage(string path, uint width, uint height, CoreDll.ImageType type);

        System.Windows.Media.Imaging.BitmapSource LoadResizedBitmapImage(string path, uint width, uint height, CoreDll.ImageType imageType, int resizedWidth);
    }
}
