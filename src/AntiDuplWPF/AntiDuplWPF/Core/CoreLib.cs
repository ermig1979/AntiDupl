using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.Core
{
    public class CoreLib : IDisposable
    {
        private CoreDll _dll = null;

        //-----------Public functions----------------------------------------------

        public CoreLib()
        {
            try
            {
                _dll = new CoreDll();
            }
            catch (Exception ex)
            {
                throw new Exception("Can't load core library! " + Environment.NewLine + ex.Message);
            }
            _dll.adCreate();
        }

        ~CoreLib()
        {
            Dispose();
        }

        public void Dispose()
        {
            Release();
            if (_dll != null)
            {
                _dll.Dispose();
                _dll = null;
            }
            GC.SuppressFinalize(this);
        }

        public void Release()
        {
            if (_dll != null)
            {
                if (_dll.adRelease() == CoreDll.Error.AccessDenied)
                {
                    Stop();
                    Thread.Sleep(10);
                    _dll.adRelease();
                }
            }
        }

        public bool Stop()
        {
            return _dll.adStop() == CoreDll.Error.Ok;
        }

        public bool Search()
        {
            return _dll.adSearch() == CoreDll.Error.Ok;
        }

        //public CoreDll.Status2 GetStatus()
        //{
        //    return _dll.GetStatus();
        //}

        public CoreStatus StatusGet(CoreDll.ThreadType threadType, int threadId)
        {
            object statusObject = new CoreDll.adStatusW();
            byte[] status = new byte[Marshal.SizeOf(statusObject)];
            IntPtr pStatus = Marshal.UnsafeAddrOfPinnedArrayElement(status, 0);
            if (_dll.adStatusGetW(threadType, new IntPtr(threadId), pStatus) == CoreDll.Error.Ok)
            {
                CoreDll.adStatusW statusW = (CoreDll.adStatusW)Marshal.PtrToStructure(pStatus, statusObject.GetType());
                try
                {
                    return new CoreStatus(ref statusW);
                }
                catch (Exception)
                {
                    return null;
                }
            }
            return null;
        }

        public bool SetLocation(IEnumerable<Location> locations, CoreDll.PathType pathType)
        {
            _dll.adClearLocations(pathType);
            foreach (Location item in locations)
            {
                _dll.adAddLocations(pathType, item);
            }
            return true;
        }

        public DuplPairViewModel[] GetResult()
        {
            List<DuplPairViewModel> result = new List<DuplPairViewModel>();
            uint size;
            size = _dll.adGetResultSize();
            // For efficiency, only compute the element size once
            int elemSize = Marshal.SizeOf(typeof(CoreDll.adResultW));
            // Populate the array
            for (uint i = 0; i < size; i++)
            {
                // Initialize unmanged memory to hold the struct.
                IntPtr ptr = Marshal.AllocHGlobal(elemSize);

                _dll.adGetResult(ptr, i);

                CoreDll.adResultW dpExport = (CoreDll.adResultW)Marshal.PtrToStructure(ptr, typeof(CoreDll.adResultW));
                result.Add(new DuplPairViewModel(dpExport));

                // Clean-up
                Marshal.DestroyStructure(ptr, typeof(CoreDll.adResultW));
                // Free the unmanaged memory.
                Marshal.FreeHGlobal(ptr);
            }
            return result.ToArray();
        }

        /*public void SetOptions(CoreDll.OptionsType type, object options)
        {
            _dll.adOptionsSet(type, Marshal.StructureToPtr(options);
        }*/

        public void SetSearchOptions(SearchOption options)
        {
            _dll.adSetSearchOptions(options);
        }
    }
}
