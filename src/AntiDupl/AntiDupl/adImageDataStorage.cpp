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
#include "adFileUtils.h"
#include "adEngine.h"
#include "adStatus.h"
#include "adOptions.h"
#include "adImageDataStorage.h"
#include "adIO.h"

namespace ad
{
	const TUInt32 FILE_VERSION = 1;

	const TChar FILE_EXTENSION[] = TEXT(".adi");

    const TChar INDEX_FILE_NAME[] = TEXT("index");
	const TChar BACKUP_FILE_NAME[] = TEXT("backup");

	const char INDEX_CONTROL_BYTES[] = "adii";
	const char DATA_CONTROL_BYTES[] = "adid";

	const size_t DATA_SIZE_MAX = 0x10000;

    //-------------------------------------------------------------------------
	TImageDataStorage::TImageDataStorage(TEngine *pEngine)
		:m_pStatus(pEngine->Status()),
		m_pOptions(pEngine->Options())
	{
	}

	TImageDataStorage::TStorage::iterator TImageDataStorage::Find(const TFileInfo& fileInfo)
	{
		TStorage::iterator it = m_storage.lower_bound(fileInfo.hash);
		TStorage::iterator itEnd = m_storage.upper_bound(fileInfo.hash);
		for(;it != itEnd; ++it)
		{
			if(TPath::EqualByPath(it->second->path, fileInfo.path))
				return it;
		}
		return m_storage.end();
	}

	TImageDataStorage::TStorage::iterator TImageDataStorage::Insert(TImageData* pImageData)
	{
		return m_storage.insert(TStorage::value_type(pImageData->hash, pImageData));
	}

	void TImageDataStorage::Clear()
	{
		for(TStorage::iterator it = m_storage.begin(); it != m_storage.end(); ++it)
			delete it->second;
		m_storage.clear();
	}

	void TImageDataStorage::Check()
	{
		m_pStatus->Reset();
		size_t size = m_storage.size(), i = 0;
		for(TStorage::iterator it = m_storage.begin(); it != m_storage.end(); ++it)
		{
			if(m_pStatus->Stopped())
				break;

			if(!it->second->Actual())
			{
				delete it->second;
				it = m_storage.erase(it);
			}

			m_pStatus->SetProgress(i++, size);
		}
		m_pStatus->Reset();
	}

	TImageDataPtr TImageDataStorage::Get(const TFileInfo& fileInfo)
	{
		TStorage::iterator it = Find(fileInfo);
		if(it != m_storage.end())
		{
			if(it->second->size != fileInfo.size || it->second->time != fileInfo.time)
			{
				delete it->second;
				it->second = new TImageData(fileInfo);
			}
		}
		else
		{
			it = Insert(new TImageData(fileInfo));
		}
		it->second->SetData(m_pOptions->advanced.reducedImageSize);
		return it->second;
	}

	adError TImageDataStorage::Load(const TChar *path)
	{
		if(!IsDirectoryExists(path))
			return AD_ERROR_DIRECTORY_IS_NOT_EXIST;

		TIndex index;
		if( LoadIndex(index, CreatePath(path, TString(INDEX_FILE_NAME) + FILE_EXTENSION).c_str()) || 
			LoadIndex(index, CreatePath(path, TString(BACKUP_FILE_NAME) + FILE_EXTENSION).c_str()))
		{
			size_t size = 0;
			for(TIndex::iterator it = index.begin(); it != index.end(); ++it)
				if(it->second.type == TData::Old)
					size += it->second.size;

			m_pStatus->Reset();
			size_t i = 0;
			for(TIndex::iterator it = index.begin(); it != index.end(); ++it)
			{
				if(it->second.type == TData::Old)
				{
					LoadData(it->second, path, it->second.key);
					m_pStatus->SetProgress(i, size);
					i += it->second.size;
				}
			}
			m_pStatus->Reset();
			return AD_OK;
		}
		return AD_ERROR_UNKNOWN;
	}

	adError TImageDataStorage::Save(const TChar *path) const
	{
		if(!IsDirectoryExists(path))
			return AD_ERROR_DIRECTORY_IS_NOT_EXIST;

		TIndex index;
		if(!LoadIndex(index, CreatePath(path, TString(INDEX_FILE_NAME) + FILE_EXTENSION).c_str()))
			LoadIndex(index, CreatePath(path, TString(BACKUP_FILE_NAME) + FILE_EXTENSION).c_str());
		UpdateIndex(index);
		if(SaveIndex(index, path))
		{
			for(TIndex::const_iterator it = index.begin(); it != index.end(); ++it)
			{
				if(it->second.type == TData::Old)
				{
					TString fileName = CreatePath(path, GetDataFileName(it->second.key));
					if(IsFileExists(fileName.c_str()))
					{
						::DeleteFile(fileName.c_str());
					}
				}
			}
			::CopyFile(
				CreatePath(path, TString(INDEX_FILE_NAME) + FILE_EXTENSION).c_str(),
				CreatePath(path, TString(BACKUP_FILE_NAME) + FILE_EXTENSION).c_str(),
				FALSE);
			return AD_OK;
		}
		return AD_ERROR_UNKNOWN;
	}

