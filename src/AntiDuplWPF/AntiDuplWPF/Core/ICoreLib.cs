using System;
namespace AntiDuplWPF.Core
{
    public interface ICoreLib : IDisposable
    {
        void Dispose();

        bool LoadBitmap(string fileName, IntPtr pBitmap);

        AntiDuplWPF.ViewModel.DuplPairViewModel[] GetResult();
        void Release();
        bool Search();
        void SetAdvancedOptions(AdvancedOption options);
        void SetCompareOptions(CompareOption options);
        void SetDefectOptions(DefectOption options);
        bool SetLocation(System.Collections.Generic.IEnumerable<Location> locations, CoreDll.PathType pathType);
        void SetSearchOptions(SearchOption options);
        CoreStatus StatusGet(CoreDll.ThreadType threadType, int threadId);
        bool Stop();

        void CalculateHistogramPeaks(CoreDll.adImageInfoW[] array, CoreDll.WorkProgressInteropNegotiator negotiator);
    }
}
