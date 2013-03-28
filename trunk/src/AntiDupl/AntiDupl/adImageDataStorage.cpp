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
#include "adFileStream.h"
#include "adException.h"

namespace ad
{
	const TChar FILE_EXTENSION[] = TEXT(".adi");

    const TChar INDEX_FILE_NAME[] = TEXT("index");
	const TChar BACKUP_FILE_NAME[] = TEXT("backup");

	const char INDEX_CONTROL_BYTES[] = "adii";
	const char DATA_CONTROL_BYTES[] = "adid";

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
				it->second = new TImageData(fileInfo, m_pOptions->advanced.reducedImageSize);
			}
		}
		else
		{
			it = Insert(new TImageData(fileInfo, m_pOptions->advanced.reducedImageSize));
		}
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
		const size_t dataSizeMax = IMAGE_DATA_FILE_SIZE_MAX/
			Simd::Square(m_pOptions->advanced.reducedImageSize/REDUCED_IMAGE_SIZE_MIN);

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

		bool dataSaveResult = true;
		m_pStatus->Reset();
		try
		{
			TString fileName = CreatePath(path, TString(INDEX_FILE_NAME) + FILE_EXTENSION);
			TOutputFileStream outputFile(fileName.c_str(), INDEX_CONTROL_BYTES);

			outputFile.Save(m_pOptions->advanced.reducedImageSize);
			outputFile.SaveSize(size);

			size_t i = 0;
			for(TIndex::const_iterator it = index.begin(); it != index.end(); ++it)
			{
				if(it->second.type == TData::New || it->second.type == TData::Skip)
				{
					outputFile.Save(it->second.key);
					outputFile.Save(it->second.first);
					outputFile.Save(it->second.last);
					outputFile.SaveSize(it->second.size);
					if(it->second.type == TData::New)
					{
						dataSaveResult = SaveData(it->second, path) && dataSaveResult;
						m_pStatus->SetProgress(i, dataSize);
						i += it->second.size;
					}
				}
			}
		}
		catch (TException e)
		{
			dataSaveResult = e.Error == AD_OK;
		}
		m_pStatus->Reset();

		return dataSaveResult;
	}

	bool TImageDataStorage::SaveData(const TData & data, const TChar *path) const
	{
		try
		{
			TString fileName = CreatePath(path, GetDataFileName(data.key));
			TOutputFileStream outputFile(fileName.c_str(), DATA_CONTROL_BYTES);

			outputFile.Save(m_pOptions->advanced.reducedImageSize);
			outputFile.Save(data.key);
			outputFile.Save(data.first);
			outputFile.Save(data.last);
			outputFile.SaveSize(data.size);
			for(size_t i = 0; i < data.data.size(); ++i)
				outputFile.Save(*data.data[i]);
		}
		catch (TException e)
		{
			return e.Error == AD_OK;
		}
		return true;
	}

	bool TImageDataStorage::LoadIndex(TIndex & index, const TChar *fileName) const
	{
		try
		{
			TInputFileStream inputFile(fileName, INDEX_CONTROL_BYTES);

			TUInt32 reducedImageSize = inputFile.Load<TUInt32>();
			if(m_pOptions->advanced.reducedImageSize != reducedImageSize)
				throw TException(AD_ERROR_INVALID_FILE_FORMAT);

			size_t size = inputFile.LoadSize();
			if(size > 0xffffffff)
				throw TException(AD_ERROR_INVALID_FILE_FORMAT);

			index.clear();
			for(size_t i = 0; i < size; i++)
			{
				TData data;
				inputFile.Load(data.key);
				inputFile.Load(data.first);
				inputFile.Load(data.last);
				inputFile.LoadSize(data.size);
				index[data.key] = data;
			}

			SetOld(index);
		}
		catch (TException e)
		{
			return e.Error == AD_OK;
		}
		return true;
	}

	bool TImageDataStorage::LoadData(TData & data, const TChar *path, short key)
	{
		try
		{
			TString fileName = CreatePath(path, GetDataFileName(key));
			TInputFileStream inputFile(fileName.c_str(), DATA_CONTROL_BYTES);

			TUInt32 reducedImageSize = inputFile.Load<TUInt32>();
			if(m_pOptions->advanced.reducedImageSize != reducedImageSize)
				throw TException(AD_ERROR_INVALID_FILE_FORMAT);

			inputFile.Load(data.key);
			inputFile.Load(data.first);
			inputFile.Load(data.last);
			inputFile.LoadSize(data.size);

			if(data.key != key || data.size > 0xffffffff)
				throw TException(AD_ERROR_INVALID_FILE_FORMAT);

			TImageData imageData(m_pOptions->advanced.reducedImageSize);
			for(size_t i = 0; i < data.size; i++)
			{
				inputFile.Load(imageData);
				if(Find(imageData) == m_storage.end())
					Insert(new TImageData(imageData));
			}
		}
		catch (TException e)
		{
			return e.Error == AD_OK;
		}
		return true;
	}
}