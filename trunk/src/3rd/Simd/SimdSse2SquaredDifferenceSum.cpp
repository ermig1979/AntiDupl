/*
* Simd Library (http://simd.sourceforge.net).
*
* Copyright (c) 2011-2015 Yermalayeu Ihar.
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
#include "Simd/SimdMemory.h"
#include "Simd/SimdInit.h"
#include "Simd/SimdExtract.h"
#include "Simd/SimdConst.h"
#include "Simd/SimdMath.h"
#include "Simd/SimdLoad.h"
#include "Simd/SimdStore.h"
#include "Simd/SimdSse2.h"

namespace Simd
{
#ifdef SIMD_SSE2_ENABLE    
    namespace Sse2
    {
        SIMD_INLINE __m128i SquaredDifference(__m128i a, __m128i b)
        {
            const __m128i aLo = _mm_unpacklo_epi8(a, _mm_setzero_si128());
            const __m128i bLo = _mm_unpacklo_epi8(b, _mm_setzero_si128());
            const __m128i dLo = _mm_sub_epi16(aLo, bLo);

            const __m128i aHi = _mm_unpackhi_epi8(a, _mm_setzero_si128());
            const __m128i bHi = _mm_unpackhi_epi8(b, _mm_setzero_si128());
            const __m128i dHi = _mm_sub_epi16(aHi, bHi);

            return _mm_add_epi32(_mm_madd_epi16(dLo, dLo), _mm_madd_epi16(dHi, dHi));
        }

		template <bool align> void SquaredDifferenceSum(
			const uint8_t *a, size_t aStride, const uint8_t *b, size_t bStride, 
			size_t width, size_t height, uint64_t * sum)
		{
			assert(width < 0x10000);
			if(align)
			{
				assert(Aligned(a) && Aligned(aStride) && Aligned(b) && Aligned(bStride));
			}

			size_t bodyWidth = AlignLo(width, A);
			__m128i tailMask = ShiftLeft(K_INV_ZERO, A - width + bodyWidth);
			__m128i fullSum = _mm_setzero_si128();
			for(size_t row = 0; row < height; ++row)
			{
				__m128i rowSum = _mm_setzero_si128();
				for(size_t col = 0; col < bodyWidth; col += A)
				{
					const __m128i a_ = Load<align>((__m128i*)(a + col));
					const __m128i b_ = Load<align>((__m128i*)(b + col)); 
					rowSum = _mm_add_epi32(rowSum, SquaredDifference(a_, b_));
				}
				if(width - bodyWidth)
				{
					const __m128i a_ = _mm_and_si128(tailMask, Load<false>((__m128i*)(a + width - A)));
					const __m128i b_ = _mm_and_si128(tailMask, Load<false>((__m128i*)(b + width - A))); 
					rowSum = _mm_add_epi32(rowSum, SquaredDifference(a_, b_));
				}
				fullSum = _mm_add_epi64(fullSum, HorizontalSum32(rowSum));
				a += aStride;
				b += bStride;
			}
			*sum = ExtractInt64Sum(fullSum);
		}

		template <bool align> void SquaredDifferenceSumMasked(
			const uint8_t *a, size_t aStride, const uint8_t *b, size_t bStride, 
			const uint8_t *mask, size_t maskStride, uint8_t index, size_t width, size_t height, uint64_t * sum)
		{
			assert(width < 0x10000);
			if(align)
			{
				assert(Aligned(a) && Aligned(aStride) && Aligned(b) && Aligned(bStride));
				assert(Aligned(mask) && Aligned(maskStride));
			}

			size_t bodyWidth = AlignLo(width, A);
			__m128i tailMask = ShiftLeft(K_INV_ZERO, A - width + bodyWidth);
			__m128i fullSum = _mm_setzero_si128();
			__m128i index_= _mm_set1_epi8(index);
			for(size_t row = 0; row < height; ++row)
			{
				__m128i rowSum = _mm_setzero_si128();
				for(size_t col = 0; col < bodyWidth; col += A)
				{
					const __m128i mask_ = LoadMaskI8<align>((__m128i*)(mask + col), index_);
					const __m128i a_ = _mm_and_si128(mask_, Load<align>((__m128i*)(a + col)));
					const __m128i b_ = _mm_and_si128(mask_, Load<align>((__m128i*)(b + col))); 
					rowSum = _mm_add_epi32(rowSum, SquaredDifference(a_, b_));
				}
				if(width - bodyWidth)
				{
					const __m128i mask_ = _mm_and_si128(tailMask, LoadMaskI8<align>((__m128i*)(mask + width - A), index_));
					const __m128i a_ = _mm_and_si128(mask_, Load<false>((__m128i*)(a + width - A)));
					const __m128i b_ = _mm_and_si128(mask_, Load<false>((__m128i*)(b + width - A))); 
					rowSum = _mm_add_epi32(rowSum, SquaredDifference(a_, b_));
				}
				fullSum = _mm_add_epi64(fullSum, HorizontalSum32(rowSum));
				a += aStride;
				b += bStride;
				mask += maskStride;
			}
			*sum = ExtractInt64Sum(fullSum);
		}

		void SquaredDifferenceSum(const uint8_t *a, size_t aStride, const uint8_t *b, size_t bStride, 
			size_t width, size_t height, uint64_t * sum)
		{
			if(Aligned(a) && Aligned(aStride) && Aligned(b) && Aligned(bStride))
				SquaredDifferenceSum<true>(a, aStride, b, bStride, width, height, sum);
			else
				SquaredDifferenceSum<false>(a, aStride, b, bStride, width, height, sum);
		}

		void SquaredDifferenceSumMasked(const uint8_t *a, size_t aStride, const uint8_t *b, size_t bStride, 
			const uint8_t *mask, size_t maskStride, uint8_t index, size_t width, size_t height, uint64_t * sum)
		{
			if(Aligned(a) && Aligned(aStride) && Aligned(b) && Aligned(bStride) && Aligned(mask) && Aligned(maskStride))
				SquaredDifferenceSumMasked<true>(a, aStride, b, bStride, mask, maskStride, index, width, height, sum);
			else
				SquaredDifferenceSumMasked<false>(a, aStride, b, bStride, mask, maskStride, index, width, height, sum);
		}

		template <bool align> void SigmaDouble(const uint8_t * srcFirst, size_t strideFirst, const uint8_t * srcSecond, size_t strideSecond, size_t width, size_t height, const float averageFirst, const float averageSecond, float * sigmaOfBoth)
        {
            assert(width >= A);
			if(align)
			{
				assert(Aligned(srcFirst) && Aligned(strideFirst) && Aligned(srcSecond) && Aligned(strideSecond));
			}

			size_t bodyWidth = AlignLo(width, A);
            __m128i tailMask = ShiftLeft(K_INV_ZERO, A - width + bodyWidth);
            __m128 fullSum = _mm_setzero_ps(); //double
			const __m128 avFir = _mm_set1_ps(averageFirst);
            const __m128 avSec = _mm_set1_ps(averageSecond);
            for(size_t row = 0; row < height; ++row)
            {
				__m128 rowSum = _mm_setzero_ps();
                for(size_t col = 0; col < bodyWidth; col += A)
                {
					const __m128i a_ = Load<align>((__m128i*)(srcFirst + col)); //Has 16 unsigned integers 16 пискелей загружается!
					const __m128i b_ = Load<align>((__m128i*)(srcSecond + col));
					
					const __m128i aLo   = _mm_unpacklo_epi8(a_, _mm_setzero_si128()); //8 чисел содержит
					const __m128i aLoLo = _mm_unpacklo_epi8(aLo, _mm_setzero_si128()); //4 числа содержит
					const __m128i aLoHi = _mm_unpackhi_epi8(aLo, _mm_setzero_si128());
					const __m128i aHi   = _mm_unpackhi_epi8(a_, _mm_setzero_si128());
					const __m128i aHiLo = _mm_unpacklo_epi8(aHi, _mm_setzero_si128());
					const __m128i aHiHi = _mm_unpackhi_epi8(aHi, _mm_setzero_si128());

					const __m128i bLo   = _mm_unpacklo_epi8(b_, _mm_setzero_si128());
					const __m128i bLoLo = _mm_unpacklo_epi8(bLo, _mm_setzero_si128()); //4 числа содержит
					const __m128i bLoHi = _mm_unpackhi_epi8(bLo, _mm_setzero_si128());
					const __m128i bHi   = _mm_unpackhi_epi8(b_, _mm_setzero_si128());
					const __m128i bHiLo = _mm_unpacklo_epi8(bHi, _mm_setzero_si128());
					const __m128i bHiHi = _mm_unpackhi_epi8(bHi, _mm_setzero_si128());

					const __m128 faLoLo = _mm_cvtepi32_ps(aLoLo);
					const __m128 faLoHi = _mm_cvtepi32_ps(aLoHi);
					const __m128 faHiLo = _mm_cvtepi32_ps(aHiLo);
					const __m128 faHiHi = _mm_cvtepi32_ps(aHiHi);
					const __m128 fbLoLo = _mm_cvtepi32_ps(bLoLo);
					const __m128 fbLoHi = _mm_cvtepi32_ps(bLoHi);
					const __m128 fbHiLo = _mm_cvtepi32_ps(bHiLo);
					const __m128 fbHiHi = _mm_cvtepi32_ps(bHiHi);

					rowSum = _mm_add_ps(rowSum, _mm_mul_ps(_mm_sub_ps(faLoLo, avFir), _mm_sub_ps(fbLoLo, avSec)));
					rowSum = _mm_add_ps(rowSum, _mm_mul_ps(_mm_sub_ps(faLoHi, avFir), _mm_sub_ps(fbLoHi, avSec)));
					rowSum = _mm_add_ps(rowSum, _mm_mul_ps(_mm_sub_ps(faHiLo, avFir), _mm_sub_ps(fbHiLo, avSec)));
					rowSum = _mm_add_ps(rowSum, _mm_mul_ps(_mm_sub_ps(faHiHi, avFir), _mm_sub_ps(fbHiHi, avSec)));
                }
                if(width - bodyWidth)
                {
					const __m128i a_ = _mm_and_si128(tailMask, Load<false>((__m128i*)(srcFirst + width - A)));
					const __m128i b_ = _mm_and_si128(tailMask, Load<false>((__m128i*)(srcSecond + width - A))); 

					const __m128i aLo   = _mm_unpacklo_epi8(a_, _mm_setzero_si128());
					const __m128i aLoLo = _mm_unpacklo_epi8(aLo, _mm_setzero_si128());
					const __m128i aLoHi = _mm_unpackhi_epi8(aLo, _mm_setzero_si128());
					const __m128i aHi   = _mm_unpackhi_epi8(a_, _mm_setzero_si128());
					const __m128i aHiLo = _mm_unpacklo_epi8(aHi, _mm_setzero_si128());
					const __m128i aHiHi = _mm_unpackhi_epi8(aHi, _mm_setzero_si128());

					const __m128i bLo   = _mm_unpacklo_epi8(b_, _mm_setzero_si128());
					const __m128i bLoLo = _mm_unpacklo_epi8(bLo, _mm_setzero_si128());
					const __m128i bLoHi = _mm_unpackhi_epi8(bLo, _mm_setzero_si128());
					const __m128i bHi   = _mm_unpackhi_epi8(b_, _mm_setzero_si128());
					const __m128i bHiLo = _mm_unpacklo_epi8(bHi, _mm_setzero_si128());
					const __m128i bHiHi = _mm_unpackhi_epi8(bHi, _mm_setzero_si128());

					const __m128 faLoLo = _mm_cvtepi32_ps(aLoLo);
					const __m128 faLoHi = _mm_cvtepi32_ps(aLoHi);
					const __m128 faHiLo = _mm_cvtepi32_ps(aHiLo);
					const __m128 faHiHi = _mm_cvtepi32_ps(aHiHi);
					const __m128 fbLoLo = _mm_cvtepi32_ps(bLoLo);
					const __m128 fbLoHi = _mm_cvtepi32_ps(bLoHi);
					const __m128 fbHiLo = _mm_cvtepi32_ps(bHiLo);
					const __m128 fbHiHi = _mm_cvtepi32_ps(bHiHi);

					rowSum = _mm_add_ps(rowSum, _mm_mul_ps(_mm_sub_ps(faLoLo, avFir), _mm_sub_ps(fbLoLo, avSec)));
					rowSum = _mm_add_ps(rowSum, _mm_mul_ps(_mm_sub_ps(faLoHi, avFir), _mm_sub_ps(fbLoHi, avSec)));
					rowSum = _mm_add_ps(rowSum, _mm_mul_ps(_mm_sub_ps(faHiLo, avFir), _mm_sub_ps(fbHiLo, avSec)));
					rowSum = _mm_add_ps(rowSum, _mm_mul_ps(_mm_sub_ps(faHiHi, avFir), _mm_sub_ps(fbHiHi, avSec)));
                }
				fullSum = _mm_add_ps(fullSum, rowSum);
                srcFirst += strideFirst;
				srcSecond += strideSecond;
            }

            *sigmaOfBoth = (ExtractSum(fullSum)/(width*height));
        }

		void SigmaDouble(const uint8_t * srcFirst, size_t strideFirst, const uint8_t * srcSecond, size_t strideSecond, size_t width, size_t height, const float averageFirst, const float averageSecond, float * sigmaOfBoth)
		{
			if(Aligned(srcFirst) && Aligned(strideFirst) && Aligned(srcSecond) && Aligned(strideSecond))
                SigmaDouble<true>(srcFirst, strideFirst, srcSecond, strideSecond, width, height, averageFirst, averageSecond, sigmaOfBoth);
            else
                SigmaDouble<false>(srcFirst, strideFirst, srcSecond, strideSecond, width, height, averageFirst, averageSecond, sigmaOfBoth);
		}
    }
#endif// SIMD_SSE2_ENABLE
}
