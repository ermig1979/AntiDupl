#include "adThreadQueue.h"

namespace ad
{
    //-------------------------------------------------------------------------
    TThreadQueue::TThreadQueue(size_t threadId)
        :m_threadId(threadId),
        m_finish(false)
    {
        m_pQueue = new TQueue();
        //m_pCS = new TCriticalSection();
    }

    TThreadQueue::~TThreadQueue()
    {
        delete m_pQueue;
        //delete m_pCS;
    }

    void TThreadQueue::Push(TImageData *pImageData, size_t threadId)
    {
        //TCriticalSection::TLocker locker(m_pCS);
		std::lock_guard<std::mutex> lock(_queueMutex);
        TData data;
        data.data = pImageData;
        data.threadId = threadId;
        m_pQueue->push(data);
    }

    void TThreadQueue::Finish()
    {
        //TCriticalSection::TLocker locker(m_pCS);
		std::lock_guard<std::mutex> lock(_queueMutex);
        m_finish = true;
    }

    TThreadQueue::QueueState TThreadQueue::Pop(TImageData **ppImageData)
    {
        //TCriticalSection::TLocker locker(m_pCS);
		std::lock_guard<std::mutex> lock(_queueMutex);
        if(m_pQueue->empty())
        {
            *ppImageData = NULL;
            if(m_finish)
                return FINISH;
            else 
                return WAITE;
        }
        else
        {
            TData data = m_pQueue->front();
            m_pQueue->pop();
            *ppImageData = data.data;
            if(data.threadId == m_threadId)
                return DO_OWN;
            else
                return DO_OTHER;
        }
    }
}
