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


#include "AntiDupl.h"

#include "adConfig.h"
#include "adThreads.h"
#include "adOptions.h"
#include "adStatus.h"
#include "adResult.h"
#include "adImageDataStorage.h"
#include "adEngine.h"
#include "adImageUtils.h"
#include "adDump.h"

#define CHECK_ACCESS \
    if(_engine->Status()->State() != AD_STATE_NONE) \
    return AD_ERROR_ACCESS_DENIED;

#define LOCK \
    ad::TCriticalSection::TLocker locker(_engine->CriticalSection());

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

ad::TEngine * _engine;

DLLAPI adError adCreate()
{
    ad::DumpInit();

	_engine = new ad::TEngine();

	return AD_OK;
}

DLLAPI adError adRelease()
{
    CHECK_ACCESS

    delete _engine;

    return AD_OK;
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


DLLAPI adError adStop()
{
    if(_engine->Status()->State() == AD_STATE_NONE)
        return AD_OK;

    _engine->Status()->Stop();

    return AD_OK;
}

DLLAPI adError adSearch()
{
	CHECK_ACCESS LOCK

	if (_engine->Options()->advanced.useImageDataBase)
	{
		const ad::TString & path = _engine->Options()->GetImageDataBasePath();
		ad::TChar *pPath = path.size() > 0 ? (ad::TChar*)path.c_str() : NULL;
		_engine->ImageDataStorage()->Load(pPath);
	}

	_engine->Search();

	if (_engine->Options()->advanced.useImageDataBase)
	{
		const ad::TString & path = _engine->Options()->GetImageDataBasePath();
		ad::TChar *pPath = path.size() > 0 ? (ad::TChar*)path.c_str() : NULL;
		_engine->ImageDataStorage()->Save(pPath);
		_engine->ImageDataStorage()->ClearMemory();
	}

	_engine->Result()->Sort(adSortType::AD_SORT_BY_DIFFERENCE, true);

	return AD_OK;
}

DLLAPI adError adCalculateMultiImageMetric(adImageInfoW* pPointArray, int size,  WorkProgressInteropNegotiator & negotiator)
{
	CHECK_ACCESS LOCK

	if (_engine->Options()->advanced.useImageDataBase)
	{
		const ad::TString & path = _engine->Options()->GetImageDataBasePath();
		ad::TChar *pPath = path.size() > 0 ? (ad::TChar*)path.c_str() : NULL;
		_engine->ImageDataStorage()->Load(pPath);
	}
	
	_engine->CalculateHistogramPeaks(pPointArray, size, negotiator);

	if (_engine->Options()->advanced.useImageDataBase)
	{
		const ad::TString & path = _engine->Options()->GetImageDataBasePath();
		ad::TChar *pPath = path.size() > 0 ? (ad::TChar*)path.c_str() : NULL;
		_engine->ImageDataStorage()->Save(pPath);
		_engine->ImageDataStorage()->ClearMemory();
	}

	return AD_OK;
}

template <class TChar> adError Load(adFileType fileType, const TChar *fileName, adBool check)
{
	CHECK_ACCESS LOCK

	ad::TString path(fileName != NULL ? fileName : (const TChar*)L"");
	ad::TChar *pPath = path.size() > 0 ? (ad::TChar*)path.c_str() : NULL;

	switch(fileType)
	{
	//case AD_FILE_OPTIONS:
	//	return _engine.Options()->Load(pPath);
	case AD_FILE_RESULT:
		return _engine->Result()->Load(pPath, check != FALSE);
	//case AD_FILE_MISTAKE_DATA_BASE:
	//	return _engine->MistakeStorage()->Load(pPath, check != FALSE);
	case AD_FILE_IMAGE_DATA_BASE:
		if(check)
			return _engine->ImageDataStorage()->ClearDatabase(pPath);
		else
			return _engine->ImageDataStorage()->Load(pPath);
	default:
		return AD_ERROR_INVALID_FILE_TYPE;
	}
}

DLLAPI adError adLoadA(adFileType fileType, const adCharA* fileName, adBool check)
{
	return Load(fileType, fileName, check);
}

DLLAPI adError adLoadW(adFileType fileType, const adCharW* fileName, adBool check)
{
	return Load(fileType, fileName, check);
}

template <class TChar> adError Save(adFileType fileType, const TChar* fileName)
{
	CHECK_ACCESS LOCK

	ad::TString path(fileName != NULL ? fileName : (const TChar*)L"");
	ad::TChar *pPath = path.size() > 0 ? (ad::TChar*)path.c_str() : NULL;

	switch(fileType)
	{
	//case AD_FILE_OPTIONS:
	//	return _engine.Options()->Save(pPath);
	case AD_FILE_RESULT:
		return _engine->Result()->Save(pPath);
	//case AD_FILE_MISTAKE_DATA_BASE:
	//	return _engine->MistakeStorage()->Save(pPath);
	case AD_FILE_IMAGE_DATA_BASE:
		return _engine->ImageDataStorage()->Save(pPath);
	default:
		return AD_ERROR_INVALID_FILE_TYPE;
	}
}

DLLAPI adError adSaveA(adFileType fileType, const adCharA* fileName)
{
	return Save(fileType, fileName);
}

DLLAPI adError adSaveW(adFileType fileType, const adCharW* fileName)
{
	return Save(fileType, fileName);
}

DLLAPI adError adClear(adFileType fileType)
{
    CHECK_ACCESS LOCK

    switch(fileType)
    {
    case AD_FILE_OPTIONS:
        _engine->Options()->SetDefault();
        break;
    case AD_FILE_RESULT:
        _engine->Result()->Clear();
        break;
    //case AD_FILE_MISTAKE_DATA_BASE:
    //    _engine->MistakeStorage()->Clear();
    //    break;
    case AD_FILE_IMAGE_DATA_BASE:
        _engine->ImageDataStorage()->ClearMemory();
        break;
    /*case AD_FILE_TEMPORARY:
        _engine.RecycleBin()->ClearTemporaryFiles(false);
        break;*/
    default:
        return AD_ERROR_INVALID_FILE_TYPE;
    }

    return AD_OK;
}

DLLAPI adError adOptionsGet(adOptionsType optionsType, void* pOptions)
{
    CHECK_ACCESS LOCK

    return _engine->Options()->Export(optionsType, pOptions);
}

DLLAPI adError adOptionsSet(adOptionsType optionsType, void* pOptions)
{
    CHECK_ACCESS LOCK

    if(optionsType == AD_OPTIONS_SET_DEFAULT)
    {
        _engine->Options()->SetDefault();
        return AD_OK;
    }
    else 
        return _engine->Options()->Import(optionsType, pOptions);
}

DLLAPI adError adSetSearchOptions(void * pOptions)
{
    CHECK_ACCESS LOCK

    return _engine->Options()->Import(AD_OPTIONS_SEARCH, pOptions);
}

DLLAPI adError adSetAdvancedOptions(void * pOptions)
{
    CHECK_ACCESS LOCK

    return _engine->Options()->Import(AD_OPTIONS_ADVANCED, pOptions);
}

DLLAPI adError adSetCompareOption(void * pOptions)
{
    CHECK_ACCESS LOCK

    return _engine->Options()->Import(AD_OPTIONS_COMPARE, pOptions);
}

DLLAPI adError adSetDefectOption(void * pOptions)
{
    CHECK_ACCESS LOCK

    return _engine->Options()->Import(AD_OPTIONS_DEFECT, pOptions);
}


/*
template <class TPathPtr> adError PathGet(adPathType pathType, TPathPtr pPath, adSizePtr pPathSize)
{
	CHECK_ACCESS LOCK

	ad::TOptions *pOptions = _engine.Options();
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

DLLAPI adError adPathGetA(adPathType pathType, adPathPtrA pPath, adSizePtr pPathSize)
{
	return PathGet(pathType, pPath, pPathSize);
}

DLLAPI adError adPathGetW(adPathType pathType, adPathPtrW pPath, adSizePtr pPathSize)
{
	return PathGet(pathType, pPath, pPathSize);
}

template <class TPathPtr> adError PathSet(adPathType pathType, TPathPtr pPath, adSize pathSize)
{
	CHECK_ACCESS LOCK

	ad::TOptions *pOptions = _engine.Options();
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

DLLAPI adError adPathSetA(adPathType pathType, adPathPtrA pPath, adSize pathSize)
{
	return PathSet(pathType, pPath, pathSize);
}

DLLAPI adError adPathSetW(adPathType pathType, adPathPtrW pPath, adSize pathSize)
{
	return PathSet(pathType, pPath, pathSize);
}

// pathType - тип пути, pPaths - указатель на переменную пути, pathSize - количество путей
DLLAPI adError adPathWithSubFolderSetW(adPathType pathType, adPathWSFPtr pPaths, adSize pathSize)
{
	CHECK_ACCESS LOCK

	ad::TOptions *pOptions = _engine.Options();
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
}*/

DLLAPI adError adClearLocations(adPathType pathType)
{
	switch(pathType)
	{
		case AD_PATH_SEARCH:
			return _engine->Options()->searchPaths.Clear();
		case AD_PATH_IGNORE:
			return _engine->Options()->ignorePaths.Clear();
		case AD_PATH_VALID:
			return _engine->Options()->validPaths.Clear();
		case AD_PATH_DELETE:
			return _engine->Options()->deletePaths.Clear();
		default:
			return AD_ERROR_INVALID_PATH_TYPE;
	}
}

DLLAPI adError adAddLocations(adPathType pathType, const Location * location)
{
	switch(pathType)
	{
		case AD_PATH_SEARCH:
			return _engine->Options()->searchPaths.AddPath(location);
		case AD_PATH_IGNORE:
			return _engine->Options()->ignorePaths.AddPath(location);
		case AD_PATH_VALID:
			return _engine->Options()->validPaths.AddPath(location);
		case AD_PATH_DELETE:
			return _engine->Options()->deletePaths.AddPath(location);
		default:
			return AD_ERROR_INVALID_PATH_TYPE;
	}
}


DLLAPI adError adStatisticGet(adStatisticPtr pStatistic)
{
    return _engine->Status()->Export(pStatistic);
}

DLLAPI adError adStatusGetA(adThreadType threadType, adSize threadId, adStatusPtrA pStatus)
{
    return _engine->Status()->Export(threadType, threadId, pStatus);
}

DLLAPI adError adStatusGetW(adThreadType threadType, adSize threadId, adStatusPtrW pStatus)
{
    return _engine->Status()->Export(threadType, threadId, pStatus);
}

DLLAPI adError adResultGetA(adSizePtr pStartFrom, adResultPtrA pResult, adSizePtr pResultSize)
{
    CHECK_ACCESS LOCK

    return _engine->Result()->Export(pStartFrom, pResult, pResultSize);
}

DLLAPI adError adResultGetW(adSizePtr pStartFrom, adResultPtrW pResult, adSizePtr pResultSize)
{
    CHECK_ACCESS LOCK

    return _engine->Result()->Export(pStartFrom, pResult, pResultSize);
}

DLLAPI adError adResultSort(adSortType sortType, adBool increasing)
{
    if(sortType < 0 || sortType >= AD_SORT_SIZE)
        return AD_ERROR_INVALID_SORT_TYPE;

    CHECK_ACCESS LOCK

		_engine->Result()->Sort(sortType, increasing != FALSE);

    return AD_OK;
}

DLLAPI adError adCanApply(adActionEnableType actionEnableType, adBoolPtr pEnable)
{
    CHECK_ACCESS LOCK

    if(actionEnableType < 0 || actionEnableType >= AD_ACTION_ENABLE_SIZE)
            return AD_ERROR_INVALID_ACTION_ENABLE_TYPE; 

    if(pEnable == NULL)
        return AD_ERROR_INVALID_POINTER;

    *pEnable = _engine->Result()->CanApply(actionEnableType) ? TRUE : FALSE;

    return AD_OK;
}

DLLAPI unsigned int * adGetResultSize()
{
	return _engine->Result()->GetResultSize();
}	

DLLAPI void adGetResult(adResultPtrW pResult, const unsigned int index) 
{
	return _engine->Result()->GetResult(pResult, index);
}	

DLLAPI adError adLoadBitmapW(const adCharW* fileName, adBitmapPtr pBitmap)
{
    return ad::LoadBitmap(fileName, pBitmap);
}