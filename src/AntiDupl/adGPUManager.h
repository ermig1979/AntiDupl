/*
* AntiDuplPlus Program (http://github.com/Sucotasch/AntiDuplPlus).
*
* Copyright (c) 2023-2026.
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
#ifndef __adGPUManager_h__
#define __adGPUManager_h__

#include "adGPU.h"
#include <mutex>

namespace ad
{
    class TGpuManager
    {
    public:
        TGpuManager();
        ~TGpuManager();

        bool IsAvailable() const { return m_available; }
        const GpuDeviceInfo& DeviceInfo() const { return m_deviceInfo; }

        bool UploadThumbnail(size_t index, const uint8_t* pData) {
            if (!m_available) return false;
            std::lock_guard<std::mutex> lock(m_mutex);
            return GpuUploadThumbnail(index, pData);
        }

        bool CompareOneVsMany(const uint8_t* pQuery, size_t startIdx, size_t count, double threshold, 
                              size_t* pMatchIndices, double* pMatchDifferences, size_t* pMatchCount, size_t maxMatches) {
            if (!m_available) return false;
            std::lock_guard<std::mutex> lock(m_mutex);
            return GpuCompareOneVsMany(pQuery, startIdx, count, threshold, 
                                                    pMatchIndices, pMatchDifferences, pMatchCount, maxMatches);
        }

        bool CompareOneVsList(const uint8_t* pQuery, const size_t* pIndices, size_t count, double threshold, 
                              size_t* pMatchIndices, double* pMatchDifferences, size_t* pMatchCount, size_t maxMatches) {
            if (!m_available) return false;
            std::lock_guard<std::mutex> lock(m_mutex);
            return GpuCompareOneVsList(pQuery, pIndices, count, threshold, 
                                                    pMatchIndices, pMatchDifferences, pMatchCount, maxMatches);
        }

        void ClearBuffer() {
            if (!m_available) return;
            std::lock_guard<std::mutex> lock(m_mutex);
            size_t size = m_capacity; // Reuse current capacity
            GpuReleaseBuffer(); 
            GpuCreateBuffer(size, m_thumbSize);
        }

        bool EnsureCapacity(size_t required, size_t thumbSize);

        // NEW: AllVsAll comparison с streaming callback
        bool CompareAllVsAll(
            const uint8_t* allThumbnails,
            const uint64_t* allCrcArray,
            size_t count,
            size_t thumbSize,
            double threshold,
            double maxDifference,
            double addDiffForCrcMismatch,
            void* callbackContext,
            GpuMatchCallback callback,
            size_t maxMatchesPerBatch) {
            if (!m_available) return false;
            std::lock_guard<std::mutex> lock(m_mutex);
            return GpuCompareAllVsAll(allThumbnails, allCrcArray, count, thumbSize,
                                                    threshold, maxDifference, addDiffForCrcMismatch,
                                                    callbackContext, callback, maxMatchesPerBatch);
        }

    private:
        bool m_available;
        GpuDeviceInfo m_deviceInfo;
        size_t m_capacity;
        size_t m_thumbSize;
        mutable std::mutex m_mutex;
    };
}

#endif//__adGPUManager_h__
