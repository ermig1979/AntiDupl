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
#ifndef __adResultStorage_h__
#define __adResultStorage_h__

#include "adConfig.h"
#include "adImageInfo.h"
#include "adStatus.h"
#include "adUndoRedoTypes.h"

namespace ad
{
    struct TOptions;
    struct TImageInfo;
    struct TResult;

    class TEngine;
    class TMistakeStorage; 
    class TImageInfoStorage; 
    class TDuplResultFilter;
	class TCriticalSection;

    typedef TResult* TResultPtr;

    //-------------------------------------------------------------------------

    class TResultStorage
    {
        typedef std::vector<TResultPtr> TResultPtrVector;
        typedef std::list<TResultPtr> TResultPtrList;
    public:
        TResultStorage(TEngine *pEngine);
        ~TResultStorage();

        bool AddDuplImagePair(const TImageInfoPtr first, const TImageInfoPtr second, 
            double difference, TTransformType transform);
        bool AddDefectImage(const TImageInfoPtr info, TDefectType defect);

        void Clear();

        void Sort(TSortType sortType, bool increasing);

        void Refresh();

        bool CanApply(adActionEnableType actionEnableType) const;

        adError Export(adSizePtr pStartFrom, adResultPtrA pResult, adSizePtr pResultSize) const;
        adError Export(adSizePtr pStartFrom, adResultPtrW pResult, adSizePtr pResultSize) const;
		
        adError Load(const TChar* fileName, bool check);
        adError Save(const TChar* fileName) const;

		unsigned int * GetResultSize() const;
		void GetResult(adResultPtrW pResult, const unsigned int & index) const;

    private:
        TImageInfoStorage *m_pImageInfoStorage;
        TCriticalSection *m_pCriticalSection;
        TOptions *m_pOptions;
        TStatus *m_pStatus;
        TMistakeStorage *m_pMistakeStorage;
        TDuplResultFilter *m_pDuplResultFilter;
        size_t m_nextId;
		// Текущее состояние результатов вектор структур TResult
        TUndoRedoStage * m_pCurrent;
    };
}
#endif//__adResultStorage_h__ 