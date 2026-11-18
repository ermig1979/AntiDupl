/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar.
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
#ifndef __adImageComparer_h__
#define __adImageComparer_h__

#include <list>
#include <map>
#include <vector>

#include "adConfig.h"

namespace ad
{
    struct TImageData;
    struct TOptions;
    class TEngine;
    class TResultStorage;
	class TImageDataStorage;
    typedef TImageData* TImageDataPtr;
    //-------------------------------------------------------------------------
	// Общий класс движка
    class TImageComparer
    {
    protected:
        typedef std::list<TImageDataPtr> TImageDataPtrList;
        struct Set
        {
            TImageDataPtrList valid; //проверенные
            TImageDataPtrList other;
        };
        typedef std::vector<Set> Sets;
        Sets m_sets;

        TOptions *m_pOptions;
    public:
        TImageComparer(TEngine *pEngine);
        virtual ~TImageComparer();

        void Accept(TImageDataPtr pImageData, bool add);

    protected:
        virtual void Add(TImageDataPtr pImageData) = 0; // pure virtual or abstract function and requires to be overwritten in an derived class
        virtual void Compare(TImageDataPtr pOriginal, TImageDataPtr pTransformed, adTransformType transform) = 0;
		virtual bool IsDuplPair(TImageDataPtr pFirst, TImageDataPtr pSecond, double *pDifference); //виртуальная функция, но не обязательно ее переопределять
        //Comparison of images with search of the optimal shift (shiftedImage option).
        virtual bool ShiftedDifference(TImageDataPtr pFirst, TImageDataPtr pSecond, double *pDifference);
        bool PassControls(TImageDataPtr pFirst, TImageDataPtr pSecond);
        //Search of a nonzero shift of the reduced images that minimizes the squared difference.
        bool FindShift(TImageDataPtr pFirst, TImageDataPtr pSecond, int &dx, int &dy);
        void AddToSet(Set &set, TImageDataPtr pImageData);
        void CompareWithSet(const Set &set, TImageDataPtr pOriginal, TImageDataPtr pTransformed, adTransformType transform);

        TUInt8* m_pMask;      //mask of the central comparison area (ignoreFrameWidth)
        int m_fastThreshold;  //threshold for the 4x4 fast signature
        // A metric's value for two images, made 0 exactly when their decoded
        // pixels are the same: reduced images can be equal for images that
        // differ, which then get MIN_DIFFERENCE_OF_DIFFERENT_PIXELS at least.
        static double PixelDifference(TImageDataPtr pFirst, TImageDataPtr pSecond, double metric);

        // Transparency shown (adCompareOptions::transparentAsShown): images
        // with transparent pixels are kept apart from the index, which
        // places an image by its thumbnails over white, while a pair may be
        // judged over black or another colour. The index comparers compare
        // every image with this set, and an image with transparency with
        // every set.
        bool m_asShown;
        Set m_alphaSet;

        // Transparent areas ignored (adCompareOptions::ignoreTransparent,
        // with transparency shown): a pair with transparency is also
        // compared only where each image with it is opaque, and is a
        // duplicate if either comparison says so, with the smaller
        // difference. An image's opaque part: per thumbnail pixel whether it
        // is opaque, and its colour there with the background taken out
        // (main grey, and B, G, R of the colour thumbnail); how many are
        // opaque; whether they vary enough to be compared alone.
        struct TOpaquePart
        {
            std::vector<TUInt8> main, mainOpaque, color, colorOpaque;
            size_t mainCount, colorCount;
            bool mainVaried, colorVaried;
        };
        bool m_ignoreTransparent;
        // Whether the pixels compared, compared of pixels in all, satisfy
        // the MASKED_* limits; first and second are the opaque counts of the
        // images with transparency, 0 for an opaque one.
        static bool MaskedArea(size_t compared, size_t pixels, size_t first, size_t second);
        // The difference of a pair over the pixels where each image with
        // transparency is opaque; a NULL part is an opaque image, all of it
        // compared. Mean square on the main thumbnails here.
        virtual bool MaskedDifference(TImageDataPtr pFirst, const TOpaquePart *pFirstPart,
            TImageDataPtr pSecond, const TOpaquePart *pSecondPart, double *pDifference);

    private:
        //Comparison of an image pair without shift and (if enabled) with shift compensation; the smaller difference is kept.
        bool IsDuplPairAny(TImageDataPtr pFirst, TImageDataPtr pSecond, double *pDifference);
        // IsDuplPairAny for images as shown, when either has transparency:
        // both over white and over black when both have it, the larger
        // difference counting; when one has, the other may be a copy whose
        // transparency was filled with one colour, judged over that colour,
        // and otherwise over white and over black too.
        bool IsDuplPairShown(TImageDataPtr pFirst, TImageDataPtr pFirstBlack, TImageDataPtr pSecond, double *pDifference);
        // The part of IsDuplPairShown over backgrounds, for a pair where at
        // least one image has transparency.
        bool IsDuplPairOver(TImageDataPtr pFirst, TImageDataPtr pFirstBlack, bool firstAlpha,
            TImageDataPtr pSecond, TImageDataPtr pSecondBlack, bool secondAlpha, double *pDifference);
        bool OverBoth(TImageDataPtr pFirst, TImageDataPtr pFirstBlack, TImageDataPtr pSecond, TImageDataPtr pSecondBlack, double *pDifference);
        bool UniformFill(TImageDataPtr pAlpha, TImageDataPtr pAlphaBlack, TImageDataPtr pOpaque, TUInt8 *bgr);
        void MakeBlack(TImageData *pBlack, TImageDataPtr pImageData);
        void MakeFilled(TImageData *pFilled, TImageDataPtr pImageData, TImageDataPtr pBlack, const TUInt8 *bgr);
        void CopyInfo(TImageData *pTo, TImageDataPtr pFrom);
        void MakeOpaquePart(TOpaquePart &part, TImageDataPtr pImageData, TImageDataPtr pBlack);

