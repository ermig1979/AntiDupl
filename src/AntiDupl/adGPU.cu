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
#include "adGPU.h"
#include "adLogger.h"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <windows.h>

#define AD_DEBUG(msg) OutputDebugStringA(msg)
#define AD_DEBUG_FMT(msg, ...) \
    do { \
        char buf[512]; \
        snprintf(buf, sizeof(buf), msg, __VA_ARGS__); \
        OutputDebugStringA(buf); \
    } while(0)

namespace ad
{
    // Global VRAM Storage
    static uint8_t* g_pDeviceThumbnailBuffer = nullptr;
    static size_t g_bufferCapacity = 0;
    static size_t g_thumbSize = 1024; // Default 32x32
    static uint8_t* g_pQueryBuffer = nullptr; 
    static double* g_pResultBuffer = nullptr; 
    static size_t* g_pIndexBuffer = nullptr; 

    // --- Kernels ---

    __global__ void SquaredSumKernel(const uint8_t* pSrc1, const uint8_t* pSrc2, size_t size, double* pResult)
    {
        extern __shared__ double shared_data[];
        size_t tid = threadIdx.x;
        size_t i = (size_t)blockIdx.x * blockDim.x + threadIdx.x;
        double sum = 0;
        if (i < size) {
            double diff = (double)pSrc1[i] - (double)pSrc2[i];
            sum = diff * diff;
        }
        shared_data[tid] = sum;
        __syncthreads();
        for (size_t s = blockDim.x / 2; s > 0; s >>= 1) {
            if (tid < s) shared_data[tid] += shared_data[tid + s];
            __syncthreads();
        }
        if (tid == 0) atomicAdd(pResult, shared_data[0]);
    }

    // NEW: AllVsAll kernel — grid-stride loop + shared memory + CRC check + final difference
    // Каждый блок обрабатывает несколько строк i с шагом gridDim.x
    __global__ void AllVsAllKernel(
        const uint8_t* thumbnails,     // Все thumbnails в VRAM
        const uint64_t* crcArray,      // CRC32c для каждого изображения
        size_t thumbSize,              // Размер одного thumbnail (1024)
        size_t count,                  // Общее количество изображений
        double threshold,              // Порог squared difference
        double maxDifference,          // Максимальная разница для нормализации
        double addDiffForCrcMismatch,  // Добавка за несовпадение CRC
        Match* results,                // Sparse buffer для результатов
        size_t* matchCount)            // Atomic counter
    {
        // Shared memory для thumb1 — ускоряет чтение в 10-100 раз
        extern __shared__ uint8_t shared_thumb[];

        // Grid-stride loop: каждый блок обрабатывает несколько строк i
        for (size_t i = blockIdx.x; i < count; i += gridDim.x) {
            const uint8_t* thumb1_global = thumbnails + i * thumbSize;

            // Загружаем thumb1 в shared memory кооперативно
            for (size_t p = threadIdx.x; p < thumbSize; p += blockDim.x) {
                shared_thumb[p] = thumb1_global[p];
            }
            __syncthreads();

            // Каждый поток обрабатывает несколько j > i с stride
            size_t numThreads = blockDim.x;

            for (size_t j = i + 1 + threadIdx.x; j < count; j += numThreads) {
                const uint8_t* thumb2 = thumbnails + j * thumbSize;

                // Вычисляем squared difference из shared memory
                double sumSqDiff = 0;
                for (size_t p = 0; p < thumbSize; p++) {
                    double diff = (double)shared_thumb[p] - (double)thumb2[p];
                    sumSqDiff += diff * diff;
                }

                // Проверяем threshold ДО нормализации (как в CPU версии)
                if (sumSqDiff <= threshold) {
                    // Вычисляем финальный difference в процентах
                    double difference = sqrt(sumSqDiff / maxDifference) * 100.0;

                    // Добавляем штраф за несовпадение CRC
                    if (crcArray[i] != crcArray[j]) {
                        difference += addDiffForCrcMismatch;
                    }

                    size_t idx = atomicAdd(matchCount, (size_t)1);
                    results[idx].image1 = (uint32_t)i;
                    results[idx].image2 = (uint32_t)j;
                    results[idx].difference = (float)difference;
                }
            }
            __syncthreads();  // Синхронизация перед следующей итерацией i
        }
    }

