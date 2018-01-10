#pragma once

#include <queue>
#include <mutex>
#include "adThreads.h"

namespace ad
{
	struct TImageData;

	class TThreadQueue
    {
    public:
        enum QueueState
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
        QueueState Pop(TImageData **ppImageData);

        size_t Id() const {return m_threadId;}
        size_t Size() const {return m_pQueue->size();}

    private:
        volatile size_t m_threadId;
        volatile bool m_finish;
        //TCriticalSection *m_pCS;
		std::mutex _queueMutex;
        TQueue *m_pQueue;
    };
}