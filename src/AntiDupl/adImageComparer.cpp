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
                if(IsDuplPairAny(pTransformed, *i, &difference))
                    m_pResult->AddDuplImagePair(pOriginal, *i, difference, transform);
            }
        }
		// Сравниваем с набором остальных
		for(TImageDataPtrList::const_iterator i = set.other.begin(); i != set.other.end(); ++i)
		{
			if(IsDuplPairAny(pTransformed, *i, &difference))
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

        double difference = PixelDifference(pFirst, pSecond, sqrt(double(mainDifference)/m_maxDifference)*100);
        if (difference > m_pOptions->compare.thresholdDifference)
            return false;
        *pDifference = difference;
        if(pFirst->crc32c != pSecond->crc32c)
            *pDifference += ADDITIONAL_DIFFERENCE_FOR_DIFFERENT_CRC32;
        return true;
    }

    //Common checks of an image pair before the difference calculation (type, size, ratio, paths).
    bool TImageComparer::PassControls(TImageDataPtr pFirst, TImageDataPtr pSecond)
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

        return true;
    }

    //Absolute value of an int.
    static int AbsInt(int value)
    {
        return value < 0 ? -value : value;
    }

    //Squared difference of two images on the overlap area at shift (dx, dy): pFirst[y][x] ~ pSecond[y + dy][x + dx].
    static uint64_t ShiftedSquaredDifference(const TUInt8 *pFirst, const TUInt8 *pSecond, size_t side, 
        int dx, int dy, int x0, int y0, int width, int height)
    {
        uint64_t sum = 0, row = 0;
        for(int y = y0; y < y0 + height; ++y)
        {
            SimdSquaredDifferenceSum(pFirst + y*side + x0, side, pSecond + (y + dy)*side + x0 + dx, side, width, 1, &row);
            sum += row;
        }
        return sum;
    }

    //Search of a nonzero shift of the second reduced image relative to the first one.
    //Two-stage search on the reduced images: a strided global scan over the whole admissible range locates a rough minimum, then a dense window of one stride around it refines the shift to pixel precision. Both stages keep only candidates whose area-normalized squared difference passes the relaxed threshold and whose overlap covers at least a half of the image.
    //Returns false if no suitable nonzero shift is found.
    bool TImageComparer::FindShift(TImageDataPtr pFirst, TImageDataPtr pSecond, int &dx, int &dy)
    {
        const size_t side = m_pOptions->advanced.reducedImageSize;
        const int maxShift = int(side)/2;
        const int stride = std::max(2, int(side)/8);
        const uint64_t mainThreshold = uint64_t(SHIFT_COARSE_FACTOR)*m_mainThreshold;
        const size_t minArea = Simd::Square(side)/2;

        int bestDx = 0, bestDy = 0;
        uint64_t bestMain = uint64_t(-1);
        for(int pass = 0; pass < 2; ++pass)
        {
            const int centerDx = pass == 0 ? 0 : bestDx;
            const int centerDy = pass == 0 ? 0 : bestDy;
            const int border = pass == 0 ? maxShift : stride;
            const int step = pass == 0 ? stride : 1;
            uint64_t passBest = bestMain; //keep the previous stage result as the fallback
            int passDx = bestDx, passDy = bestDy;
            for(int sy = centerDy - border; sy <= centerDy + border; sy += step)
            {
                for(int sx = centerDx - border; sx <= centerDx + border; sx += step)
                {
                    if(sx == 0 && sy == 0)
                        continue;
                    if(AbsInt(sx) > maxShift || AbsInt(sy) > maxShift)
                        continue;
                    int x0 = std::max(0, -sx);
                    int y0 = std::max(0, -sy);
                    int width = int(side) - AbsInt(sx);
                    int height = int(side) - AbsInt(sy);
                    if(size_t(width)*size_t(height) < minArea)
                        continue;
                    uint64_t difference = ShiftedSquaredDifference(pFirst->data->main, pSecond->data->main, side, sx, sy, x0, y0, width, height);
                    if(difference*size_t(side)*size_t(side) > mainThreshold*size_t(width)*size_t(height))
                        continue;
                    if(difference < passBest)
                    {
                        passBest = difference;
                        passDx = sx;
                        passDy = sy;
                    }
                }
            }
            if(passBest == uint64_t(-1))
                return false;
            bestMain = passBest;
            bestDx = passDx;
            bestDy = passDy;
        }

        dx = bestDx;
        dy = bestDy;
        return true;
    }

    //Comparison of images with shift compensation: the shift minimizing the difference is found, and the difference is computed on the overlap area normalized by its area.
    bool TImageComparer::ShiftedDifference(TImageDataPtr pFirst, TImageDataPtr pSecond, double *pDifference)
    {
        int dx, dy;
        if(!FindShift(pFirst, pSecond, dx, dy))
            return false;

        const size_t side = m_pOptions->advanced.reducedImageSize;
        int frame = m_pOptions->GetIgnoreWidthFrame();

        //Overlap area in coordinates of the first image, taking the ignored frame into account.
        int x0 = std::max(frame, frame - dx);
        int y0 = std::max(frame, frame - dy);
        int x1 = std::min(int(side) - frame, int(side) - frame - dx);
        int y1 = std::min(int(side) - frame, int(side) - frame - dy);
        int width = x1 - x0, height = y1 - y0;
        if(width <= 0 || height <= 0)
            return false;

        uint64_t sum = 0, row = 0;
        for(int y = y0; y < y1; ++y)
        {
            SimdSquaredDifferenceSum(pFirst->data->main + y*side + x0, side,
                pSecond->data->main + (y + dy)*side + x0 + dx, side, width, 1, &row);
            sum += row;
        }

        double difference = PixelDifference(pFirst, pSecond,
            sqrt(double(sum)/(Simd::Square(PIXEL_MAX_DIFFERENCE)*double(width)*double(height)))*100);
        if(difference > m_pOptions->compare.thresholdDifference)
            return false;
        *pDifference = difference;
        return true;
    }

    //Comparison of an image pair without shift and (if the option is enabled) with shift compensation.
    //When the option is enabled, the smaller of the two differences is chosen, so that the compensation improves the estimate even when the images match without any shift.
    bool TImageComparer::IsDuplPairAny(TImageDataPtr pFirst, TImageDataPtr pSecond, double *pDifference)
    {
        bool matched = IsDuplPair(pFirst, pSecond, pDifference);
        if(m_pOptions->compare.shiftedImage == TRUE &&
            m_pOptions->compare.checkOnEquality == TRUE &&
            PassControls(pFirst, pSecond))
        {
            double shiftedDifference;
            if(ShiftedDifference(pFirst, pSecond, &shiftedDifference))
            {
                if(pFirst->crc32c != pSecond->crc32c)
                    shiftedDifference += ADDITIONAL_DIFFERENCE_FOR_DIFFERENT_CRC32;
                if(!matched || shiftedDifference < *pDifference)
                {
                    *pDifference = shiftedDifference;
                    matched = true;
                }
            }
        }
        return matched;
    }

    double TImageComparer::PixelDifference(TImageDataPtr pFirst, TImageDataPtr pSecond, double metric)
    {
        if (pFirst->data->pixelHash != 0 && pFirst->data->pixelHash == pSecond->data->pixelHash &&
            pFirst->width == pSecond->width && pFirst->height == pSecond->height)
            return 0;
        return std::max(metric, MIN_DIFFERENCE_OF_DIFFERENT_PIXELS);
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
        :TImageComparer(pEngine)
    {
		//константы
		C1 = pow(0.01 * PIXEL_MAX_DIFFERENCE, 2);
        C2 = pow(0.03 * PIXEL_MAX_DIFFERENCE, 2);
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

        const size_t side = pFirst->data->side;
        uint64_t correlationSum = 0;
        SimdCorrelationSum(pFirst->data->main, side, pSecond->data->main, side, side, side, &correlationSum);

        double difference;
        // Sum(a*b) == Sum(a*a) == Sum(b*b) only for equal planes, since then
        // Sum((a-b)^2) is 0. The integer test makes equal planes exactly 0,
        // which floating point rounding (and /fp:fast) cannot promise.
        if (correlationSum == pFirst->data->sumSquare && correlationSum == pSecond->data->sumSquare)
            difference = 0;
        else
        {
            const double n = double(pFirst->data->size);
            const double average1 = double(pFirst->data->sum) / n;
            const double average2 = double(pSecond->data->sum) / n;
            const double variance1 = double(pFirst->data->sumSquare) / n - average1 * average1;
            const double variance2 = double(pSecond->data->sumSquare) / n - average2 * average2;
            const double covariance = double(correlationSum) / n - average1 * average2;
            const double ssim = (2 * average1 * average2 + C1) * (2 * covariance + C2) /
                ((average1 * average1 + average2 * average2 + C1) * (variance1 + variance2 + C2));
            difference = 100 * (1 - ssim);
            if (difference < 0)
                difference = 0;
        }
        difference = PixelDifference(pFirst, pSecond, difference);
		// если различие больше заданного, то не дубликаты
		if (difference > m_pOptions->compare.thresholdDifference)
			return false;

		*pDifference = difference;
        if(pFirst->crc32c != pSecond->crc32c)
            *pDifference += ADDITIONAL_DIFFERENCE_FOR_DIFFERENT_CRC32;
        return true;
    }

	// Comparison of two images by the SSIM method with shift compensation on the overlap area.
    bool TImageComparer_SSIM::ShiftedDifference(TImageDataPtr pFirst, TImageDataPtr pSecond, double *pDifference)
    {
        int dx, dy;
        if(!FindShift(pFirst, pSecond, dx, dy))
            return false;

        const size_t side = m_pOptions->advanced.reducedImageSize;
        int frame = m_pOptions->GetIgnoreWidthFrame();

        int x0 = std::max(frame, frame - dx);
        int y0 = std::max(frame, frame - dy);
        int x1 = std::min(int(side) - frame, int(side) - frame - dx);
        int y1 = std::min(int(side) - frame, int(side) - frame - dy);
        int width = x1 - x0, height = y1 - y0;
        if(width <= 0 || height <= 0)
            return false;
        float area = float(width*height);

        uint64_t sum1 = 0, sum2 = 0, squareSum1 = 0, squareSum2 = 0, correlationSum = 0, row = 0;
        for(int y = y0; y < y1; ++y)
        {
            const TUInt8 *pRow1 = pFirst->data->main + y*side + x0;
            const TUInt8 *pRow2 = pSecond->data->main + (y + dy)*side + x0 + dx;
            SimdValueSum(pRow1, side, width, 1, &row);       sum1 += row;
            SimdValueSum(pRow2, side, width, 1, &row);       sum2 += row;
            SimdSquareSum(pRow1, side, width, 1, &row);      squareSum1 += row;
            SimdSquareSum(pRow2, side, width, 1, &row);      squareSum2 += row;
            SimdCorrelationSum(pRow1, side, pRow2, side, width, 1, &row); correlationSum += row;
        }

        float average1 = float(sum1)/area;
        float average2 = float(sum2)/area;
        float varianceSquare1 = fabs(float(squareSum1)/area - average1*average1);
        float varianceSquare2 = fabs(float(squareSum2)/area - average2*average2);
        float sigmaOfBoth = float(correlationSum)/area - average1*average2;

        float res = (2*average1*average2 + C1)*(2*sigmaOfBoth + C2)/
            ((average1*average1 + average2*average2 + C1)*(varianceSquare1 + varianceSquare2 + C2));
        if(res > 2 || res < -2)
            return false;

        double difference = 100 - double(res)*100;
        if(difference < 0)
            difference = 0;
        difference = PixelDifference(pFirst, pSecond, difference);
        if(difference > m_pOptions->compare.thresholdDifference)
            return false;

        *pDifference = difference;
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
