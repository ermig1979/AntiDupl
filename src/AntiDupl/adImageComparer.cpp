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
#include <math.h>

#include "adEngine.h"
#include "adImageData.h"
#include "adOptions.h"
#include "adStatus.h"
#include "adResult.h"
#include "adResultStorage.h"
#include "adImageComparer.h"
#include "adImageDataStorage.h"

namespace ad
{
	const unsigned char FRAME_MASK_INDEX = 0xFF;

    //-------------------------------------------------------------------------

    TImageComparer::TImageComparer(TEngine *pEngine)
        :m_pOptions(pEngine->Options()),
        m_pResult(pEngine->Result()),
        m_pTransformedImageData(NULL),
        m_pBuffer(NULL),
        m_pMask(NULL) 
    {
		int thresholdPerPixel = Simd::Square(m_pOptions->compare.thresholdDifference*PIXEL_MAX_DIFFERENCE)/
			Simd::Square(DENOMINATOR);
        m_fastThreshold = FAST_DATA_SIZE*thresholdPerPixel;
        m_mainSize = Simd::Square(m_pOptions->advanced.reducedImageSize);
        
        if(m_pOptions->advanced.ignoreFrameWidth > 0)
        {
            int ignoreFrameWidth = m_pOptions->GetIgnoreWidthFrame();
            size_t effectiveMainSize = Simd::Square(m_pOptions->advanced.reducedImageSize - 2*ignoreFrameWidth);
            m_mainThreshold = int(effectiveMainSize*thresholdPerPixel);
            m_maxDifference = int(Simd::Square(PIXEL_MAX_DIFFERENCE)*effectiveMainSize);
            m_pMask = (TUInt8*)SimdAllocate(m_mainSize, SimdAlignment());
            memset(m_pMask, 0, m_mainSize);
            for(int row = ignoreFrameWidth; row < m_pOptions->advanced.reducedImageSize - ignoreFrameWidth; ++row)
            {
                for(int col = ignoreFrameWidth; col < m_pOptions->advanced.reducedImageSize - ignoreFrameWidth; ++col)
                {
                    m_pMask[row*m_pOptions->advanced.reducedImageSize + col] = FRAME_MASK_INDEX;
                }
            }
        }
        else
        {
            m_mainThreshold = int(m_mainSize*thresholdPerPixel);
            m_maxDifference = int(Simd::Square(PIXEL_MAX_DIFFERENCE)*m_mainSize);
        }

        if(m_pOptions->compare.transformedImage == TRUE)
        {
            m_pTransformedImageData = new TImageData(m_pOptions->advanced.reducedImageSize);
            m_pBuffer = (TUInt8*)SimdAllocate(m_mainSize + FAST_DATA_SIZE, SimdAlignment());
        }
    }

    TImageComparer::~TImageComparer()
    {
        if(m_pOptions->advanced.ignoreFrameWidth > 0)
        {
            SimdFree(m_pMask);
        }

        if(m_pOptions->compare.transformedImage == TRUE)
        {
            delete m_pTransformedImageData;
            SimdFree(m_pBuffer); 
        }
    }

    void TImageComparer::Accept(TImageDataPtr pImageData, bool add)
    {
        Compare(pImageData, pImageData, AD_TRANSFORM_TURN_0);
        if(m_pOptions->compare.transformedImage == TRUE)
        {
            *m_pTransformedImageData = *pImageData;
            for(int i_transform = AD_TRANSFORM_TURN_90; i_transform < AD_TRANSFORM_SIZE; i_transform++)
            {
                m_pTransformedImageData->Turn(m_pBuffer);
                if(i_transform == AD_TRANSFORM_MIRROR_TURN_0)
                    m_pTransformedImageData->Mirror(m_pBuffer);
                Compare(pImageData, m_pTransformedImageData, (adTransformType)i_transform);
            }
        }
        if(add)
            Add(pImageData);
    }

