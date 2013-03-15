/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2013 Yermalayeu Ihar.
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

#define OPJ_STATIC
#include "LibOpenJpeg/openjpeg.h"

#include <windows.h>

namespace ad
{
	class TEngine;
}
typedef ad::TEngine* adHandle;

#define __AD_HANDLE__

#include "AntiDupl.h"

#include "adConfig.h"
#include "adThreads.h"
#include "adOptions.h"
#include "adStatus.h"
#include "adResult.h"
#include "adResultStorage.h"
#include "adMistakeStorage.h"
#include "adImageDataStorage.h"
#include "adEngine.h"
#include "adImageUtils.h"
#include "adRecycleBin.h"
#include "adExternal.h"

#define CHECK_HANDLE \
    if(handle == NULL) \
    return AD_ERROR_INVALID_POINTER;

#define CHECK_ACCESS \
    if(handle->Status()->State() != AD_STATE_NONE) \
    return AD_ERROR_ACCESS_DENIED;

#define LOCK \
    ad::TCriticalSection::TLocker locker(handle->CriticalSection());

#define CHECK_POINTER(p) \
    if(p == NULL) \
    return AD_ERROR_INVALID_POINTER;

//using namespace ad;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReasonForCall, LPVOID lpReserved)
{
    switch(dwReasonForCall)
    {
    case DLL_PROCESS_DETACH:
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        return TRUE;
    }
    return TRUE;
}

DLLAPI adError adVersionGet(adVersionType versionType, adCharA * pVersion, adSizePtr pVersionSize)
{
	CHECK_POINTER(pVersion);
	CHECK_POINTER(pVersionSize);

	std::string version;
	switch(versionType)
	{
	case AD_VERSION_TYPE_ANTIDUPL:
		version = AD_VERSION;
		break;
	case AD_VERSION_TYPE_SIMD:
		version = SIMD_VERSION;
		break;
	case AD_VERSION_TYPE_OPENJPEG:
		version = opj_version();
		break;
	default:
		return AD_ERROR_INVALID_VERSION_TYPE;
	}

	if(version.size() + 1 > *pVersionSize)
	{
		*pVersionSize = version.size() + 1;
		return AD_ERROR_OUTPUT_BUFFER_IS_TOO_SMALL;
	}

	memcpy(pVersion, version.c_str(), version.size() + 1);

	return AD_OK;
}

DLLAPI adHandle adCreate()
{
	return new ad::TEngine();
}

DLLAPI adError adRelease(adHandle handle)
{
    CHECK_HANDLE CHECK_ACCESS

    delete handle;

    return AD_OK;
}

DLLAPI adError adStop(adHandle handle)
{
    CHECK_HANDLE

    if(handle->Status()->State() == AD_STATE_NONE)
        return AD_OK;

    handle->Status()->Stop();

    return AD_OK;
}

DLLAPI adError adSearch(adHandle handle)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	handle->Search();

	return AD_OK;
}

template <class TChar> adError Load(adHandle handle, adFileType fileType, const TChar *fileName, adBool check)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	ad::TString path(fileName != NULL ? fileName : (const TChar*)L"");
	ad::TChar *pPath = path.size() > 0 ? (ad::TChar*)path.c_str() : NULL;

	switch(fileType)
	{
	case AD_FILE_OPTIONS:
		return handle->Options()->Load(pPath);
	case AD_FILE_RESULT:
		return handle->Result()->Load(pPath, check != FALSE);
	case AD_FILE_MISTAKE_DATA_BASE:
		return handle->MistakeStorage()->Load(pPath, check != FALSE);
	case AD_FILE_IMAGE_DATA_BASE:
		return handle->ImageDataStorage()->Load(pPath, check != FALSE);
	default:
		return AD_ERROR_INVALID_FILE_TYPE;
	}
}

DLLAPI adError adLoadA(adHandle handle, adFileType fileType, const adCharA* fileName, adBool check)
{
	return Load(handle, fileType, fileName, check);
}

