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
#include "adFileUtils.h"
#include "adEngine.h"
#include "adStatus.h"
#include "adOptions.h"
#include "adResult.h"
#include "adRecycleBin.h"
#include "adMistakeStorage.h"
#include "adImageInfoStorage.h"
#include "adUndoRedoTypes.h"
#include "adUndoRedoEngine.h"

namespace ad
{
    TUndoRedoEngine::TUndoRedoEngine(TEngine *pEngine, TImageInfoStorage *pImageInfoStorage)
        :m_pOptions(pEngine->Options()),
        m_pStatus(pEngine->Status()),
        m_pMistakeStorage(pEngine->MistakeStorage()),
        m_pImageInfoStorage(pImageInfoStorage),
        m_pRecycleBin(pEngine->RecycleBin())
    {
        m_pUndoDeque = new TUndoRedoStagePtrDeque();
        m_pRedoDeque = new TUndoRedoStagePtrDeque();
        m_pCurrent = new TUndoRedoStage();
    }

    TUndoRedoEngine::~TUndoRedoEngine()
    {
        Clear();
        delete m_pUndoDeque;
        delete m_pRedoDeque;
        delete m_pCurrent;
    }

    bool TUndoRedoEngine::ApplyTo(adLocalActionType localActionType, adTargetType targetType)
    {
        bool onceMaked = false;

        TUndoRedoChange *pOldChange = m_pCurrent->change;
        m_pCurrent->change = new TUndoRedoChange();

        if(targetType == AD_TARGET_CURRENT)
        {
            if(m_pCurrent->currentIndex != AD_UNDEFINED)
            {
                onceMaked = ApplyTo(localActionType, m_pCurrent->results[m_pCurrent->currentIndex]);
            }
        }
        else
        {
            m_pStatus->SetProgress(0, 0);
            size_t current = 0, total = 0;
            for(TResultPtrVector::iterator it = m_pCurrent->results.begin(); it != m_pCurrent->results.end(); ++it)
            {
                if((*it)->selected)
                    total++;
            }
            for(TResultPtrVector::iterator it = m_pCurrent->results.begin(); it != m_pCurrent->results.end() && !m_pStatus->Stopped(); it++)
            {
                if((*it)->selected)
                {
                    m_pStatus->SetProgress(current, total);
                    onceMaked = ApplyTo(localActionType, (*it)) || onceMaked;
                    current++;
                }
            }
            m_pStatus->Reset();
        }

        if(!onceMaked)
        {
            delete m_pCurrent->change;
            m_pCurrent->change = pOldChange;
            return false;
        }

        m_pUndoDeque->push_back(m_pCurrent->Clone());

        if(localActionType == AD_LOCAL_ACTION_MISTAKE)
            m_pCurrent->RemoveMistaken(m_pStatus, m_pMistakeStorage);
        else
            m_pCurrent->RemoveDeleted(m_pStatus);

        m_pCurrent->UpdateGroups();
        m_pCurrent->UpdateHints(m_pOptions, false);

        m_pCurrent->change = NULL;

        if(pOldChange)
            delete pOldChange;

        ClearRedo();
        AdjustUndoDequeSize(m_pOptions->advanced.undoQueueSize);

        return true;
    }
    
    bool TUndoRedoEngine::Rename(TImageInfo *pImageInfo, const TString & newFileName)
    {
        TUndoRedoChange *pOldChange = m_pCurrent->change;
        m_pCurrent->change = new TUndoRedoChange();
        if(::MoveFileEx(pImageInfo->path.Original().c_str(), newFileName.c_str(), 
			MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) != FALSE)
        {
            m_pCurrent->change->renamedImages.push_back(TRename(pImageInfo, pImageInfo->path.Original(), newFileName));
            m_pStatus->RenameImage(1);
            m_pMistakeStorage->Rename(pImageInfo, newFileName);
            pImageInfo->Rename(newFileName);

            m_pUndoDeque->push_back(m_pCurrent->Clone());

            m_pCurrent->groups.Get(pImageInfo->group)->invalidHint = true;
            m_pCurrent->UpdateHints(m_pOptions, false);

            m_pCurrent->change = NULL;

            if(pOldChange)
                delete pOldChange;

            ClearRedo();
            AdjustUndoDequeSize(m_pOptions->advanced.undoQueueSize);

            return true;
        }
        else
        {
            delete m_pCurrent->change;
            m_pCurrent->change = pOldChange;
            return false;
        }    
    }

