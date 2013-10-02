/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2013 Yermalayeu Ihar, 2013 Borisov Dmitry.
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
#ifndef __AntiDupl_h__
#define __AntiDupl_h__

#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /*------------Common defines-------------------------------------------------*/

#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#endif
#if !defined(UNICODE) && defined(_UNICODE)
#define UNICODE
#endif

#ifdef ANTIDUPL_EXPORTS
#define DLLAPI __declspec(dllexport)
#else
#define DLLAPI __declspec(dllimport)
#endif

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef MAX_PATH_EX
#define MAX_PATH_EX 32768
#endif

#define AD_UNKNOWN_ID   -1
#define AD_IS_NOT_EXIST -1
#define AD_UNDEFINED    -1

    /*------------Types------------------------------------------------------*/

    typedef __int8 adInt8;
    typedef __int16 adInt16;
    typedef __int32 adInt32;
    typedef __int64 adInt64;

    typedef unsigned __int8 adUInt8;
    typedef unsigned __int16 adUInt16;
    typedef unsigned __int32 adUInt32;
    typedef unsigned __int64 adUInt64;

    typedef adInt8* adInt8Ptr;
    typedef adInt16* adInt16Ptr;
    typedef adInt32* adInt32Ptr;
    typedef adInt64* adInt64Ptr;

    typedef adUInt8* adUInt8Ptr;
    typedef adUInt16* adUInt16Ptr;
    typedef adUInt32* adUInt32Ptr;
    typedef adUInt64* adUInt64Ptr;

    typedef adInt32 adBool;
    typedef adBool* adBoolPtr;
    typedef size_t adSize;
    typedef adSize* adSizePtr;

#ifndef __AD_HANDLE__
    typedef void* adHandle;
