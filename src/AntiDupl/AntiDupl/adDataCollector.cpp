/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2013 Yermalayeu Ihar.
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

namespace ad
{
    TDataCollector::TDataCollector(TEngine *pEngine)
        :m_pOptions(pEngine->Options()),
        m_pResult(pEngine->Result())
    {
        for(int size = INITIAL_REDUCED_IMAGE_SIZE; size > m_pOptions->advanced.reducedImageSize; size >>= 1)
            m_pGrayBuffers.push_back(new TPixelData(size));
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
        if(CanAddDefect(pImageData))
            m_pResult->AddDefectImage(pImageData, pImageData->defect);
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

            GetGrayResized(pImage, INITIAL_REDUCED_IMAGE_SIZE, INITIAL_REDUCED_IMAGE_SIZE, m_pGrayBuffers.front()->main);
            for(size_t i = 1; i < m_pGrayBuffers.size(); ++i)
                ReduceGray2x2(m_pGrayBuffers[i - 1], m_pGrayBuffers[i]);
            ReduceGray2x2(m_pGrayBuffers.back(), pImageData->data);
            pImageData->data->filled = true;

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

    void TDataCollector::ReduceGray2x2(const TPixelData* pSrc, TPixelData* pDst)
    {
       Simd::ReduceGray2x2(pSrc->main, pSrc->side, pSrc->side, pSrc->side, pDst->main, pDst->side, pDst->side, pDst->side);
    }

	bool TDataCollector::CanAddDefect(const TImageData * pImageData)
	{
		const adCheckOptions & check = m_pOptions->check;
		return 
			check.checkOnDefect == TRUE && pImageData->defect > AD_DEFECT_NONE &&
			pImageData->width >= (TUInt32)check.minimalImageSize && pImageData->width <= (TUInt32)check.maximalImageSize &&
			pImageData->height >= (TUInt32)check.minimalImageSize && pImageData->height <= (TUInt32)check.maximalImageSize;
	}
}