    __global__ void OneVsManyKernel(const uint8_t* pQuery, const uint8_t* pDatabase, 
                                    size_t thumbSize, size_t count, double* pResults)
    {
        size_t dbIdx = blockIdx.x; 
        if (dbIdx >= count) return;

        extern __shared__ double shared_sum[];
        size_t tid = threadIdx.x;
        
        double localSum = 0;
        const uint8_t* pTarget = pDatabase + (size_t)dbIdx * thumbSize;

        for (size_t i = tid; i < thumbSize; i += blockDim.x) {
            double diff = (double)pQuery[i] - (double)pTarget[i];
            localSum += diff * diff;
        }

        shared_sum[tid] = localSum;
        __syncthreads();

        for (size_t s = blockDim.x / 2; s > 0; s >>= 1) {
            if (tid < s) shared_sum[tid] += shared_sum[tid + s];
            __syncthreads();
        }

        if (tid == 0) {
            pResults[dbIdx] = shared_sum[0];
        }
    }

    __global__ void OneVsListKernel(const uint8_t* pQuery, const uint8_t* pDatabase, 
                                    const size_t* pIndices, size_t thumbSize, size_t count, 
                                    double* pResults, size_t maxBufferCapacity)
    {
        size_t listIdx = blockIdx.x; 
        if (listIdx >= count) return;

        extern __shared__ double shared_sum_list[];
        size_t tid = threadIdx.x;
        
        double localSum = 0;
        size_t dbIdx = pIndices[listIdx];
        
        // Check bounds to prevent out-of-bounds access
        if (dbIdx >= maxBufferCapacity) {
            if (tid == 0) {
                pResults[listIdx] = 1e100;  // Maximum difference (no match)
            }
            return;
        }
        
        const uint8_t* pTarget = pDatabase + (size_t)dbIdx * thumbSize;

        for (size_t i = tid; i < thumbSize; i += blockDim.x) {
            double diff = (double)pQuery[i] - (double)pTarget[i];
            localSum += diff * diff;
        }

        shared_sum_list[tid] = localSum;
        __syncthreads();

        for (size_t s = blockDim.x / 2; s > 0; s >>= 1) {
            if (tid < s) shared_sum_list[tid] += shared_sum_list[tid + s];
            __syncthreads();
        }

        if (tid == 0) {
            pResults[listIdx] = shared_sum_list[0];
        }
    }

    // --- Implementation ---

    bool GpuInit(GpuDeviceInfo* pInfo)
    {
        AD_DEBUG("GpuInit: Starting\n");
        
        int deviceCount = 0;
        cudaError_t err = cudaGetDeviceCount(&deviceCount);
        if (err != cudaSuccess || deviceCount == 0) {
            AD_DEBUG("GpuInit: cudaGetDeviceCount failed\n");
            return false;
        }

        AD_DEBUG("GpuInit: Got device count\n");

        cudaDeviceProp prop;
        err = cudaGetDeviceProperties(&prop, 0);
        if (err != cudaSuccess) {
            AD_DEBUG("GpuInit: cudaGetDeviceProperties failed\n");
            return false;
        }

        AD_DEBUG("GpuInit: Got device properties\n");

        if (pInfo) {
            strncpy(pInfo->name, prop.name, 256);
            pInfo->totalGlobalMem = prop.totalGlobalMem;
            pInfo->computeMajor = prop.major;
            pInfo->computeMinor = prop.minor;
            pInfo->isCompatible = (prop.major >= 8);
        }
        
        AD_DEBUG("GpuInit: successful\n");
        return true;
    }

