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
#include "adImageInfo.h"
#include "adMistakeStorage.h"

namespace ad
{
    //-------------------------------------------------------------------------

    const TChar DEFAULT_fileName[] = TEXT("AntiDupl.adm");

    const char CONTROL_bytes[] = "adm2";

    //-------------------------------------------------------------------------
    adError TMistakeStorage::Save(const TChar *fileName)
    {
        HANDLE hFile;
        size_t size;
        DWORD byte_was_written;
        BOOL result;
        TUInt64 value;

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

        size = m_singles.size();
        value = size;
        result = WriteFile(hFile, &value, sizeof(TUInt64), 
            &byte_was_written, NULL);
        if(result != TRUE || byte_was_written < sizeof(TUInt64))
        {
            CloseHandle(hFile);
            return AD_ERROR_CANT_WRITE_FILE;
        }

        for(TImageInfoPtrMultiSet::iterator it = m_singles.begin(); it != m_singles.end(); ++it)
        {
            if(!(*it)->Save(hFile))
            {
                CloseHandle(hFile);
                return AD_ERROR_CANT_WRITE_FILE;
            }
        }

        size = m_pairs.size();
        value = size;
        result = WriteFile(hFile, &value, sizeof(TUInt64), 
            &byte_was_written, NULL);
        if(result != TRUE || byte_was_written < sizeof(TUInt64))
        {
            CloseHandle(hFile);
            return AD_ERROR_CANT_WRITE_FILE;
        }

        for(TImageInfoPtrPairMultiSet::iterator it = m_pairs.begin(); it != m_pairs.end(); ++it)
        {
            if(!it->first->Save(hFile))
            {
                CloseHandle(hFile);
                return AD_ERROR_CANT_WRITE_FILE;
            }
            if(!it->second->Save(hFile))
            {
                CloseHandle(hFile);
                return AD_ERROR_CANT_WRITE_FILE;
            }
        }

        CloseHandle(hFile);
        return AD_OK;
    }

    adError TMistakeStorage::Load(const TChar *fileName, bool check)
    {
        HANDLE hFile;
        size_t size;
        DWORD byte_was_read;
        BOOL result;
        TUInt64 value;

        Clear();

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
            TImageInfo single;
            if(!single.Load(hFile))
            {
                CloseHandle(hFile);
                Clear();
                return AD_ERROR_CANT_READ_FILE;
            }
            if(!check || single.Actual())
                m_singles.insert(new TImageInfo(single));
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
            TImageInfo first;
            if(!first.Load(hFile))
            {
                CloseHandle(hFile);
                Clear();
                return AD_ERROR_CANT_READ_FILE;
            }
            TImageInfo second;
            if(!second.Load(hFile))
            {
                CloseHandle(hFile);
                Clear();
                return AD_ERROR_CANT_READ_FILE;
            }
            if(!check || (first.Actual() && second.Actual()))
                m_pairs.insert(TImageInfoPtrPair(new TImageInfo(first), new TImageInfo(second)));
        }

