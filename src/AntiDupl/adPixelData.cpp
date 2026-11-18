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
#include <vector>

#include "adIO.h"
#include "adPixelData.h"

namespace ad
{
    TPixelData::TPixelData(size_t side_)
        :side(side_),
		size(Simd::Square(side_)),
        full(Simd::Square(side_) + FAST_DATA_SIZE),
        fast(static_cast<TUInt8*>(SimdAllocate(full, SimdAlignment()))),
        main(fast + FAST_DATA_SIZE),
        color(static_cast<TUInt8*>(SimdAllocate(COLOR_DATA_SIZE, SimdAlignment()))),
        filled(false),
		average(0),
		varianceSquare(0),
		sum(0),
		sumSquare(0),
		pixelHash(0),
		asShown(false),
		alpha(false),
		fastBlack(NULL),
		mainBlack(NULL),
		colorBlack(NULL)
    {
    }

    TPixelData::TPixelData(const TPixelData& pixelData)
        :side(pixelData.side),
        size(Simd::Square(pixelData.side)),
        full(Simd::Square(pixelData.side) + FAST_DATA_SIZE),
        fast(static_cast<TUInt8*>(SimdAllocate(full, SimdAlignment()))),
        main(fast + FAST_DATA_SIZE),
        color(static_cast<TUInt8*>(SimdAllocate(COLOR_DATA_SIZE, SimdAlignment()))),
        filled(false),
		average(pixelData.average),
		varianceSquare(pixelData.varianceSquare),
		sum(pixelData.sum),
		sumSquare(pixelData.sumSquare),
		pixelHash(pixelData.pixelHash),
		asShown(false),
		alpha(false),
		fastBlack(NULL),
		mainBlack(NULL),
		colorBlack(NULL)
    {
        if(pixelData.filled)
        {
            CopyFrom(pixelData);
            filled = true;
        }
    }

    TPixelData::~TPixelData()
    {
        SetAlpha(false);
        SimdFree((void*)fast);
        SimdFree((void*)color);
    }

    // Allocates or frees the over-black buffers.
    void TPixelData::SetAlpha(bool alpha_)
    {
        if(alpha_ && fastBlack == NULL)
        {
            fastBlack = static_cast<TUInt8*>(SimdAllocate(full, SimdAlignment()));
            mainBlack = fastBlack + FAST_DATA_SIZE;
            colorBlack = static_cast<TUInt8*>(SimdAllocate(COLOR_DATA_SIZE, SimdAlignment()));
        }
        else if(!alpha_ && fastBlack != NULL)
        {
            SimdFree(fastBlack);
            SimdFree(colorBlack);
            fastBlack = mainBlack = colorBlack = NULL;
        }
        alpha = alpha_;
    }

    // The thumbnails and the values computed from them; not filled, which
    // the callers set.
    void TPixelData::CopyFrom(const TPixelData& pixelData)
    {
        memcpy(fast, pixelData.fast, full);
        memcpy(color, pixelData.color, COLOR_DATA_SIZE);
        average = pixelData.average;
        varianceSquare = pixelData.varianceSquare;
        sum = pixelData.sum;
        sumSquare = pixelData.sumSquare;
        pixelHash = pixelData.pixelHash;
        asShown = pixelData.asShown;
        SetAlpha(pixelData.alpha);
        if(alpha)
        {
            memcpy(fastBlack, pixelData.fastBlack, full);
            memcpy(colorBlack, pixelData.colorBlack, COLOR_DATA_SIZE);
        }
    }

    // The colour thumbnail turned or mirrored the way Turn and Mirror move
    // main: pixel (x, y) goes to to(x, y), 3 bytes each.
    template<class To> static void TransformColor(TUInt8 *color, To to)
    {
        const size_t n = COLOR_IMAGE_SIZE;
        std::vector<TUInt8> old(color, color + COLOR_DATA_SIZE);
        for(size_t y = 0; y < n; ++y)
        {
            for(size_t x = 0; x < n; ++x)
            {
                const size_t target = to(x, y);
                memcpy(color + 3*target, old.data() + 3*(y*n + x), 3);
            }
        }
    }

    // Before each search, while one thread owns the image: the compare
    // threads then only read the sums.
    void TPixelData::FillMoments()
    {
        sum = 0;
        sumSquare = 0;
        SimdValueSum(main, side, side, side, &sum);
        SimdSquareSum(main, side, side, side, &sumSquare);
        const double mean = double(sum) / size;
        average = float(mean);
        varianceSquare = float(double(sumSquare) / size - mean * mean);
    }

