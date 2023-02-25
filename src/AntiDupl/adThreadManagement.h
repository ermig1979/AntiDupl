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
#ifndef __adThreadManagement_h__
#define __adThreadManagement_h__

#include "adThreads.h"

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
    class TThreadQueue
    {
    public:
        enum TPop
        {
            DO_OTHER = 0,
            DO_OWN = 1,
            WAITE = 2,
            FINISH = 3,
            SIZE
        };
    private:
        struct TData
        {
            TImageData *data;
            size_t threadId;
        };
        typedef std::queue<TData> TQueue;

    public:
        TThreadQueue(size_t threadId);
        ~TThreadQueue();

        void Push(TImageData *pImageData, size_t threadId);
        void Finish();
        TPop Pop(TImageData **ppImageData);

        size_t Id() const {return m_threadId;}
        size_t Size() const {return m_pQueue->size();}

    private:
        volatile size_t m_threadId;
        volatile bool m_finish;
        TCriticalSection *m_pCS;
        TQueue *m_pQueue;
    };
    //-------------------------------------------------------------------------
    class TThreadTask : public TThread::TTask
    {
    public:
        TThreadTask(TThreadType threadType, size_t threadId, TEngine *pEngine);
        ~TThreadTask();

        virtual void Work();

        TThreadQueue* Queue() {return m_pQueue;}

        void SetSleepInterval(TUInt32 sleepInterval); 

    protected:
        virtual void DoOwn(TImageData *pImageData) = 0;
        virtual void DoOther(TImageData *pImageData) = 0;

        TEngine *m_pEngine;
        TStatus *m_pStatus;
        const TThreadType m_threadType;
    private:
        TThreadQueue *m_pQueue;
        TUInt32 m_sleepInterval;
    };
    //-------------------------------------------------------------------------
    class TCompareTask : public TThreadTask
    {
    public:
        TCompareTask(size_t threadId, TEngine *pEngine);
        ~TCompareTask();

    protected:
        virtual void DoOwn(TImageData *pImageData);
        virtual void DoOther(TImageData *pImageData);

    private:
        TImageComparer* m_pImageComparer;
    };
    //-------------------------------------------------------------------------
    class TCollectTask : public TThreadTask
    {
    public:
        TCollectTask(size_t threadId, TEngine *pEngine, TCompareManager *pCompareManager);
        ~TCollectTask();

    protected:
        virtual void DoOwn(TImageData *pImageData);
        virtual void DoOther(TImageData *pImageData) {};

    private:
        TDataCollector* m_pDataCollector;
        TCompareManager *m_pCompareManager;
    };
    //-------------------------------------------------------------------------
    class TThreadManager
    {
    protected:
        struct TThread
        {
            TThreadTask *task;
			ad::TThread *thread;
        };
        typedef std::vector<TThread> TThreads;
    public:
        TThreadManager(TEngine *pEngine);
        ~TThreadManager();

        virtual void Add(TImageData *pImageData) = 0;
        void Finish();

        bool SetPriority(int priority);
        void SetSleepInterval(TUInt32 sleepInterval); 

    protected:
        static size_t GetProcessorCount();

        TThreads *m_pThreads;
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
}
#endif//__adThreadManagement_h__ 