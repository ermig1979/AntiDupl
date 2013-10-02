/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2013 Yermalayeu Ihar, 2013 Borisov Dmitry.
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

namespace ad
{
    TDataCollector::TDataCollector(TEngine *pEngine)
        :m_pOptions(pEngine->Options()),
        m_pResult(pEngine->Result())
    {
        for(int size = INITIAL_REDUCED_IMAGE_SIZE; size > m_pOptions->advanced.reducedImageSize; size >>= 1)
			m_pGrayBuffers.push_back(new TView(size, size, size, Simd::View::Gray8, NULL));
    }

    TDataCollector::~TDataCollector() 
    {
        for(size_t i = 0; i < m_pGrayBuffers.size(); ++i)
            delete m_pGrayBuffers[i];
    }
    
    void TDataCollector::Fill(TImageData* pImageData)
    {
        AD_FUNCTION_PERFORMANCE_TEST
        if(!pImageData->crc32)
            SetCrc32(pImageData);
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

    void TDataCollector::FillPixelData(TImageData* pImageData)
    {
        AD_FUNCTION_PERFORMANCE_TEST
        TImage *pImage = TImage::Load(pImageData->hGlobal);
        if(pImage)
        {
            pImageData->height = (TUInt32)pImage->View()->height; 
            pImageData->width = (TUInt32)pImage->View()->width;
            pImageData->type = (TImageType)pImage->Format();

			TView gray(pImage->View()->width, pImage->View()->height, Simd::View::Gray8, NULL);
			Simd::BgraToGray(*pImage->View(), gray);

			pImageData->blockiness = GetBlockiness(gray);

			TBlurringDetector blurringDetector;
			double radius = blurringDetector.Radius(gray);

			Simd::ResizeBilinear(gray, *m_pGrayBuffers.front());
            for(size_t i = 1; i < m_pGrayBuffers.size(); ++i)
				Simd::ReduceGray2x2(*m_pGrayBuffers[i - 1], *m_pGrayBuffers[i]);
			TPixelData & data = *pImageData->data;
            ReduceGray2x2(*m_pGrayBuffers.back(), TView(data.side, data.side, data.side, Simd::View::Gray8, data.main));
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
    
    void TDataCollector::SetCrc32(TImageData* pImageData)
    {
        AD_FUNCTION_PERFORMANCE_TEST
        if(pImageData->hGlobal)
        {
            void *data = ::GlobalLock(pImageData->hGlobal);
            size_t size = ::GlobalSize(pImageData->hGlobal);
            pImageData->crc32 = Simd::Crc32(data, size);
            ::GlobalUnlock(pImageData->hGlobal);
        }
        else
        {
            pImageData->crc32 = -1;
        }
    }

	double TDataCollector::GetBlockiness(const TView & gray)
	{
		if(gray.height < BLOCKINESS_SIZE + 1 || gray.width < BLOCKINESS_SIZE + 1)
			return 0;

		std::vector<unsigned int> rowSums(gray.height);
		Simd::GetAbsDyRowSums(gray, &rowSums[0]);
		double verticalBlockiness = GetBlockiness(rowSums);

		std::vector<unsigned int> colSums(gray.width);
		Simd::GetAbsDxColSums(gray, &colSums[0]);
		double horizontalBlockiness = GetBlockiness(colSums);

		return std::min(verticalBlockiness, horizontalBlockiness);
	}

	double TDataCollector::GetBlockiness(const std::vector<unsigned int> & sums)
	{
		std::vector<unsigned int> block(BLOCKINESS_SIZE, 0);
		size_t end = (sums.size() - 1)/block.size()*block.size(); 
		for(size_t i = 0; i < end; ++i)
			block[i%block.size()] += sums[i];
		std::sort(block.rbegin(), block.rend());
		return double(block[0] - block[1])/double(block[0] + block[1])*100.0;
	}
}