    bool GpuCreateBuffer(size_t capacity, size_t thumbSize)
    {
        GpuReleaseBuffer();
        if (capacity == 0 || thumbSize == 0) return true;

        // Use temporary variable for validation before setting global state
        size_t testThumbSize = thumbSize;

        size_t freeMem = 0, totalMem = 0;
        cudaMemGetInfo(&freeMem, &totalMem);
        
        size_t requiredMem = capacity * testThumbSize + capacity * sizeof(double) + capacity * sizeof(size_t) + testThumbSize;
        
        if (requiredMem > (size_t)(freeMem * 0.8)) 
        {
#ifdef AD_LOGGER_ENABLE
            std::stringstream ss;
            ss << "GPU: Not enough free VRAM! Required: " << (requiredMem / 1024 / 1024)
               << " MB, Free: " << (freeMem / 1024 / 1024) << " MB.";
            AD_LOG(ss.str().c_str());
#endif
            return false;
        }

        // Only set g_thumbSize after memory check passes
        g_thumbSize = testThumbSize;

        cudaError_t err;
        err = cudaMalloc(&g_pDeviceThumbnailBuffer, capacity * g_thumbSize);
        if (err != cudaSuccess) goto error;

        err = cudaMalloc(&g_pQueryBuffer, g_thumbSize);
        if (err != cudaSuccess) goto error;

        err = cudaMalloc(&g_pResultBuffer, capacity * sizeof(double));
        if (err != cudaSuccess) goto error;

        err = cudaMalloc(&g_pIndexBuffer, capacity * sizeof(size_t));
        if (err != cudaSuccess) goto error;

        g_bufferCapacity = capacity;
#ifdef AD_LOGGER_ENABLE
        {
            std::stringstream ss;
            ss << "GPU: VRAM Allocated. Capacity: " << capacity << " units. Thumbnail Size: " << g_thumbSize << " bytes. Required: " << (requiredMem / 1024 / 1024) << " MB. Free VRAM: " << (freeMem / 1024 / 1024) << " MB.";
            AD_LOG(ss.str().c_str());
        }
#endif
        return true;

    error:
        GpuReleaseBuffer();
        return false;
    }

    void GpuReleaseBuffer()
    {
        if (g_pDeviceThumbnailBuffer) cudaFree(g_pDeviceThumbnailBuffer);
        if (g_pQueryBuffer) cudaFree(g_pQueryBuffer);
        if (g_pResultBuffer) cudaFree(g_pResultBuffer);
        if (g_pIndexBuffer) cudaFree(g_pIndexBuffer);
        g_pDeviceThumbnailBuffer = nullptr;
        g_pQueryBuffer = nullptr;
        g_pResultBuffer = nullptr;
        g_pIndexBuffer = nullptr;
        g_bufferCapacity = 0;
    }

    void GpuRelease()
    {
        GpuReleaseBuffer();
        cudaDeviceReset();
    }

    bool GpuUploadThumbnail(size_t index, const uint8_t* pData)
    {
        fprintf(stderr, "GpuUploadThumbnail: index=%zu, g_pDeviceThumbnailBuffer=%p, g_bufferCapacity=%zu\n",
                index, (void*)g_pDeviceThumbnailBuffer, g_bufferCapacity);
        fflush(stderr);

        if (!g_pDeviceThumbnailBuffer) {
            fprintf(stderr, "GpuUploadThumbnail: FAILED - buffer not allocated\n");
            fflush(stderr);
            return false;
        }

        if (index >= g_bufferCapacity) {
            fprintf(stderr, "GpuUploadThumbnail: FAILED - index %zu exceeds capacity %zu\n",
                    index, g_bufferCapacity);
            fflush(stderr);
            return false;
        }

        if (pData == nullptr) {
            fprintf(stderr, "GpuUploadThumbnail: FAILED - null data pointer\n");
            fflush(stderr);
            return false;
        }

        cudaError_t err = cudaMemcpy(g_pDeviceThumbnailBuffer + (size_t)index * g_thumbSize,
                                     pData, g_thumbSize, cudaMemcpyHostToDevice);
        if (err != cudaSuccess) {
            fprintf(stderr, "GpuUploadThumbnail: FAILED - cudaMemcpy error: %s\n",
                    cudaGetErrorString(err));
            fflush(stderr);
            return false;
        }

        fprintf(stderr, "GpuUploadThumbnail: Success\n");
        fflush(stderr);
        return true;
    }

