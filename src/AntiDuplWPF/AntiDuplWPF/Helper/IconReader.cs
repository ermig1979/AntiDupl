using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media.Imaging;

namespace AntiDuplWPF.Helper
{
    class IconReader
    {
        #region PInvoke
        [DllImport("shell32.dll", CharSet = CharSet.Auto)]
        private static extern IntPtr SHGetFileInfo(string pszPath, uint dwFileAttributes, ref SHFILEINFO psfi, uint cbFileInfo, uint uFlags);

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
        private struct SHFILEINFO
        {
            public IntPtr hIcon;
            public int iIcon;
            public uint dwAttributes;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
            public string szDisplayName;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 80)]
            public string szTypeName;
        };

        [Flags]
        enum SHGFI : uint
        {
            /// <summary>get icon</summary>
            Icon = 0x000000100,
            /// <summary>get display name</summary>
            DisplayName = 0x000000200,
            /// <summary>get type name</summary>
            TypeName = 0x000000400,
            /// <summary>get attributes</summary>
            Attributes = 0x000000800,
            /// <summary>get icon location</summary>
            IconLocation = 0x000001000,
            /// <summary>return exe type</summary>
            ExeType = 0x000002000,
            /// <summary>get system icon index</summary>
            SysIconIndex = 0x000004000,
            /// <summary>put a link overlay on icon</summary>
            LinkOverlay = 0x000008000,
            /// <summary>show icon in selected state</summary>
            Selected = 0x000010000,
            /// <summary>get only specified attributes</summary>
            Attr_Specified = 0x000020000,
            /// <summary>get large icon</summary>
            LargeIcon = 0x000000000,
            /// <summary>get small icon</summary>
            SmallIcon = 0x000000001,
            /// <summary>get open icon</summary>
            OpenIcon = 0x000000002,
            /// <summary>get shell size icon</summary>
            ShellIconSize = 0x000000004,
            /// <summary>pszPath is a pidl</summary>
            PIDL = 0x000000008,
            /// <summary>use passed dwFileAttribute</summary>
            UseFileAttributes = 0x000000010,
            /// <summary>apply the appropriate overlays</summary>
            AddOverlays = 0x000000020,
            /// <summary>Get the index of the overlay in the upper 8 bits of the iIcon</summary>
            OverlayIndex = 0x000000040,
        }

        private const uint FILE_ATTRIBUTE_DIRECTORY = 0x10;
        private const uint FILE_ATTRIBUTE_NORMAL = 0x80;

        [DllImport("user32.dll", EntryPoint = "DestroyIcon", SetLastError = true)]
        private static extern int DestroyIcon(IntPtr hIcon);
        #endregion

        private static Dictionary<string, BitmapSource> _cache;
        //private static Dictionary<string, Icon> _cache;

        public static BitmapSource GetIcon(string path, bool isDirectory)
        {
            string key = null;

            if (_cache == null)
                _cache = new Dictionary<string, BitmapSource>();

            if (isDirectory)
                key = "directory";
            else if (!String.IsNullOrEmpty(path))
                key = Path.GetExtension(path).ToLower();

            if (!String.IsNullOrEmpty(key))
            {
                if (!_cache.ContainsKey(key))
                    _cache.Add(key, GetFileIcon(path));

                return _cache[key];
            }
            return null;
        }

        private static BitmapSource GetFileIcon(string fileName)
        {
            uint attribs;
            if (Directory.Exists(fileName))
                attribs = FILE_ATTRIBUTE_DIRECTORY;
            else
                attribs = FILE_ATTRIBUTE_NORMAL;

            SHFILEINFO fileInfo = new SHFILEINFO();
            if (IntPtr.Zero == SHGetFileInfo(fileName,
                attribs,
                ref fileInfo,
                (uint)Marshal.SizeOf(fileInfo),
                (uint)(SHGFI.Icon | SHGFI.UseFileAttributes | SHGFI.SmallIcon)))
            {
                return null;
            }

            if (fileInfo.hIcon == (IntPtr)0)
                return null;

            //Icon icon = (Icon)Icon.FromHandle(fileInfo.hIcon).Clone();
            var icon = Imaging.CreateBitmapSourceFromHIcon(
                        fileInfo.hIcon,
                        Int32Rect.Empty,
                        BitmapSizeOptions.FromEmptyOptions());
            DestroyIcon(fileInfo.hIcon);

            return icon;
        }
    }
}
