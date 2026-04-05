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
#include "adGPUManager.h"
#include "adStatus.h"
#include <windows.h>
#include <vector>
#include <sstream>

#define AD_DEBUG(msg) OutputDebugStringA(msg)
#define AD_DEBUG_FMT(msg, ...) \
    do { \
        char buf[512]; \
        snprintf(buf, sizeof(buf), msg, __VA_ARGS__); \
        OutputDebugStringA(buf); \
    } while(0)

namespace ad
{
    TEngine::TEngine(const TString & userPath)
        : _userPath(userPath)
    {
        AD_DEBUG("TEngine: Constructor starting\n");

#ifdef AD_LOGGER_ENABLE
        TLogger::s_logger.SetFileOut((UserPath() + TEXT("\\debug_log.txt")).c_str(), true);
#endif//AD_LOGGER_ENABLE

        AD_DEBUG("TEngine: Creating TInit\n");
        m_pInit = new TInit();

        AD_DEBUG("TEngine: Creating TOptions\n");
        m_pOptions = new TOptions(userPath);

        AD_DEBUG("TEngine: Creating TStatus\n");
        m_pStatus = new TStatus();

        AD_DEBUG("TEngine: Creating TGpuManager\n");
        m_pGpuManager = new TGpuManager();

        AD_DEBUG("TEngine: TGpuManager created, IsAvailable=1\n");

        if (m_pGpuManager->IsAvailable())
        {
            AD_DEBUG("TEngine: GPU is available, getting device info\n");

            const GpuDeviceInfo& info = m_pGpuManager->DeviceInfo();
            std::stringstream ss;
            ss << "GPU acceleration initialized: " << info.name
               << " (" << (info.totalGlobalMem / (1024 * 1024)) << " MB VRAM, Compute "
               << info.computeMajor << "." << info.computeMinor << ")";
#ifdef AD_LOGGER_ENABLE
            AD_LOG(ss.str().c_str());
#endif//AD_LOGGER_ENABLE

            // GPU Sanity Check: Test mathematical parity
            AD_DEBUG("TEngine: Starting GPU sanity check\n");

            const size_t testSize = 1024;
            uint8_t h_test1[testSize], h_test2[testSize];
            double cpuSum = 0;
            for(size_t i = 0; i < testSize; ++i) {
                h_test1[i] = (uint8_t)(i % 256);
                h_test2[i] = (uint8_t)(255 - (i % 256));
                double diff = (double)h_test1[i] - (double)h_test2[i];
                cpuSum += diff * diff;
            }

            AD_DEBUG("TEngine: Calling GpuCompareSquaredSum\n");

            double gpuSum = GpuCompareSquaredSum(h_test1, h_test2, testSize);

            AD_DEBUG("TEngine: GpuCompareSquaredSum returned\n");

            std::stringstream ts;
            ts << "CUDA Sanity Check: CPU=" << cpuSum << ", GPU=" << gpuSum;
            double tolerance = cpuSum * 0.001;  // 0.1% relative tolerance
            if (fabs(cpuSum - gpuSum) <= tolerance) {
                ts << " [SUCCESS - PARITY MATCH]";
            } else {
                ts << " [FAILURE - MATH MISMATCH] Tolerance: " << tolerance;
            }
#ifdef AD_LOGGER_ENABLE
            AD_LOG(ts.str().c_str());
#endif//AD_LOGGER_ENABLE
        }
        else
        {
            AD_DEBUG("TEngine: GPU not available\n");
#ifdef AD_LOGGER_ENABLE
            AD_LOG("GPU acceleration not available.");
#endif//AD_LOGGER_ENABLE
        }

        AD_DEBUG("TEngine: Creating storage objects\n");

        m_pMistakeStorage = new TMistakeStorage(this);
        m_pImageDataStorage = new TImageDataStorage(this);
        m_pRecycleBin = new TRecycleBin(this);
        m_pResult = new TResultStorage(this);
        m_pImageDataPtrs = new TImageDataPtrs();
        m_pCriticalSection = new TCriticalSection();
        m_pCompareManager = new TCompareManager(this);
        m_pCollectManager = new TCollectManager(this, m_pCompareManager);
        m_pSearcher = new TSearcher(this, m_pImageDataPtrs);
        m_skipComparisonDuringCollection = false;

        AD_DEBUG("TEngine: Constructor finished successfully\n");
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
        delete m_pGpuManager;
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

    void TEngine::UpdateGpuDatabase()
    {
        AD_DEBUG("UpdateGpuDatabase: Starting\n");

        if (m_pGpuManager && m_pGpuManager->IsAvailable())
        {
            AD_DEBUG("UpdateGpuDatabase: GPU is available\n");

            const TImageDataStorage::TStorage& storage = m_pImageDataStorage->Storage();
            AD_DEBUG("UpdateGpuDatabase: Storage size\n");

            size_t reducedImageSize = m_pOptions->advanced.reducedImageSize;
            size_t thumbSize = reducedImageSize * reducedImageSize;
            AD_DEBUG("UpdateGpuDatabase: reducedImageSize and thumbSize calculated\n");

            // Ensure GPU has enough capacity for the current database
            AD_DEBUG("UpdateGpuDatabase: Calling EnsureCapacity\n");

            if (!m_pGpuManager->EnsureCapacity(storage.size(), thumbSize))
            {
                AD_DEBUG("UpdateGpuDatabase: EnsureCapacity FAILED\n");
#ifdef AD_LOGGER_ENABLE
                AD_LOG("GPU: Failed to ensure capacity for database.");
#endif
                return;
            }

            AD_DEBUG("UpdateGpuDatabase: EnsureCapacity succeeded\n");

            size_t count = 0;
            for (TImageDataStorage::TStorage::const_iterator it = storage.begin(); it != storage.end(); ++it)
            {
                TImageDataPtr pImageData = it->second;
                if (pImageData->data && pImageData->data->filled && pImageData->data->main != nullptr)
                {
                    if (m_pGpuManager->UploadThumbnail(pImageData->globalIdx, pImageData->data->main))
                    {
                        count++;
                    }
                    else
                    {
                        AD_DEBUG("UpdateGpuDatabase: Upload FAILED\n");
                    }
                }
            }
            AD_DEBUG("UpdateGpuDatabase: Uploaded thumbnails\n");

#ifdef AD_LOGGER_ENABLE
            if (count > 0)
            {
                std::stringstream ss;
                ss << "GPU: Synchronized " << count << " thumbnails to VRAM.";
                AD_LOG(ss.str().c_str());
            }
#endif
        }
        else
        {
            AD_DEBUG("UpdateGpuDatabase: GPU not available\n");
        }
        AD_DEBUG("UpdateGpuDatabase: Finished\n");
    }

    // NEW: GPU AllVsAll comparison
    void TEngine::ExecuteGpuAllVsAllComparison()
    {
        AD_DEBUG("ExecuteGpuAllVsAllComparison: Starting\n");

        if (!m_pGpuManager || !m_pGpuManager->IsAvailable()) {
            AD_DEBUG("ExecuteGpuAllVsAllComparison: GPU not available\n");
            return;
        }

        const TImageDataStorage::TStorage& storage = m_pImageDataStorage->Storage();
        size_t count = storage.size();
        if (count == 0) {
            AD_DEBUG("ExecuteGpuAllVsAllComparison: Empty storage\n");
            return;
        }

        size_t reducedImageSize = m_pOptions->advanced.reducedImageSize;
        size_t thumbSize = reducedImageSize * reducedImageSize;

        AD_DEBUG_FMT("ExecuteGpuAllVsAllComparison: Preparing data for %zu images\n", count);

        // Собираем все thumbnails в один массив
        std::vector<uint8_t> allThumbnails(count * thumbSize);
        size_t validCount = 0;

        size_t idx = 0;
        for (TImageDataStorage::TStorage::const_iterator it = storage.begin(); it != storage.end(); ++it, ++idx) {
            TImageDataPtr pImageData = it->second;
            if (pImageData->data && pImageData->data->filled && pImageData->data->main != nullptr) {
                memcpy(&allThumbnails[idx * thumbSize], pImageData->data->main, thumbSize);
                validCount++;
            }
        }

        AD_DEBUG_FMT("ExecuteGpuAllVsAllComparison: %zu valid thumbnails\n", validCount);

        // Создаём vector для O(1) доступа по индексу (вместо O(N) std::advance)
        std::vector<TImageDataPtr> imageByIndex(count);
        idx = 0;
        for (TImageDataStorage::TStorage::const_iterator it = storage.begin(); it != storage.end(); ++it, ++idx) {
            TImageDataPtr pImageData = it->second;
            if (pImageData->data && pImageData->data->filled && pImageData->data->main != nullptr) {
                imageByIndex[idx] = pImageData;
            }
        }

        // Вычисляем threshold как в оригинальном TImageComparer
        int thresholdPerPixel = Simd::Square(m_pOptions->compare.thresholdDifference * PIXEL_MAX_DIFFERENCE) /
            Simd::Square(DENOMINATOR);
        int mainThreshold = (int)(thumbSize * thresholdPerPixel);
        double threshold = (double)mainThreshold;

        AD_DEBUG_FMT("ExecuteGpuAllVsAllComparison: thresholdPerPixel=%d, mainThreshold=%d, threshold=%f\n",
                     thresholdPerPixel, mainThreshold, threshold);

        // Выделяем память для результатов (предполагаем ~5% дубликатов)
        size_t maxMatches = count * (count - 1) / 2;
        if (maxMatches > 10000000) maxMatches = 10000000; // Ограничиваем 10M

        std::vector<uint32_t> outImage1(maxMatches);
        std::vector<uint32_t> outImage2(maxMatches);
        std::vector<float> outDifference(maxMatches);
        size_t matchCount = 0;

        AD_DEBUG("ExecuteGpuAllVsAllComparison: Calling GPU\n");

        if (m_pGpuManager->CompareAllVsAll(
                allThumbnails.data(), count, thumbSize, threshold,
                outImage1.data(), outImage2.data(), outDifference.data(),
                &matchCount, maxMatches))
        {
            AD_DEBUG_FMT("ExecuteGpuAllVsAllComparison: GPU returned %zu matches\n", matchCount);

            // Обрабатываем результаты — O(1) доступ вместо O(N) std::advance
            for (size_t i = 0; i < matchCount; i++) {
                TImageDataPtr pImage1 = imageByIndex[outImage1[i]];
                TImageDataPtr pImage2 = imageByIndex[outImage2[i]];

                double maxDifference = (double)(Simd::Square(PIXEL_MAX_DIFFERENCE) * thumbSize);
                double difference = sqrt((double)outDifference[i] / maxDifference) * 100;
                if (pImage1->crc32c != pImage2->crc32c)
                    difference += ADDITIONAL_DIFFERENCE_FOR_DIFFERENT_CRC32;

                m_pResult->AddDuplImagePair(pImage1, pImage2, difference, AD_TRANSFORM_TURN_0);
            }

            AD_DEBUG("ExecuteGpuAllVsAllComparison: Results processed\n");
        }
        else {
            AD_DEBUG("ExecuteGpuAllVsAllComparison: GPU comparison FAILED\n");
        }

        AD_DEBUG("ExecuteGpuAllVsAllComparison: Finished\n");
    }

    void TEngine::Search()
    {
        AD_DEBUG("Search: Starting\n");

        AD_FUNCTION_PERFORMANCE_TEST
        m_pStatus->ClearStatistic();
        m_pStatus->SetProgress(0, 0);
        m_pResult->Clear();

        // 1. First, search for images on disk or load from DB
        AD_DEBUG("Search: Calling SearchImages\n");
        m_pSearcher->SearchImages();
        AD_DEBUG("Search: SearchImages completed\n");

        // 2. Start collection threads
        AD_DEBUG("Search: Starting collection manager\n");
        m_pCollectManager->Start();
        m_pCollectManager->SetPriority(THREAD_PRIORITY_BELOW_NORMAL);

        // 3. GPU AllVsAll comparison (если включено и доступно)
        bool useGpu = (m_pGpuManager && m_pGpuManager->IsAvailable() &&
                       m_pOptions->compare.algorithmComparing == AD_COMPARING_SQUARED_SUM &&
                       m_pOptions->advanced.ignoreFrameWidth == 0);

        if (useGpu)
        {
            m_skipComparisonDuringCollection = true;  // Отключаем старое сравнение ДО цикла
        }
        else
        {
            m_skipComparisonDuringCollection = false;
            // 4. CPU comparison (старый подход) - нужно запустить CompareManager ДО сбора данных
            AD_DEBUG("Search: Starting CPU comparison\n");

            if(m_pOptions->compare.checkOnEquality == TRUE)
            {
                AD_DEBUG("Search: Starting compare manager\n");
                m_pCompareManager->Start(m_pImageDataPtrs->size());
                m_pCompareManager->SetPriority(THREAD_PRIORITY_NORMAL);
                AD_DEBUG("Search: Compare manager started\n");
            }
        }

        size_t current = 0, total = m_pImageDataPtrs->size();
        AD_DEBUG("Search: Total images to process\n");

        for(TImageDataPtrs::iterator it = m_pImageDataPtrs->begin();
            it != m_pImageDataPtrs->end() && !m_pStatus->Stopped(); ++it, ++current)
        {
            TImageDataPtr pImageData = *it;
            m_pCollectManager->Add(pImageData);
            m_pStatus->SetProgress(current, total);
        }
        AD_DEBUG("Search: Collection loop finished\n");

        m_pCollectManager->Finish();
        AD_DEBUG("Search: Collection manager finished\n");

        if (useGpu)
        {
            AD_DEBUG("Search: Using GPU AllVsAll comparison\n");
            ExecuteGpuAllVsAllComparison();
            m_skipComparisonDuringCollection = false;
            AD_DEBUG("Search: GPU comparison completed\n");
        }
        else
        {
            if(m_pOptions->compare.checkOnEquality == TRUE)
            {
                AD_DEBUG("Search: Waiting for compare manager to finish\n");
                m_pCompareManager->Finish();
                AD_DEBUG("Search: Compare manager finished\n");
            }
        }

        m_pImageDataPtrs->clear();
        m_pStatus->Reset();

        AD_DEBUG("Search: Completed successfully\n");
    }
}