    bool GpuCompareOneVsMany(const uint8_t* pQuery, size_t startIdx, size_t count, double threshold, 
                             size_t* pMatchIndices, double* pMatchDifferences, size_t* pMatchCount, size_t maxMatches)
    {
        if (!g_pDeviceThumbnailBuffer || (startIdx + count) > g_bufferCapacity || count == 0 || 
            pQuery == nullptr || pMatchIndices == nullptr || pMatchDifferences == nullptr || 
            pMatchCount == nullptr || maxMatches == 0) 
            return false;

        if (cudaMemcpy(g_pQueryBuffer, pQuery, g_thumbSize, cudaMemcpyHostToDevice) != cudaSuccess) return false;
        
        // Check for integer overflow before kernel launch
        if (count > INT_MAX) {
#ifdef AD_LOGGER_ENABLE
            AD_LOG("GPU: Count exceeds INT_MAX, cannot launch kernel");
#endif
            return false;
        }
        
        int threadsPerBlock = 256; 
        OneVsManyKernel<<< (int)count, threadsPerBlock, threadsPerBlock * sizeof(double) >>>(
            g_pQueryBuffer, g_pDeviceThumbnailBuffer + (size_t)startIdx * g_thumbSize, g_thumbSize, count, g_pResultBuffer);

        if (cudaGetLastError() != cudaSuccess) return false;
        if (cudaDeviceSynchronize() != cudaSuccess) return false;

        std::vector<double> results(count);
        if (cudaMemcpy(results.data(), g_pResultBuffer, count * sizeof(double), cudaMemcpyDeviceToHost) != cudaSuccess) return false;

        size_t found = 0;
        for (size_t i = 0; i < count && found < maxMatches; ++i) {
            if (results[i] <= threshold) {
                pMatchIndices[found] = startIdx + i;
                pMatchDifferences[found] = results[i];
                found++;
            }
        }
        *pMatchCount = found;
        return true;
    }

