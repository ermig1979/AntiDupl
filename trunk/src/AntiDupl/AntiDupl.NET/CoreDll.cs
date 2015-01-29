/*
* AntiDupl.NET Program.
*
* Copyright (c) 2002-2015 Yermalayeu Ihar, 2013-2015 Borisov Dmitry.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy 
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
* copies of the Software, and to permit persons to whom the Software is 
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in 
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace AntiDupl.NET
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
        
        public enum PathType : int
        {
            Search = 0,
            Ignore = 1,
            Valid = 2,
            Delete = 3,
        }

        public enum OptionsType : int
        {
            SetDefault = -1,
            Search = 0,
            Compare = 1,
            Defect = 2,
            Advanced = 3,
        }

        public enum FileType : int
        {
            Options = 0,
            Result = 1,
            MistakeDataBase = 2,
            ImageDataBase = 3,
            Temporary = 4,
        }

        public enum SortType : int
        {
            ByType = 0,
            BySortedPath = 1,
            BySortedName = 2,
            BySortedDirectory = 3,
            BySortedSize = 4,
            BySortedTime = 5,
            BySortedType = 6,
            BySortedWidth = 7,
            BySortedHeight = 8,
            BySortedArea = 9,
            BySortedRatio = 10,
            BySortedBlockiness = 11,
            BySortedBlurring = 12,
            ByFirstPath = 13,
            ByFirstName = 14,
            ByFirstDirectory = 15,
            ByFirstSize = 16,
            ByFirstTime = 17,
            ByFirstType = 18,
            ByFirstWidth = 19,
            ByFirstHeight = 20,
            ByFirstArea = 21,
            ByFirstRatio = 22,
            ByFirstBlockiness = 23,
            ByFirstBlurring = 24,
            BySecondPath = 25,
            BySecondName = 26,
            BySecondDirectory = 27,
            BySecondSize = 28,
            BySecondTime = 29,
            BySecondType = 30,
            BySecondWidth = 31,
            BySecondHeight = 32,
            BySecondArea = 33,
            BySecondRatio = 34,
            BySecondBlockiness = 35,
            BySecondBlurring = 36,
            ByDefect = 37,
            ByDifference = 38,
            ByTransform = 39,
            ByGroup = 40,
            ByHint = 41,
        }

        public enum GlobalActionType : int
        {
            SetHint = 0,
            SetGroup = 1,
            Refresh = 2,
            Undo = 3,
            Redo = 4,
        }

        public enum LocalActionType : int
        {
            DeleteDefect = 0,
            DeleteFirst = 1,
            DeleteSecond = 2,
            DeleteBoth = 3,
            RenameFirstToSecond = 4,
            RenameSecondToFirst = 5,
            PerformHint = 6,
            Mistake = 7,
        }

        public enum ActionEnableType : int
        {
            Any = 0,
            Defect = 1,
            DuplPair = 2,
            PerformHint = 3,
            Undo = 4,
            Redo = 5,
        }

        public enum TargetType : int
        {
            Current = 0,
            Selected = 1,
        }

        public enum RenameCurrentType : int
        {
            First = 0,
            Second = 1,
        }

        public enum StateType : int
        {
            None = 0,
            Work = 1,
            Wait = 2,
            Stop = 3,
        }

        public enum ResultType : int
        {
            None = 0,
            DefectImage = 1,
            DuplImagePair = 2,
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

        public enum PixelFormatType : int
        {
            None = 0,
            Argb32 = 1,
        }

        public enum ThreadType : int
        {
            Main = 0,
            Collect = 1,
            Compare = 2,
        }

        public enum VersionType : int
        {
            AntiDupl = 0,
            Simd = 1,
            OpenJpeg = 2,
        }

        public enum SelectionType : int
        {
            SelectCurrent = 0,
            UnselectCurrent = 1,
            SelectAll = 2,
            UnselectAll = 3,
            SelectAllButThis = 4,
        }
        
        //-----------API structures--------------------------------------------

        [StructLayout(LayoutKind.Sequential)]
        public struct adSearchOptions
        {
            public int system;
            public int hidden;
            public int JPEG;
            public int BMP;
            public int GIF;
            public int PNG;
            public int TIFF;
            public int EMF;
            public int WMF;
            public int EXIF;
            public int ICON;
            public int JP2;
            public int PSD;
            public int DDS;
            public int TGA;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct adCompareOptions
        {
            public int checkOnEquality;
            public int transformedImage;
            public int sizeControl;
            public int typeControl;
            public int ratioControl;
            public int thresholdDifference;
            public int minimalImageSize;
            public int maximalImageSize;
            public int compareInsideOneFolder;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct adDefectOptions
        {
            public int checkOnDefect;
            public int checkOnBlockiness;
            public int blockinessThreshold;
            public int checkOnBlockinessOnlyNotJpeg;
            public int checkOnBlurring;
            public int blurringThreshold;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct adAdvancedOptions
        {
            public int deleteToRecycleBin;
            public int mistakeDataBase;
            public int ratioResolution;
            public int compareThreadCount;
            public int collectThreadCount;
            public int reducedImageSize;
            public int undoQueueSize;
            public int resultCountMax;
            public int ignoreFrameWidth;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct adStatistic
        {
            public UIntPtr scanedFolderNumber;
            public UIntPtr searchedImageNumber;
            public UIntPtr collectedImageNumber;
            public UIntPtr comparedImageNumber;
            public UIntPtr collectThreadCount;
            public UIntPtr compareThreadCount;
            public UIntPtr defectImageNumber;
            public UIntPtr duplImagePairNumber;
            public UIntPtr renamedImageNumber;
            public UIntPtr deletedImageNumber;
            public ulong deletedImageSize;
        };

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
            public HintType hint;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct adGroup
        {
            public IntPtr id;
            public IntPtr size;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct adBitmap
        {
            public uint width;
            public uint height;
            public int stride;
            public PixelFormatType format;
            public IntPtr data;
        }

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct adPathWithSubFolderW
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = CoreDll.MAX_PATH_EX)]
            public string path;
            public int enableSubFolder;
        }
    
        //-------------------API functions:------------------------------------

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adVersionGet_fn(VersionType versionType, IntPtr pVersion, IntPtr pVersionSize);
        [DynamicModuleApi]
        public adVersionGet_fn adVersionGet = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate IntPtr adCreate_fn();
        [DynamicModuleApi]
        public adCreate_fn adCreate = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adRelease_fn(IntPtr handle);
        [DynamicModuleApi]
        public adRelease_fn adRelease = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adStop_fn(IntPtr handle);
        [DynamicModuleApi]
        public adStop_fn adStop = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adSearch_fn(IntPtr handle);
        [DynamicModuleApi]
        public adSearch_fn adSearch = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adLoadW_fn(IntPtr handle, FileType fileType, string fileName, int check);
        [DynamicModuleApi]
        public adLoadW_fn adLoadW = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adSaveW_fn(IntPtr handle, FileType fileType, string fileName);
        [DynamicModuleApi]
        public adSaveW_fn adSaveW = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adClear_fn(IntPtr handle, FileType fileType);
        [DynamicModuleApi]
        public adClear_fn adClear = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adOptionsGet_fn(IntPtr handle, OptionsType optionsType, IntPtr pOptions);
        [DynamicModuleApi]
        public adOptionsGet_fn adOptionsGet = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adOptionsSet_fn(IntPtr handle, OptionsType optionsType, IntPtr pOptions);
        [DynamicModuleApi]
        public adOptionsSet_fn adOptionsSet = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adPathWithSubFolderSetW_fn(IntPtr handle, PathType pathType, IntPtr pPaths, IntPtr pathSize);
        [DynamicModuleApi]
        public adPathWithSubFolderSetW_fn adPathWithSubFolderSetW = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adPathGetW_fn(IntPtr handle, PathType pathType, IntPtr pPath, IntPtr pPathSize);
        [DynamicModuleApi]
        public adPathGetW_fn adPathGetW = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adPathSetW_fn(IntPtr handle, PathType pathType, IntPtr pPath, IntPtr pathSize);
        [DynamicModuleApi]
        public adPathSetW_fn adPathSetW = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adStatisticGet_fn(IntPtr handle, IntPtr pStatistic);
        [DynamicModuleApi]
        public adStatisticGet_fn adStatisticGet = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adStatusGetW_fn(IntPtr handle, ThreadType threadType, IntPtr threadId, IntPtr pStatusW);
        [DynamicModuleApi]
        public adStatusGetW_fn adStatusGetW = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adResultSort_fn(IntPtr handle, SortType sortType, int increasing);
        [DynamicModuleApi]
        public adResultSort_fn adResultSort = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adResultApply_fn(IntPtr handle, GlobalActionType globalActionType);
        [DynamicModuleApi]
        public adResultApply_fn adResultApply = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adResultApplyTo_fn(IntPtr handle, LocalActionType localActionType, TargetType targetType);
        [DynamicModuleApi]
        public adResultApplyTo_fn adResultApplyTo = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adCanApply_fn(IntPtr handle, ActionEnableType actionEnableType, IntPtr pEnable);
        [DynamicModuleApi]
        public adCanApply_fn adCanApply = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adRenameCurrentW_fn(IntPtr handle, RenameCurrentType renameCurrentType, string newFileName);
        [DynamicModuleApi]
        public adRenameCurrentW_fn adRenameCurrentW = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adResultGetW_fn(IntPtr handle, IntPtr pStartFrom, IntPtr pResult, IntPtr pResultSize);
        [DynamicModuleApi]
        public adResultGetW_fn adResultGetW = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adSelectionSet_fn(IntPtr handle, IntPtr pStartFrom, UIntPtr size, int value);
        [DynamicModuleApi]
        public adSelectionSet_fn adSelectionSet = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adSelectionGet_fn(IntPtr handle, IntPtr pStartFrom, IntPtr pSelection, IntPtr pSelectionSize);
        [DynamicModuleApi]
        public adSelectionGet_fn adSelectionGet = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adCurrentSet_fn(IntPtr handle, IntPtr index);
        [DynamicModuleApi]
        public adCurrentSet_fn adCurrentSet = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adCurrentGet_fn(IntPtr handle, IntPtr pIndex);
        [DynamicModuleApi]
        public adCurrentGet_fn adCurrentGet = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adGroupGet_fn(IntPtr handle, IntPtr pStartFrom, IntPtr pGroup, IntPtr pGroupSize);
        [DynamicModuleApi]
        public adGroupGet_fn adGroupGet = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adImageInfoGetW_fn(IntPtr handle, IntPtr groupId, IntPtr pStartFrom, IntPtr pImageInfo, IntPtr pImageInfoSize);
        [DynamicModuleApi]
        public adImageInfoGetW_fn adImageInfoGetW = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adImageInfoSelectionSet_fn(IntPtr handle, IntPtr groupId, IntPtr index, SelectionType selectionType);
        [DynamicModuleApi]
        public adImageInfoSelectionSet_fn adImageInfoSelectionSet = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adImageInfoSelectionGet_fn(IntPtr handle, IntPtr groupId, IntPtr pStartFrom, IntPtr pSelection, IntPtr pSelectionSize);
        [DynamicModuleApi]
        public adImageInfoSelectionGet_fn adImageInfoSelectionGet = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adImageInfoRenameW_fn(IntPtr handle, IntPtr groupId, IntPtr index, string newFileName);
        [DynamicModuleApi]
        public adImageInfoRenameW_fn adImageInfoRenameW = null;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public delegate Error adLoadBitmapW_fn(IntPtr handle, string fileName, IntPtr pBitmap);
        [DynamicModuleApi]
        public adLoadBitmapW_fn adLoadBitmapW = null;
    }
}