        TResultStorage *m_pResult;
        TImageData *m_pTransformedImageData;
        TUInt8* m_pBuffer;
        // The over-black twin of each image with transparency in this
        // comparer's sets, and of the image being compared; an image laid
        // over a filled copy's colour.
        std::map<TImageDataPtr, TImageData*> m_blackTwins;
        TImageData *m_pIncomingBlack;
        TImageData *m_pFilled;
        // The opaque part of each image with transparency in the sets, and
        // of the image being compared (transparent areas ignored).
        std::map<TImageDataPtr, TOpaquePart*> m_opaqueParts;
        TOpaquePart m_incomingPart;

        int m_mainThreshold;
        int m_maxDifference;
        size_t m_mainSize;
    };
    //-------------------------------------------------------------------------
    class TImageComparer_0D : public TImageComparer 
    {
    public:
        TImageComparer_0D(TEngine *pEngine); //конструктор

    protected:
        virtual void Add(TImageDataPtr pImageData);
        virtual void Compare(TImageDataPtr pOriginal, TImageDataPtr pTransformed, adTransformType transform);
    };
    //-------------------------------------------------------------------------
    class TImageComparer_1D : public TImageComparer
    {
        static const int RANGE = 256;

    public:
        TImageComparer_1D(TEngine *pEngine);

    protected:
        virtual void Add(TImageDataPtr pImageData);
        virtual void Compare(TImageDataPtr pOriginal, TImageDataPtr pTransformed, adTransformType transform);

    private:
        int GetIndex(TImageDataPtr pImageData);

        int m_halfCompareRange;
    };
    //-------------------------------------------------------------------------
    class TImageComparer_3D : public TImageComparer
    {
		// Многомерные индексы
        struct TIndex
        {
            int s;//total sum of fast data pixels;
            int x;//difference between left and right half of fast data pixels;
            int y;//difference between bottom and top half of fast data pixels;
        };
    public:
        TImageComparer_3D(TEngine *pEngine);

    protected:
        virtual void Add(TImageDataPtr pImageData);
        virtual void Compare(TImageDataPtr pOriginal, TImageDataPtr pTransformed, adTransformType transform);

    private:
        void GetIndex(TImageDataPtr pImageData, TIndex& index);

        int m_maxRange;
        int m_halfCompareRange;
        TIndex m_shift, m_range, m_stride;
    };
	//-------------------------------------------------------------------------
    class TImageComparer_SSIM : public TImageComparer 
    {
    public:
        TImageComparer_SSIM(TEngine *pEngine); //конструктор

    protected:
        virtual void Add(TImageDataPtr pImageData);
        virtual void Compare(TImageDataPtr pOriginal, TImageDataPtr pTransformed, adTransformType transform);
		virtual bool IsDuplPair(TImageDataPtr pFirst, TImageDataPtr pSecond, double *pDifference);
        virtual bool ShiftedDifference(TImageDataPtr pFirst, TImageDataPtr pSecond, double *pDifference);
        virtual bool MaskedDifference(TImageDataPtr pFirst, const TOpaquePart *pFirstPart,
            TImageDataPtr pSecond, const TOpaquePart *pSecondPart, double *pDifference);

    private:
		double C1;
		double C2;
    };
    //-------------------------------------------------------------------------
    // Local colour difference: the colour thumbnails split into
    // COLOR_BLOCK_SIZE square blocks, each block's RMS difference over its
    // pixels and B, G, R, and the worst block as the difference, in percent
    // of 255. A change confined to a small area counts in full, and so does
    // a change of colour that keeps the brightness.
    class TImageComparer_LocalColor : public TImageComparer
    {
    public:
        TImageComparer_LocalColor(TEngine *pEngine);

    protected:
        virtual void Add(TImageDataPtr pImageData);
        virtual void Compare(TImageDataPtr pOriginal, TImageDataPtr pTransformed, adTransformType transform);
        virtual bool IsDuplPair(TImageDataPtr pFirst, TImageDataPtr pSecond, double *pDifference);
        // None here: the base's shift search measures the grey reduced images
        // in mean square's percents, which aren't this comparer's differences.
        virtual bool ShiftedDifference(TImageDataPtr pFirst, TImageDataPtr pSecond, double *pDifference);
        // On the colour thumbnails: blocks less than half opaque are left
        // out, the others measured over their opaque pixels.
        virtual bool MaskedDifference(TImageDataPtr pFirst, const TOpaquePart *pFirstPart,
            TImageDataPtr pSecond, const TOpaquePart *pSecondPart, double *pDifference);

    private:
        size_t m_frameBlocks; // blocks left out along each edge (Ignore frame width)
        uint64_t m_blockThreshold; // a block's squared-difference sum above this fails the pair
        int m_fastCellThreshold; // so does a fast-data cell difference above this
    };
    //-------------------------------------------------------------------------
    TImageComparer* CreateImageComparer(TEngine *pEngine);
    //-------------------------------------------------------------------------
}
#endif//__adImageComparer_h__ 