        CloseHandle(hFile);
        return AD_OK;
    }

    void TMistakeStorage::Add(const TImageInfoPtr first, const TImageInfoPtr second)
    {
        if(!IsHas(first, second))
        {
            if(TPath::LesserByPath(first->path, second->path))
                m_pairs.insert(TImageInfoPtrPair(new TImageInfo(*first), new TImageInfo(*second)));
            else
                m_pairs.insert(TImageInfoPtrPair(new TImageInfo(*second), new TImageInfo(*first)));
        }
    }

    void TMistakeStorage::Add(const TImageInfoPtr single)
    {
        if(!IsHas(single))
        {
            m_singles.insert(new TImageInfo(*single));
        }
    }

    void TMistakeStorage::Erase(const TImageInfoPtr first, const TImageInfoPtr second)
    {
        TImageInfoPtrPairMultiSet::iterator it = Find(first, second);
        if(it != m_pairs.end())
        {
            delete it->first;
            delete it->second;
            m_pairs.erase(it);
        }
    }

    void TMistakeStorage::Erase(const TImageInfoPtr single)
    {
        TImageInfoPtrMultiSet::iterator it = Find(single);
        if(it != m_singles.end())
        {
            delete *it;
            m_singles.erase(it);
        }
    }

    bool TMistakeStorage::IsHas(const TImageInfoPtr first, const TImageInfoPtr second) const
    {
        return Find(first, second) != m_pairs.end();
    }

    bool TMistakeStorage::IsHas(const TImageInfoPtr single) const
    {
        return Find(single) != m_singles.end();
    }

    void TMistakeStorage::Clear() 
    {
        for(TImageInfoPtrMultiSet::iterator it = m_singles.begin(); it != m_singles.end(); ++it)
        {
            delete *it;
        }
        m_singles.clear();
        for(TImageInfoPtrPairMultiSet::iterator it = m_pairs.begin(); it != m_pairs.end(); ++it)
        {
            delete it->first;
            delete it->second;
        }
        m_pairs.clear();
    }

    void TMistakeStorage::Rename(const TImageInfoPtr pImageInfo, const TString& newPath)
    {
        typedef std::list<TImageInfoPtr> TImageInfoPtrList;
        TImageInfoPtrList renamedSingles;
        for(TImageInfoPtrMultiSet::iterator it = m_singles.begin(); it != m_singles.end();)
        {
            if(**it == *pImageInfo)
            {
                renamedSingles.push_back(*it);
                it = m_singles.erase(it);
            }
            else
                ++it;
        }
        for(TImageInfoPtrList::iterator it = renamedSingles.begin(); it != renamedSingles.end(); ++it)
        {
            (*it)->Rename(newPath);
            m_singles.insert(*it);
        }
        
        typedef std::list<TImageInfoPtrPair> TImageInfoPtrPairList;
        TImageInfoPtrPairList renamedPairs;
        for(TImageInfoPtrPairMultiSet::iterator it = m_pairs.begin(); it != m_pairs.end();)
        {
            if(*it->first == *pImageInfo || *it->second == *pImageInfo)
            {
                renamedPairs.push_back(*it);
                it = m_pairs.erase(it);
            }
            else
                ++it;
        }
        for(TImageInfoPtrPairList::iterator it = renamedPairs.begin(); it != renamedPairs.end(); ++it)
        {
            if(*it->first == *pImageInfo)
                it->first->Rename(newPath);
            if(*it->second == *pImageInfo)
                it->second->Rename(newPath);
            if(TPath::LesserByPath(it->first->path, it->second->path))
                m_pairs.insert(TImageInfoPtrPair(it->first, it->second));
            else
                m_pairs.insert(TImageInfoPtrPair(it->second, it->first));
        }
    }
    
    bool TMistakeStorage::TImageInfoPtrLesser::operator ()(TImageInfoPtr first, TImageInfoPtr second) const
    {
        return TPath::LesserByPath(first->path, second->path);
    }

    bool TMistakeStorage::TImageInfoPtrPairLesser::operator () (const TImageInfoPtrPair& first, const TImageInfoPtrPair& second) const
    {
        return TPath::LesserByPath(first.first->path, second.first->path) ||
            (TPath::EqualByPath(first.first->path, second.first->path) && 
            TPath::LesserByPath(first.second->path, second.second->path));
    }
    
    TMistakeStorage::TImageInfoPtrPairMultiSet::const_iterator TMistakeStorage::Find(const TImageInfoPtr first, const TImageInfoPtr second) const
    {
        TImageInfoPtrPair pair;
        if(TPath::LesserByPath(first->path, second->path))
            pair = TImageInfoPtrPair(first, second);
        else
            pair = TImageInfoPtrPair(second, first);
        TImageInfoPtrPairMultiSet::const_iterator it = m_pairs.lower_bound(pair);
        TImageInfoPtrPairMultiSet::const_iterator itEnd = m_pairs.upper_bound(pair);
        for(;it != itEnd; ++it)
        {
            if(*it->first == *pair.first && *it->second == *pair.second)
                return it;
        }
        return m_pairs.end();
    }
    
    TMistakeStorage::TImageInfoPtrPairMultiSet::iterator TMistakeStorage::Find(const TImageInfoPtr first, const TImageInfoPtr second)
    {
        TImageInfoPtrPair pair;
        if(TPath::LesserByPath(first->path, second->path))
            pair = TImageInfoPtrPair(first, second);
        else
            pair = TImageInfoPtrPair(second, first);
        TImageInfoPtrPairMultiSet::iterator it = m_pairs.lower_bound(pair);
        TImageInfoPtrPairMultiSet::iterator itEnd = m_pairs.upper_bound(pair);
        for(;it != itEnd; ++it)
        {
            if(*it->first == *pair.first && *it->second == *pair.second)
                return it;
        }
        return m_pairs.end();
    }
    
    TMistakeStorage::TImageInfoPtrMultiSet::const_iterator TMistakeStorage::Find(const TImageInfoPtr single) const
    {
        TImageInfoPtrMultiSet::const_iterator it = m_singles.lower_bound(single);
        TImageInfoPtrMultiSet::const_iterator itEnd = m_singles.upper_bound(single);
        for(;it != itEnd; ++it)
        {
            if(**it == *single)
                return it;
        }
        return m_singles.end();
    }
    
    TMistakeStorage::TImageInfoPtrMultiSet::iterator TMistakeStorage::Find(const TImageInfoPtr single)
    {
        TImageInfoPtrMultiSet::iterator it = m_singles.lower_bound(single);
        TImageInfoPtrMultiSet::iterator itEnd = m_singles.upper_bound(single);
        for(;it != itEnd; ++it)
        {
            if(**it == *single)
                return it;
        }
        return m_singles.end();
    }
    //-------------------------------------------------------------------------
}