DLLAPI adError adLoadW(adHandle handle, adFileType fileType, const adCharW* fileName, adBool check)
{
	return Load(handle, fileType, fileName, check);
}

template <class TChar> adError Save(adHandle handle, adFileType fileType, const TChar* fileName)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	ad::TString path(fileName != NULL ? fileName : (const TChar*)L"");
	ad::TChar *pPath = path.size() > 0 ? (ad::TChar*)path.c_str() : NULL;

	switch(fileType)
	{
	case AD_FILE_OPTIONS:
		return handle->Options()->Save(pPath);
	case AD_FILE_RESULT:
		return handle->Result()->Save(pPath);
	case AD_FILE_MISTAKE_DATA_BASE:
		return handle->MistakeStorage()->Save(pPath);
	case AD_FILE_IMAGE_DATA_BASE:
		return handle->ImageDataStorage()->Save(pPath);
	default:
		return AD_ERROR_INVALID_FILE_TYPE;
	}
}

DLLAPI adError adSaveA(adHandle handle, adFileType fileType, const adCharA* fileName)
{
	return Save(handle, fileType, fileName);
}

DLLAPI adError adSaveW(adHandle handle, adFileType fileType, const adCharW* fileName)
{
	return Save(handle, fileType, fileName);
}

DLLAPI adError adClear(adHandle handle, adFileType fileType)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    switch(fileType)
    {
    case AD_FILE_OPTIONS:
        handle->Options()->SetDefault();
        break;
    case AD_FILE_RESULT:
        handle->Result()->Clear();
        break;
    case AD_FILE_MISTAKE_DATA_BASE:
        handle->MistakeStorage()->Clear();
        break;
    case AD_FILE_IMAGE_DATA_BASE:
        handle->ImageDataStorage()->Clear();
        break;
    case AD_FILE_TEMPORARY:
        handle->RecycleBin()->ClearTemporaryFiles(false);
        break;
    default:
        return AD_ERROR_INVALID_FILE_TYPE;
    }

    return AD_OK;
}

DLLAPI adError adOptionsGet(adHandle handle, adOptionsType optionsType, void* pOptions)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Options()->Export(optionsType, pOptions);
}

DLLAPI adError adOptionsSet(adHandle handle, adOptionsType optionsType, void* pOptions)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    if(optionsType == AD_OPTIONS_SET_DEFAULT)
    {
        handle->Options()->SetDefault();
        return AD_OK;
    }
    else 
        return handle->Options()->Import(optionsType, pOptions);
}

template <class TPathPtr> adError PathGet(adHandle handle, adPathType pathType, TPathPtr pPath, adSizePtr pPathSize)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	ad::TOptions *pOptions = handle->Options();
	switch(pathType)
	{
		case AD_PATH_SEARCH:
			return pOptions->searchPaths.Export(pPath, pPathSize);
		case AD_PATH_IGNORE:
			return pOptions->ignorePaths.Export(pPath, pPathSize);
		case AD_PATH_VALID:
			return pOptions->validPaths.Export(pPath, pPathSize);
		case AD_PATH_DELETE:
			return pOptions->deletePaths.Export(pPath, pPathSize);
		default:
			return AD_ERROR_INVALID_PATH_TYPE;
	}
}

DLLAPI adError adPathGetA(adHandle handle, adPathType pathType, adPathPtrA pPath, adSizePtr pPathSize)
{
	return PathGet(handle, pathType, pPath, pPathSize);
}

DLLAPI adError adPathGetW(adHandle handle, adPathType pathType, adPathPtrW pPath, adSizePtr pPathSize)
{
	return PathGet(handle, pathType, pPath, pPathSize);
}

