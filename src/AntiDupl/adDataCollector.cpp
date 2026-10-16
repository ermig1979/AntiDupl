/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar, 2013-2018 Borisov Dmitry.
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
#include "adPerformance.h"
#include "adEngine.h"
#include "adImageData.h"
#include "adOptions.h"
#include "adStatus.h"
#include "adResult.h"
#include "adResultStorage.h"
#include "adDataCollector.h"
#include "adImageUtils.h"
#include "adPixelData.h"
#include "adBlurringDetector.h"

#define XXH_INLINE_ALL
#include <xxhash.h>

namespace ad
{
    // Агрегация сумм по BLOCKINESS_SIZE-блокам: (max−next)/(max+next)×100.
    static double GetBlockinessSums(const std::vector<unsigned int> & sums)
    {
        std::vector<unsigned int> block(BLOCKINESS_SIZE, 0);
        size_t end = (sums.size() - 1)/block.size()*block.size(); 
        for(size_t i = 0; i < end; ++i)
            block[i%block.size()] += sums[i];
        std::sort(block.rbegin(), block.rend());
        const double denominator = double(block[0]) + double(block[1]);
        // Плоское изображение (суммы нулевые) — нулевой знаменатель: 0 вместо NaN.
        return denominator == 0.0 ? 0.0 : double(block[0] - block[1])/denominator*100.0;
    }

    // Алгоритм идентичен классическому пайплайну (TDataCollector::GetBlockiness):
    // суммы |dY| по строкам и |dX| по столбцам серого растра, агрегация по 8×8-блокам,
    // (max−next)/(max+next)×100 по минимальному из двух направлений.
    double ComputeBlockiness(const TView & gray)
    {
        AD_FUNCTION_PERFORMANCE_TEST

        if(gray.height < BLOCKINESS_SIZE + 1 || gray.width < BLOCKINESS_SIZE + 1)
            return 0;

        std::vector<unsigned int> rowSums(gray.height);
        Simd::GetAbsDyRowSums(gray, &rowSums[0]);
        double verticalBlockiness = GetBlockinessSums(rowSums);

        std::vector<unsigned int> colSums(gray.width);
        Simd::GetAbsDxColSums(gray, &colSums[0]);
        double horizontalBlockiness = GetBlockinessSums(colSums);

        return std::min(verticalBlockiness, horizontalBlockiness);
    }
  
    // XXH3-64 of the image as BGRA rows, the layout most decoders give; the
    // others are converted row by row, so the same pixels hash the same
    // whatever the file format. 0 is kept for "unknown".
    static uint64_t GetPixelHash(const TView & view)
    {
        if (view.format != TView::Bgra32 && view.format != TView::Rgba32 && view.format != TView::Rgb24)
            return 0;
        XXH3_state_t * state = XXH3_createState();
        if (state == NULL)
            return 0;
        XXH3_64bits_reset(state);
        const size_t rowSize = view.width * 4;
        std::vector<TUInt8> row(rowSize);
        for (size_t y = 0; y < view.height; ++y)
        {
            const TUInt8 * src = view.data + y * view.stride;
            if (view.format == TView::Bgra32)
            {
                XXH3_64bits_update(state, src, rowSize);
                continue;
            }
            if (view.format == TView::Rgba32)
                SimdBgraToRgba(src, view.width, 1, view.stride, row.data(), rowSize); // swaps red and blue
            else
                SimdRgbToBgra(src, view.width, 1, view.stride, row.data(), rowSize, 0xFF);
            XXH3_64bits_update(state, row.data(), rowSize);
        }
        uint64_t hash = XXH3_64bits_digest(state);
        XXH3_freeState(state);
        return hash == 0 ? 1 : hash;
    }

    TDataCollector::TDataCollector(TEngine *pEngine)
        :m_pOptions(pEngine->Options()),
        m_pResult(pEngine->Result())
    {
        for(int size = INITIAL_REDUCED_IMAGE_SIZE; size > m_pOptions->advanced.reducedImageSize; size >>= 1)
			m_pGrayBuffers.push_back(new TView(size, size, size, TView::Gray8, NULL));
    }

    TDataCollector::~TDataCollector() 
    {
        for(size_t i = 0; i < m_pGrayBuffers.size(); ++i)
            delete m_pGrayBuffers[i];
    }
    
