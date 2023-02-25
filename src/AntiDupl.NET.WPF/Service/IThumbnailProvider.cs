using System;
using System.ComponentModel;
namespace AntiDupl.NET.WPF.Service
{
    public interface IThumbnailProvider : IDisposable
    {
        event PropertyChangedEventHandler PropertyChanged;
        void ClearThumbnailCache();
        void Dispose();
        void Enqueue(AntiDupl.NET.WPF.ObjectModel.ImageInfoClass item);
        string ThumbnailCacheStatus { get; set; }
    }
}
