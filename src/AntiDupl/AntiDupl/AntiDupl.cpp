/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2015 Yermalayeu Ihar, 2014 Borisov Dmitry.
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
typedef ad::TEngine* adEngineHandle;

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
#include "adDump.h"
#include "adNeuralNetwork.h"

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
		version = SimdVersion();
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

DLLAPI adEngineHandle adCreate()
{
    ad::DumpInit();

	return new ad::TEngine();
}

DLLAPI adError adRelease(adEngineHandle handle)
{
    CHECK_HANDLE CHECK_ACCESS

    delete handle;

    return AD_OK;
}

DLLAPI adError adStop(adEngineHandle handle)
{
    CHECK_HANDLE

    if(handle->Status()->State() == AD_STATE_NONE)
        return AD_OK;

    handle->Status()->Stop();

    return AD_OK;
}

DLLAPI adError adSearch(adEngineHandle handle)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	handle->Search();

	return AD_OK;
}

template <class TChar> adError Load(adEngineHandle handle, adFileType fileType, const TChar *fileName, adBool check)
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
		if(check)
			return handle->ImageDataStorage()->ClearDatabase(pPath);
		else
			return handle->ImageDataStorage()->Load(pPath);
	default:
		return AD_ERROR_INVALID_FILE_TYPE;
	}
}

DLLAPI adError adLoadA(adEngineHandle handle, adFileType fileType, const adCharA* fileName, adBool check)
{
	return Load(handle, fileType, fileName, check);
}

DLLAPI adError adLoadW(adEngineHandle handle, adFileType fileType, const adCharW* fileName, adBool check)
{
	return Load(handle, fileType, fileName, check);
}

template <class TChar> adError Save(adEngineHandle handle, adFileType fileType, const TChar* fileName)
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

DLLAPI adError adSaveA(adEngineHandle handle, adFileType fileType, const adCharA* fileName)
{
	return Save(handle, fileType, fileName);
}

DLLAPI adError adSaveW(adEngineHandle handle, adFileType fileType, const adCharW* fileName)
{
	return Save(handle, fileType, fileName);
}

DLLAPI adError adClear(adEngineHandle handle, adFileType fileType)
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
        handle->ImageDataStorage()->ClearMemory();
        break;
    case AD_FILE_TEMPORARY:
        handle->RecycleBin()->ClearTemporaryFiles(false);
        break;
    default:
        return AD_ERROR_INVALID_FILE_TYPE;
    }

    return AD_OK;
}

DLLAPI adError adOptionsGet(adEngineHandle handle, adOptionsType optionsType, void* pOptions)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Options()->Export(optionsType, pOptions);
}

DLLAPI adError adOptionsSet(adEngineHandle handle, adOptionsType optionsType, void* pOptions)
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

template <class TPathPtr> adError PathGet(adEngineHandle handle, adPathType pathType, TPathPtr pPath, adSizePtr pPathSize)
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

DLLAPI adError adPathGetA(adEngineHandle handle, adPathType pathType, adPathPtrA pPath, adSizePtr pPathSize)
{
	return PathGet(handle, pathType, pPath, pPathSize);
}

DLLAPI adError adPathGetW(adEngineHandle handle, adPathType pathType, adPathPtrW pPath, adSizePtr pPathSize)
{
	return PathGet(handle, pathType, pPath, pPathSize);
}

template <class TPathPtr> adError PathSet(adEngineHandle handle, adPathType pathType, TPathPtr pPath, adSize pathSize)
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

DLLAPI adError adPathSetA(adEngineHandle handle, adPathType pathType, adPathPtrA pPath, adSize pathSize)
{
	return PathSet(handle, pathType, pPath, pathSize);
}

DLLAPI adError adPathSetW(adEngineHandle handle, adPathType pathType, adPathPtrW pPath, adSize pathSize)
{
	return PathSet(handle, pathType, pPath, pathSize);
}

