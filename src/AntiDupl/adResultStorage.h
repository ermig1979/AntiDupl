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
#ifndef __adResultStorage_h__
#define __adResultStorage_h__

#include "adConfig.h"

namespace ad
{
    struct TOptions;
    struct TImageInfo;
    struct TResult;

    class TEngine;
    class TMistakeStorage; 
    class TImageInfoStorage; 
    class TDuplResultFilter;
    class TUndoRedoEngine;
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

        void SetGroup();
        void SetHint();
        void Refresh();

        bool Undo();
        bool Redo();

        adError ApplyTo(adLocalActionType localActionType, adTargetType targetType);

        adError RenameCurrent(adRenameCurrentType renameCurrentType, const TString& newFileName);
		adError MoveCurrentGroup(const TString& directory);
		adError RenameCurrentGroupAs(const TString& fileName);

        bool CanApply(adActionEnableType actionEnableType) const;

        adError SetCurrent(adSize currentIndex);
        adError GetCurrent(adSizePtr pCurrentIndex);

        adError SetSelection(adSizePtr pStartFrom, adSize size, adBool value);
        adError GetSelection(adSizePtr pStartFrom, adBoolPtr pSelection, adSizePtr pSelectionSize);

        adError Export(adSizePtr pStartFrom, adResultPtrA pResult, adSizePtr pResultSize) const;
        adError Export(adSizePtr pStartFrom, adResultPtrW pResult, adSizePtr pResultSize) const;

		adError Export(adSizePtr pStartFrom, adGroupPtr pGroup, adSizePtr pGroupSize) const;
		adError Export(adSize groupId, adSizePtr pStartFrom, adImageInfoPtrA pImageInfo, adSizePtr pImageInfoSize) const;
		adError Export(adSize groupId, adSizePtr pStartFrom, adImageInfoPtrW pImageInfo, adSizePtr pImageInfoSize) const;

		adError SetSelection(adSize groupId, adSize index, adSelectionType selectionType);
		adError GetSelection(adSize groupId, adSizePtr pStartFrom, adBoolPtr pSelection, adSizePtr pSelectionSize) const;
		
        adError Rename(adSize groupId, adSize index, const TString& newFileName);

        adError Load(const TChar* fileName, bool check);
        adError Save(const TChar* fileName) const;

    private:
        TImageInfoStorage *m_pImageInfoStorage;
        TCriticalSection *m_pCriticalSection;
        TOptions *m_pOptions;
        TStatus *m_pStatus;
        TMistakeStorage *m_pMistakeStorage;
        TDuplResultFilter *m_pDuplResultFilter;
        TUndoRedoEngine *m_pUndoRedoEngine;
        size_t m_nextId;
    };
}
#endif//__adResultStorage_h__ 