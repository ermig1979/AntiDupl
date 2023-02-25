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
#include "adBlurringDetector.h"

namespace ad
{
	const double AD_BLURRING_DETECTOR_EDGE_CROP = 0.1;
	const double AD_BLURRING_DETECTOR_QUANTILE = 0.001;
	const size_t AD_BLURRING_DETECTOR_LEVEL_COUNT_MAX = 16;
	const size_t AD_BLURRING_DETECTOR_SIZE_MIN = 64;
    const size_t AD_BLURRING_DETECTOR_AREA_MIN = 256;
    const double AD_BLURRING_DETECTOR_RANGE_THRESOLD = 64.0;
    const double AD_BLURRING_DETECTOR_RANGE_MIN = 16.0;

    TBlurringDetector::TBlurringDetector()
    {
    }

    TBlurringDetector::~TBlurringDetector() 
    {
    }

	double TBlurringDetector::Detect(const TView & view) const
	{
		AD_FUNCTION_PERFORMANCE_TEST

		if(view.height < AD_BLURRING_DETECTOR_SIZE_MIN || view.width < AD_BLURRING_DETECTOR_SIZE_MIN)
			return 0.0;

		TLevels levels;

		InitLevels(view, levels);

		EstimateAbsSecondDerivativeHistograms(levels);

        double range = Range(levels);

		double threshold = Threshold(range);

        double radius = Radius(levels, range, threshold);

        return radius;
	}

	void TBlurringDetector::InitLevels(const TView & view, TLevels & levels) const
	{
		TView cropped = view.Region(
			(ptrdiff_t)::floor(AD_BLURRING_DETECTOR_EDGE_CROP*view.width),
			(ptrdiff_t)::floor(AD_BLURRING_DETECTOR_EDGE_CROP*view.height),
			(ptrdiff_t)::ceil((1.0 - AD_BLURRING_DETECTOR_EDGE_CROP)*view.width),
			(ptrdiff_t)::ceil((1.0 - AD_BLURRING_DETECTOR_EDGE_CROP)*view.height));

		TPoint size = cropped.Size();
        levels.reserve(AD_BLURRING_DETECTOR_LEVEL_COUNT_MAX);
        for(size_t i = 0; i < AD_BLURRING_DETECTOR_LEVEL_COUNT_MAX; ++i)
        {
			levels.push_back(TLevel());
            levels[i].scale = 1 << i;
            if(i == 0)
                levels[i].view = cropped;
            else
                levels[i].view.Recreate(size, TView::Gray8);

			size = TPoint((size.x + 1) >> 1, (size.y + 1) >> 1); //делим размеры на 2
			if(size.x < 3 || size.y < 3 || size.x*size.y < AD_BLURRING_DETECTOR_AREA_MIN)
				break;
        }
	}

	void TBlurringDetector::EstimateAbsSecondDerivativeHistograms(TLevels & levels) const
	{
		for(size_t i = 0; i < levels.size(); ++i)
        {
            if(i > 0)
                Simd::ReduceGray2x2(levels[i - 1].view, levels[i].view);
            Simd::AbsSecondDerivativeHistogram(levels[i].view, 1, 1, levels[i].histogram);
			levels[i].quantile = Quantile(levels[i].histogram, 1.0 - AD_BLURRING_DETECTOR_QUANTILE);
        }
	}
	
	//Обратная функция распределения (Какое количество пикселей лежит в заданном пороге)
	double TBlurringDetector::Quantile(const TUInt32 * histogram, double threshold) const
	{
		TUInt32 total = 0; 
		for(size_t i = 0; i < HISTOGRAM_SIZE; ++i)
			total += histogram[i];
        TUInt32 bound = TUInt32(threshold < 0.5 ? ::ceil(total*threshold) : ::floor(total*threshold));
		
		size_t index = 0;
		TUInt32 sum = 0;
		for(; index < HISTOGRAM_SIZE && sum <= bound; ++index)
			sum += histogram[index];
        index--;

		double last = double(sum - histogram[index])/total;
        double current = double(sum)/total;
        return index + (threshold - last)/(current - last);
	}

    double TBlurringDetector::Range(const TLevels & levels) const
    {
        size_t index = levels.size()/3;
        TView view = levels[index].view;
        TUInt32 histogram[HISTOGRAM_SIZE];
        Simd::Histogram(view, histogram);
        return Quantile(histogram, 1.0 - AD_BLURRING_DETECTOR_QUANTILE) - 
            Quantile(histogram, AD_BLURRING_DETECTOR_QUANTILE);
    }

	double TBlurringDetector::Threshold(double range) const
	{
        return range * ::pow(AD_BLURRING_DETECTOR_RANGE_THRESOLD/range, 
            range < AD_BLURRING_DETECTOR_RANGE_THRESOLD ? -0.125 : 0.125) / 6.0;
	}

	double TBlurringDetector::Radius(const TLevels & levels, double range, double threshold) const
	{
        size_t firstMinimum = -1;
        for(size_t i = 0; i < levels.size() - 1; ++i)
        {
            if(levels[i].quantile < levels[i + 1].quantile)
            {
                firstMinimum = i;
                break;
            }
        }

        double radius = 0.0;
        if(firstMinimum != -1)
        {
            size_t transition = -1;
            for(size_t i = firstMinimum; i < levels.size(); ++i)
            {
                if(levels[i].quantile < threshold)
                    transition = i;
                if(levels[i].quantile > threshold)
                    break;
            }
            if(transition != -1)
            {
                if(transition < levels.size() - 1)
                {
                    double before = levels[transition].quantile;
                    double after = levels[transition + 1].quantile;
                    radius = double(levels[transition].scale*(after - threshold) + 
                        levels[transition + 1].scale*(threshold - before))/(after - before);
                }
            }
            else if(levels[firstMinimum].quantile < threshold)
            {
                if(firstMinimum == 0)
                {
                    radius = threshold / levels[0].quantile;
                }
                else
                {
                    double y0 = levels[firstMinimum - 1].quantile;
                    double y1 = levels[firstMinimum].quantile;
                    double y2 = levels[firstMinimum + 1].quantile;
                    double x = -(y2 - y0)/(y2 + y0 - 2*y1)/2.0;
                    if(x > 0)
                        radius = levels[firstMinimum].scale*(1 - x) + levels[firstMinimum + 1].scale*x;
                    else
                        radius = levels[firstMinimum].scale*(1 + x) - levels[firstMinimum - 1].scale*x;
                }
            }
			else
			{
				radius = threshold / levels[0].quantile;
			}
        }

        return range >= AD_BLURRING_DETECTOR_RANGE_MIN ? radius : levels.back().scale;
	}
}
