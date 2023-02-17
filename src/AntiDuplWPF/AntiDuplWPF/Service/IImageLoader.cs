using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.Service
{
    interface IImageLoader
    {
        System.Windows.Media.Imaging.BitmapSource LoadImage(string path, uint width, uint height, Core.CoreDll.ImageType type);

        System.Windows.Media.Imaging.BitmapSource LoadResizedBitmapImage(string path, uint width, uint height, Core.CoreDll.ImageType imageType, int resizedWidth);
    }
}
