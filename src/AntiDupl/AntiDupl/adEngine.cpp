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
#include "adInit.h"
#include "adStatus.h"
#include "adImageInfo.h"
#include "adOptions.h"
#include "adResult.h"
#include "adResultStorage.h"
#include "adImageDataStorage.h"
#include "adMistakeStorage.h"
#include "adThreadManagement.h"
#include "adSearcher.h"
#include "adRecycleBin.h"
#include "adEngine.h"
#include "adPerformance.h"
#include "adLogger.h"
#include "adFileUtils.h"

namespace ad
{
    TEngine::TEngine()
    {
#ifdef AD_LOGGER_ENABLE
        TLogger::s_logger.SetFileOut(TString(GetApplicationDirectory() + TEXT("\\AntiDupl.log")).c_str(), true);
#endif//AD_LOGGER_ENABLE
        m_pInit = new TInit();
        m_pOptions = new TOptions();
        m_pStatus = new TStatus();
        m_pMistakeStorage = new TMistakeStorage(this);
        m_pImageDataStorage = new TImageDataStorage(this);
        m_pRecycleBin = new TRecycleBin(this);        
        m_pResult = new TResultStorage(this);
        m_pImageDataPtrs = new TImageDataPtrs();
        m_pCriticalSection = new TCriticalSection();
        m_pCompareManager = new TCompareManager(this);
        m_pCollectManager = new TCollectManager(this, m_pCompareManager);
        m_pSearcher = new TSearcher(this, m_pImageDataPtrs);
    }

    TEngine::~TEngine()
    {
        delete m_pMistakeStorage;
        delete m_pImageDataStorage;
        delete m_pResult;
        delete m_pImageDataPtrs;
        delete m_pCriticalSection;
        delete m_pInit;
        delete m_pCompareManager;
        delete m_pCollectManager;
        delete m_pSearcher;
        delete m_pRecycleBin;
        delete m_pStatus;
        delete m_pOptions;
#ifdef AD_LOGGER_ENABLE
#ifdef AD_PERFORMANCE_TEST
        TLogger::s_logger.SetThreadIdAnnotation(false);
        AD_LOG(TPerformanceMeasurerStorage::s_storage.Statistic());
#endif//AD_PERFORMANCE_TEST
        TLogger::s_logger.ResetOut();
#endif//AD_LOGGER_ENABLE
    }

    void TEngine::Search()
    {
        AD_FUNCTION_PERFORMANCE_TEST
        m_pStatus->ClearStatistic();
        m_pStatus->SetProgress(0, 0);
        m_pResult->Clear();

        m_pSearcher->SearchImages();

        if(m_pOptions->check.checkOnEquality == TRUE)
        {
            m_pCompareManager->Start(m_pImageDataPtrs->size());
            m_pCompareManager->SetPriority(THREAD_PRIORITY_LOWEST);
        }
        m_pCollectManager->Start();
        m_pCollectManager->SetPriority(THREAD_PRIORITY_BELOW_NORMAL);

        size_t current = 0, total = m_pImageDataPtrs->size(); 
        for(TImageDataPtrs::iterator it = m_pImageDataPtrs->begin(); 
            it != m_pImageDataPtrs->end() && !m_pStatus->Stopped(); ++it, ++current)
        {
            TImageDataPtr pImageData = *it;
            m_pCollectManager->Add(pImageData);
            m_pStatus->SetProgress(current, total);
        }
        m_pCollectManager->Finish();

        if(m_pOptions->check.checkOnEquality == TRUE)
        {
            m_pStatus->SetProgress(current, total);
            m_pStatus->Wait(AD_THREAD_TYPE_MAIN, 0);
            m_pCompareManager->SetPriority(THREAD_PRIORITY_NORMAL);
            m_pCompareManager->Finish();
        }

        m_pImageDataPtrs->clear();
        m_pStatus->Reset();
    }
}
