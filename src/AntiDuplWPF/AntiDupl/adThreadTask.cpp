#include "adThreadTask.h"
#include "adImageComparer.h"
#include "adImageData.h"
#include "adThreadManagement.h"

namespace ad
{
 TThreadTask::TThreadTask(TThreadType threadType,  size_t threadId, TEngine *pEngine)
        :TThread::TTask((TThread::TStatus*)pEngine->Status()),
        m_pStatus(pEngine->Status()),
        m_pEngine(pEngine),
        m_threadType(threadType),
        m_sleepInterval(DEAFAULT_THREAD_SLEEP_INTERVAL)
    {
        m_pQueue = new TThreadQueue(threadId);
    }

    TThreadTask::~TThreadTask()
    {
        delete m_pQueue; 
    }

    void TThreadTask::SetSleepInterval(TUInt32 sleepInterval)
    {
        m_sleepInterval = sleepInterval;
    }

    void TThreadTask::Work()
    {
        while(!m_pStatus->Stopped())
        {
            TImageDataPtr pImageData = NULL;

            switch(m_pQueue->Pop(&pImageData))
            {
            case TThreadQueue::DO_OTHER:
                DoOther(pImageData);
                continue;
            case TThreadQueue::DO_OWN:
                DoOwn(pImageData);
                continue;
            case TThreadQueue::WAITE:
                m_pStatus->Wait(m_threadType, m_pQueue->Id()); 
                Sleep(m_sleepInterval);
                continue;
            case TThreadQueue::FINISH:
            default: 
                return;
            }
        }
    }
    //-------------------------------------------------------------------------
    TCompareTask::TCompareTask(size_t threadId, TEngine *pEngine)
        :TThreadTask(AD_THREAD_TYPE_COMPARE, threadId, pEngine)
    {
        m_pImageComparer = CreateImageComparer(pEngine);
    }

    TCompareTask::~TCompareTask()
    {
        delete m_pImageComparer;
    }

    void TCompareTask::DoOwn(TImageData *pImageData)
    {
        m_pImageComparer->Accept(pImageData, true);
        m_pStatus->Process(AD_THREAD_TYPE_COMPARE, Queue()->Id(), pImageData->path.Original().c_str());
    }

    void TCompareTask::DoOther(TImageData *pImageData)
    {
        m_pImageComparer->Accept(pImageData, false);
    }

    //-------------------------------------------------------------------------
    TCollectTask::TCollectTask(size_t threadId, TEngine *pEngine, TCompareManager *pCompareManager)
        :TThreadTask(AD_THREAD_TYPE_COLLECT, threadId, pEngine),
        m_pCompareManager(pCompareManager)
    {
        m_pDataCollector = new TDataCollector(pEngine);
    }

    TCollectTask::~TCollectTask()
    {
        delete m_pDataCollector;
    }

    void TCollectTask::DoOwn(TImageData *pImageData)
    {
        m_pDataCollector->Fill(pImageData);
        m_pCompareManager->Add(pImageData);
        m_pStatus->Process(AD_THREAD_TYPE_COLLECT, Queue()->Id(), pImageData->path.Original().c_str());
    }
}