	void TImageDataStorage::CreateSorted(TVector & sorted) const
	{
		sorted.clear();

		size_t size = 0;
		for(TStorage::const_iterator it = m_storage.begin(); it != m_storage.end(); ++it)
		{
			if(it->second->NeedToSave())
				size++;
		}

		if(size)
		{
			sorted.reserve(size);
			size_t i = 0;
			for(TStorage::const_iterator it = m_storage.begin(); it != m_storage.end(); ++it)
			{
				if(it->second->NeedToSave())
					sorted.push_back(it->second);
			}

			struct TPathComparer
			{
				bool operator ()(const TImageDataPtr pImageData1, const TImageDataPtr pImageData2) const
				{
					return TPath::LesserByPath(pImageData1->path, pImageData2->path);
				}
			};
			std::sort(sorted.begin(), sorted.end(), TPathComparer());
		}
	}

	void TImageDataStorage::SetOld(TIndex & index) const
	{
		for(TIndex::iterator it = index.begin(); it != index.end(); ++it)
		{
			TData & data = it->second;
			for(size_t i = 0; i < m_pOptions->searchPaths.Size(); ++i)
			{
				const TPath & path = m_pOptions->searchPaths[i];
				if((!TPath::LesserByPath(path, data.first) && !TPath::BiggerByPath(path, data.last)) 
					|| path.IsSubPath(data.first) || path.IsSubPath(data.last))
				{
					data.type = TData::Old;
					break;
				}
			}
		}
	}

	void TImageDataStorage::UpdateIndex(TIndex & index) const
	{
		const size_t dataSizeMax = DATA_SIZE_MAX/Simd::Square(m_pOptions->advanced.reducedImageSize/REDUCED_IMAGE_SIZE_MIN);

		TVector sorted;
		CreateSorted(sorted);

		short key = 0;
		for(size_t i = 0; i < sorted.size(); i += dataSizeMax)
		{
			size_t begin = i;
			size_t end = std::min(i + dataSizeMax, sorted.size());

			while(index.count(key))
				key++;

			TData & data = index[key];
			data.type = TData::New;
			data.key = key;
			data.size = end - begin;
			data.first = sorted[begin]->path;
			data.last = sorted[end - 1]->path;
			data.data = TVector(sorted.begin() + begin, sorted.begin() + end);
		}
	}

	TString TImageDataStorage::GetDataFileName(short key) const
	{
		TStringStream ss;
		ss << std::hex << ((key >> 12)&0xF) << ((key >> 8)&0xF) << ((key >> 4)&0xF) << ((key >> 0)&0xF) << FILE_EXTENSION;
		return ss.str();
	}

