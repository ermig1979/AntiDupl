/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2015 Yermalayeu Ihar.
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
#ifndef __adBlurringDetector_h__
#define __adBlurringDetector_h__

#include "adConfig.h"

namespace ad
{
    class TBlurringDetector
    {
		struct TLevel
		{
			int scale;
            TView view;
            TUInt32 histogram[HISTOGRAM_SIZE];
            double quantile;
		};
		typedef std::vector<TLevel> TLevels;

    public:
        TBlurringDetector();
        ~TBlurringDetector();

        double Detect(const TView & view) const;

    private:
		void InitLevels(const TView & view, TLevels & levels) const;
		void EstimateAbsSecondDerivativeHistograms(TLevels & levels) const;
		double Quantile(const TUInt32 * histogram, double threshold) const;
        double Range(const TLevels & levels) const;
		double Threshold(double range) const;
        double Radius(const TLevels & levels, double range, double threshold) const;
	};
}
#endif//__adBlurringDetector_h__ 