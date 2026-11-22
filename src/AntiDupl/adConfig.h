/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2023 Yermalayeu Ihar.
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

//#define AD_PERFORMANCE_TEST_ENABLE
//#define AD_LOGGER_ENABLE
//#define AD_DUMP_ENABLE
#define AD_TURBO_JPEG_ENABLE

#include "adSimd.h"

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

    typedef Simd::View<Simd::Allocator> TView;
	typedef Simd::Point<ptrdiff_t> TPoint;

    //-------------------------------------------------------------------------

    const TUInt32 DENOMINATOR = 100;
    const TUInt32 PIXEL_MAX_DIFFERENCE = 0xFF;
    const TUInt32 FAST_IMAGE_SIZE = 4;
    const size_t FAST_DATA_SIZE = FAST_IMAGE_SIZE*FAST_IMAGE_SIZE;
    const int SHIFT_COARSE_FACTOR = 6;               //admissible threshold excess at the coarse stage of the shift compensation search
    const TUInt8 JPEG_FILE_MARKER = 0xFF;
    const TUInt8 JPEG_FILE_MARKER_END = 0xD9;
    const TInt32 RATIO_THRESHOLD_DIFFERENCE = 1;
    const TUInt32 AUTO_DELETE_DIFFERENCE_FACTOR = 2;
    const TUInt32 AUTO_DELETE_DIFFERENCE_MAX = TUInt32(DENOMINATOR*0.050);
    const double ADDITIONAL_DIFFERENCE_FOR_DIFFERENT_CRC32 = 0.000001;
    // The least difference of images whose decoded pixels differ: their
    // reduced images can be equal, which would give 0.
    const double MIN_DIFFERENCE_OF_DIFFERENT_PIXELS = 0.0001;
    // Two names of one file: below every real difference, so the pair sorts
    // first. The program shows it as HL (DifferenceFormat).
    const double HARD_LINK_DIFFERENCE = -1;
    const TUInt32 D0_SEARCHED_FILE_NUMBER_MAX = 1000;
    const TUInt32 D1_SEARCHED_FILE_NUMBER_MAX = 10000;
    const TUInt32 D3_THRESHOLD_DIFFERENCE_MAX = TUInt32(DENOMINATOR*0.100);
    const TUInt32 D3_MAX_RANGES_STEP = TUInt32(DENOMINATOR*0.010);
    const TUInt32 INITIAL_REDUCED_IMAGE_SIZE = 256;
    const TUInt32 REDUCED_IMAGE_SIZE_MIN = 16;
    // The colour thumbnail the local colour comparer reads: COLOR_IMAGE_SIZE
    // square, B, G, R bytes, compared in COLOR_BLOCK_SIZE square blocks.
    const size_t COLOR_IMAGE_SIZE = 64;
    const size_t COLOR_BLOCK_SIZE = 4;
    const size_t COLOR_DATA_SIZE = COLOR_IMAGE_SIZE*COLOR_IMAGE_SIZE*3;
    // Butteraugli scores only the pairs whose local colour difference is
    // within this many percent: the settings study's pairs up to a score of
    // 15 at 512 px all were within 5.21 %.
    const double BUTTERAUGLI_CANDIDATE_THRESHOLD = 10.0;
    const TInt32 BUTTERAUGLI_SIZE_DEFAULT = 768;
    const TInt32 BUTTERAUGLI_SIZE_MIN = 128;
    const TInt32 BUTTERAUGLI_SIZE_MAX = 2048;
    // The brightness of white, in nits, Butteraugli judges a pair as seen at.
    // At libjxl's default, 80 (sRGB's reference), the eye it models hardly
    // tells near-black shades apart, so two different dark pictures score
    // like copies of one. At 400, a bright monitor, copies and other pictures
    // separate well on pairs judged by hand, dark ones included.
    const float BUTTERAUGLI_INTENSITY_TARGET = 400.0f;
    // Images decoded at the working size are kept up to this many bytes.
    const size_t BUTTERAUGLI_IMAGE_CACHE_BYTES = size_t(512) << 20;
    // Part of the stored scores' key: a change to how a score is computed
    // (the working images, the metric) makes the stored ones stale.
    const TUInt32 BUTTERAUGLI_SCORE_VERSION = 2;
    // The aligned search (adAlign.h). Keypoints are found on a grey copy of
    // about this many pixels, whatever the shape: a long strip keeps a width
    // to find corners in, which a limit on the longest side wouldn't leave.
    const size_t ALIGN_IMAGE_AREA = 512*384;
    const size_t ALIGN_KEYPOINTS_MAX = 500;
    // Hits an image needs in the keypoint index, and the images a key may
    // hold before it says too little. A key that fills up stops counting, so
    // more entries find the crops of a picture a folder has many copies of,
    // and pass more unrelated pairs on to be aligned.
    const size_t ALIGN_INDEX_HITS_MIN = 16;
    const size_t ALIGN_INDEX_KEY_ENTRIES_MAX = 255;
    // A pair local colour lets through scores under this without needing
    // alignment: what alignment gains is worth a second score only above it.
    const double ALIGN_PLAIN_SCORE_MIN = 7.5;
    // Pairs share at least this share of the smaller image...
    const double ALIGN_OVERLAP_MIN = 0.5;
    // ...and a box at least this wide and high at the working size.
    const size_t ALIGN_BOX_SIDE_MIN = 32;
    // Above this scale the larger image is area-reduced before it is warped:
    // bicubic sampling alone aliases as it shrinks.
    const double ALIGN_REDUCE_SCALE = 1.02;
    // Part of an aligned score's key, as BUTTERAUGLI_SCORE_VERSION is of all.
    const TInt32 ALIGNED_SCORE_VERSION = 1;
    // Transparency shown: when could an opaque image be a copy of one with
    // transparency, its transparent area filled with one colour
    // (TImageComparer::UniformFill). Pixels of the colour thumbnail count as
    // transparent below UNIFORM_FILL_COVERAGE_PERCENT coverage and as opaque
    // above 100 minus it; each kind must make UNIFORM_FILL_AREA_PERCENT of the
    // thumbnail at least. The opaque image under the transparent pixels may
    // deviate from its mean colour by UNIFORM_FILL_DEVIATION_MAX levels (RMS,
    // each of B, G, R), and the visible pixels must differ from that colour by
    // UNIFORM_FILL_CONTRAST_MIN levels on average. A 4x4 cell of the opaque
    // image may differ by UNIFORM_FILL_FAST_DIFFERENCE_MAX levels from the
    // other laid over the best grey, a loose look before the colour thumbnail.
    const size_t UNIFORM_FILL_COVERAGE_PERCENT = 5;
    const size_t UNIFORM_FILL_AREA_PERCENT = 2;
    const double UNIFORM_FILL_DEVIATION_MAX = 12;
    const double UNIFORM_FILL_CONTRAST_MIN = 16;
    const double UNIFORM_FILL_FAST_DIFFERENCE_MAX = 48;
    // Transparent areas ignored (TImageComparer::MaskedDifference): a pair
    // is compared only where each image with transparency is opaque (above
    // 100 minus UNIFORM_FILL_COVERAGE_PERCENT coverage). Those pixels must
    // make MASKED_AREA_PERCENT of the thumbnail and, when both images have
    // transparency, MASKED_OVERLAP_PERCENT of the smaller opaque part, or two
    // drawings in different places would compare on a few pixels. The
    // opaque part of each image with transparency must vary by
    // MASKED_DEVIATION_MIN grey levels (RMS), or a drawing in one colour
    // would match any picture showing that colour where the drawing is.
    const size_t MASKED_AREA_PERCENT = 2;
    const size_t MASKED_OVERLAP_PERCENT = 50;
    const double MASKED_DEVIATION_MIN = 16;
    const TUInt32 COLLECT_THREAD_QUEUE_SIZE_MAX = 16;
    const TUInt32 DEAFAULT_THREAD_SLEEP_INTERVAL = 10;
    const TUInt32 LARGE_IMAGE_COLLECTION_SIZE_MIN = 100000;

	const size_t IMAGE_DATA_FILE_SIZE_MAX = 0x10000;
	// Version 5: images decoded by TurboJPEG had red and blue swapped before,
	// so their cached pixel data, blockiness and blurring are recomputed.
	// Version 6: the image database stores a hash of the decoded pixels;
	// images from an older database are decoded again for it.
	// Version 7: the reduced image is area-averaged instead of bilinear, and
	// the database stores a colour thumbnail; older records are decoded again.
	// Version 8: each record says whether its thumbnails show transparency
	// (TPixelData::asShown) and, for an image with transparent pixels, holds
	// a second set over black; older records of formats that can hold
	// transparency are decoded again.
	const TUInt32 FILE_VERSION = 8;
	const size_t SIZE_CHECK_LIMIT = 2147483646; //string.max_size()

	const size_t BLOCKINESS_SIZE = 8;
	const static size_t HISTOGRAM_SIZE = 256;

    //-------------------------------------------------------------------------
}

#endif/*__adConfig_h__*/
