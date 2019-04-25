/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2015 Yermalayeu Ihar.
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
#include "adStatus.h"

namespace ad
{
    //-------------------------------------------------------------------------
    TThreadStatus::TThreadStatus()
        :m_state(AD_STATE_NONE),
        m_path(),
        m_assignedCount(0),
        m_processedCount(0)
    {
    }

    adError TThreadStatus::Export(adStatusPtrA pStatus) const
    {
        if(pStatus == NULL)
            return AD_ERROR_INVALID_POINTER;

        pStatus->state = m_state;
        m_path.CopyTo(pStatus->path, MAX_PATH);
        pStatus->total = m_assignedCount;
        pStatus->current = m_processedCount;

        return AD_OK;
    }

    adError TThreadStatus::Export(adStatusPtrW pStatus) const
    {
        if(pStatus == NULL)
            return AD_ERROR_INVALID_POINTER;

        pStatus->state = m_state;
        m_path.CopyTo(pStatus->path, MAX_PATH_EX);
        pStatus->total = m_assignedCount;
        pStatus->current = m_processedCount;

        return AD_OK;
    }

    void TThreadStatus::Assign()
    {
        m_assignedCount++;
    }

    void TThreadStatus::Process(const TChar* fileName)
    {
        m_state = AD_STATE_WORK;
        m_processedCount++;
        m_path = fileName;
    }

    void TThreadStatus::Wait()
    {
        m_state = AD_STATE_WAIT;
    }
    //-------------------------------------------------------------------------
    TStatus::TStatus()
    {
        Reset();
    }

    void TStatus::Reset()
    {
        TCriticalSection::TLocker locker(&m_criticalSection);

        m_stopped = false;
        m_state = AD_STATE_NONE;
        m_path.clear();
        m_current = 0;
        m_total = 0;

        m_compareThreadStatuses.clear();
        m_collectThreadStatuses.clear();
    }

    void TStatus::Stop()
    {
        TCriticalSection::TLocker locker(&m_criticalSection);

        m_stopped = true;

        m_state = AD_STATE_STOP;
    }

    adError TStatus::Export(adStatisticPtr pStatistic) const
    {
        if(pStatistic == NULL)
            return AD_ERROR_INVALID_POINTER;

        TCriticalSection::TLocker locker(&m_criticalSection);

        memcpy(pStatistic, (void*)&m_statistic, sizeof(adStatistic));

        return AD_OK;
    }

    adError TStatus::Export(adThreadType threadType, adSize threadId, adStatusPtrA pStatus) const
    {
        if(pStatus == NULL)
            return AD_ERROR_INVALID_POINTER;

        if(threadType < AD_THREAD_TYPE_MAIN || threadType >= AD_THREAD_TYPE_SIZE)            
            return AD_ERROR_INVALID_THREAD_TYPE;

        if((threadType == AD_THREAD_TYPE_COLLECT || threadId >= m_collectThreadStatuses.size()) ||
           (threadType == AD_THREAD_TYPE_COMPARE || threadId >= m_compareThreadStatuses.size()))
            return AD_ERROR_INVALID_THREAD_ID;

        TCriticalSection::TLocker locker(&m_criticalSection);

        switch(threadType)
        {
        case AD_THREAD_TYPE_MAIN:
            pStatus->state = m_state;
            m_path.CopyTo(pStatus->path, MAX_PATH);
            pStatus->current = m_current;
            pStatus->total = m_total;
            break;
        case AD_THREAD_TYPE_COLLECT:
            return m_collectThreadStatuses[threadId].Export(pStatus);
        case AD_THREAD_TYPE_COMPARE:
            return m_compareThreadStatuses[threadId].Export(pStatus);
        }

        return AD_OK;
    }

    adError TStatus::Export(adThreadType threadType, adSize threadId, adStatusPtrW pStatus) const
    {
        if(pStatus == NULL)
            return AD_ERROR_INVALID_POINTER;

        if(threadType < AD_THREAD_TYPE_MAIN || threadType >= AD_THREAD_TYPE_SIZE)            
            return AD_ERROR_INVALID_THREAD_TYPE;

        if( (threadType == AD_THREAD_TYPE_COLLECT && threadId >= m_collectThreadStatuses.size()) ||
            (threadType == AD_THREAD_TYPE_COMPARE && threadId >= m_compareThreadStatuses.size()))
            return AD_ERROR_INVALID_THREAD_ID;

        TCriticalSection::TLocker locker(&m_criticalSection);

        switch(threadType)
        {
        case AD_THREAD_TYPE_MAIN:
            pStatus->state = m_state;
            m_path.CopyTo(pStatus->path, MAX_PATH_EX);
            pStatus->current = m_current;
            pStatus->total = m_total;
            break;
        case AD_THREAD_TYPE_COLLECT:
            return m_collectThreadStatuses[threadId].Export(pStatus);
        case AD_THREAD_TYPE_COMPARE:
            return m_compareThreadStatuses[threadId].Export(pStatus);
        }

        return AD_OK;
    }

