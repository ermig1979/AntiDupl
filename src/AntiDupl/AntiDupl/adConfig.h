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
#ifndef __adConfig_h__
#define __adConfig_h__

#include "AntiDupl.h"

#define NOMINMAX
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <memory.h>

#include <vector>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <string>
#include <sstream>
#include <iostream>
#include <exception>
#include <algorithm>
#include <limits>

#define SIMD_STATIC
#include "Simd/SimdUtils.h"
#include "Simd/SimdConversion.h"

//#define AD_PERFORMANCE_TEST_ENABLE
//#define AD_LOGGER_ENABLE

#define AD_INLINE SIMD_INLINE

namespace ad
{
    //-------------------------------------------------------------------------

    typedef adInt8 TInt8;
    typedef adInt16 TInt16;
    typedef adInt32 TInt32;
    typedef adInt64 TInt64;

    typedef adUInt8 TUInt8;
    typedef adUInt16 TUInt16;
    typedef adUInt32 TUInt32;
    typedef adUInt64 TUInt64;

    typedef adChar TChar;
    typedef adSize TSize;

    typedef adStateType TStateType;
    typedef adImageType TImageType;
    typedef adResultType TResultType;
    typedef adDefectType TDefectType;
    typedef adTransformType TTransformType;
    typedef adHintType THintType; 
    typedef adSortType TSortType;
    typedef adThreadType TThreadType;

	typedef std::basic_stringstream<adChar, std::char_traits<adChar>, std::allocator<adChar> > TStringStream;

	typedef Simd::View TView;
	typedef Simd::Point<ptrdiff_t> TPoint;

    //-------------------------------------------------------------------------

    const TUInt32 DENOMINATOR = 100;
    const TUInt32 PIXEL_MAX_DIFFERENCE = 0xFF;
    const TUInt32 FAST_IMAGE_SIZE = 4;
    const size_t FAST_DATA_SIZE = FAST_IMAGE_SIZE*FAST_IMAGE_SIZE;
    const TUInt8 JPEG_FILE_MARKER = 0xFF;
    const TUInt8 JPEG_FILE_MARKER_END = 0xD9;
    const TInt32 RATIO_THRESHOLD_DIFFERENCE = 1;
    const TUInt32 AUTO_DELETE_DIFFERENCE_FACTOR = 2;
    const TUInt32 AUTO_DELETE_DIFFERENCE_MAX = TUInt32(DENOMINATOR*0.050);
    const double ADDITIONAL_DIFFERENCE_FOR_DIFFERENT_CRC32 = 0.000001;
    const TUInt32 D0_SEARCHED_FILE_NUMBER_MAX = 1000;
    const TUInt32 D1_SEARCHED_FILE_NUMBER_MAX = 10000;
    const TUInt32 D3_THRESHOLD_DIFFERENCE_MAX = TUInt32(DENOMINATOR*0.100);
    const TUInt32 D3_MAX_RANGES_STEP = TUInt32(DENOMINATOR*0.010);
    const TUInt32 INITIAL_REDUCED_IMAGE_SIZE = 256;
    const TUInt32 REDUCED_IMAGE_SIZE_MIN = 16;
    const TUInt32 COLLECT_THREAD_QUEUE_SIZE_MAX = 16;
    const TUInt32 DEAFAULT_THREAD_SLEEP_INTERVAL = 10;
    const TUInt32 LARGE_IMAGE_COLLECTION_SIZE_MIN = 100000;

	const size_t IMAGE_DATA_FILE_SIZE_MAX = 0x10000;
	const TUInt32 FILE_VERSION = 3;
	const size_t SIZE_CHECK_LIMIT = UINT_MAX;

	const size_t BLOCKINESS_SIZE = 8;
	const static size_t HISTOGRAM_SIZE = 256;
    //-------------------------------------------------------------------------
}

#endif/*__adConfig_h__*/