    bool GpuCompareOneVsList(const uint8_t* pQuery, const size_t* pIndices, size_t count, double threshold, 
                             size_t* pMatchIndices, double* pMatchDifferences, size_t* pMatchCount, size_t maxMatches)
    {
        fprintf(stderr, "GpuCompareOneVsList: Starting, count=%zu, threshold=%f\n", count, threshold);
        fflush(stderr);
        
        if (!g_pDeviceThumbnailBuffer || count > g_bufferCapacity || count == 0 || 
            pQuery == nullptr || pIndices == nullptr || pMatchIndices == nullptr || 
            pMatchDifferences == nullptr || pMatchCount == nullptr || maxMatches == 0) {
            fprintf(stderr, "GpuCompareOneVsList: Parameter validation FAILED\n");
            fflush(stderr);
            return false;
        }

        fprintf(stderr, "GpuCompareOneVsList: Copying query to device\n");
        fflush(stderr);
        if (cudaMemcpy(g_pQueryBuffer, pQuery, g_thumbSize, cudaMemcpyHostToDevice) != cudaSuccess) {
            fprintf(stderr, "GpuCompareOneVsList: Query copy FAILED\n");
            fflush(stderr);
            return false;
        }
        
        fprintf(stderr, "GpuCompareOneVsList: Copying indices to device\n");
        fflush(stderr);
        if (cudaMemcpy(g_pIndexBuffer, pIndices, count * sizeof(size_t), cudaMemcpyHostToDevice) != cudaSuccess) {
            fprintf(stderr, "GpuCompareOneVsList: Indices copy FAILED\n");
            fflush(stderr);
            return false;
        }
        
        // Check for integer overflow before kernel launch
        if (count > INT_MAX) {
            fprintf(stderr, "GpuCompareOneVsList: Count exceeds INT_MAX\n");
            fflush(stderr);
#ifdef AD_LOGGER_ENABLE
            AD_LOG("GPU: Count exceeds INT_MAX, cannot launch kernel");
#endif
            return false;
        }
        
        fprintf(stderr, "GpuCompareOneVsList: Launching kernel with count=%zu\n", count);
        fflush(stderr);
        
        int threadsPerBlock = 256; 
        OneVsListKernel<<< (int)count, threadsPerBlock, threadsPerBlock * sizeof(double) >>>(
            g_pQueryBuffer, g_pDeviceThumbnailBuffer, g_pIndexBuffer, g_thumbSize, count, g_pResultBuffer, g_bufferCapacity);

        fprintf(stderr, "GpuCompareOneVsList: Kernel launched, checking for errors\n");
        fflush(stderr);
        
        cudaError_t err = cudaGetLastError();
        if (err != cudaSuccess) {
            fprintf(stderr, "GpuCompareOneVsList: Kernel launch error: %s\n", cudaGetErrorString(err));
            fflush(stderr);
            return false;
        }
        
        fprintf(stderr, "GpuCompareOneVsList: Synchronizing device\n");
        fflush(stderr);
        err = cudaDeviceSynchronize();
        if (err != cudaSuccess) {
            fprintf(stderr, "GpuCompareOneVsList: Device sync error: %s\n", cudaGetErrorString(err));
            fflush(stderr);
            return false;
        }

        fprintf(stderr, "GpuCompareOneVsList: Copying results from device\n");
        fflush(stderr);
        
        std::vector<double> results(count);
        if (cudaMemcpy(results.data(), g_pResultBuffer, count * sizeof(double), cudaMemcpyDeviceToHost) != cudaSuccess) {
            fprintf(stderr, "GpuCompareOneVsList: Results copy FAILED\n");
            fflush(stderr);
            return false;
        }

        fprintf(stderr, "GpuCompareOneVsList: Processing results\n");
        fflush(stderr);
        
        size_t found = 0;
        for (size_t i = 0; i < count && found < maxMatches; ++i) {
            if (results[i] <= threshold) {
                pMatchIndices[found] = pIndices[i];
                pMatchDifferences[found] = results[i];
                found++;
            }
        }
        *pMatchCount = found;
        
        fprintf(stderr, "GpuCompareOneVsList: Completed, found=%zu matches\n", found);
        fflush(stderr);
        
        return true;
    }