template <class TPathPtr> adError PathSet(adHandle handle, adPathType pathType, TPathPtr pPath, adSize pathSize)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	ad::TOptions *pOptions = handle->Options();
	switch(pathType)
	{
		case AD_PATH_SEARCH:
			return pOptions->searchPaths.Import(pPath, pathSize);
		case AD_PATH_IGNORE:
			return pOptions->ignorePaths.Import(pPath, pathSize);
		case AD_PATH_VALID:
			return pOptions->validPaths.Import(pPath, pathSize);
		case AD_PATH_DELETE:
			return pOptions->deletePaths.Import(pPath, pathSize);
		default:
			return AD_ERROR_INVALID_PATH_TYPE;
	}
}

DLLAPI adError adPathSetA(adHandle handle, adPathType pathType, adPathPtrA pPath, adSize pathSize)
{
	return PathSet(handle, pathType, pPath, pathSize);
}

DLLAPI adError adPathSetW(adHandle handle, adPathType pathType, adPathPtrW pPath, adSize pathSize)
{
	return PathSet(handle, pathType, pPath, pathSize);
}

DLLAPI adError adStatisticGet(adHandle handle, adStatisticPtr pStatistic)
{
    CHECK_HANDLE

    return handle->Status()->Export(pStatistic);
}

DLLAPI adError adStatusGetA(adHandle handle, adThreadType threadType, adSize threadId, adStatusPtrA pStatus)
{
    CHECK_HANDLE

    return handle->Status()->Export(threadType, threadId, pStatus);
}

DLLAPI adError adStatusGetW(adHandle handle, adThreadType threadType, adSize threadId, adStatusPtrW pStatus)
{
    CHECK_HANDLE

    return handle->Status()->Export(threadType, threadId, pStatus);
}

DLLAPI adError adResultGetA(adHandle handle, adSizePtr pStartFrom, adResultPtrA pResult, adSizePtr pResultSize)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Result()->Export(pStartFrom, pResult, pResultSize);
}

DLLAPI adError adResultGetW(adHandle handle, adSizePtr pStartFrom, adResultPtrW pResult, adSizePtr pResultSize)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Result()->Export(pStartFrom, pResult, pResultSize);
}

DLLAPI adError adResultSort(adHandle handle, adSortType sortType, adBool increasing)
{
    if(sortType < 0 || sortType >= AD_SORT_SIZE)
        return AD_ERROR_INVALID_SORT_TYPE;

    CHECK_HANDLE CHECK_ACCESS LOCK

    handle->Result()->Sort(sortType, increasing != FALSE);

    return AD_OK;
}

DLLAPI adError adResultApply(adHandle handle, adGlobalActionType globalActionType)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

	ad::TResultStorage *pResult = handle->Result();
    switch(globalActionType)
    {
    case AD_GLOBAL_ACTION_SET_HINT:
        pResult->SetHint();
        break;
    case AD_GLOBAL_ACTION_SET_GROUP:
        pResult->SetGroup();
        break;
    case AD_GLOBAL_ACTION_REFRESH:
        pResult->Refresh();
        break;
    case AD_GLOBAL_ACTION_UNDO:
        return pResult->Undo() ? AD_OK : AD_ERROR_UNKNOWN;
    case AD_GLOBAL_ACTION_REDO:
        return pResult->Redo() ? AD_OK : AD_ERROR_UNKNOWN;
    default:
        return AD_ERROR_INVALID_GLOBAL_ACTION_TYPE; 
    }

    return AD_OK;
}

DLLAPI adError adResultApplyTo(adHandle handle, adLocalActionType localActionType, adTargetType targetType)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Result()->ApplyTo(localActionType, targetType);
}

DLLAPI adError adCanApply(adHandle handle, adActionEnableType actionEnableType, adBoolPtr pEnable)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    if(actionEnableType < 0 || actionEnableType >= AD_ACTION_ENABLE_SIZE)
            return AD_ERROR_INVALID_ACTION_ENABLE_TYPE; 

    if(pEnable == NULL)
        return AD_ERROR_INVALID_POINTER;

    *pEnable = handle->Result()->CanApply(actionEnableType) ? TRUE : FALSE;

    return AD_OK;
}

