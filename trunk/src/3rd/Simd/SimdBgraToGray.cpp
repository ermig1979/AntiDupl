/*
* Simd Library.
*
* Copyright (c) 2011-2013 Yermalayeu Ihar.
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
#include "Simd/SimdEnable.h"
#include "Simd/SimdLoad.h"
#include "Simd/SimdStore.h"
#include "Simd/SimdConst.h"
#include "Simd/SimdMemory.h"
#include "Simd/SimdBgrToGray.h"
#include "Simd/SimdBgraToGray.h"

namespace Simd
{
    namespace Base
    {
        void BgraToGray(const uchar * bgra, size_t width, size_t height, size_t bgraStride, uchar * gray, size_t grayStride)
        {
            for(size_t row = 0; row < height; ++row)
            {
				const uchar * pBgra = bgra + row*bgraStride;
				uchar * pGray = gray + row*grayStride;
				for(const uchar *pGrayEnd = pGray + width; pGray < pGrayEnd; pGray += 1, pBgra += 4)
				{
					*pGray = BgrToGray(pBgra[0], pBgra[1], pBgra[2]);
				}
            }
        }
    }

#ifdef SIMD_SSE2_ENABLE    
    namespace Sse2
    {
        const __m128i K16_BLUE_RED = SIMD_MM_SET2_EPI16(Base::BLUE_TO_GRAY_WEIGHT, Base::RED_TO_GRAY_WEIGHT);        
        const __m128i K16_GREEN_0000 = SIMD_MM_SET2_EPI16(Base::GREEN_TO_GRAY_WEIGHT, 0x0000);
        const __m128i K32_ROUND_TERM = SIMD_MM_SET1_EPI32(Base::BGR_TO_GRAY_ROUND_TERM);

        SIMD_INLINE __m128i BgraToGray32(__m128i bgra)
        {
            const __m128i g0a0 = _mm_and_si128(_mm_srli_si128(bgra, 1), K16_00FF);
            const __m128i b0r0 = _mm_and_si128(bgra, K16_00FF);
            const __m128i weightedSum = _mm_add_epi32(_mm_madd_epi16(g0a0, K16_GREEN_0000), _mm_madd_epi16(b0r0, K16_BLUE_RED));
            return _mm_srli_epi32(_mm_add_epi32(weightedSum, K32_ROUND_TERM), Base::BGR_TO_GRAY_AVERAGING_SHIFT);
        }

        SIMD_INLINE __m128i BgraToGray(__m128i bgra[4])
        {
            const __m128i lo = _mm_packs_epi32(BgraToGray32(bgra[0]), BgraToGray32(bgra[1]));
            const __m128i hi = _mm_packs_epi32(BgraToGray32(bgra[2]), BgraToGray32(bgra[3]));
            return _mm_packus_epi16(lo, hi);
        }

		template <bool align> SIMD_INLINE void Load(const uchar* p, __m128i a[4])
		{
			a[0] = Load<align>((__m128i*)p + 0);
			a[1] = Load<align>((__m128i*)p + 1);
			a[2] = Load<align>((__m128i*)p + 2);
			a[3] = Load<align>((__m128i*)p + 3);
		}

        template <bool align> void BgraToGray(const uchar *bgra, size_t width, size_t height, size_t bgraStride, uchar *gray, size_t grayStride)
        {
            assert(width >= A);
			if(align)
				assert(Aligned(bgra) && Aligned(bgraStride) && Aligned(gray) && Aligned(grayStride));

			size_t alignedWidth = AlignLo(width, A);
			__m128i a[4];
			for(size_t row = 0; row < height; ++row)
			{
				for(size_t col = 0; col < alignedWidth; col += A)
				{
					Load<align>(bgra + 4*col, a);
					Store<align>((__m128i*)(gray + col), BgraToGray(a));
				}
				if(alignedWidth != width)
				{
					Load<false>(bgra + 4*(width - A), a);
					Store<false>((__m128i*)(gray + width - A), BgraToGray(a));
				}
				bgra += bgraStride;
				gray += grayStride;
			}
        }

		void BgraToGray(const uchar *bgra, size_t width, size_t height, size_t bgraStride, uchar *gray, size_t grayStride)
		{
			if(Aligned(bgra) && Aligned(gray) && Aligned(bgraStride) && Aligned(grayStride))
				BgraToGray<true>(bgra, width, height, bgraStride, gray, grayStride);
			else
				BgraToGray<false>(bgra, width, height, bgraStride, gray, grayStride);
		}
    }
#endif// SIMD_SSE2_ENABLE

    void BgraToGray(const uchar *bgra, size_t width, size_t height, size_t bgraStride, uchar *gray, size_t grayStride)
    {
#ifdef SIMD_AVX2_ENABLE
        if(Avx2::Enable && width >= Avx2::A)
            Avx2::BgraToGray(bgra, width, height, bgraStride, gray, grayStride);
        else
#endif//SIMD_AVX2_ENABLE 
#ifdef SIMD_SSE2_ENABLE
        if(Sse2::Enable && width >= Sse2::A)
            Sse2::BgraToGray(bgra, width, height, bgraStride, gray, grayStride);
        else
#endif//SIMD_SSE2_ENABLE       
            Base::BgraToGray(bgra, width, height, bgraStride, gray, grayStride);
    }
}