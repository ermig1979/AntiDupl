/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar.
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
#include "adOptions.h"
#include "adStatus.h"
#include "adImageInfo.h"
#include "adRecycleBin.h"

namespace ad
{
    const TChar *RECYCLE_BIN_PREFIX_FORMAT = TEXT("~~adt%08x~~");
    const int RECYCLE_BIN_PREFIX_SIZE = 16;

    TRecycleBin::TRecycleBin(TEngine *pEngine)
        :m_pOptions(pEngine->Options()),
        m_pStatus(pEngine->Status()),
        m_counter(0)
    {
    }

    TRecycleBin::~TRecycleBin()
    {
        FreeAll();
        ClearTemporaryFiles(false);
    }

    bool TRecycleBin::Delete(TImageInfo *pImageInfo)
    {
        if(m_pOptions->advanced.undoQueueSize)
        {
            TMap::iterator it = m_map.find(pImageInfo);
			// Если в карте не найдено переданное изображение, то созлаем запись и удаляем файл
            if(it == m_map.end())
            {
                TRecord *pRecord = new TRecord();
                pRecord->oldPath = pImageInfo->path.Original();
                pRecord->newPath = GetUniquePath(pImageInfo->path);
                if(::MoveFile(pRecord->oldPath.c_str(), pRecord->newPath.c_str()) != FALSE)
                {
                    pRecord->attribute = ::GetFileAttributes(pRecord->newPath.c_str());
                    ::SetFileAttributes(pRecord->newPath.c_str(), pRecord->attribute|FILE_ATTRIBUTE_HIDDEN);
                    m_map[pImageInfo] = pRecord;
                    pImageInfo->removed = true;
                    m_pStatus->DeleteImage(1, pImageInfo->size);
                    return true;
                }
                delete pRecord;
            }
            return false;
        }
        else
        {
            if(FileDelete(pImageInfo->path.Original().c_str(), m_pOptions->advanced.deleteToRecycleBin == TRUE))
            {
                pImageInfo->removed = true;
                m_pStatus->DeleteImage(1, pImageInfo->size);
                return true;
            }
            return false;
        }
    }

    bool TRecycleBin::Restore(TImageInfo *pImageInfo)
    {
        TMap::iterator it = m_map.find(pImageInfo);
        if(it != m_map.end())
        {
            TRecord *pRecord = it->second;
            if(::MoveFile(pRecord->newPath.c_str(), pRecord->oldPath.c_str()) != FALSE)
            {
                ::SetFileAttributes(pRecord->oldPath.c_str(), pRecord->attribute);
                m_map.erase(it);
                delete pRecord;
                pImageInfo->removed = false;
                m_pStatus->DeleteImage(-1, -TInt64(pImageInfo->size));
                return true;
            }
        }
        return false;
    }

    bool TRecycleBin::Free(TImageInfo *pImageInfo)
    {
        TMap::iterator it = m_map.find(pImageInfo);
        if(it != m_map.end())
        {
            m_temporary.push_back(it->second);
            m_map.erase(it);
            return true;
        }
        return false;
    }

    void TRecycleBin::FreeAll()
    {
        for(TMap::iterator it = m_map.begin(); it != m_map.end(); ++it)
        {
            m_temporary.push_back(it->second);
            m_map.erase(it);
        }
        m_map.clear();
    }

	// Создает иникальное имя для временного хранения удаленного файла, например: F:\Dupli\~~adt00000000~~seehofer.jpg
    TString TRecycleBin::GetUniquePath(const TPath &path)
    {
        TString uniquePath;
        TChar buffer[RECYCLE_BIN_PREFIX_SIZE];
        do 
        {
            _stprintf_s(buffer, RECYCLE_BIN_PREFIX_FORMAT, m_counter++);
            uniquePath = CreatePath(path.GetDirectory(), TString(buffer) + path.GetName());
        } while (IsFileExists(uniquePath.c_str()));
        return uniquePath;
    }

     bool TRecycleBin::ClearTemporaryFiles(bool permanent)
     {
         bool result = true;
         size_t current = 0, total = m_temporary.size(); 
         for(TList::iterator it = m_temporary.begin(); it != m_temporary.end(); ++it, ++current)
         {
             m_pStatus->SetProgress(current, total);             
             result = result && ClearTemporaryFile(*it, permanent);
             delete *it;
         }
         m_temporary.clear();
         m_pStatus->Reset();
         return result;
     }

     bool TRecycleBin::ClearTemporaryFile(TRecord *pRecord, bool permanent)
     {
         if(permanent || m_pOptions->advanced.deleteToRecycleBin != TRUE || IsFileExists(pRecord->oldPath.c_str()))
         {
             return (::DeleteFile(pRecord->newPath.c_str()) != FALSE);
         }
         else
         {
             if(::MoveFile(pRecord->newPath.c_str(), pRecord->oldPath.c_str()) != FALSE)
             {
                 ::SetFileAttributes(pRecord->oldPath.c_str(), pRecord->attribute);
                 return FileDelete(pRecord->oldPath.c_str(), true);
             }
             else
             {
                 return FileDelete(pRecord->newPath.c_str(), true);
             }
         }
     }
}