	// Переданное изображение свравнивается с набором проверенных и остальных.
	// pOriginal - оригинальное изображение.
	// pTransformed - трансформированное, если применяется трансформация или то же что и оригинальное.
    void TImageComparer::CompareWithSet(const Set &set, TImageDataPtr pOriginal, TImageDataPtr pTransformed, adTransformType transform)
    {
        double difference;
		// Если картинка не в проверенных
        if(!pTransformed->valid)
        {
			// Сравниваем с набором проверенных
            for(TImageDataPtrList::const_iterator i = set.valid.begin(); i != set.valid.end(); ++i)
            {
                if(IsDuplPair(pTransformed, *i, &difference))
                    m_pResult->AddDuplImagePair(pOriginal, *i, difference, transform);
            }
        }
		// Сравниваем с набором остальных
		for(TImageDataPtrList::const_iterator i = set.other.begin(); i != set.other.end(); ++i)
		{
			if(IsDuplPair(pTransformed, *i, &difference))
				m_pResult->AddDuplImagePair(pOriginal, *i, difference, transform);
		}
	}

	void TImageComparer::AddToSet(Set &set, TImageDataPtr pImageData)
	{
		if(pImageData->valid)
			set.valid.push_back(pImageData);
		else
			set.other.push_back(pImageData);
    }

	// Сравнение двух картинок
	bool TImageComparer::IsDuplPair(TImageDataPtr pFirst, TImageDataPtr pSecond, double *pDifference)
	{
		if(m_pOptions->compare.typeControl == TRUE && 
			pFirst->type != pSecond->type)
			return false;

		if(m_pOptions->compare.sizeControl == TRUE &&
			(pFirst->height != pSecond->height || 
			pFirst->width != pSecond->width))
			return false;

		if(m_pOptions->compare.ratioControl == TRUE)
		{
			if(Simd::Square(pFirst->ratio - pSecond->ratio) > Simd::Square(RATIO_THRESHOLD_DIFFERENCE))
				return false;
		}

		if(m_pOptions->compare.compareInsideOneFolder == FALSE && TPath::EqualByDirectory(pFirst->path, pSecond->path))
			return false;

		if(m_pOptions->compare.compareInsideOneSearchPath == FALSE && pFirst->index == pSecond->index)
			return false;

		uint64_t fastDifference = 0;
		SimdSquaredDifferenceSum(pFirst->data->fast, FAST_DATA_SIZE, pSecond->data->fast, FAST_DATA_SIZE, 
			FAST_DATA_SIZE, 1, &fastDifference);
		if(fastDifference > m_fastThreshold)
			return false;

        uint64_t mainDifference = 0;
        if(m_pOptions->advanced.ignoreFrameWidth > 0)
        {
            SimdSquaredDifferenceSumMasked(pFirst->data->main, m_mainSize, pSecond->data->main, m_mainSize, 
				m_pMask, m_mainSize, FRAME_MASK_INDEX, m_mainSize, 1, &mainDifference);
        }
        else
        {
			SimdSquaredDifferenceSum(pFirst->data->main, m_mainSize, pSecond->data->main, m_mainSize, 
				m_mainSize, 1, &mainDifference);
        }
        if(mainDifference > m_mainThreshold)
            return false;

        *pDifference = sqrt(double(mainDifference)/m_maxDifference)*100;
        if(pFirst->crc32c != pSecond->crc32c)
            *pDifference += ADDITIONAL_DIFFERENCE_FOR_DIFFERENT_CRC32;
        return true;
    }
    //-------------------------------------------------------------------------
    TImageComparer_0D::TImageComparer_0D(TEngine *pEngine)
        :TImageComparer(pEngine)
    {
        m_sets.resize(1);
    }

    void TImageComparer_0D::Add(TImageDataPtr pImageData)
    {
        AddToSet(m_sets[0], pImageData);
    }