#endif//__AD_HANDLE__

    typedef wchar_t adCharW;
    typedef char adCharA;
    typedef adCharW adPathW[MAX_PATH_EX];
    typedef adCharA adPathA[MAX_PATH];
    typedef adPathW* adPathPtrW;
    typedef adPathA* adPathPtrA;

    /*------------Enumerations-----------------------------------------------*/

    enum adError : adInt32
    {
        AD_OK = 0,
        AD_ERROR_UNKNOWN = 1,
        AD_ERROR_ACCESS_DENIED = 2,
        AD_ERROR_INVALID_POINTER = 3,
        AD_ERROR_INVALID_FILE_FORMAT = 4,
        AD_ERROR_INVALID_PATH_TYPE = 5,
        AD_ERROR_INVALID_OPTIONS_TYPE = 6,
        AD_ERROR_INVALID_FILE_TYPE = 7,
        AD_ERROR_INVALID_SORT_TYPE = 8,
        AD_ERROR_INVALID_GLOBAL_ACTION_TYPE = 9,
        AD_ERROR_INVALID_THREAD_ID = 10,
        AD_ERROR_INVALID_START_POSITION = 11,
        AD_ERROR_OUTPUT_BUFFER_IS_TOO_SMALL = 12,
        AD_ERROR_FILE_IS_NOT_EXIST = 13,
        AD_ERROR_CANT_OPEN_FILE = 14,
        AD_ERROR_CANT_CREATE_FILE = 15,
        AD_ERROR_CANT_READ_FILE = 16,
        AD_ERROR_CANT_WRITE_FILE = 17,
        AD_ERROR_INVALID_FILE_NAME = 18,
        AD_ERROR_INVALID_LOCAL_ACTION_TYPE = 19,
        AD_ERROR_INVALID_TARGET_TYPE = 20,
        AD_ERROR_INVALID_INDEX = 21,
        AD_ERROR_ZERO_TARGET = 22,
        AD_ERROR_PATH_TO_LONG = 23,
        AD_ERROR_CANT_LOAD_IMAGE = 24,
        AD_ERROR_INVALID_BITMAP = 25,
        AD_ERROR_INVALID_THREAD_TYPE = 26,
        AD_ERROR_INVALID_ACTION_ENABLE_TYPE = 27,
        AD_ERROR_INVALID_PARAMETER_COMBINATION = 28,
        AD_ERROR_INVALID_RENAME_CURRENT_TYPE = 29,
        AD_ERROR_INVALID_VERSION_TYPE = 30,
		AD_ERROR_INVALID_GROUP_ID = 31,
		AD_ERROR_INVALID_SELECTION_TYPE = 32,
		AD_ERROR_DIRECTORY_IS_NOT_EXIST = 33,
	};
    
    enum adPathType : adInt32
    {
        AD_PATH_SEARCH = 0,
        AD_PATH_IGNORE = 1,
        AD_PATH_VALID = 2,
        AD_PATH_DELETE = 3,
        AD_PATH_SIZE
    };

    enum adOptionsType : adInt32
    {
        AD_OPTIONS_SET_DEFAULT = -1,
        AD_OPTIONS_SEARCH = 0,
        AD_OPTIONS_COMPARE = 1,
        AD_OPTIONS_DEFECT = 2,
        AD_OPTIONS_ADVANCED = 3,
        AD_OPTIONS_SIZE
    };

    enum adFileType : adInt32
    {
        AD_FILE_OPTIONS = 0,
        AD_FILE_RESULT = 1,
        AD_FILE_MISTAKE_DATA_BASE = 2,
        AD_FILE_IMAGE_DATA_BASE = 3,
        AD_FILE_TEMPORARY = 4,
        AD_FILE_SIZE
    };

    enum adSortType : adInt32
    {
        AD_SORT_BY_TYPE = 0,
        AD_SORT_BY_SORTED_PATH = 1,
        AD_SORT_BY_SORTED_NAME = 2,
        AD_SORT_BY_SORTED_DIRECTORY = 3,
        AD_SORT_BY_SORTED_SIZE = 4,
        AD_SORT_BY_SORTED_TIME = 5,
        AD_SORT_BY_SORTED_TYPE = 6,
        AD_SORT_BY_SORTED_WIDTH = 7,
        AD_SORT_BY_SORTED_HEIGHT = 8,
        AD_SORT_BY_SORTED_AREA = 9,
        AD_SORT_BY_SORTED_RATIO = 10,
		AD_SORT_BY_SORTED_BLOCKINESS = 11,
		AD_SORT_BY_SORTED_BLURRING = 12,
        AD_SORT_BY_FIRST_PATH = 13,
        AD_SORT_BY_FIRST_NAME = 14,
        AD_SORT_BY_FIRST_DIRECTORY = 15,
        AD_SORT_BY_FIRST_SIZE = 16,
        AD_SORT_BY_FIRST_TIME = 17,
        AD_SORT_BY_FIRST_TYPE = 18,
        AD_SORT_BY_FIRST_WIDTH = 19,
        AD_SORT_BY_FIRST_HEIGHT = 20,
        AD_SORT_BY_FIRST_AREA = 21,
        AD_SORT_BY_FIRST_RATIO = 22,
		AD_SORT_BY_FIRST_BLOCKINESS = 23,
		AD_SORT_BY_FIRST_BLURRING = 24,
        AD_SORT_BY_SECOND_PATH = 25,
        AD_SORT_BY_SECOND_NAME = 26,
        AD_SORT_BY_SECOND_DIRECTORY = 27,
        AD_SORT_BY_SECOND_SIZE = 28,
        AD_SORT_BY_SECOND_TIME = 29,
        AD_SORT_BY_SECOND_TYPE = 30,
        AD_SORT_BY_SECOND_WIDTH = 31,
        AD_SORT_BY_SECOND_HEIGHT = 32,
        AD_SORT_BY_SECOND_AREA = 33,
        AD_SORT_BY_SECOND_RATIO = 34,
		AD_SORT_BY_SECOND_BLOCKINESS = 35,
		AD_SORT_BY_SECOND_BLURRING = 36,
        AD_SORT_BY_DEFECT = 37,
        AD_SORT_BY_DIFFERENCE = 38,
        AD_SORT_BY_TRANSFORM = 39,
        AD_SORT_BY_GROUP = 40,
        AD_SORT_BY_HINT = 41,
        AD_SORT_SIZE
    };

    enum adGlobalActionType : adInt32
    {
        AD_GLOBAL_ACTION_SET_HINT = 0,
        AD_GLOBAL_ACTION_SET_GROUP = 1,
        AD_GLOBAL_ACTION_REFRESH = 2,
        AD_GLOBAL_ACTION_UNDO = 3,
        AD_GLOBAL_ACTION_REDO = 4,
        AD_GLOBAL_ACTION_SIZE
    };

    enum adLocalActionType : adInt32
    {
        AD_LOCAL_ACTION_DELETE_DEFECT = 0,
        AD_LOCAL_ACTION_DELETE_FIRST = 1,
        AD_LOCAL_ACTION_DELETE_SECOND = 2,
        AD_LOCAL_ACTION_DELETE_BOTH = 3,
        AD_LOCAL_ACTION_RENAME_FIRST_TO_SECOND = 4,
        AD_LOCAL_ACTION_RENAME_SECOND_TO_FIRST = 5,
        AD_LOCAL_ACTION_PERFORM_HINT = 6,
        AD_LOCAL_ACTION_MISTAKE = 7,
        AD_LOCAL_ACTION_SIZE
    };

    enum adActionEnableType : adInt32
    {
        AD_ACTION_ENABLE_ANY = 0,
        AD_ACTION_ENABLE_DEFECT = 1,
        AD_ACTION_ENABLE_DUPL_PAIR = 2,
        AD_ACTION_ENABLE_PEFORM_HINT = 3,
        AD_ACTION_ENABLE_UNDO = 4,
        AD_ACTION_ENABLE_REDO = 5,
        AD_ACTION_ENABLE_SIZE
    };

    enum adTargetType : adInt32
    {
        AD_TARGET_CURRENT = 0,
        AD_TARGET_SELECTED = 1,
        AD_TARGET_SIZE
    };

    enum adRenameCurrentType : adInt32
    {
        AD_RENAME_CURRENT_FIRST = 0,
        AD_RENAME_CURRENT_SECOND = 1,
        AD_RENAME_CURRENT_SIZE
    };

    enum adStateType : adInt32
    {
        AD_STATE_NONE = 0,
        AD_STATE_WORK = 1,
        AD_STATE_WAIT = 2,
        AD_STATE_STOP = 3,
        AD_STATE_SIZE
    };

    enum adResultType : adInt32
    {
        AD_RESULT_NONE = 0,
        AD_RESULT_DEFECT_IMAGE = 1,
        AD_RESULT_DUPL_IMAGE_PAIR = 2,
        AD_RESULT_SIZE
    };

    enum adImageType : adInt32
    {
        AD_IMAGE_UNDEFINE = -1,
        AD_IMAGE_NONE = 0,
        AD_IMAGE_BMP = 1,
        AD_IMAGE_GIF = 2,
        AD_IMAGE_JPEG = 3,
        AD_IMAGE_PNG = 4,
        AD_IMAGE_TIFF = 5,
        AD_IMAGE_EMF = 6,
        AD_IMAGE_WMF = 7,
        AD_IMAGE_EXIF = 8,
        AD_IMAGE_ICON = 9,
        AD_IMAGE_JP2 = 10,
        AD_IMAGE_PSD = 11,
		AD_IMAGE_DDS = 12,
		AD_IMAGE_TGA = 13,
        AD_IMAGE_SIZE
    };

    enum adDefectType : adInt32
    {
        AD_DEFECT_UNDEFINE = -1,
        AD_DEFECT_NONE = 0,
        AD_DEFECT_UNKNOWN = 1,
        AD_DEFECT_JPEG_END_MARKER_IS_ABSENT = 2,
        AD_DEFECT_BLOCKINESS = 3,
        AD_DEFECT_BLURRING = 4,
        AD_DEFECT_SIZE
    };

    enum adTransformType : adInt32
    {
        AD_TRANSFORM_TURN_0 = 0,
        AD_TRANSFORM_TURN_90 = 1,
        AD_TRANSFORM_TURN_180 = 2,
        AD_TRANSFORM_TURN_270 = 3,
        AD_TRANSFORM_MIRROR_TURN_0 = 4,
        AD_TRANSFORM_MIRROR_TURN_90 = 5,
        AD_TRANSFORM_MIRROR_TURN_180 = 6,
        AD_TRANSFORM_MIRROR_TURN_270 = 7,
        AD_TRANSFORM_SIZE
    };

    enum adHintType : adInt32
    {
        AD_HINT_NONE = 0,
        AD_HINT_DELETE_FIRST = 1,
        AD_HINT_DELETE_SECOND = 2,
        AD_HINT_RENAME_FIRST_TO_SECOND = 3,
        AD_HINT_RENAME_SECOND_TO_FIRST = 4,
        AD_HINT_SIZE
    };

    enum adPixelFormat : adInt32
    {
        AD_PIXEL_FORMAT_NONE = 0,
        AD_PIXEL_FORMAT_ARGB32 = 1,
        AD_PIXEL_FORMAT_SIZE
    };

    enum adThreadType : adInt32
    {
        AD_THREAD_TYPE_MAIN = 0,
        AD_THREAD_TYPE_COLLECT = 1,
        AD_THREAD_TYPE_COMPARE = 2,
        AD_THREAD_TYPE_SIZE
    };

	enum adVersionType : adInt32
	{
		AD_VERSION_TYPE_ANTIDUPL = 0,
		AD_VERSION_TYPE_SIMD = 1,
		AD_VERSION_TYPE_OPENJPEG = 2,
		AD_VERSION_TYPE_SIZE
	};

	enum adSelectionType : adInt32
	{
		AD_SELECTION_SELECT_CURRENT = 0,
		AD_SELECTION_UNSELECT_CURRENT = 1,
		AD_SELECTION_SELECT_ALL = 2,
		AD_SELECTION_UNSELECT_ALL = 3,
		AD_SELECTION_SELECT_ALL_BUT_THIS = 4,
		AD_SELECTION_SIZE
	};

    /*------------Structures-----------------------------------------------------*/

    struct adSearchOptions
    {
        adBool subFolders;
        adBool system;
        adBool hidden;
        adBool JPEG;
        adBool BMP;
        adBool GIF;
        adBool PNG;
        adBool TIFF;
        adBool EMF;
        adBool WMF;
        adBool EXIF;
        adBool ICON;
        adBool JP2;
        adBool PSD;
		adBool DDS;
		adBool TGA;
    };
    typedef adSearchOptions* adSearchOptionsPtr;

    struct adCompareOptions
    {
        adBool checkOnEquality;    
        adBool transformedImage;    
        adBool sizeControl;
        adBool typeControl;
        adBool ratioControl;
        adInt32 thresholdDifference;
        adInt32 minimalImageSize;
		adInt32 maximalImageSize;
        adBool compareInsideOneFolder;
    };
    typedef adCompareOptions* adCompareOptionsPtr;
	
    struct adDefectOptions
    {
        adBool checkOnDefect;
        adBool checkOnBlockiness;
        adInt32 blockinessThreshold;
		adBool checkOnBlockinessOnlyNotJpeg;
        adBool checkOnBlurring;
        adInt32 blurringThreshold;
    };
    typedef adDefectOptions* adDefectOptionsPtr;

    struct adAdvancedOptions
    {
        adBool deleteToRecycleBin;
        adBool mistakeDataBase;
        adInt32 ratioResolution;
        adInt32 compareThreadCount;
        adInt32 collectThreadCount;
        adInt32 reducedImageSize;
        adInt32 undoQueueSize;
        adInt32 resultCountMax;        
        adInt32 ignoreFrameWidth;
    };
    typedef adAdvancedOptions* adAdvancedOptionsPtr;

    struct adStatistic
    {
        adSize scanedFolderNumber;
        adSize searchedImageNumber;
        adSize collectedImageNumber;
        adSize comparedImageNumber;
        adSize collectThreadCount;
        adSize compareThreadCount;
        adSize defectImageNumber;
        adSize duplImagePairNumber;
        adSize renamedImageNumber;
        adSize deletedImageNumber;
        adUInt64 deletedImageSize;
    };
    typedef adStatistic* adStatisticPtr;

    struct adStatusA
    {
        adStateType state;
        adPathA path;
        adSize current;
        adSize total;
    };
    typedef adStatusA* adStatusPtrA;

    struct adStatusW
    {
        adStateType state;
        adPathW path;
        adSize current;
        adSize total;
    };
    typedef adStatusW* adStatusPtrW;

    struct adImageInfoA
    {
        adSize id;
        adPathA path;
        adUInt64 size;
        adUInt64 time;
        adImageType type;
        adUInt32 width;
        adUInt32 height;
		double blockiness;
		double blurring;
    };
    typedef adImageInfoA* adImageInfoPtrA;

    struct adImageInfoW
    {
        adSize id;
        adPathW path;
        adUInt64 size;
        adUInt64 time;
        adImageType type;
        adUInt32 width;
        adUInt32 height;
 		double blockiness;
		double blurring;
   };
    typedef adImageInfoW* adImageInfoPtrW;

    struct adResultA
    {
        adResultType type;
        adImageInfoA first;
        adImageInfoA second;
        adDefectType defect;
        double difference;
        adTransformType transform;
        adSize group;
        adHintType hint;
    };
    typedef adResultA* adResultPtrA;

    struct adResultW
    {
        adResultType type;
        adImageInfoW first;
        adImageInfoW second;
        adDefectType defect;
        double difference;
        adTransformType transform;
        adSize group;
        adHintType hint;
    };
    typedef adResultW* adResultPtrW;

	struct adGroup
	{
		adSize id;
		adSize size;
	};
	typedef adGroup* adGroupPtr;

    struct adBitmap
    {
        adUInt32 width;
        adUInt32 height;
        adInt32 stride;
        adPixelFormat format;
        adUInt8Ptr data;
    };
    typedef adBitmap* adBitmapPtr;

    /*------------Functions-------------------------------------------------------*/

    DLLAPI adError adVersionGet(adVersionType versionType, adCharA * pVersion, adSizePtr pVersionSize);

    DLLAPI adHandle adCreate();
    DLLAPI adError adRelease(adHandle handle);

	DLLAPI adError adStop(adHandle handle);
    DLLAPI adError adSearch(adHandle handle);

    DLLAPI adError adLoadA(adHandle handle, adFileType fileType, const adCharA* fileName, adBool check);
    DLLAPI adError adLoadW(adHandle handle, adFileType fileType, const adCharW* fileName, adBool check);
    DLLAPI adError adSaveA(adHandle handle, adFileType fileType, const adCharA* fileName);
    DLLAPI adError adSaveW(adHandle handle, adFileType fileType, const adCharW* fileName);
    DLLAPI adError adClear(adHandle handle, adFileType fileType);

    DLLAPI adError adOptionsGet(adHandle handle, adOptionsType optionsType, void* pOptions);
    DLLAPI adError adOptionsSet(adHandle handle, adOptionsType optionsType, void* pOptions);

    DLLAPI adError adPathGetA(adHandle handle, adPathType pathType, adPathPtrA pPath, adSizePtr pPathSize);
    DLLAPI adError adPathGetW(adHandle handle, adPathType pathType, adPathPtrW pPath, adSizePtr pPathSize);
    DLLAPI adError adPathSetA(adHandle handle, adPathType pathType, adPathPtrA pPath, adSize pathSize);
    DLLAPI adError adPathSetW(adHandle handle, adPathType pathType, adPathPtrW pPath, adSize pathSize);

    DLLAPI adError adStatisticGet(adHandle handle, adStatisticPtr pStatistic);
    DLLAPI adError adStatusGetA(adHandle handle, adThreadType threadType, adSize threadId, adStatusPtrA pStatus);
    DLLAPI adError adStatusGetW(adHandle handle, adThreadType threadType, adSize threadId, adStatusPtrW pStatus);

    DLLAPI adError adResultGetA(adHandle handle, adSizePtr pStartFrom, adResultPtrA pResult, adSizePtr pResultSize);
    DLLAPI adError adResultGetW(adHandle handle, adSizePtr pStartFrom, adResultPtrW pResult, adSizePtr pResultSize);
    DLLAPI adError adResultSort(adHandle handle, adSortType sortType, adBool increasing);
    DLLAPI adError adResultApply(adHandle handle, adGlobalActionType globalActionType);
    DLLAPI adError adResultApplyTo(adHandle handle, adLocalActionType localActionType, adTargetType targetType);
    DLLAPI adError adCanApply(adHandle handle, adActionEnableType actionEnableType, adBoolPtr pEnable);

    DLLAPI adError adRenameCurrentA(adHandle handle, adRenameCurrentType renameCurrentType, const adCharA* newFileName);
    DLLAPI adError adRenameCurrentW(adHandle handle, adRenameCurrentType renameCurrentType, const adCharW* newFileName);

    DLLAPI adError adSelectionSet(adHandle handle, adSizePtr pStartFrom, adSize size, adBool value);
    DLLAPI adError adSelectionGet(adHandle handle, adSizePtr pStartFrom, adBoolPtr pSelection, adSizePtr pSelectionSize);
    DLLAPI adError adCurrentSet(adHandle handle, adSize index);
    DLLAPI adError adCurrentGet(adHandle handle, adSizePtr pIndex);

	DLLAPI adError adGroupGet(adHandle handle, adSizePtr pStartFrom, adGroupPtr pGroup, adSizePtr pGroupSize);
	DLLAPI adError adImageInfoGetA(adHandle handle, adSize groupId, adSizePtr pStartFrom, adImageInfoPtrA pImageInfo, adSizePtr pImageInfoSize);
	DLLAPI adError adImageInfoGetW(adHandle handle, adSize groupId, adSizePtr pStartFrom, adImageInfoPtrW pImageInfo, adSizePtr pImageInfoSize);
	DLLAPI adError adImageInfoSelectionSet(adHandle handle, adSize groupId, adSize index, adSelectionType selectionType);
	DLLAPI adError adImageInfoSelectionGet(adHandle handle, adSize groupId, adSizePtr pStartFrom, adBoolPtr pSelection, adSizePtr pSelectionSize);
	DLLAPI adError adImageInfoRenameA(adHandle handle, adSize groupId, adSize index, const adCharA* newFileName);
	DLLAPI adError adImageInfoRenameW(adHandle handle, adSize groupId, adSize index, const adCharW* newFileName);

    DLLAPI adError adLoadBitmapA(adHandle handle, const adCharA* fileName, adBitmapPtr pBitmap);
    DLLAPI adError adLoadBitmapW(adHandle handle, const adCharW* fileName, adBitmapPtr pBitmap);

    /*------------Unicode/Ansi defines-------------------------------------------*/

