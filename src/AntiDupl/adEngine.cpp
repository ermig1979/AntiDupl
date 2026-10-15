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
    TEngine::TEngine(const TString & userPath)
        : _userPath(userPath)
    {
#ifdef AD_LOGGER_ENABLE
        TLogger::s_logger.SetFileOut((UserPath() + TEXT("\\debug_log.txt")).c_str(), true);
#endif//AD_LOGGER_ENABLE
        m_pInit = new TInit();
        m_pOptions = new TOptions(userPath);
        m_pStatus = new TStatus();
        m_pMistakeStorage = new TMistakeStorage(this);
        m_pImageDataStorage = new TImageDataStorage(this);
        m_pRecycleBin = new TRecycleBin(this);        
        m_pResult = new TResultStorage(this);
        m_pImageDataPtrs = new TImageDataPtrs();
        m_pScanCandidates = new std::vector<TScanCandidate>();
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
        delete m_pScanCandidates;
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

        if(m_pOptions->compare.checkOnEquality == TRUE)
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

        if(m_pOptions->compare.checkOnEquality == TRUE)
        {
            m_pStatus->SetProgress(current, total);
            m_pStatus->Wait(AD_THREAD_TYPE_MAIN, 0);
            m_pCompareManager->SetPriority(THREAD_PRIORITY_NORMAL);
            m_pCompareManager->Finish();
        }

        m_pImageDataPtrs->clear();
        m_pStatus->Reset();
    }

    adError TEngine::ScanCandidates(adCandidateInfoPtrW pCandidates, adSizePtr pCandidateSize)
    {
        // pCandidateSize обязателен всегда; pCandidates — только при ненулевой ёмкости.
        // Первый вызов двухшагового соглашения (size=0, запрос размера) разрешает NULL-буфер.
        if(pCandidateSize == NULL)
            return AD_ERROR_INVALID_POINTER;
        if(*pCandidateSize != 0 && pCandidates == NULL)
            return AD_ERROR_INVALID_POINTER;

        if(*pCandidateSize == 0)
        {
            // First call of the two-call pattern: run the scan-only pass
            // (TSearcher filters: hidden/system, ignore/valid paths,
            // ignoreFilenameFilter regex, per-format flags, subfolders)
            // and cache the result as plain path/size/time triples.
            m_pStatus->ClearStatistic();
            m_pStatus->SetProgress(0, 0);

            m_pImageDataPtrs->clear();
            m_pSearcher->SearchImages();

            m_pScanCandidates->clear();
            for(TImageDataPtrs::iterator it = m_pImageDataPtrs->begin(); 
                it != m_pImageDataPtrs->end(); ++it)
            {
                TImageDataPtr pImageData = *it;
                m_pScanCandidates->push_back(
                    TScanCandidate(pImageData->path.Original(), pImageData->size, pImageData->time));
            }

            // Release TImageData objects back to the storage pool: the plain
            // triple copy above is what the caller consumes, so a following
            // adSearch starts from an empty candidate list as usual.
            m_pImageDataPtrs->clear();
            m_pStatus->Reset();
        }

        if(m_pScanCandidates->size() > *pCandidateSize)
        {
            *pCandidateSize = m_pScanCandidates->size();
            return AD_ERROR_OUTPUT_BUFFER_IS_TOO_SMALL;
        }

        for(size_t i = 0; i < m_pScanCandidates->size(); i++)
        {
            const TScanCandidate &candidate = (*m_pScanCandidates)[i];
            if(candidate.path.length() >= MAX_PATH_EX)
                return AD_ERROR_PATH_TO_LONG;

            candidate.path.CopyTo(pCandidates[i].path, MAX_PATH_EX);
            pCandidates[i].size = candidate.size;
            pCandidates[i].time = candidate.time;
        }

        *pCandidateSize = m_pScanCandidates->size();

        return AD_OK;
    }
}
