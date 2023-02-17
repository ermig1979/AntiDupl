using System;
using System.ComponentModel;
namespace AntiDuplWPF.Service
{
    public interface IThumbnailProvider : IDisposable
    {
        event PropertyChangedEventHandler PropertyChanged;
        void ClearThumbnailCache();
        void Dispose();
        void Enqueue(AntiDuplWPF.ObjectModel.ImageInfoClass item);
        string ThumbnailCacheStatus { get; set; }
    }
}
