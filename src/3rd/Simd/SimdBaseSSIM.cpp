/*
* Simd Library (http://simd.sourceforge.net).
*
* Copyright (c) 2011-2015 Yermalayeu Ihar, 2015 Borisov Dmitry.
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
#include "Simd/SimdBase.h"

namespace Simd
{
	namespace Base
	{
		void SigmaDouble(const uint8_t * srcFirst, size_t strideFirst, const uint8_t * srcSecond, size_t strideSecond, size_t width, size_t height, const float averageFirst, const float averageSecond, float * sigmaOfBoth)
		{
			assert(width < 0x10000);

			float sum = 0;
			for(size_t row = 0; row < height; ++row)
			{
				float rowSum = 0;
				for(size_t col = 0; col < width; ++col)
				{
					rowSum += ((srcFirst[col] - averageFirst) * (srcSecond [col] - averageSecond));
				}
				sum += rowSum;
				srcFirst += strideFirst;
				srcSecond += strideSecond;
			}

			*sigmaOfBoth = sum / (width * height);
		}
	}
}