    bool TUndoRedoEngine::RenameCurrent(adRenameCurrentType renameCurrentType, const TString & newFileName)
    {
        TResult *pResult = m_pCurrent->results[m_pCurrent->currentIndex];
        TImageInfo *pImageInfo = renameCurrentType == AD_RENAME_CURRENT_FIRST ? pResult->first : pResult->second;
        return Rename(pImageInfo, newFileName);
    }
    
    bool TUndoRedoEngine::Rename(adSize groupId, adSize index, const TString & newFileName)
    {
        TImageGroupPtr pImageGroup = m_pCurrent->groups.Get(groupId, false);
        if(pImageGroup && index < pImageGroup->images.size())
        {
            TImageInfoPtr pImageInfo = pImageGroup->images[index];
            return Rename(pImageInfo, newFileName);
        }
        return false; 
    }

    bool TUndoRedoEngine::Undo()
    {
        if(m_pUndoDeque->empty())
            return false;

        m_pRedoDeque->push_back(m_pCurrent);
        m_pCurrent = m_pUndoDeque->back();
        m_pUndoDeque->pop_back();

        m_pStatus->SetProgress(0, 0);
        size_t current = 0, total = m_pCurrent->change->renamedImages.size() + m_pCurrent->change->deletedImages.size();

        TRenameList & renamedImages = m_pCurrent->change->renamedImages;
        for(TRenameList::iterator it = renamedImages.begin(); it != renamedImages.end(); ++it, ++current)
        {
            if(::MoveFileEx(it->second.c_str(), it->first.c_str(), 
				MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) != TRUE)
                return false;
            m_pMistakeStorage->Rename(it->info, it->first);
            it->info->Rename(it->first);
            m_pStatus->RenameImage(-1);
            m_pStatus->SetProgress(current, total);
        }

        TImageInfoPtrList & deletedImages = m_pCurrent->change->deletedImages;
        for(TImageInfoPtrList::iterator it = deletedImages.begin(); it != deletedImages.end(); ++it, ++current)
        {
            m_pRecycleBin->Restore(*it);
            m_pStatus->SetProgress(current, total);
        }

        TResultPtrList & removedResults = m_pCurrent->change->removedResults;
        for(TResultPtrList::iterator it = removedResults.begin(); it != removedResults.end(); ++it)
        {
            TResult *pResult = *it;
            if(pResult->type == AD_RESULT_DEFECT_IMAGE)
                m_pStatus->AddDefectImage(1);
            else if(pResult->type == AD_RESULT_DUPL_IMAGE_PAIR)
                m_pStatus->AddDuplImagePair(1);
        }

        TResultPtrList & mistakenResults = m_pCurrent->change->mistakenResults;
        for(TResultPtrList::iterator it = mistakenResults.begin(); it != mistakenResults.end(); ++it)
        {
            TResult *pResult = *it;
            if(pResult->type == AD_RESULT_DEFECT_IMAGE)
                m_pMistakeStorage->Erase(pResult->first);
            else if(pResult->type == AD_RESULT_DUPL_IMAGE_PAIR)
                m_pMistakeStorage->Erase(pResult->first, pResult->second);
        }

        m_pStatus->Reset();

        return true;
    }