    double GpuCompareSquaredSum(const uint8_t* pSrc1, const uint8_t* pSrc2, size_t size)
    {
        fprintf(stderr, "GpuCompareSquaredSum: Starting, size=%zu\n", size);
        fflush(stderr);

        if (pSrc1 == nullptr || pSrc2 == nullptr) {
            fprintf(stderr, "GpuCompareSquaredSum: Null input pointers\n");
            fflush(stderr);
            return 1e10;
        }

        // Check CUDA device availability
        int deviceCount = 0;
        cudaError_t testErr = cudaGetDeviceCount(&deviceCount);
        if (testErr != cudaSuccess || deviceCount == 0) {
            fprintf(stderr, "GpuCompareSquaredSum: No CUDA devices available\n");
            fflush(stderr);
            return 1e10;
        }

        uint8_t *d_1 = nullptr, *d_2 = nullptr;
        double *d_r = nullptr, h_r = 0;
        size_t numBlocks = 0;

        fprintf(stderr, "GpuCompareSquaredSum: Allocating device memory\n");
        fflush(stderr);

        cudaError_t err;
        err = cudaMalloc(&d_1, size);
        if (err != cudaSuccess) {
            fprintf(stderr, "GpuCompareSquaredSum: cudaMalloc d_1 failed: %s\n", cudaGetErrorString(err));
            fflush(stderr);
            goto cleanup;
        }

        err = cudaMalloc(&d_2, size);
        if (err != cudaSuccess) {
            fprintf(stderr, "GpuCompareSquaredSum: cudaMalloc d_2 failed: %s\n", cudaGetErrorString(err));
            fflush(stderr);
            goto cleanup;
        }

        err = cudaMalloc(&d_r, sizeof(double));
        if (err != cudaSuccess) {
            fprintf(stderr, "GpuCompareSquaredSum: cudaMalloc d_r failed: %s\n", cudaGetErrorString(err));
            fflush(stderr);
            goto cleanup;
        }

        fprintf(stderr, "GpuCompareSquaredSum: Copying data to device\n");
        fflush(stderr);

        if (cudaMemcpy(d_1, pSrc1, size, cudaMemcpyHostToDevice) != cudaSuccess) {
            fprintf(stderr, "GpuCompareSquaredSum: cudaMemcpy d_1 failed\n");
            fflush(stderr);
            goto cleanup;
        }
        if (cudaMemcpy(d_2, pSrc2, size, cudaMemcpyHostToDevice) != cudaSuccess) {
            fprintf(stderr, "GpuCompareSquaredSum: cudaMemcpy d_2 failed\n");
            fflush(stderr);
            goto cleanup;
        }
        if (cudaMemset(d_r, 0, sizeof(double)) != cudaSuccess) {
            fprintf(stderr, "GpuCompareSquaredSum: cudaMemset d_r failed\n");
            fflush(stderr);
            goto cleanup;
        }

        // Check for integer overflow before kernel launch
        numBlocks = (size + 255) / 256;
        if (numBlocks > INT_MAX) {
            fprintf(stderr, "GpuCompareSquaredSum: Block count exceeds INT_MAX\n");
            fflush(stderr);
#ifdef AD_LOGGER_ENABLE
            AD_LOG("GPU: Block count exceeds INT_MAX, cannot launch kernel");
#endif
            goto cleanup;
        }

        fprintf(stderr, "GpuCompareSquaredSum: Launching kernel with %zu blocks\n", numBlocks);
        fflush(stderr);

        SquaredSumKernel<<< (int)numBlocks, 256, 256 * sizeof(double) >>>(d_1, d_2, size, d_r);
        
        err = cudaGetLastError();
        if (err != cudaSuccess) {
            fprintf(stderr, "GpuCompareSquaredSum: Kernel launch failed: %s\n", cudaGetErrorString(err));
            fflush(stderr);
            goto cleanup;
        }

        err = cudaDeviceSynchronize();
        if (err != cudaSuccess) {
            fprintf(stderr, "GpuCompareSquaredSum: Device synchronize failed: %s\n", cudaGetErrorString(err));
            fflush(stderr);
            goto cleanup;
        }

        fprintf(stderr, "GpuCompareSquaredSum: Copying result from device\n");
        fflush(stderr);

        err = cudaMemcpy(&h_r, d_r, sizeof(double), cudaMemcpyDeviceToHost);
        if (err != cudaSuccess) {
            fprintf(stderr, "GpuCompareSquaredSum: cudaMemcpy result failed: %s\n", cudaGetErrorString(err));
            fflush(stderr);
            goto cleanup;
        }

        fprintf(stderr, "GpuCompareSquaredSum: Success, result=%f\n", h_r);
        fflush(stderr);

    cleanup:
        if (d_1) cudaFree(d_1);
        if (d_2) cudaFree(d_2);
        if (d_r) cudaFree(d_r);
        return h_r;
    }

