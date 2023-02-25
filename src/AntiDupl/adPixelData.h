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
#ifndef __adPixelData_h__
#define __adPixelData_h__

#include "adConfig.h"

namespace ad
{
    struct TPixelData
    {
        TUInt8* const fast; //уменьшенное изображение (4x4) для быстрого сравнения
        TUInt8* const main;
        const size_t side; //сторона квадрата
        const size_t size; //width * height
        const size_t full; //width * height + FAST_DATA_SIZE
        bool filled; //true, если создано уменьшенное изображение в main
		float average; //average of image - среднее
		float varianceSquare; //variance of image - Дисперсия случайной величины

        TPixelData(size_t side_);
        TPixelData(const TPixelData& pixelData);
        ~TPixelData();

        void FillFast(int ignoreFrameWidth);
        void Turn(TUInt8 *buffer);
        void Mirror(TUInt8 *buffer);
    };

    typedef TPixelData* TPixelDataPtr;
}

#endif/*__adPixelData_h__*/