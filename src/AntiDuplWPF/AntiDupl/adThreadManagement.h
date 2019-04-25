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
#ifndef __adThreadManagement_h__
#define __adThreadManagement_h__

#include "adThreads.h"
#include "adThreadQueue.h"
#include "adThreadTask.h"

namespace ad
{
    struct TImageData;
    struct TOptions;
    class TEngine;
    class TStatus;
    class TCompareManager;
    class TImageComparer;
    class TDataCollector;

    //-------------------------------------------------------------------------
    class TThreadManager
    {
    protected:
        struct TThread
        {
            TThreadTask *task;
			ad::TThread *thread;
        };
    public:
        TThreadManager(TEngine *pEngine);
        ~TThreadManager();

        virtual void Add(TImageData *pImageData) = 0;
        void Finish();

        bool SetPriority(int priority);
        void SetSleepInterval(TUInt32 sleepInterval); 

    protected:
        static size_t GetProcessorCount();

        std::vector<TThread> * m_pThreads;
        TEngine *m_pEngine;
        TOptions *m_pOptions;
        size_t m_addCounter;
    };

    //-------------------------------------------------------------------------
    class TCompareManager : public TThreadManager
    {
    public:
        TCompareManager(TEngine *pEngine);
        ~TCompareManager();

        void Start(size_t imageCount);
        virtual void Add(TImageData *pImageData);

    protected:
        size_t DefaultThreadCount(size_t imageCount);

    private:
        bool CanCompare(TImageData *pImageData) const;

        TCriticalSection *m_pCS;
   };
    //-------------------------------------------------------------------------
    class TCollectManager : public TThreadManager
    {
    public:
        TCollectManager(TEngine *pEngine, TCompareManager* pCompareManager);
        ~TCollectManager() {};

        void Start();
        virtual void Add(TImageData *pImageData);

    protected:
        size_t DefaultThreadCount();

    private:
        TCompareManager *m_pCompareManager;
        
        size_t GetThreadId() const;
    };
	 //-------------------------------------------------------------------------
    class TDctHistogramPeakManager : public TThreadManager
    {
    public:
        TDctHistogramPeakManager(TEngine *pEngine);
        ~TDctHistogramPeakManager();

        void Start(size_t imageCount);
        //virtual void Add(adImageInfoW pImageInfo);
		virtual void Add(TImageData *pImageData);

    //protected:
    //    size_t DefaultThreadCount(size_t imageCount);

    private:


   };
    //-------------------------------------------------------------------------
}
#endif//__adThreadManagement_h__ 