#ifdef UNICODE

    typedef adCharW adChar;
    typedef adPathW adPath;
    typedef adPathPtrW adPathPtr;
    typedef adStatusW adStatus;
    typedef adStatusPtrW adStatusPtr;
    typedef adImageInfoW adImageInfo; 
    typedef adImageInfoPtrW adImageInfoPtr;
    typedef adResultW adResult; 
    typedef adResultPtrW adResultPtr;

#define adLoad adLoadW
#define adSave adSaveW
#define adPathGet adPathGetW
#define adPathSet adPathSetW
#define adStatusGet adStatusGetW
#define adResultGet adResultGetW
#define adRenameCurrent adRenameCurrentW
#define adImageInfoGet adImageInfoGetW
#define adLoadBitmap adLoadBitmapW

#else/*UNICODE*/

    typedef adCharA adChar;
    typedef adPathA adPath;
    typedef adPathPtrA adPathPtr;
    typedef adStatusA adStatus;
    typedef adStatusPtrA adStatusPtr;
    typedef adImageInfoA adImageInfo; 
    typedef adImageInfoPtrA adImageInfoPtr;
    typedef adResultA adResult; 
    typedef adResultPtrA adResultPtr;

#define adLoad adLoadA
#define adSave adSaveA
#define adPathGet adPathGetA
#define adPathSet adPathSetA
#define adStatusGet adStatusGetA
#define adResultGet adResultGetA
#define adRenameCurrent adRenameCurrentA
#define adImageInfoGet adImageInfoGetA
#define adLoadBitmap adLoadBitmapA

#endif/*UNICODE*/

#ifdef __cplusplus
}
#endif

#endif /*__AntiDupl_h__*/