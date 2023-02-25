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
#include "adIO.h"
#include "adPixelData.h"

namespace ad
{
    TPixelData::TPixelData(size_t side_)
        :side(side_),
		size(Simd::Square(side_)),
        full(Simd::Square(side_) + FAST_DATA_SIZE),
        fast(NULL),
        main(NULL),
        filled(false),
		average(0),
		varianceSquare(0)
    {
        (TUInt8*)fast = (TUInt8*)SimdAllocate(full, SimdAlignment());
        (TUInt8*)main = fast + FAST_DATA_SIZE;
    }

    TPixelData::TPixelData(const TPixelData& pixelData)
        :side(pixelData.side),
        size(Simd::Square(pixelData.side)),
        full(Simd::Square(pixelData.side) + FAST_DATA_SIZE),
        fast(NULL),
        main(NULL),
        filled(false),
		average(pixelData.average),
		varianceSquare(pixelData.varianceSquare)
    {
        (TUInt8*)fast = (TUInt8*)SimdAllocate(full, SimdAlignment());
        (TUInt8*)main = fast + FAST_DATA_SIZE;
        if(pixelData.filled)
        {
            memcpy(fast, pixelData.fast, full);
            filled = true;
        }
    }

    TPixelData::~TPixelData()
    {
        SimdFree((void*)fast);
    }

	// Делаем очень уменьшенное изображение (4x4) для быстрого сравнения.
    void TPixelData::FillFast(int ignoreFrameWidth)
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

    void TPixelData::Turn(TUInt8* buffer)
    {
        bool inner = false;
        if(buffer == NULL)
        {
            buffer = (TUInt8*)SimdAllocate(full, SimdAlignment());
            inner = true;
        }

        TUInt8* pMain = main;
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

        if(inner)
            SimdFree(buffer);
    }

    void TPixelData::Mirror(TUInt8* buffer)
    {
        bool inner = false;
        if(buffer == NULL)
        {
            buffer = (TUInt8*)SimdAllocate(full, SimdAlignment());
            inner = true;
        }

        TUInt8* pMain = main;
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

        if(inner)
            SimdFree(buffer);
    }
}

