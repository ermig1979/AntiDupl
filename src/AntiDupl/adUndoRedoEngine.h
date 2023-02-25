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
#ifndef __adUndoRedoEngine_h__
#define __adUndoRedoEngine_h__

#include <queue>

#include "adConfig.h"

namespace ad
{
    struct TOptions;
    struct TUndoRedoStage;
    struct TUndoRedoChange;
    struct TResult;

    class TEngine;
    class TRecycleBin;
    class TStatus;
    class TMistakeStorage;
    class TImageInfoStorage;
	class TStatisticsOfDeleting;

    typedef TUndoRedoStage* TUndoRedoStagePtr;
    typedef std::deque<TUndoRedoStagePtr> TUndoRedoStagePtrDeque;
    //-------------------------------------------------------------------------
    class TUndoRedoEngine
    {
    public:
        TUndoRedoEngine(TEngine *pEngine, TImageInfoStorage *pImageInfoStorage);
        ~TUndoRedoEngine();

        bool ApplyTo(adLocalActionType localActionType, adTargetType targetType);

        bool Undo();
        bool Redo();

        bool UndoEnable() const {return !m_pUndoDeque->empty();} // A read-only function
        bool RedoEnable() const {return !m_pRedoDeque->empty();}

        void Clear();

		bool RenameCurrent(adRenameCurrentType renameCurrentType, const TString & newFileName);
        bool Rename(adSize groupId, adSize index, const TString & newFileName);
		bool MoveCurrentGroup(const TString& directory);
		bool RenameCurrentGroupAs(const TString & fileName);

        TUndoRedoStagePtr Current() const {return m_pCurrent;}

    private:
        void ClearRedo();
        void AdjustUndoDequeSize(size_t size);

        bool ApplyTo(adLocalActionType localActionType, TResult *pResult);

        bool Rename(TImageInfo *pImageInfo, const TString & newFileName);

        bool Delete(TImageInfo *pImageInfo);
        bool Rename(TImageInfo *pOldImageInfo, TImageInfo *pNewImageInfo);
		bool RenameLike(TImageInfo *pOldImageInfo, TImageInfo *pNewImageInfo);
		bool Move(TImageInfo *pOldImageInfo, TImageInfo *pNewImageInfo);
		bool MoveAndRenameLike(TImageInfo *pOldImageInfo, TImageInfo *pNewImageInfo);

        TOptions *m_pOptions;
        TRecycleBin *m_pRecycleBin;
        TStatus *m_pStatus;
        TMistakeStorage *m_pMistakeStorage;
        TImageInfoStorage *m_pImageInfoStorage;
		TStatisticsOfDeleting *m_pStatisticsOfDeleting;

        TUndoRedoStagePtrDeque *m_pUndoDeque;
        TUndoRedoStagePtrDeque *m_pRedoDeque;
		// Текущее состояние результатов вектор структур TResult
        TUndoRedoStagePtr m_pCurrent;
    };
}
#endif//__adUndoRedoEngine_h__ 