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

        static bool ImageInfoLesser(TImageInfoPtr pFirst, TImageInfoPtr pSecond, TSortType sortType, bool increasing);
        void Swap();

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
        TSortType m_sortType;
        bool m_increasing;
    };
}
#endif//__adResult_h__ 