    void TImageComparer_0D::Compare(TImageDataPtr pOriginal, TImageDataPtr pTransformed, adTransformType transform)
    {
        CompareWithSet(m_sets[0], pOriginal, pTransformed, transform);
    }
    //-------------------------------------------------------------------------
    TImageComparer_1D::TImageComparer_1D(TEngine *pEngine)
        :TImageComparer(pEngine)
    {
        m_sets.resize(RANGE);
        m_halfCompareRange = (int)ceil(0.5 + double(RANGE)*m_pOptions->compare.thresholdDifference/DENOMINATOR);
    }

    void TImageComparer_1D::Add(TImageDataPtr pImageData)
    {
        AddToSet(m_sets[GetIndex(pImageData)], pImageData);
    }

    void TImageComparer_1D::Compare(TImageDataPtr pOriginal, TImageDataPtr pTransformed, adTransformType transform)
    {
        int index = GetIndex(pTransformed);
        for(int i = std::max(0, index - m_halfCompareRange), end = std::min(index + m_halfCompareRange, RANGE); i < end; ++i)
            CompareWithSet(m_sets[i], pOriginal, pTransformed, transform);
    }

    int TImageComparer_1D::GetIndex(TImageDataPtr pImageData)
    {
        int sum = 8;
        for(int i = 0; i < FAST_DATA_SIZE; ++i)
            sum += pImageData->data->fast[i];
        return sum >> 4;
    }
    //-------------------------------------------------------------------------
    TImageComparer_3D::TImageComparer_3D(TEngine *pEngine)
        :TImageComparer(pEngine)
    {
        const int MAX_RANGES[] = {48, 48, 48, 48, 48, 48, 40, 32, 28, 24, 24};
        m_maxRange = MAX_RANGES[m_pOptions->compare.thresholdDifference/D3_MAX_RANGES_STEP];

        m_shift.s = 0;
        m_shift.x = m_maxRange >> 2;
        m_shift.y = m_maxRange >> 2;

        m_range.s = m_maxRange;
        m_range.x = m_maxRange >> 1;
        m_range.y = m_maxRange >> 1;

        m_stride.s = m_range.x*m_range.y;
        m_stride.x = m_range.y;
        m_stride.y = 1;

        m_sets.resize(m_range.s*m_range.x*m_range.y);
        m_halfCompareRange = (int)ceil(0.5 + double(m_maxRange)*m_pOptions->compare.thresholdDifference/DENOMINATOR);
    }

    void TImageComparer_3D::Add(TImageDataPtr pImageData)
    {
        TIndex i;
        GetIndex(pImageData, i);
        AddToSet(m_sets[i.s*m_stride.s + i.x*m_stride.x + i.y*m_stride.y], pImageData);
    }

    void TImageComparer_3D::Compare(TImageDataPtr pOriginal, TImageDataPtr pTransformed, adTransformType transform)
    {
        TIndex i, lo, hi;
        GetIndex(pTransformed, i);

        lo.s = std::max(0, i.s - m_halfCompareRange)*m_stride.s;
        lo.x = std::max(0, i.x - m_halfCompareRange)*m_stride.x;
        lo.y = std::max(0, i.y - m_halfCompareRange)*m_stride.y;

        hi.s = std::min(m_range.s, i.s + m_halfCompareRange)*m_stride.s;
        hi.x = std::min(m_range.x, i.x + m_halfCompareRange)*m_stride.x;
        hi.y = std::min(m_range.y, i.y + m_halfCompareRange)*m_stride.y;

        for(int s = lo.s; s < hi.s; s += m_stride.s)
        {
            for(int x = lo.x; x < hi.x; x += m_stride.x)
            {
                for(int y = lo.y; y < hi.y; y += m_stride.y)
                {
                    CompareWithSet(m_sets[s + x + y], pOriginal, pTransformed, transform);
                }
            }
        }
    }

