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
#ifndef __adGPU_h__
#define __adGPU_h__

#include "adConfig.h"

namespace ad
{
    struct GpuDeviceInfo
    {
        char name[256];
        size_t totalGlobalMem;
        int computeMajor;
        int computeMinor;
        bool isCompatible;
    };

    // Match result structure
    struct Match {
        uint32_t image1;
        uint32_t image2;
        float difference;
    };

    bool GpuInit(GpuDeviceInfo* pInfo);
    void GpuRelease();

    // Persistent Buffer Management
    bool GpuCreateBuffer(size_t capacity, size_t thumbSize);
    void GpuReleaseBuffer();
    bool GpuUploadThumbnail(size_t index, const uint8_t* pData);

    // Batch Comparison: Compare one query thumbnail against a range of thumbnails in the buffer
    // Returns number of matches found (difference <= threshold)
    bool GpuCompareOneVsMany(const uint8_t* pQuery, size_t startIdx, size_t count, double threshold, 
                             size_t* pMatchIndices, double* pMatchDifferences, size_t* pMatchCount, size_t maxMatches);

    // Compare one query thumbnail against a list of thumbnail indices in the buffer
    bool GpuCompareOneVsList(const uint8_t* pQuery, const size_t* pIndices, size_t count, double threshold, 
                             size_t* pMatchIndices, double* pMatchDifferences, size_t* pMatchCount, size_t maxMatches);

    // Single comparison (optimized, no malloc inside)
    double GpuCompareSquaredSum(const uint8_t* pSrc1, const uint8_t* pSrc2, size_t size);

    // NEW: AllVsAll comparison с streaming callback
    typedef void (*GpuMatchCallback)(const void* batch, size_t count, void* context);

    bool GpuCompareAllVsAll(
        const uint8_t* allThumbnails,     // Все thumbnails в RAM
        const uint64_t* allCrcArray,      // CRC32c для каждого изображения
        size_t count,                      // Количество изображений
        size_t thumbSize,                  // Размер одного thumbnail (1024)
        double threshold,                  // Порог squared difference
        double maxDifference,              // Максимальная разница для нормализации
        double addDiffForCrcMismatch,      // Добавка за несовпадение CRC
        void* callbackContext,             // Контекст для callback
        GpuMatchCallback callback,         // Callback для streaming обработки
        size_t maxMatchesPerBatch);        // Максимум matches за один вызов
}

#endif//__adGPU_h__