    // NEW: AllVsAll comparison с массовым upload + streaming callback
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
        size_t maxMatchesPerBatch)         // Максимум matches за один вызов
    {
        AD_DEBUG("GpuCompareAllVsAll: Starting\n");

        if (!allThumbnails || !allCrcArray || count == 0 || thumbSize == 0 || !callback) {
            AD_DEBUG("GpuCompareAllVsAll: Invalid parameters\n");
            return false;
        }

        size_t totalPairs = count * (count - 1) / 2;
        AD_DEBUG_FMT("GpuCompareAllVsAll: Comparing %zu images, %zu pairs\n", count, totalPairs);

        // Выделяем VRAM для thumbnails
        uint8_t* d_thumbnails = nullptr;
        uint64_t* d_crcArray = nullptr;
        Match* d_results = nullptr;
        size_t* d_matchCount = nullptr;

        cudaError_t err;

        // 1. Выделяем память для thumbnails
        AD_DEBUG("GpuCompareAllVsAll: Allocating VRAM for thumbnails\n");
        err = cudaMalloc(&d_thumbnails, count * thumbSize);
        if (err != cudaSuccess) {
            AD_DEBUG_FMT("GpuCompareAllVsAll: Failed to allocate thumbnails VRAM: %s\n", cudaGetErrorString(err));
            return false;
        }

        // 2. Выделяем память для CRC массива
        AD_DEBUG("GpuCompareAllVsAll: Allocating VRAM for CRC array\n");
        err = cudaMalloc(&d_crcArray, count * sizeof(uint64_t));
        if (err != cudaSuccess) {
            AD_DEBUG_FMT("GpuCompareAllVsAll: Failed to allocate CRC VRAM: %s\n", cudaGetErrorString(err));
            cudaFree(d_thumbnails);
            return false;
        }

        // 3. Выделяем память для результатов (batch buffer)
        AD_DEBUG("GpuCompareAllVsAll: Allocating VRAM for results\n");
        err = cudaMalloc(&d_results, maxMatchesPerBatch * sizeof(Match));
        if (err != cudaSuccess) {
            AD_DEBUG_FMT("GpuCompareAllVsAll: Failed to allocate results VRAM: %s\n", cudaGetErrorString(err));
            cudaFree(d_thumbnails);
            cudaFree(d_crcArray);
            return false;
        }

        // 4. Выделяем память для counter
        err = cudaMalloc(&d_matchCount, sizeof(size_t));
        if (err != cudaSuccess) {
            AD_DEBUG("GpuCompareAllVsAll: Failed to allocate counter VRAM\n");
            cudaFree(d_thumbnails);
            cudaFree(d_crcArray);
            cudaFree(d_results);
            return false;
        }

        // 5. Upload всех thumbnails в VRAM
        AD_DEBUG("GpuCompareAllVsAll: Uploading all thumbnails to VRAM\n");
        err = cudaMemcpy(d_thumbnails, allThumbnails, count * thumbSize, cudaMemcpyHostToDevice);
        if (err != cudaSuccess) {
            AD_DEBUG_FMT("GpuCompareAllVsAll: Upload thumbnails failed: %s\n", cudaGetErrorString(err));
            cudaFree(d_thumbnails);
            cudaFree(d_crcArray);
            cudaFree(d_results);
            cudaFree(d_matchCount);
            return false;
        }
        AD_DEBUG("GpuCompareAllVsAll: Upload thumbnails complete\n");

        // 6. Upload CRC массива
        AD_DEBUG("GpuCompareAllVsAll: Uploading CRC array to VRAM\n");
        err = cudaMemcpy(d_crcArray, allCrcArray, count * sizeof(uint64_t), cudaMemcpyHostToDevice);
        if (err != cudaSuccess) {
            AD_DEBUG_FMT("GpuCompareAllVsAll: Upload CRC failed: %s\n", cudaGetErrorString(err));
            cudaFree(d_thumbnails);
            cudaFree(d_crcArray);
            cudaFree(d_results);
            cudaFree(d_matchCount);
            return false;
        }
        AD_DEBUG("GpuCompareAllVsAll: Upload CRC complete\n");

        // 7. Инициализируем counter
        size_t h_matchCount = 0;
        err = cudaMemcpy(d_matchCount, &h_matchCount, sizeof(size_t), cudaMemcpyHostToDevice);
        if (err != cudaSuccess) {
            AD_DEBUG_FMT("GpuCompareAllVsAll: Counter init failed: %s\n", cudaGetErrorString(err));
            cudaFree(d_thumbnails);
            cudaFree(d_crcArray);
            cudaFree(d_results);
            cudaFree(d_matchCount);
            return false;
        }

        // 8. Запускаем kernel
        AD_DEBUG("GpuCompareAllVsAll: Launching kernel\n");

        int threadsPerBlock = 256;
        size_t blocks = count;
        if (blocks > 65535) blocks = 65535;
        if (blocks == 0) blocks = 1;

        AD_DEBUG_FMT("GpuCompareAllVsAll: Launching %zu blocks, %d threads/block\n", blocks, threadsPerBlock);

        AllVsAllKernel<<<(int)blocks, threadsPerBlock, thumbSize>>>(
            d_thumbnails, d_crcArray, thumbSize, count, threshold, maxDifference, addDiffForCrcMismatch,
            d_results, d_matchCount);

        err = cudaGetLastError();
        if (err != cudaSuccess) {
            AD_DEBUG_FMT("GpuCompareAllVsAll: Kernel launch failed: %s\n", cudaGetErrorString(err));
            cudaFree(d_thumbnails);
            cudaFree(d_crcArray);
            cudaFree(d_results);
            cudaFree(d_matchCount);
            return false;
        }

        // 9. Ждём завершения kernel
        AD_DEBUG("GpuCompareAllVsAll: Synchronizing\n");
        err = cudaDeviceSynchronize();
        if (err != cudaSuccess) {
            AD_DEBUG_FMT("GpuCompareAllVsAll: Sync failed: %s\n", cudaGetErrorString(err));
            cudaFree(d_thumbnails);
            cudaFree(d_crcArray);
            cudaFree(d_results);
            cudaFree(d_matchCount);
            return false;
        }
        AD_DEBUG("GpuCompareAllVsAll: Kernel complete\n");

        // 10. Считываем total match count
        cudaMemcpy(&h_matchCount, d_matchCount, sizeof(size_t), cudaMemcpyDeviceToHost);
        AD_DEBUG_FMT("GpuCompareAllVsAll: Found %zu total matches\n", h_matchCount);

        // 11. Streaming readback — читаем батчами и вызываем callback
        if (h_matchCount > 0) {
            std::vector<Match> h_batch(maxMatchesPerBatch);
            size_t remaining = h_matchCount;
            size_t offset = 0;

            while (remaining > 0) {
                size_t batchSize = (remaining < maxMatchesPerBatch) ? remaining : maxMatchesPerBatch;
                AD_DEBUG_FMT("GpuCompareAllVsAll: Reading batch %zu matches (offset %zu)\n", batchSize, offset);

                err = cudaMemcpy(h_batch.data(), d_results + offset, batchSize * sizeof(Match), cudaMemcpyDeviceToHost);
                if (err != cudaSuccess) {
                    AD_DEBUG_FMT("GpuCompareAllVsAll: Readback failed: %s\n", cudaGetErrorString(err));
                    break;
                }

                // Вызываем callback с батчем
                callback(h_batch.data(), batchSize, callbackContext);
                
                remaining -= batchSize;
                offset += batchSize;
            }
            AD_DEBUG("GpuCompareAllVsAll: All batches processed\n");
        }

        // 12. Освобождаем VRAM
        cudaFree(d_thumbnails);
        cudaFree(d_crcArray);
        cudaFree(d_results);
        cudaFree(d_matchCount);

        AD_DEBUG("GpuCompareAllVsAll: Complete\n");
        return true;
    }
}
