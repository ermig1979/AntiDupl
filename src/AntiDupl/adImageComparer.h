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

        void AddToSet(Set &set, TImageDataPtr pImageData);
        void CompareWithSet(const Set &set, TImageDataPtr pOriginal, TImageDataPtr pTransformed, adTransformType transform);

    private:
        TResultStorage *m_pResult;
        TImageData *m_pTransformedImageData;
        TUInt8* m_pBuffer;
        TUInt8* m_pMask;

        int m_mainThreshold;
        int m_fastThreshold;
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

    private:
		float C1;
		float C2;
		TImageDataStorage *m_pImageDataStorage;
    };
    //-------------------------------------------------------------------------
    TImageComparer* CreateImageComparer(TEngine *pEngine);
    //-------------------------------------------------------------------------
}
#endif//__adImageComparer_h__ 