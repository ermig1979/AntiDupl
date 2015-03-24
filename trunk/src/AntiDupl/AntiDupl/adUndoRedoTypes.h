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
#ifndef __adResultStage_h__
#define __adResultStage_h__

#include "adImageGroup.h"

namespace ad
{
    struct TOptions;
	struct TResult;
	struct TImageInfo;

    class TStatus;
    class TMistakeStorage; 

	typedef TResult* TResultPtr;
    typedef std::vector<TResultPtr> TResultPtrVector;
    typedef std::list<TResultPtr> TResultPtrList;

	typedef TImageInfo* TImageInfoPtr;
    typedef std::list<TImageInfoPtr> TImageInfoPtrList;
    //-------------------------------------------------------------------------
    struct TRename
    {
        TImageInfoPtr info;
        TString first;
        TString second;
        TRename(TImageInfoPtr i, const TString &f, const TString &s) :info(i), first(f), second(s) {}
    };
    typedef std::list<TRename> TRenameList;
    //-------------------------------------------------------------------------
    struct TUndoRedoChange
    {
        TResultPtrList removedResults;
        TResultPtrList mistakenResults;
        TImageInfoPtrList deletedImages;
        TRenameList renamedImages;
    };
    //-------------------------------------------------------------------------
    struct TUndoRedoStage
    {
        TResultPtrVector results;
        TImageGroupStorage groups;
        size_t currentIndex;
        TUndoRedoChange *change;

        TUndoRedoStage();
        ~TUndoRedoStage();

        void Clear();
        TUndoRedoStage* Clone();

        void Sort(TSortType sortType, bool increasing);

        adError SetCurrent(adSize newCurrentIndex);
        adError GetCurrent(adSizePtr pCurrentIndex);

        adError SetSelection(adSizePtr pStartFrom, adSize size, adBool value);
        adError GetSelection(adSizePtr pStartFrom, adBoolPtr pSelection, adSizePtr pSelectionSize);

        adError Export(adSizePtr pStartFrom, adResultPtrA pResult, adSizePtr pResultSize) const;
        adError Export(adSizePtr pStartFrom, adResultPtrW pResult, adSizePtr pResultSize) const;

        void RemoveInvalid(TStatus *pStatus, TMistakeStorage *pMistakeStorage);
        void RemoveDeleted(TStatus *pStatus);
        void RemoveMistaken(TStatus *pStatus, TMistakeStorage *pMistakeStorage);
		void RemoveSkipped(TStatus *pStatus, TOptions *pOptions);
        
        void SetGroups(TStatus *pStatus);
        void UpdateGroups();
        void UpdateHints(TOptions *pOptions, bool force);

    private:
        void UpdateCurrentIndex();

        template <class TValidator> void RemoveInvalid(const TValidator &validator, TStatus *pStatus, bool canCancel = false);
    };
}
#endif//__adResultStage_h__ 