    void TImageComparer_3D::GetIndex(TImageDataPtr pImageData, TIndex& index)
    {
        unsigned char *p = pImageData->data->fast;
        int s[2][2];
        s[0][0] = p[0x0] + p[0x1] + p[0x4] + p[0x5];
        s[0][1] = p[0x2] + p[0x3] + p[0x6] + p[0x7];
        s[1][0] = p[0x8] + p[0x9] + p[0xC] + p[0xD];
        s[1][1] = p[0xA] + p[0xB] + p[0xE] + p[0xF];

        index.s = (s[0][0] + s[0][1] + s[1][0] + s[1][1])*m_maxRange >> 12;
        index.x = (s[0][0] - s[0][1] + s[1][0] - s[1][1] + 0x7FF)*m_maxRange >> 12;
        index.y = (s[0][0] + s[0][1] - s[1][0] - s[1][1] + 0x7FF)*m_maxRange >> 12;

        index.s = std::max(0, std::min(m_range.s - 1, index.s - m_shift.s));
        index.x = std::max(0, std::min(m_range.x - 1, index.x - m_shift.x));
        index.y = std::max(0, std::min(m_range.y - 1, index.y - m_shift.y));
    }
	//-------------------------------------------------------------------------
    TImageComparer_SSIM::TImageComparer_SSIM(TEngine *pEngine)
        :TImageComparer(pEngine),
		m_pImageDataStorage(pEngine->ImageDataStorage())
    {
		//константы
		C1 = (float)pow(0.01 * PIXEL_MAX_DIFFERENCE, 2);
        C2 = (float)pow(0.03 * PIXEL_MAX_DIFFERENCE, 2);
        m_sets.resize(1);
    }

    void TImageComparer_SSIM::Add(TImageDataPtr pImageData)
    {
        AddToSet(m_sets[0], pImageData);
    }

    void TImageComparer_SSIM::Compare(TImageDataPtr pOriginal, TImageDataPtr pTransformed, adTransformType transform)
    {
        CompareWithSet(m_sets[0], pOriginal, pTransformed, transform);
    }

