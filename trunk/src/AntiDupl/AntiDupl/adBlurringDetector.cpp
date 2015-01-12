/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2015 Yermalayeu Ihar, 2013-2015 Borisov Dmitry.
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
	const double AD_EDGE_CROP = 0.1;
	const double AD_QUANTILE = 0.001;
	const size_t AD_LEVEL_COUNT = 6;
	const size_t AD_SIZE_MIN = 16;

    TBlurringDetector::TBlurringDetector()
    {
    }

    TBlurringDetector::~TBlurringDetector() 
    {
    }

	double TBlurringDetector::Detect(const TView & view) const
	{
		AD_FUNCTION_PERFORMANCE_TEST

		if(view.height < AD_SIZE_MIN || view.width < AD_SIZE_MIN)
			return 0.0;

		TLevels levels;

		InitLevels(view, levels);

		EstimateAbsSecondDerivativeHistograms(levels);

		double threshold = Threshold(levels);

		return Radius(levels, threshold);
	}

	void TBlurringDetector::InitLevels(const TView & view, TLevels & levels) const
	{
		TView cropped = view.Region(
			(ptrdiff_t)::floor(AD_EDGE_CROP*view.width),
			(ptrdiff_t)::floor(AD_EDGE_CROP*view.height),
			(ptrdiff_t)::ceil((1.0 - AD_EDGE_CROP)*view.width),
			(ptrdiff_t)::ceil((1.0 - AD_EDGE_CROP)*view.height));

		TPoint size = cropped.Size();
        levels.reserve(AD_LEVEL_COUNT);
        for(size_t i = 0; i < AD_LEVEL_COUNT; ++i)
        {
			levels.push_back(TLevel());
            levels[i].scale = 1 << i;
            if(i == 0)
                levels[i].view = cropped;
            else
                levels[i].view.Recreate(size, TView::Gray8);

			size = TPoint((size.x + 1) >> 1, (size.y + 1) >> 1);
			if(size.x < 3 || size.y < 3)
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
			levels[i].quantile = Quantile(levels[i].histogram, 1.0 - AD_QUANTILE);
        }
	}
	
	double TBlurringDetector::Quantile(const TUInt32 * histogram, double threshold) const
	{
		TUInt32 total = 0; 
		for(size_t i = 0; i < HISTOGRAM_SIZE; ++i)
			total += histogram[i];
		TUInt32 bound = TUInt32(::ceil(total*threshold));
		
		size_t index = 0;
		TUInt32 sum = 0;
		for(TUInt32 sum = 0; index < HISTOGRAM_SIZE && sum <= bound; ++index)
			sum += histogram[index];

		double last = double(sum - histogram[index])/total;
        double current = double(sum)/total;
        return index + (threshold - last)/(current - last);
	}

	double TBlurringDetector::Threshold(const TLevels & levels) const
	{
		size_t index = levels.size()/2;
		TView view = levels[index].view;
		TUInt32 histogram[HISTOGRAM_SIZE];
		Simd::Histogram(view, histogram);
		double range = Quantile(histogram, 1.0 - AD_QUANTILE) - Quantile(histogram, AD_QUANTILE);
		return Simd::Max(range * ::pow(64.0/range, 0.125) / 4.0, 16.0);
	}

	double TBlurringDetector::Radius(const TLevels & levels, double threshold) const
	{
		size_t transition = -1;
        for(size_t i = 0; i < levels.size(); ++i)
        {
            if(levels[i].quantile < threshold)
                transition = i;
            if(levels[i].quantile > threshold)
                break;
        }
        double radius = 0.0;
        if(transition != -1)
        {
            if(transition < levels.size() - 1)
            {
                double before = levels[transition].quantile;
                double after = levels[transition + 1].quantile;
                radius = double(levels[transition].scale*(after - threshold) + 
                    levels[transition + 1].scale*(threshold - before))/(after - before);
            }
            else
            {
                radius = levels.back().scale;
            }
        }
        else 
        {
            radius = threshold / levels[0].quantile;
        }
  		return radius;
	}
}
