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
#ifndef __adResult_h__
#define __adResult_h__

#include "adImageInfo.h"

namespace ad
{
    template<class T> inline int CompareValues(const T& value1, const T& value2)
    {
        return value1 < value2 ? -1 : (value2 < value1 ? 1 : 0);
    }

	// Структура хранит пары дубликатов
    struct TResult
    {
        size_t id;
        bool selected;
        bool current;

        TResultType type;
        TImageInfoPtr first;
		//Используется только в режиме работы с парами
        TImageInfoPtr second; 
        TDefectType defect;
        double difference;
        TTransformType transform;
        TSize group;
		TSize groupSize;
        THintType hint;
		// Флаг значит, что файл удаляется по автоматической подсказке а не в ручную пользователем.
		bool deleteByHint;

        TResult();
        TResult(const TResult& result);

        // Ascending: negative, zero or positive. sortType is one of the
        // AD_SORT_BY_SORTED_* types.
        static int ImageInfoCompare(TImageInfoPtr pFirst, TImageInfoPtr pSecond, TSortType sortType);
        static bool ImageInfoLesser(TImageInfoPtr pFirst, TImageInfoPtr pSecond, TSortType sortType, bool increasing);
        void Swap();
        // Puts the image with the lesser path first, so a pair's sides
        // don't depend on the order the search found its images in.
        void OrientByPath();

        bool Export(adResultPtrA pResult) const;
        bool Export(adResultPtrW pResult) const;
    };
    typedef TResult* TResultPtr;

    //-------------------------------------------------------------------------

    class TResultPtrLesser
    {
    public:
        TResultPtrLesser(TSortType sortType, bool increasing); 

        bool operator() (TResultPtr pFirst, TResultPtr pSecond);

    private:
        int Compare(TResultPtr pFirst, TResultPtr pSecond) const;
        int CompareOtherImage(TResultPtr pFirst, TResultPtr pSecond) const;

        TSortType m_sortType;
        bool m_increasing;
    };
}
#endif//__adResult_h__ 