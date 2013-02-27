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

namespace ad
{
    //-------------------------------------------------------------------------

    const TChar DEFAULT_fileName[] = TEXT("AntiDupl.adi");

    const char CONTROL_bytes[] = "adi06";

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

    TImageDataStorage::TStorage::iterator TImageDataStorage::Insert(TImageMultiData* pImageMultiData)
    {
        return m_storage.insert(TRecord(pImageMultiData->hash, pImageMultiData));
    }

    void TImageDataStorage::Clear()
    {
        //Bs::HeapCompacter heapCompacter;
        for(TStorage::iterator it = m_storage.begin(); it != m_storage.end(); ++it)
            delete it->second;
        m_storage.clear();
    }

    TImageDataPtr TImageDataStorage::Get(const TFileInfo& fileInfo)
    {
        TStorage::iterator it = Find(fileInfo);
        if(it != m_storage.end())
        {
            if(it->second->size != fileInfo.size || it->second->time != fileInfo.time)
            {
                delete it->second;
                it->second = new TImageMultiData(fileInfo);
            }
        }
        else
        {
            it = Insert(new TImageMultiData(fileInfo));
        }
        it->second->SetData(m_pOptions->advanced.reducedImageSize);
        return it->second;
    }

    adError TImageDataStorage::Load(const TChar *fileName, bool check)
    {
        HANDLE hFile;
        size_t size;
        DWORD byte_was_read;
        BOOL result;
        TUInt64 value;

        TString path;
        if(fileName == NULL)
            path = CreatePath(GetApplicationDirectory(), DEFAULT_fileName);
        else
            path = fileName;

        if(!IsFileExists(path.c_str()))
            return AD_ERROR_FILE_IS_NOT_EXIST;

        hFile = CreateFile(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
            return AD_ERROR_CANT_OPEN_FILE;

        char control_bytes[sizeof(CONTROL_bytes)];
        result = ReadFile(hFile, &control_bytes, sizeof(CONTROL_bytes), 
            &byte_was_read, NULL); 
        if(result != TRUE || byte_was_read < sizeof(CONTROL_bytes) ||
            memcmp(control_bytes, CONTROL_bytes, sizeof(CONTROL_bytes)) != 0)
        {
            CloseHandle(hFile);
            return AD_ERROR_INVALID_FILE_FORMAT;
        }

        result = ReadFile(hFile, &value, sizeof(TUInt64), &byte_was_read, NULL); 
        if(result != TRUE || byte_was_read < sizeof(TUInt64) || value > 0xffffffff)
        {
            CloseHandle(hFile);
            return AD_ERROR_INVALID_FILE_FORMAT;
        }
        size = (size_t)value;

        for(size_t i = 0; i < size; i++)
        {
            m_pStatus->SetProgress(i, size);
            TImageMultiData *pImageMultiData = new TImageMultiData();
            if(!pImageMultiData->Load(hFile))
            {
				CloseHandle(hFile);
                m_pStatus->Reset();
                delete pImageMultiData;
                return AD_ERROR_CANT_READ_FILE;
            }
			else if(m_pStatus->Stopped())
			{
				Clear();
				break;
			}
			else
            {
                TStorage::iterator it = Find(*pImageMultiData);
                bool add = (!check || pImageMultiData->Actual()) && it == m_storage.end();
                if(add)
                {
                    Insert(pImageMultiData);
                }
                else
                {
                    delete pImageMultiData;
                }
            }
        }
        m_pStatus->Reset();
        CloseHandle(hFile);
        return AD_OK;
    }

    adError TImageDataStorage::Save(const TChar *fileName) const
    {
        HANDLE hFile;
        DWORD byte_was_written;
        BOOL result;

        TString path;
        if(fileName == NULL)
            path = CreatePath(GetApplicationDirectory(), DEFAULT_fileName);
        else
            path = fileName;

        hFile = CreateFile(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
            return AD_ERROR_CANT_CREATE_FILE;

        result = WriteFile(hFile, CONTROL_bytes, sizeof(CONTROL_bytes), 
            &byte_was_written, NULL);
        if(result != TRUE || byte_was_written < sizeof(CONTROL_bytes))
        {
            CloseHandle(hFile);
            return AD_ERROR_CANT_WRITE_FILE;
        }

        size_t size = 0;
        for(TStorage::const_iterator it = m_storage.begin(); it != m_storage.end(); ++it)
        {
            if(it->second->FilledDataSize() > 0 || it->second->defect != AD_DEFECT_UNDEFINE)
                size++;
        }

        TUInt64 value = size;
        result = WriteFile(hFile, &value, sizeof(TUInt64), 
            &byte_was_written, NULL);
        if(result != TRUE || byte_was_written < sizeof(TUInt64))
        {
            CloseHandle(hFile);
            return AD_ERROR_CANT_WRITE_FILE;
        }

        size_t i = 0;
        for(TStorage::const_iterator it = m_storage.begin(); it != m_storage.end(); ++it)
        {
            if(it->second->FilledDataSize() > 0 || it->second->defect != AD_DEFECT_UNDEFINE)
            {
                m_pStatus->SetProgress(++i, size);
                if(!it->second->Save(hFile) || m_pStatus->Stopped())
                {
                    m_pStatus->Reset();
                    CloseHandle(hFile);
                    return AD_ERROR_CANT_WRITE_FILE;
                }
            }
        }

        m_pStatus->Reset();
        CloseHandle(hFile);
        return AD_OK;
    }
}