    bool TUndoRedoEngine::Redo()
    {
        if(m_pRedoDeque->empty())
            return false;

        m_pStatus->SetProgress(0, 0);
        size_t current = 0, total = m_pCurrent->change->renamedImages.size() + m_pCurrent->change->deletedImages.size();

        TImageInfoPtrList & deletedImages = m_pCurrent->change->deletedImages;
        for(TImageInfoPtrList::iterator it = deletedImages.begin(); it != deletedImages.end(); ++it, ++current)
        {
            m_pRecycleBin->Delete(*it);
            m_pStatus->SetProgress(current, total);
        }

        TRenameList & renamedImages = m_pCurrent->change->renamedImages;
        for(TRenameList::iterator it = renamedImages.begin(); it != renamedImages.end(); ++it, ++current)
        {
            if(::MoveFileEx(it->first.c_str(), it->second.c_str(), 
				MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) != TRUE)
                return false;
            m_pMistakeStorage->Rename(it->info, it->second);
            it->info->Rename(it->second);
            m_pStatus->RenameImage(1);
            m_pStatus->SetProgress(current, total);
        }

        TResultPtrList & removedResults = m_pCurrent->change->removedResults;
        for(TResultPtrList::iterator it = removedResults.begin(); it != removedResults.end(); ++it)
        {
            TResult *pResult = *it;
            if(pResult->type == AD_RESULT_DEFECT_IMAGE)
                m_pStatus->AddDefectImage(-1);
            else if(pResult->type == AD_RESULT_DUPL_IMAGE_PAIR)
                m_pStatus->AddDuplImagePair(-1);
        }

        TResultPtrList & mistakenResults = m_pCurrent->change->mistakenResults;
        for(TResultPtrList::iterator it = mistakenResults.begin(); it != mistakenResults.end(); ++it)
        {
            TResult *pResult = *it;
            if(pResult->type == AD_RESULT_DEFECT_IMAGE)
                m_pMistakeStorage->Add(pResult->first);
            else if(pResult->type == AD_RESULT_DUPL_IMAGE_PAIR)
                m_pMistakeStorage->Add(pResult->first, pResult->second);
        }

        m_pStatus->Reset();

        m_pUndoDeque->push_back(m_pCurrent);
        m_pCurrent = m_pRedoDeque->back();
        m_pRedoDeque->pop_back();

        return true;
    }

    void TUndoRedoEngine::Clear()
    {
        ClearRedo();
        AdjustUndoDequeSize(0);
    }

    void TUndoRedoEngine::ClearRedo()
    {
        for(TUndoRedoStagePtrDeque::iterator it = m_pRedoDeque->begin(); it != m_pRedoDeque->end(); ++it)
            delete *it;
        m_pRedoDeque->clear();
    }

    void TUndoRedoEngine::AdjustUndoDequeSize(size_t size)
    {
        size_t current = 0, total = 0;
        for(size_t i = size; i < m_pUndoDeque->size(); i++)
            total += m_pUndoDeque->at(m_pUndoDeque->size() - 1 - i)->change->deletedImages.size();
        m_pStatus->SetProgress(0, 0);
        bool unlink = false;
        while(m_pUndoDeque->size() > size)
        {
            TUndoRedoStagePtr pStage = m_pUndoDeque->front();
            TResultPtrList &removedResults = pStage->change->removedResults;
            for(TResultPtrList::iterator it = removedResults.begin(); it != removedResults.end(); ++it)
            {
                unlink = true;
                TResult *pResult = *it;
                pResult->first->links--;
                if(pResult->type == AD_RESULT_DUPL_IMAGE_PAIR)
                    pResult->second->links--;
            }
            TImageInfoPtrList& deletedImages = pStage->change->deletedImages;
            for(TImageInfoPtrList::iterator it = deletedImages.begin(); it != deletedImages.end(); ++it)
            {
                m_pRecycleBin->Free(*it);
                m_pStatus->SetProgress(current++, total);
            }
            delete pStage;
            m_pUndoDeque->pop_front();
        }
        if(unlink)
            m_pImageInfoStorage->RemoveUnlinked();
        m_pStatus->Reset();
    }