	bool TImageDataStorage::SaveIndex(const TIndex & index, const TChar *path) const
	{
		HANDLE hFile;
		DWORD byte_was_written;
		BOOL result;

		TString fileName = CreatePath(path, TString(INDEX_FILE_NAME) + FILE_EXTENSION);

		hFile = CreateFile(fileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return false;

		if(!SaveFormatVersion(hFile, INDEX_CONTROL_BYTES, FILE_VERSION))
		{
			CloseHandle(hFile);
			return false;
		}

		result = WriteFile(hFile, &m_pOptions->advanced.reducedImageSize, sizeof(TUInt32), 
			&byte_was_written, NULL);
		if(result != TRUE || byte_was_written < sizeof(TUInt32))
		{
			CloseHandle(hFile);
			return false;
		}

		size_t size = 0, dataSize = 0;
		for(TIndex::const_iterator it = index.begin(); it != index.end(); ++it)
		{
			if(it->second.type == TData::New || it->second.type == TData::Skip)
			{
				size++;
				if(it->second.type == TData::New)
					dataSize += it->second.size;
			}
		}

		TUInt64 value = size;
		result = WriteFile(hFile, &value, sizeof(TUInt64), 
			&byte_was_written, NULL);
		if(result != TRUE || byte_was_written < sizeof(TUInt64))
		{
			CloseHandle(hFile);
			return false;
		}

		m_pStatus->Reset();
		size_t i = 0;
		bool dataSaveResult = true;
		for(TIndex::const_iterator it = index.begin(); it != index.end(); ++it)
		{
			if(it->second.type == TData::New || it->second.type == TData::Skip)
			{
				if(!SaveDataHeader(it->second, hFile))
				{
					CloseHandle(hFile);
					return false;
				}
				if(it->second.type == TData::New)
				{
					dataSaveResult =  SaveData(it->second, path) && dataSaveResult;
					m_pStatus->SetProgress(i, dataSize);
					i += it->second.size;
				}
			}
		}
		m_pStatus->Reset();

		CloseHandle(hFile);
		return dataSaveResult;
	}

	bool TImageDataStorage::SaveData(const TData & data, const TChar *path) const
	{
		HANDLE hFile;
		DWORD byte_was_written;
		BOOL result;

		TString fileName = CreatePath(path, GetDataFileName(data.key));

		hFile = CreateFile(fileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return false;

		if(!SaveFormatVersion(hFile, DATA_CONTROL_BYTES, FILE_VERSION))
		{
			CloseHandle(hFile);
			return false;
		}

		result = WriteFile(hFile, &m_pOptions->advanced.reducedImageSize, sizeof(TUInt32), 
			&byte_was_written, NULL);
		if(result != TRUE || byte_was_written < sizeof(TUInt32))
		{
			CloseHandle(hFile);
			return false;
		}

		if(!SaveDataHeader(data, hFile))
		{
			CloseHandle(hFile);
			return false;
		}
		
		for(size_t i = 0; i < data.data.size(); ++i)
		{
			if(!data.data[i]->Save(hFile))
			{
				CloseHandle(hFile);
				return false;
			}
		}

		CloseHandle(hFile);
		return true;
	}

	bool TImageDataStorage::SaveDataHeader(const TData & data, HANDLE hOut) const
	{
		AD_WRITE_VALUE_TO_FILE(hOut, data.key);
		if(!data.first.Save(hOut))
			return false;
		if(!data.last.Save(hOut))
			return false;
		AD_WRITE_SIZE_TO_FILE(hOut, data.size);
		return true;
	}

	bool TImageDataStorage::LoadIndex(TIndex & index, const TChar *fileName) const
	{
		HANDLE hFile;
		size_t size;
		DWORD byte_was_read;
		BOOL result;
		TUInt64 value;

		if(!IsFileExists(fileName))
			return false;

		hFile = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return false;

		if(!LoadFormatVersion(hFile, INDEX_CONTROL_BYTES, FILE_VERSION))
		{
			CloseHandle(hFile);
			return false;
		}

		TUInt32 reducedImageSize;
		result = ReadFile(hFile, &reducedImageSize, sizeof(TUInt32), &byte_was_read, NULL); 
		if(result != TRUE || byte_was_read < sizeof(TUInt32) || reducedImageSize != m_pOptions->advanced.reducedImageSize)
		{
			CloseHandle(hFile);
			return false;
		}

		result = ReadFile(hFile, &value, sizeof(TUInt64), &byte_was_read, NULL); 
		if(result != TRUE || byte_was_read < sizeof(TUInt64) || value > 0xffffffff)
		{
			CloseHandle(hFile);
			return false;
		}
		size = (size_t)value;

		index.clear();
		for(size_t i = 0; i < size; i++)
		{
			TData data;
			if(!LoadDataHeader(data, hFile))
			{
				CloseHandle(hFile);
				return false;
			}
			index[data.key] = data;
		}
		CloseHandle(hFile);

		SetOld(index);

		return true;
	}

	bool TImageDataStorage::LoadData(TData & data, const TChar *path, short key)
	{
		HANDLE hFile;
		DWORD byte_was_read;
		BOOL result;

		TString fileName = CreatePath(path, GetDataFileName(key));

		if(!IsFileExists(fileName.c_str()))
			return false;

		hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return false;

		if(!LoadFormatVersion(hFile, DATA_CONTROL_BYTES, FILE_VERSION))
		{
			CloseHandle(hFile);
			return false;
		}

		TUInt32 reducedImageSize;
		result = ReadFile(hFile, &reducedImageSize, sizeof(TUInt32), &byte_was_read, NULL); 
		if(result != TRUE || byte_was_read < sizeof(TUInt32) || reducedImageSize != m_pOptions->advanced.reducedImageSize)
		{
			CloseHandle(hFile);
			return false;
		}

		if(!LoadDataHeader(data, hFile) || data.key != key)
		{
			CloseHandle(hFile);
			return false;
		}

		for(size_t i = 0; i < data.size; i++)
		{
			TImageData *pImageData = new TImageData();
			if(!pImageData->Load(hFile))
			{
				CloseHandle(hFile);
				delete pImageData;
				return false;
			}
			else
			{
				TStorage::iterator it = Find(*pImageData);
				if(it == m_storage.end())
					Insert(pImageData);
				else
					delete pImageData;
			}
		}
		CloseHandle(hFile);
		return true;
	}

	bool TImageDataStorage::LoadDataHeader(TData & data, HANDLE hIn) const
	{
		AD_READ_VALUE_FROM_FILE(hIn, data.key);
		if(!data.first.Load(hIn))
			return false;
		if(!data.last.Load(hIn))
			return false;
		AD_READ_SIZE_FROM_FILE(hIn, data.size);

		return true;
	}
}