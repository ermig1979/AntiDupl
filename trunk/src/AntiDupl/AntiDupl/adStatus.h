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
#ifndef __adStatus_h__
#define __adStatus_h__

#include "adStrings.h"
#include "adThreads.h"

namespace ad
{
    //-------------------------------------------------------------------------
    class TThreadStatus
    {
    public:
        TThreadStatus();

        adError Export(adStatusPtrA pStatus) const;
        adError Export(adStatusPtrW pStatus) const;

        size_t ProcessedCount() const {return m_processedCount;};

        void Assign();
        void Process(const TChar* fileName);
        void Wait();

    private:
        volatile TStateType m_state;
        TString m_path;
        volatile size_t m_assignedCount;
        volatile size_t m_processedCount;
    };
    typedef std::vector<TThreadStatus> TThreadStatuses;
    //-------------------------------------------------------------------------
    class TStatus : public TThread::TStatus
    {
    public:
        TStatus();
        ~TStatus(){};

        virtual void Reset();
        virtual void Stop();

        TStateType State() const {return m_state;};

        adError Export(adStatisticPtr pStatistic) const;
        adError Export(adThreadType threadType, adSize threadId, adStatusPtrA pStatus) const;
        adError Export(adThreadType threadType, adSize threadId, adStatusPtrW pStatus) const;

        void ClearStatistic();

        void Search(const TChar* folder, size_t searchedImageNumber);

        void SetThreadCount(TThreadType threadType, size_t count);
        void Assign(TThreadType threadType, size_t threadId);
        void Process(TThreadType threadType, size_t threadId, const TChar* fileName);
        void Wait(TThreadType threadType, size_t threadId);

        void AddDefectImage(ptrdiff_t count = 1);
        void AddDuplImagePair(ptrdiff_t count = 1);

        void DeleteImage(ptrdiff_t count, TInt64 size);
        void RenameImage(ptrdiff_t count);

        void SetProgress(size_t current, size_t total);

    private:
        mutable TCriticalSection m_criticalSection;

        volatile TStateType m_state;
        TString m_path;
        volatile size_t m_current;
        volatile size_t m_total;

        volatile adStatistic m_statistic;

        TThreadStatuses m_compareThreadStatuses;
        TThreadStatuses m_collectThreadStatuses;
    };
    //-------------------------------------------------------------------------
}
#endif//__adStatus_h__ 