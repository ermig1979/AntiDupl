using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.Core
{
    public class CoreDll : DynamicModule
    {
        public CoreDll()
            : base(IntPtr.Size == 8 ? "AntiDupl64.dll" : "AntiDupl32.dll")
        {
        }

        //-----------API constants:--------------------------------------------

        public const int FALSE = 0;
        public const int TRUE = 1;

        public const int MAX_PATH = 260;
        public const int MAX_PATH_EX = 32768;

        //-----------API enumerations------------------------------------------

        public enum Error : int
        {
            Ok = 0,
            Unknown = 1,
            AccessDenied = 2,
            InvalidPointer = 3,
            InvalidFileFormat = 4,
            InvalidPathType = 5,
            InvalidOptionsType = 6,
            InvalidFileType = 7,
            InvalidSortType = 8,
            InvalidGlobalActionType = 9,
            InvalidThreadId = 10,
            InvalidStartPosition = 11,
            OutputBufferIsTooSmall = 12,
            FileIsNotExists = 13,
            CantOpenFile = 14,
            CantCreateFile = 15,
            CantReadFile = 16,
            CantWriteFile = 17,
            InvalidFileName = 18,
            InvalidLocalActionType = 19,
            InvalidTargetType = 20,
            InvalidIndex = 21,
            ZeroTarget = 22,
            PathTooLong = 23,
            CantLoadImage = 24,
            InvalidBitmap = 25,
            InvalidThreadType = 26,
            InvalidActionEnableType = 27,
            InvalidParameterCombination = 28,
            InvalidRenameCurrentType = 29,
            InvalidInfoType = 30,
            InvalidGroupId = 31,
            InvalidSelectionType = 32,
        }

        public enum StateType : int
        {
            None = 0,
            Work = 1,
            Wait = 2,
            Stop = 3,
        }

        public enum ThreadType : int
        {
            Main = 0,
            Collect = 1,
            Compare = 2,
        }

        public enum PathType : int
        {
            Search = 0,
            Ignore = 1,
            Valid = 2,
            Delete = 3,
        }

        public enum ResultType : int
        {
            None = 0,
            DefectImage = 1,
            DuplImagePair = 2,
        }

        public enum DefectType : int
        {
            None = 0,
            Unknown = 1,
            JpegEndMarkerIsAbsent = 2,
            Blockiness = 3,
            Blurring = 4,
        }

        public enum TransformType : int
        {
            Turn_0 = 0,
            Turn_90 = 1,
            Turn_180 = 2,
            Turn_270 = 3,
            MirrorTurn_0 = 4,
            MirrorTurn_90 = 5,
            MirrorTurn_180 = 6,
            MirrorTurn_270 = 7,
        }

        public enum HintType : int
        {
            None = 0,
            DeleteFirst = 1,
            DeleteSecond = 2,
            RenameFirstToSecond = 3,
            RenameSecondToFirst = 4,
        }

        public enum ImageType : int
        {
            None = 0,
            Bmp = 1,
            Gif = 2,
            Jpeg = 3,
            Png = 4,
            Tiff = 5,
            Emf = 6,
            Wmf = 7,
            Exif = 8,
            Icon = 9,
            Jp2 = 10,
            Psd = 11,
            Dds = 12,
            Tga = 13,
        }

        public enum AlgorithmComparing : int
        {
            SquaredSum = 0,
            SSIM = 1,
        };

        public enum OptionsType : int
        {
            SetDefault = -1,
            Search = 0,
            Compare = 1,
            Defect = 2,
            Advanced = 3,
        }

        //-----------API structures--------------------------------------------

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct adStatusW
        {
            public StateType state;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = CoreDll.MAX_PATH_EX)]
            public string path;
            public UIntPtr current;
            public UIntPtr total;
        }

        public const int MAX_EXIF_SIZE = 260;
        // Она же class TImageExif decimal в dll
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct adImageExifW
        {
            public int isEmpty;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = MAX_EXIF_SIZE)]
            public string imageDescription;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = MAX_EXIF_SIZE)]
            public string equipMake;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = MAX_EXIF_SIZE)]
            public string equipModel;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = MAX_EXIF_SIZE)]
            public string softwareUsed;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = MAX_EXIF_SIZE)]
            public string dateTime;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = MAX_EXIF_SIZE)]
            public string artist;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = MAX_EXIF_SIZE)]
            public string userComment;
        }

        // Она же структура TImageInfo в dll.
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct adImageInfoW
        {
            public IntPtr id;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = CoreDll.MAX_PATH_EX)]
            public string path;
            public ulong size;
            public ulong time;
            public ImageType type;
            public uint width;
            public uint height;
            public double blockiness;
            public double blurring;
            public adImageExifW exifInfo;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public class adResultW
        {
            public ResultType type;
            [MarshalAs(UnmanagedType.Struct)]
            public adImageInfoW first;
            [MarshalAs(UnmanagedType.Struct)]
            public adImageInfoW second;
            public DefectType defect;
            public double difference;
            public TransformType transform;
            public IntPtr group;
            public IntPtr groupSize;
            public HintType hint;
        }

        //-------------------API functions:------------------------------------

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void adCreate_fn();
        [DynamicModuleApi]
        public adCreate_fn adCreate = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adRelease_fn();
        [DynamicModuleApi]
        public adRelease_fn adRelease = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adSearch_fn();
        [DynamicModuleApi]
        public adSearch_fn adSearch = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adStop_fn();
        [DynamicModuleApi]
        public adStop_fn adStop = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adStatusGetW_fn(ThreadType threadType, IntPtr threadId, IntPtr pStatusW);
        [DynamicModuleApi]
        public adStatusGetW_fn adStatusGetW = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate Error adClearLocations_fn(PathType pathType);
        [DynamicModuleApi]
        public adClearLocations_fn adClearLocations = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate Error adAddLocations_fn([In]PathType pathType, [In] Location location);
        [DynamicModuleApi]
        public adAddLocations_fn adAddLocations = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate uint adGetResultSize_fn();
        [DynamicModuleApi]
        public adGetResultSize_fn adGetResultSize = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void adGetResult_fn(IntPtr pResult, [In] uint index);
        [DynamicModuleApi]
        public adGetResult_fn adGetResult = null;

        /*[UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adOptionsSet_fn(OptionsType optionsType, IntPtr pOptions);
        [DynamicModuleApi]
        public adOptionsSet_fn adOptionsSet = null;*/

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate Error adSetSearchOptions_fn([In, Out] SearchOption options);
        [DynamicModuleApi]
        public adSetSearchOptions_fn adSetSearchOptions = null;
    }
}