    bool TUndoRedoEngine::ApplyTo(adLocalActionType localActionType, TResult *pResult)
    {
        if(pResult->type == AD_RESULT_DEFECT_IMAGE)
        {
            switch(localActionType)
            {
            case AD_LOCAL_ACTION_DELETE_DEFECT:
                Delete(pResult->first);
                return true;
            case AD_LOCAL_ACTION_PERFORM_HINT:
                if(pResult->hint == AD_HINT_DELETE_FIRST)
                {
                    Delete(pResult->first);
                    return true;
                }
                break;
            case AD_LOCAL_ACTION_MISTAKE:
                m_pMistakeStorage->Add(pResult->first);
                m_pCurrent->change->mistakenResults.push_back(pResult);
                return true;
            }
        }
        else if(pResult->type == AD_RESULT_DUPL_IMAGE_PAIR)
        {
            switch(localActionType)
            {
            case AD_LOCAL_ACTION_DELETE_FIRST:
                Delete(pResult->first);
                return true;
            case AD_LOCAL_ACTION_DELETE_SECOND:
                Delete(pResult->second);
                return true;
            case AD_LOCAL_ACTION_DELETE_BOTH:
                Delete(pResult->first);
                Delete(pResult->second);
                return true;
            case AD_LOCAL_ACTION_RENAME_FIRST_TO_SECOND:
                Rename(pResult->first, pResult->second);
                return true;
            case AD_LOCAL_ACTION_RENAME_SECOND_TO_FIRST:
                Rename(pResult->second, pResult->first);
                return true;
            case AD_LOCAL_ACTION_PERFORM_HINT:
                switch(pResult->hint)
                {
                case AD_HINT_DELETE_FIRST:
                    Delete(pResult->first);
                    return true;
                case AD_HINT_DELETE_SECOND:
                    Delete(pResult->second);
                    return true;
                case AD_HINT_RENAME_FIRST_TO_SECOND:
                    Rename(pResult->first, pResult->second);
                    return true;
                case AD_HINT_RENAME_SECOND_TO_FIRST:
                    Rename(pResult->second, pResult->first);
                    return true;
                }
                break;
            case AD_LOCAL_ACTION_MISTAKE:
                m_pMistakeStorage->Add(pResult->first, pResult->second);
                m_pCurrent->change->mistakenResults.push_back(pResult);
                return true;
            }
        }
        return false;
    }

    bool TUndoRedoEngine::Delete(TImageInfo *pImageInfo)
    {
        const TChar *fileName = pImageInfo->path.Original().c_str();
        if(pImageInfo->removed || !IsFileExists(fileName))
            return true;

        if(m_pRecycleBin->Delete(pImageInfo))
        {
            m_pCurrent->change->deletedImages.push_back(pImageInfo);
            return true;
        }
        return false;
    }

    bool TUndoRedoEngine::Rename(TImageInfo *pOldImageInfo, TImageInfo *pNewImageInfo)
    {
        const TChar *oldName = pOldImageInfo->path.Original().c_str();
        const TChar *newName = pNewImageInfo->path.Original().c_str();

        if(pOldImageInfo->removed || !IsFileExists(oldName))
            return false;

        if(newName == TString())
            return false;

        TString oldExtension = GetFileExtension(oldName);
        TString newExtension = GetFileExtension(newName);

        TString newPath;
        if(oldExtension.GetUpper() == newExtension.GetUpper())
        {
            newPath = newName;
        }
        else
        {
            TString templatePath(newName);
            templatePath = templatePath.substr(0, templatePath.size() - newExtension.size() - 1);
            adPath path;
            _stprintf_s(path, TEXT("%s.%s"), templatePath.c_str(), oldExtension.c_str());
            for(int i = 0; IsFileExists(path) && i <= 0xffff; ++i)
                _stprintf_s(path, TEXT("%s_%d.%s"), templatePath.c_str(), i, oldExtension.c_str());
            newPath = path;
        }

        if(Delete(pNewImageInfo))
        {
            if(::MoveFileEx(oldName, newPath.c_str(), 
				MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) != FALSE)
            {
                m_pCurrent->change->renamedImages.push_back(TRename(pOldImageInfo, pOldImageInfo->path.Original(), newPath));
                m_pStatus->RenameImage(1);
                m_pMistakeStorage->Rename(pOldImageInfo, newPath);
                pOldImageInfo->Rename(newPath);
                return true;
            }
        }

        return false;
    }
}