	// Сравнение двух картинок SSIM методом
    bool TImageComparer_SSIM::IsDuplPair(TImageDataPtr pFirst, TImageDataPtr pSecond, double *pDifference)
    {
        if(m_pOptions->compare.typeControl == TRUE && 
            pFirst->type != pSecond->type)
            return false;

        if(m_pOptions->compare.sizeControl == TRUE &&
            (pFirst->height != pSecond->height || 
            pFirst->width != pSecond->width))
            return false;

        if(m_pOptions->compare.ratioControl == TRUE)
        {
            if(Simd::Square(pFirst->ratio - pSecond->ratio) > Simd::Square(RATIO_THRESHOLD_DIFFERENCE))
                return false;
        }

        if(m_pOptions->compare.compareInsideOneFolder == FALSE && TPath::EqualByDirectory(pFirst->path, pSecond->path))
            return false;

		if(m_pOptions->compare.compareInsideOneSearchPath == FALSE && pFirst->index == pSecond->index)
            return false;

		if (pFirst->data->average == 0)
		{
			uint64_t sum = 0;
			SimdValueSum(pFirst->data->main,  m_pOptions->advanced.reducedImageSize, 
					m_pOptions->advanced.reducedImageSize, m_pOptions->advanced.reducedImageSize, &sum);
			pFirst->data->average = (float)sum / (m_pOptions->advanced.reducedImageSize * m_pOptions->advanced.reducedImageSize);
			m_pImageDataStorage->SetSaveState(true);
		}
		if (pSecond->data->average == 0)
		{
			uint64_t sum = 0;
			SimdValueSum(pSecond->data->main,  m_pOptions->advanced.reducedImageSize, 
					m_pOptions->advanced.reducedImageSize, m_pOptions->advanced.reducedImageSize, &sum);
			pSecond->data->average = (float)sum / (m_pOptions->advanced.reducedImageSize * m_pOptions->advanced.reducedImageSize);
			m_pImageDataStorage->SetSaveState(true);
		}

		if (pFirst->data->varianceSquare == 0)
		{
			uint64_t sumSquare = 0;
			SimdSquareSum(pFirst->data->main,  m_pOptions->advanced.reducedImageSize, 
						m_pOptions->advanced.reducedImageSize, m_pOptions->advanced.reducedImageSize, &sumSquare);
			float averageSquare = (float)sumSquare / (m_pOptions->advanced.reducedImageSize * m_pOptions->advanced.reducedImageSize);
			pFirst->data->varianceSquare = fabs(averageSquare - (pFirst->data->average * pFirst->data->average));
			m_pImageDataStorage->SetSaveState(true);
		}

		if (pSecond->data->varianceSquare == 0)
		{
			uint64_t sumSquare = 0;
			SimdSquareSum(pSecond->data->main,  m_pOptions->advanced.reducedImageSize, 
						m_pOptions->advanced.reducedImageSize, m_pOptions->advanced.reducedImageSize, &sumSquare);
			float averageSquareSecond = (float)sumSquare / (m_pOptions->advanced.reducedImageSize * m_pOptions->advanced.reducedImageSize);
			pSecond->data->varianceSquare = fabs(averageSquareSecond - (pSecond->data->average * pSecond->data->average));
			m_pImageDataStorage->SetSaveState(true);
		}

        uint64_t correlationSum = 0;
        SimdCorrelationSum(
            pFirst->data->main, m_pOptions->advanced.reducedImageSize, 
            pSecond->data->main, m_pOptions->advanced.reducedImageSize, 
            m_pOptions->advanced.reducedImageSize, m_pOptions->advanced.reducedImageSize, &correlationSum);
        float sigmaOfBoth = (float)correlationSum/(m_pOptions->advanced.reducedImageSize * m_pOptions->advanced.reducedImageSize) - 
            pFirst->data->average*pSecond->data->average;

		float res = (2 * pFirst->data->average * pSecond->data->average + C1) * (2 * sigmaOfBoth + C2) / 
			((pow(pFirst->data->average, 2) + pow(pSecond->data->average, 2) + C1) * 
			(pFirst->data->varianceSquare + pSecond->data->varianceSquare + C2));  

		if (res > 2 || res < -2) // может быть равным 1.0000000594991703 и должно быть от 0 до 1
			return false;

		double difference = 100 - (res * 100);
		if (difference < 0)
			difference = 0;
		// если различие больше заданного, то не дубликаты
		if (difference > m_pOptions->compare.thresholdDifference)
			return false;

		*pDifference = difference;
        if(pFirst->crc32c != pSecond->crc32c)
            *pDifference += ADDITIONAL_DIFFERENCE_FOR_DIFFERENT_CRC32;
        return true;
    }
    //-------------------------------------------------------------------------
	// Фабрика возврашает движок
    TImageComparer* CreateImageComparer(TEngine *pEngine)
    {
		if (pEngine->Options()->compare.algorithmComparing == AD_COMPARING_SQUARED_SUM)
		{
			adStatistic statistic;
			pEngine->Status()->Export(&statistic);
			if(statistic.searchedImageNumber < D0_SEARCHED_FILE_NUMBER_MAX)
			{
				return new TImageComparer_0D(pEngine);
			}
			else if(statistic.searchedImageNumber < D1_SEARCHED_FILE_NUMBER_MAX || 
				pEngine->Options()->compare.thresholdDifference > D3_THRESHOLD_DIFFERENCE_MAX)
			{
				return new TImageComparer_1D(pEngine);
			} 
			else
			{
				return new TImageComparer_3D(pEngine);
			}
		}
		else
			return new TImageComparer_SSIM(pEngine);
    }
    //-------------------------------------------------------------------------
}