// pathType - тип пути, pPaths - указатель на переменную пути, pathSize - количество путей
DLLAPI adError adPathWithSubFolderSetW(adEngineHandle handle, adPathType pathType, adPathWSFPtr pPaths, adSize pathSize)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	ad::TOptions *pOptions = handle->Options();
	switch(pathType)
	{
		case AD_PATH_SEARCH:
			return pOptions->searchPaths.Import(pPaths, pathSize);
		case AD_PATH_IGNORE:
			return pOptions->ignorePaths.Import(pPaths, pathSize);
		case AD_PATH_VALID:
			return pOptions->validPaths.Import(pPaths, pathSize);
		case AD_PATH_DELETE:
			return pOptions->deletePaths.Import(pPaths, pathSize);
		default:
			return AD_ERROR_INVALID_PATH_TYPE;
	}
}

DLLAPI adError adStatisticGet(adEngineHandle handle, adStatisticPtr pStatistic)
{
    CHECK_HANDLE

    return handle->Status()->Export(pStatistic);
}

DLLAPI adError adStatusGetA(adEngineHandle handle, adThreadType threadType, adSize threadId, adStatusPtrA pStatus)
{
    CHECK_HANDLE

    return handle->Status()->Export(threadType, threadId, pStatus);
}

DLLAPI adError adStatusGetW(adEngineHandle handle, adThreadType threadType, adSize threadId, adStatusPtrW pStatus)
{
    CHECK_HANDLE

    return handle->Status()->Export(threadType, threadId, pStatus);
}

DLLAPI adError adResultGetA(adEngineHandle handle, adSizePtr pStartFrom, adResultPtrA pResult, adSizePtr pResultSize)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Result()->Export(pStartFrom, pResult, pResultSize);
}

DLLAPI adError adResultGetW(adEngineHandle handle, adSizePtr pStartFrom, adResultPtrW pResult, adSizePtr pResultSize)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Result()->Export(pStartFrom, pResult, pResultSize);
}

DLLAPI adError adResultSort(adEngineHandle handle, adSortType sortType, adBool increasing)
{
    if(sortType < 0 || sortType >= AD_SORT_SIZE)
        return AD_ERROR_INVALID_SORT_TYPE;

    CHECK_HANDLE CHECK_ACCESS LOCK

    handle->Result()->Sort(sortType, increasing != FALSE);

    return AD_OK;
}

DLLAPI adError adResultApply(adEngineHandle handle, adGlobalActionType globalActionType)
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

DLLAPI adError adResultApplyTo(adEngineHandle handle, adLocalActionType localActionType, adTargetType targetType)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Result()->ApplyTo(localActionType, targetType);
}

DLLAPI adError adCanApply(adEngineHandle handle, adActionEnableType actionEnableType, adBoolPtr pEnable)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    if(actionEnableType < 0 || actionEnableType >= AD_ACTION_ENABLE_SIZE)
            return AD_ERROR_INVALID_ACTION_ENABLE_TYPE; 

    if(pEnable == NULL)
        return AD_ERROR_INVALID_POINTER;

    *pEnable = handle->Result()->CanApply(actionEnableType) ? TRUE : FALSE;

    return AD_OK;
}

DLLAPI adError adRenameCurrentA(adEngineHandle handle, adRenameCurrentType renameCurrentType, const adCharA* newFileName)
{
    CHECK_HANDLE CHECK_ACCESS LOCK CHECK_POINTER(newFileName) 

    return handle->Result()->RenameCurrent(renameCurrentType, newFileName);
}

DLLAPI adError adRenameCurrentW(adEngineHandle handle, adRenameCurrentType renameCurrentType, const adCharW* newFileName)
{
    CHECK_HANDLE CHECK_ACCESS LOCK CHECK_POINTER(newFileName) 

    return handle->Result()->RenameCurrent(renameCurrentType, newFileName);
}

DLLAPI adError adMoveCurrentGroupW(adEngineHandle handle, const adCharW* directory)
{
    CHECK_HANDLE CHECK_ACCESS LOCK CHECK_POINTER(directory) 

    return handle->Result()->MoveCurrentGroup(directory);
}