    void TStatus::ClearStatistic()
    {
        ZeroMemory((void*)&m_statistic, sizeof(adStatistic));
    }

    void TStatus::Search(const TChar* folder, size_t searchedImageNumber)
    {
        TCriticalSection::TLocker locker(&m_criticalSection);

        m_state = AD_STATE_WORK;
        if(folder != NULL)
        {
            m_statistic.scanedFolderNumber++; 
            m_path = folder;
        }
        else
            m_path.clear();
        m_statistic.searchedImageNumber = searchedImageNumber;
        m_current = 0;
        m_total = m_statistic.searchedImageNumber;
    }

    void TStatus::SetThreadCount(TThreadType threadType, size_t count)
    {
        TCriticalSection::TLocker locker(&m_criticalSection);

        switch(threadType)
        {
        case AD_THREAD_TYPE_COLLECT:
            m_statistic.collectThreadCount = count;
            m_collectThreadStatuses.resize(count);
            break;
        case AD_THREAD_TYPE_COMPARE:
            m_statistic.compareThreadCount = count;
            m_compareThreadStatuses.resize(count);
            break;
        }
    }

    void TStatus::Assign(TThreadType threadType, size_t threadId)
    {
        TCriticalSection::TLocker locker(&m_criticalSection);

        switch(threadType)
        {
        case AD_THREAD_TYPE_COLLECT:
            m_collectThreadStatuses[threadId].Assign();
            break;
        case AD_THREAD_TYPE_COMPARE:
            m_compareThreadStatuses[threadId].Assign();
            break;
        }
    }

    void TStatus::Process(TThreadType threadType, size_t threadId, const TChar* fileName)
    {
        TCriticalSection::TLocker locker(&m_criticalSection);

        switch(threadType)
        {
        case AD_THREAD_TYPE_COLLECT:
            m_statistic.collectedImageNumber++;
            m_collectThreadStatuses[threadId].Process(fileName);
            break;
        case AD_THREAD_TYPE_COMPARE:
            m_statistic.comparedImageNumber++;
            m_compareThreadStatuses[threadId].Process(fileName);
            break;
        }
    }

    void TStatus::Wait(TThreadType threadType, size_t threadId)
    {
        TCriticalSection::TLocker locker(&m_criticalSection);

        switch(threadType)
        {
        case AD_THREAD_TYPE_MAIN:
            m_state = AD_STATE_WAIT;
            break;
        case AD_THREAD_TYPE_COLLECT:
            m_collectThreadStatuses[threadId].Wait();
            break;
        case AD_THREAD_TYPE_COMPARE:
            m_compareThreadStatuses[threadId].Wait();
            break;
        }
    }

    void TStatus::AddDefectImage(ptrdiff_t count)
    {
        TCriticalSection::TLocker locker(&m_criticalSection);

        m_statistic.defectImageNumber += count;
    }

    void TStatus::AddDuplImagePair(ptrdiff_t count)
    {
        TCriticalSection::TLocker locker(&m_criticalSection);

        m_statistic.duplImagePairNumber += count;
    }

    void TStatus::SetProgress(size_t current, size_t total)
    {
        TCriticalSection::TLocker locker(&m_criticalSection);

        m_path.clear();
        m_state = AD_STATE_WORK;
        m_current = current;
        m_total = total;
    }

    void TStatus::DeleteImage(ptrdiff_t count, TInt64 size)
    {
        TCriticalSection::TLocker locker(&m_criticalSection);

        m_statistic.deletedImageNumber += count;
        m_statistic.deletedImageSize += size;
    }

    void TStatus::RenameImage(ptrdiff_t count)
    {
        TCriticalSection::TLocker locker(&m_criticalSection);

        m_statistic.renamedImageNumber += count;
    }
    //-------------------------------------------------------------------------
}
