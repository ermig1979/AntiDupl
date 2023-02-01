/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2023 Yermalayeu Ihar.
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
#ifndef __adSimd_h__
#define __adSimd_h__

#define SIMD_STATIC
#include "Simd/SimdLib.hpp"

#define AD_INLINE SIMD_INLINE

namespace Simd
{
    template <class T> SIMD_INLINE T Min(T a, T b)
    {
        return a < b ? a : b;
    }

    template <class T> SIMD_INLINE T Max(T a, T b)
    {
        return a > b ? a : b;
    }

    template <class T> SIMD_INLINE T Square(T a)
    {
        return a * a;
    }

    namespace Base
    {
        const int Y_ADJUST = 16;
        const int UV_ADJUST = 128;
        const int YUV_TO_BGR_AVERAGING_SHIFT = 13;
        const int YUV_TO_BGR_ROUND_TERM = 1 << (YUV_TO_BGR_AVERAGING_SHIFT - 1);
        const int Y_TO_RGB_WEIGHT = int(1.164 * (1 << YUV_TO_BGR_AVERAGING_SHIFT) + 0.5);
        const int U_TO_BLUE_WEIGHT = int(2.018 * (1 << YUV_TO_BGR_AVERAGING_SHIFT) + 0.5);
        const int U_TO_GREEN_WEIGHT = -int(0.391 * (1 << YUV_TO_BGR_AVERAGING_SHIFT) + 0.5);
        const int V_TO_GREEN_WEIGHT = -int(0.813 * (1 << YUV_TO_BGR_AVERAGING_SHIFT) + 0.5);
        const int V_TO_RED_WEIGHT = int(1.596 * (1 << YUV_TO_BGR_AVERAGING_SHIFT) + 0.5);

        SIMD_INLINE int RestrictRange(int value, int min = 0, int max = 255)
        {
            return Max(min, Min(max, value));
        }

        SIMD_INLINE int YuvToBlue(int y, int u)
        {
            return RestrictRange((Y_TO_RGB_WEIGHT * (y - Y_ADJUST) + U_TO_BLUE_WEIGHT * (u - UV_ADJUST) +
                YUV_TO_BGR_ROUND_TERM) >> YUV_TO_BGR_AVERAGING_SHIFT);
        }

        SIMD_INLINE int YuvToGreen(int y, int u, int v)
        {
            return RestrictRange((Y_TO_RGB_WEIGHT * (y - Y_ADJUST) + U_TO_GREEN_WEIGHT * (u - UV_ADJUST) +
                V_TO_GREEN_WEIGHT * (v - UV_ADJUST) + YUV_TO_BGR_ROUND_TERM) >> YUV_TO_BGR_AVERAGING_SHIFT);
        }

        SIMD_INLINE int YuvToRed(int y, int v)
        {
            return RestrictRange((Y_TO_RGB_WEIGHT * (y - Y_ADJUST) + V_TO_RED_WEIGHT * (v - UV_ADJUST) +
                YUV_TO_BGR_ROUND_TERM) >> YUV_TO_BGR_AVERAGING_SHIFT);
        }
    }
}

#endif/*__adSimd_h__*/