	// Собирание информации об изображение из переданной TImageData
    void TDataCollector::Fill(TImageData* pImageData)
    {
        AD_FUNCTION_PERFORMANCE_TEST
        if(!pImageData->crc32c)
            SetCrc32c(pImageData);
        if(pImageData->PixelDataFillingNeed(m_pOptions))
            FillPixelData(pImageData);
        if(pImageData->DefectCheckingNeed(m_pOptions))
            CheckOnDefect(pImageData);
		TDefectType defect = pImageData->GetDefect(m_pOptions);
        if(defect > AD_DEFECT_NONE)
            m_pResult->AddDefectImage(pImageData, defect);
        pImageData->FillOther(m_pOptions);
        pImageData->FreeGlobal();
    }

	// Заполняем переданный TImageData из TImage хранящейся в глобальном хуке pImageData, создание уменьшенного изображения
    void TDataCollector::FillPixelData(TImageData* pImageData)
    {
        AD_FUNCTION_PERFORMANCE_TEST
        TImage *pImage = TImage::Load(pImageData->hGlobal, m_pOptions);
        if(pImage)
        {
            pImageData->height = (TUInt32)pImage->View()->height; 
            pImageData->width = (TUInt32)pImage->View()->width;
            pImageData->type = (TImageType)pImage->Format();

			TView gray(pImage->View()->width, pImage->View()->height, TView::Gray8, NULL);
            if (pImage->View()->format == TView::Format::Rgb24)
            {
                Simd::RgbToGray(*pImage->View(), gray);
            }
            else if (pImage->View()->format == TView::Format::Rgba32)
            {
                Simd::RgbaToGray(*pImage->View(), gray);
            }
            else
            {
                Simd::BgraToGray(*pImage->View(), gray);
            }

			pImageData->blockiness = GetBlockiness(gray);

			TBlurringDetector blurringDetector;
			pImageData->blurring = blurringDetector.Detect(gray);

			pImageData->imageExif = pImage->ImageExif();

			Simd::Resize(gray, *m_pGrayBuffers.front());
            for(size_t i = 1; i < m_pGrayBuffers.size(); ++i)
				Simd::ReduceGray2x2(*m_pGrayBuffers[i - 1], *m_pGrayBuffers[i]);
			TPixelData & data = *pImageData->data;
            TView reducedView(data.side, data.side, data.side, TView::Gray8, data.main);
            ReduceGray2x2(*m_pGrayBuffers.back(), reducedView);
            data.pixelHash = GetPixelHash(*pImage->View());
            data.filled = true;

			delete pImage;
        }
        else
        {
            pImageData->height = 0; 
            pImageData->width = 0;
            pImageData->type = AD_IMAGE_NONE;
        }
    }

    void TDataCollector::CheckOnDefect(TImageData* pImageData)
    {
        if(pImageData->type == AD_IMAGE_NONE || pImageData->hGlobal == NULL)
        {
            pImageData->defect = AD_DEFECT_UNKNOWN;
            return;
        }

        if(pImageData->type == AD_IMAGE_JPEG || pImageData->type == AD_IMAGE_JP2)
        {
            unsigned char *data = (unsigned char*)::GlobalLock(pImageData->hGlobal);
            size_t size = ::GlobalSize(pImageData->hGlobal);
            bool isJpegEndMarkerAbsent = true;
            for(size_t i = size; i > 1; i--)
            {
                if(data[i - 2] == JPEG_FILE_MARKER && data[i - 1] == JPEG_FILE_MARKER_END)
                {
                    isJpegEndMarkerAbsent = false;
                    break;
                }
            }
            ::GlobalUnlock(pImageData->hGlobal);
            if(isJpegEndMarkerAbsent)
            {
                pImageData->defect = AD_DEFECT_JPEG_END_MARKER_IS_ABSENT;
                return;
            }
        }

        pImageData->defect = AD_DEFECT_NONE;
    }
    
    void TDataCollector::SetCrc32c(TImageData* pImageData)
    {
        AD_FUNCTION_PERFORMANCE_TEST
        if(pImageData->hGlobal)
        {
            void *data = ::GlobalLock(pImageData->hGlobal);
            size_t size = ::GlobalSize(pImageData->hGlobal);
            pImageData->crc32c = SimdCrc32c(data, size);
            ::GlobalUnlock(pImageData->hGlobal);
        }
        else
        {
            pImageData->crc32c = -1;
        }
    }

	double TDataCollector::GetBlockiness(const TView & gray)
	{
		return ComputeBlockiness(gray);
	}

	double TDataCollector::GetBlockiness(const std::vector<unsigned int> & sums)
	{
		return GetBlockinessSums(sums);
	}
}