DLLAPI adError adRenameCurrentA(adHandle handle, adRenameCurrentType renameCurrentType, const adCharA* newFileName)
{
    CHECK_HANDLE CHECK_ACCESS LOCK CHECK_POINTER(newFileName) 

    return handle->Result()->RenameCurrent(renameCurrentType, newFileName);
}

DLLAPI adError adRenameCurrentW(adHandle handle, adRenameCurrentType renameCurrentType, const adCharW* newFileName)
{
    CHECK_HANDLE CHECK_ACCESS LOCK CHECK_POINTER(newFileName) 

    return handle->Result()->RenameCurrent(renameCurrentType, newFileName);
}

DLLAPI adError adSelectionSet(adHandle handle, adSizePtr pStartFrom, adSize size, adBool value)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Result()->SetSelection(pStartFrom, size, value);
}

DLLAPI adError adSelectionGet(adHandle handle, adSizePtr pStartFrom, adBoolPtr pSelection, adSizePtr pSelectionSize)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Result()->GetSelection(pStartFrom, pSelection, pSelectionSize);
}

DLLAPI adError adCurrentSet(adHandle handle, adSize index)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Result()->SetCurrent(index);
}

DLLAPI adError adCurrentGet(adHandle handle, adSizePtr pIndex)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Result()->GetCurrent(pIndex);
}

DLLAPI adError adGroupGet(adHandle handle, adSizePtr pStartFrom, adGroupPtr pGroup, adSizePtr pGroupSize)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	return handle->Result()->Export(pStartFrom, pGroup, pGroupSize);
}

DLLAPI adError adImageInfoGetA(adHandle handle, adSize groupId, adSizePtr pStartFrom, adImageInfoPtrA pImageInfo, adSizePtr pImageInfoSize)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	return handle->Result()->Export(groupId, pStartFrom, pImageInfo, pImageInfoSize);
}

DLLAPI adError adImageInfoGetW(adHandle handle, adSize groupId, adSizePtr pStartFrom, adImageInfoPtrW pImageInfo, adSizePtr pImageInfoSize)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	return handle->Result()->Export(groupId, pStartFrom, pImageInfo, pImageInfoSize);
}

DLLAPI adError adImageInfoSelectionSet(adHandle handle, adSize groupId, adSize index, adSelectionType selectionType)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	return handle->Result()->SetSelection(groupId, index, selectionType);
}

DLLAPI adError adImageInfoSelectionGet(adHandle handle, adSize groupId, adSizePtr pStartFrom, adBoolPtr pSelection, adSizePtr pSelectionSize)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	return handle->Result()->GetSelection(groupId, pStartFrom, pSelection, pSelectionSize);
}

DLLAPI adError adImageInfoRenameA(adHandle handle, adSize groupId, adSize index, const adCharA* newFileName)
{
    CHECK_HANDLE CHECK_ACCESS LOCK CHECK_POINTER(newFileName) 

    return handle->Result()->Rename(groupId, index, newFileName);
}

DLLAPI adError adImageInfoRenameW(adHandle handle, adSize groupId, adSize index, const adCharW* newFileName)
{
    CHECK_HANDLE CHECK_ACCESS LOCK CHECK_POINTER(newFileName) 

    return handle->Result()->Rename(groupId, index, newFileName);
}

DLLAPI adError adLoadBitmapA(adHandle handle, const adCharA* fileName, adBitmapPtr pBitmap)
{
    CHECK_HANDLE CHECK_POINTER(fileName) 

    return ad::LoadBitmap(fileName, pBitmap);
}

DLLAPI adError adLoadBitmapW(adHandle handle, const adCharW* fileName, adBitmapPtr pBitmap)
{
    CHECK_HANDLE CHECK_POINTER(fileName)

    return ad::LoadBitmap(fileName, pBitmap);
}

