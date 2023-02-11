using System;
using System.Runtime.InteropServices;
using AntiDupl.NET.Core.Enums;

namespace AntiDupl.NET.Core.Original
{
    [StructLayout(LayoutKind.Sequential)]
    public struct AdBitmap
    {
        public uint width;
        public uint height;
        public int stride;
        public PixelFormatType format;
        public IntPtr data;
    }
}
