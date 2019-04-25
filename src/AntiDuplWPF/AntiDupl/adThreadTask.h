#pragma once

#include "adThreads.h"
#include "adThreadQueue.h"
#include "adEngine.h"
#include "adDataCollector.h"

namespace ad
{
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
	class TDctHistogramPeakTask : public TThreadTask
	{
	public:
		TDctHistogramPeakTask(size_t threadId, TEngine *pEngine);
		~TDctHistogramPeakTask();

	protected:
		virtual void DoOwn(TImageData *pImageData);
		virtual void DoOther(TImageData *pImageData) {};
		
	};
}