	// Делаем очень уменьшенное изображение (4x4) для быстрого сравнения.
    static void FillFast(TUInt8 *fast, const TUInt8 *main, size_t side, int ignoreFrameWidth)
    {
        size_t step = (side - 2*ignoreFrameWidth)/FAST_IMAGE_SIZE;
        size_t area = Simd::Square(step);
        size_t roundTerm = area/2;

        for(int fast_x = 0; fast_x < FAST_IMAGE_SIZE; fast_x++)
        {
            for(int fast_y = 0; fast_y < FAST_IMAGE_SIZE; fast_y++)
            {
                size_t sum = roundTerm;
                for(size_t x = ignoreFrameWidth + fast_x*step, x_end = x + step; x < x_end; x++)
                {
                    for(size_t y = ignoreFrameWidth + fast_y*step, y_end = y + step; y < y_end; y++)
                    {
                        sum += main[x*side + y];
                    }
                }
                fast[fast_x*FAST_IMAGE_SIZE + fast_y] = TUInt8(sum/area);
            }
        }
    }

    void TPixelData::FillFast(int ignoreFrameWidth)
    {
        ad::FillFast(fast, main, side, ignoreFrameWidth);
        if(alpha)
            ad::FillFast(fastBlack, mainBlack, side, ignoreFrameWidth);
    }

    // fast (FAST_DATA_SIZE bytes) and main after it, turned through buffer.
    static void Turn(TUInt8 *fast, size_t side, size_t full, TUInt8 *buffer)
    {
        TUInt8* pMain = fast + FAST_DATA_SIZE;
        TUInt8* pBufferMain = buffer + FAST_DATA_SIZE;
        for(size_t new_x = 0; new_x < side; new_x++)
        {
            for(ptrdiff_t new_y = side - 1; new_y >= 0; new_y--)
            {
                pBufferMain[side*new_y + new_x] = *pMain++;
            }
        }

        TUInt8* pFast = fast;
        TUInt8* pBufferFast = buffer;
        for(int new_x = 0; new_x < FAST_IMAGE_SIZE; new_x++)
        {
            for(int new_y = FAST_IMAGE_SIZE - 1; new_y >= 0; new_y--)
            {
                pBufferFast[FAST_IMAGE_SIZE*new_y + new_x] = *pFast++;
            }
        }

        memcpy(fast, buffer, full);
    }

    void TPixelData::Turn(TUInt8* buffer)
    {
        bool inner = false;
        if(buffer == NULL)
        {
            buffer = (TUInt8*)SimdAllocate(full, SimdAlignment());
            inner = true;
        }

        ad::Turn(fast, side, full, buffer);
        const size_t n = COLOR_IMAGE_SIZE;
        auto to = [n](size_t x, size_t y) { return n*(n - 1 - x) + y; };
        TransformColor(color, to);
        if(alpha)
        {
            ad::Turn(fastBlack, side, full, buffer);
            TransformColor(colorBlack, to);
        }
        pixelHash = 0;

        if(inner)
            SimdFree(buffer);
    }

    // fast (FAST_DATA_SIZE bytes) and main after it, mirrored through buffer.
    static void Mirror(TUInt8 *fast, size_t side, size_t full, TUInt8 *buffer)
    {
        TUInt8* pMain = fast + FAST_DATA_SIZE;
        TUInt8* pBufferMain = buffer + FAST_DATA_SIZE;
        for(size_t new_y = 0; new_y < side; new_y++)
        {
            for(ptrdiff_t new_x = side - 1; new_x >= 0; new_x--)
            {
                pBufferMain[side*new_y + new_x] = *pMain++;
            }
        }

        TUInt8* pFast= fast;
        TUInt8* pBufferFast = buffer;
        for(int new_y = 0; new_y < FAST_IMAGE_SIZE; new_y++)
        {
            for(int new_x = FAST_IMAGE_SIZE - 1; new_x >= 0; new_x--)
            {
                pBufferFast[FAST_IMAGE_SIZE*new_y + new_x] = *pFast++;
            }
        }

        memcpy(fast, buffer, full);
    }

    void TPixelData::Mirror(TUInt8* buffer)
    {
        bool inner = false;
        if(buffer == NULL)
        {
            buffer = (TUInt8*)SimdAllocate(full, SimdAlignment());
            inner = true;
        }

        ad::Mirror(fast, side, full, buffer);
        const size_t n = COLOR_IMAGE_SIZE;
        auto to = [n](size_t x, size_t y) { return n*y + (n - 1 - x); };
        TransformColor(color, to);
        if(alpha)
        {
            ad::Mirror(fastBlack, side, full, buffer);
            TransformColor(colorBlack, to);
        }
        pixelHash = 0;

        if(inner)
            SimdFree(buffer);
    }
}