DLLAPI adError adRenameCurrentGroupAsW(adEngineHandle handle, const adCharW* fileName)
{
    CHECK_HANDLE CHECK_ACCESS LOCK CHECK_POINTER(fileName) 

    return handle->Result()->RenameCurrentGroupAs(fileName);
}

DLLAPI adError adSelectionSet(adEngineHandle handle, adSizePtr pStartFrom, adSize size, adBool value)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Result()->SetSelection(pStartFrom, size, value);
}

DLLAPI adError adSelectionGet(adEngineHandle handle, adSizePtr pStartFrom, adBoolPtr pSelection, adSizePtr pSelectionSize)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Result()->GetSelection(pStartFrom, pSelection, pSelectionSize);
}

DLLAPI adError adCurrentSet(adEngineHandle handle, adSize index)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Result()->SetCurrent(index);
}

DLLAPI adError adCurrentGet(adEngineHandle handle, adSizePtr pIndex)
{
    CHECK_HANDLE CHECK_ACCESS LOCK

    return handle->Result()->GetCurrent(pIndex);
}

DLLAPI adError adGroupGet(adEngineHandle handle, adSizePtr pStartFrom, adGroupPtr pGroup, adSizePtr pGroupSize)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	return handle->Result()->Export(pStartFrom, pGroup, pGroupSize);
}

DLLAPI adError adImageInfoGetA(adEngineHandle handle, adSize groupId, adSizePtr pStartFrom, adImageInfoPtrA pImageInfo, adSizePtr pImageInfoSize)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	return handle->Result()->Export(groupId, pStartFrom, pImageInfo, pImageInfoSize);
}

DLLAPI adError adImageInfoGetW(adEngineHandle handle, adSize groupId, adSizePtr pStartFrom, adImageInfoPtrW pImageInfo, adSizePtr pImageInfoSize)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	return handle->Result()->Export(groupId, pStartFrom, pImageInfo, pImageInfoSize);
}

DLLAPI adError adImageInfoSelectionSet(adEngineHandle handle, adSize groupId, adSize index, adSelectionType selectionType)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	return handle->Result()->SetSelection(groupId, index, selectionType);
}

DLLAPI adError adImageInfoSelectionGet(adEngineHandle handle, adSize groupId, adSizePtr pStartFrom, adBoolPtr pSelection, adSizePtr pSelectionSize)
{
	CHECK_HANDLE CHECK_ACCESS LOCK

	return handle->Result()->GetSelection(groupId, pStartFrom, pSelection, pSelectionSize);
}

DLLAPI adError adImageInfoRenameA(adEngineHandle handle, adSize groupId, adSize index, const adCharA* newFileName)
{
    CHECK_HANDLE CHECK_ACCESS LOCK CHECK_POINTER(newFileName) 

    return handle->Result()->Rename(groupId, index, newFileName);
}

DLLAPI adError adImageInfoRenameW(adEngineHandle handle, adSize groupId, adSize index, const adCharW* newFileName)
{
    CHECK_HANDLE CHECK_ACCESS LOCK CHECK_POINTER(newFileName) 

    return handle->Result()->Rename(groupId, index, newFileName);
}

DLLAPI adError adLoadBitmapA(adEngineHandle handle, const adCharA* fileName, adBitmapPtr pBitmap)
{
    CHECK_HANDLE CHECK_POINTER(fileName) 

    return ad::LoadBitmap(fileName, pBitmap);
}

DLLAPI adError adLoadBitmapW(adEngineHandle handle, const adCharW* fileName, adBitmapPtr pBitmap)
{
    CHECK_HANDLE CHECK_POINTER(fileName)

    return ad::LoadBitmap(fileName, pBitmap);
}

DLLAPI adError adTrainNeuralNetwork(adEngineHandle handle)
{
	CHECK_HANDLE

	return handle->GetNeuralNetworkPonter()->Train();
    //return ad::TraineuralNetwork();
}
