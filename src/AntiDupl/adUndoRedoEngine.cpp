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
#include "adStatisticsOfDeleting.h"

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
		m_pStatisticsOfDeleting = new TStatisticsOfDeleting(m_pOptions->statisticsPath);
    }

    TUndoRedoEngine::~TUndoRedoEngine()
    {
        Clear();
        delete m_pUndoDeque;
        delete m_pRedoDeque;
        delete m_pCurrent;
    }

	//public Выполняем определнный тип действий с текщим или выделенными результатами.
    bool TUndoRedoEngine::ApplyTo(adLocalActionType localActionType, adTargetType targetType)
    {
        bool onceMaked = false;

		// Сохраняем сотояние
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

		// Если ничего сделано не было.
        if(!onceMaked)
        {
            delete m_pCurrent->change;
            m_pCurrent->change = pOldChange;
            return false;
        }

        m_pUndoDeque->push_back(m_pCurrent->Clone());

		// Удаляем из списка ошибочные или удаленные
        if(localActionType == AD_LOCAL_ACTION_MISTAKE)
            m_pCurrent->RemoveMistaken(m_pStatus, m_pMistakeStorage);
        else
            m_pCurrent->RemoveDeleted(m_pStatus);

        m_pCurrent->UpdateGroups();
        m_pCurrent->UpdateHints(m_pOptions, false, m_pStatus);

        m_pCurrent->change = NULL;

        if(pOldChange)
            delete pOldChange;

        ClearRedo();
        AdjustUndoDequeSize(m_pOptions->advanced.undoQueueSize);

        return true;
    }
    
	// private Переименовывает/перемещает файл с заменой
    bool TUndoRedoEngine::Rename(TImageInfo *pImageInfo, const TString & newFileName)
    {
		// Сохраняем состояние
        TUndoRedoChange *pOldChange = m_pCurrent->change;
        m_pCurrent->change = new TUndoRedoChange();
		// Если удается переименовать/переместить файл с заменой
        if(::MoveFileEx(pImageInfo->path.Original().c_str(), newFileName.c_str(), 
			MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) != FALSE)
        {
			// Добавляем в список переименованных действие
            m_pCurrent->change->renamedImages.push_back(TRename(pImageInfo, pImageInfo->path.Original(), newFileName));
			// Изменяем статус
            m_pStatus->RenameImage(1);
			// В хранилише ошибочных меняем имя
            m_pMistakeStorage->Rename(pImageInfo, newFileName);
			// Путь в информации о файле меняем
            pImageInfo->Rename(newFileName);

			// Отмечаем новое состояние в очереди действий
            m_pUndoDeque->push_back(m_pCurrent->Clone());

			// Обновляем подсказки в текущей группе.
            m_pCurrent->groups.Get(pImageInfo->group)->invalidHint = true;
            m_pCurrent->UpdateHints(m_pOptions, false, m_pStatus);

            m_pCurrent->change = NULL;

            if(pOldChange)
                delete pOldChange;

			// Очишаем точки возврата в будушее.
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

	//private заменяем NewImageInfo файлом OldImageInfo
	bool TUndoRedoEngine::Rename(TImageInfo *pOldImageInfo, TImageInfo *pNewImageInfo)
    {
        const TChar *oldName = pOldImageInfo->path.Original().c_str();
        const TChar *newName = pNewImageInfo->path.Original().c_str();

        if(pOldImageInfo->removed || !IsFileExists(oldName))
            return false;

		//Проверяем нет ли в уже переименованных нашего файла
		TRenameList & renamedImages = m_pCurrent->change->renamedImages;
		for(TRenameList::iterator it = renamedImages.begin(); it != renamedImages.end(); ++it)
        {
			if (it->info == pOldImageInfo)
				return false;
        }

		// если новое имя пустое
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
			// Перименовываем имя файла, если расширения разные.
            TString templatePath(newName);
            templatePath = templatePath.substr(0, templatePath.size() - newExtension.size() - 1);
            adPath path;
            _stprintf_s(path, TEXT("%s.%s"), templatePath.c_str(), oldExtension.c_str());
            for(int i = 0; IsFileExists(path) && i <= 0xffff; ++i)
                _stprintf_s(path, TEXT("%s_%d.%s"), templatePath.c_str(), i, oldExtension.c_str());
            newPath = path;
        }

		// Если NewImageInfo удален, то заменяем его pOldImageInfo
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

	//private Переименовываем old в new без замены
	bool TUndoRedoEngine::RenameLike(TImageInfo *pOldImageInfo, TImageInfo *pNewImageInfo)
    {
		const TChar *oldPath = pOldImageInfo->path.Original().c_str();
        const TChar *newPath = pNewImageInfo->path.Original().c_str();

		if(pOldImageInfo->removed || !IsFileExists(oldPath))
            return false;

		// если новый путь пустой
        if(newPath == TString())
            return false;

		//если имена без расширений равны
		if (pNewImageInfo->path.GetName(false) == pOldImageInfo->path.GetName(false))
			return false;

		TString target = CreatePath(pOldImageInfo->path.GetDirectory(), pNewImageInfo->path.GetName(false) + pOldImageInfo->path.GetExtension());
		if (IsFileExists(target.c_str()))
			target = GetSimilarPath(TPath(target));

		if(::MoveFileEx(oldPath, target.c_str(), MOVEFILE_COPY_ALLOWED) != FALSE)
        {
            m_pCurrent->change->renamedImages.push_back(TRename(pOldImageInfo, pOldImageInfo->path.Original(), target));
            m_pStatus->RenameImage(1);
            m_pMistakeStorage->Rename(pOldImageInfo, target);
            pOldImageInfo->Rename(target);
            return true;
        }
        return false;
    }

	//private Перемещаем файл из old в new без замены
	bool TUndoRedoEngine::Move(TImageInfo *pOldImageInfo, TImageInfo *pNewImageInfo)
    {
		const TChar *oldPath = pOldImageInfo->path.Original().c_str();
		const TString newDir = pNewImageInfo->path.GetDirectory();

		if(pOldImageInfo->removed || !IsFileExists(oldPath) || !IsDirectoryExists(newDir.c_str()))
            return false;

		//Если картинка уже в заданной директории
		if (pOldImageInfo->path.GetDirectory() == newDir)
			return false;

		TString target = CreatePath(newDir, pOldImageInfo->path.GetName(false) + pOldImageInfo->path.GetExtension());
		if (IsFileExists(target.c_str()))
			target = GetSimilarPath(TPath(target));

		if(::MoveFileEx(oldPath, target.c_str(), MOVEFILE_COPY_ALLOWED) != FALSE)
        {
            m_pCurrent->change->renamedImages.push_back(TRename(pOldImageInfo, pOldImageInfo->path.Original(), target));
            m_pStatus->RenameImage(1);
            m_pMistakeStorage->Rename(pOldImageInfo, target);
            pOldImageInfo->Rename(target);
            return true;
        }
        return false;
    }

	//private Перемещаем и переименовываем файл из old в new как соседа
	bool TUndoRedoEngine::MoveAndRenameLike(TImageInfo *pOldImageInfo, TImageInfo *pNewImageInfo)
    {
		const TChar *oldPath = pOldImageInfo->path.Original().c_str();

		if(pOldImageInfo->removed || !IsFileExists(oldPath) || !IsDirectoryExists(pNewImageInfo->path.GetDirectory().c_str()))
            return false;

		TString target = CreatePath(pNewImageInfo->path.GetDirectory(), pNewImageInfo->path.GetName(false) + pOldImageInfo->path.GetExtension());
		if (IsFileExists(target.c_str()))
			target = GetSimilarPath(TPath(target));

		if(::MoveFileEx(oldPath, target.c_str(), MOVEFILE_COPY_ALLOWED) != FALSE)
        {
            m_pCurrent->change->renamedImages.push_back(TRename(pOldImageInfo, pOldImageInfo->path.Original(), target));
            m_pStatus->RenameImage(1);
            m_pMistakeStorage->Rename(pOldImageInfo, target);
            pOldImageInfo->Rename(target);
            return true;
        }
        return false;
    }

    bool TUndoRedoEngine::Undo()
    {
        if(m_pUndoDeque->empty())
            return false;

		// Возврашаем прошлое состояние в хранилишах.
        m_pRedoDeque->push_back(m_pCurrent);
        m_pCurrent = m_pUndoDeque->back();
        m_pUndoDeque->pop_back();

		//?? FIX Может надо Apply вызывать? иначе после undo группы пустые
		m_pCurrent->UpdateGroups();

        m_pStatus->SetProgress(0, 0);
        size_t current = 0, total = m_pCurrent->change->renamedImages.size() + m_pCurrent->change->deletedImages.size();

		// Переименовываем обратно переименованные файлы.
        TRenameList & renamedImages = m_pCurrent->change->renamedImages;
        for(TRenameList::iterator it = renamedImages.begin(); it != renamedImages.end(); ++it, ++current)
        {
            if(::MoveFileEx(it->second.c_str(), it->first.c_str(), 
				MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) != TRUE)
			{
				m_pStatus->Reset();
                return false;
			}
            m_pMistakeStorage->Rename(it->info, it->first);
            it->info->Rename(it->first);
            m_pStatus->RenameImage(-1);
            m_pStatus->SetProgress(current, total);
        }

		// Восстанавливаем удаленные файлы.
        TImageInfoPtrList & deletedImages = m_pCurrent->change->deletedImages;
        for(TImageInfoPtrList::iterator it = deletedImages.begin(); it != deletedImages.end(); ++it, ++current)
        {
            m_pRecycleBin->Restore(*it);
            m_pStatus->SetProgress(current, total);
        }

		// Добавляем в список результатов удаленные элементы.
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

				m_pStatisticsOfDeleting->Write(pResult);
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

	//private
    bool TUndoRedoEngine::ApplyTo(adLocalActionType localActionType, TResult *pResult)
    {
		bool result;

        if(pResult->type == AD_RESULT_DEFECT_IMAGE)
        {
            switch(localActionType)
            {
            case AD_LOCAL_ACTION_DELETE_DEFECT:
                return Delete(pResult->first);
            case AD_LOCAL_ACTION_PERFORM_HINT:
                if(pResult->hint == AD_HINT_DELETE_FIRST)
                    return Delete(pResult->first);
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
                return Delete(pResult->first);
            case AD_LOCAL_ACTION_DELETE_SECOND:
                return Delete(pResult->second);
            case AD_LOCAL_ACTION_DELETE_BOTH:
                return (Delete(pResult->first) && Delete(pResult->second));
            case AD_LOCAL_ACTION_RENAME_FIRST_TO_SECOND:
                return Rename(pResult->first, pResult->second);
            case AD_LOCAL_ACTION_RENAME_SECOND_TO_FIRST:
                return Rename(pResult->second, pResult->first);
			case AD_LOCAL_ACTION_RENAME_FIRST_LIKE_SECOND:
                return RenameLike(pResult->first, pResult->second);
			case AD_LOCAL_ACTION_RENAME_SECOND_LIKE_FIRST:
				return RenameLike(pResult->second, pResult->first);
			case AD_LOCAL_ACTION_MOVE_FIRST_TO_SECOND:
				return Move(pResult->first, pResult->second);
			case AD_LOCAL_ACTION_MOVE_SECOND_TO_FIRST:
				return Move(pResult->second, pResult->first);
			case AD_LOCAL_ACTION_MOVE_AND_RENAME_FIRST_TO_SECOND:
				return MoveAndRenameLike(pResult->first, pResult->second);
			case AD_LOCAL_ACTION_MOVE_AND_RENAME_SECOND_TO_FIRST:
				return MoveAndRenameLike(pResult->second, pResult->first);
            case AD_LOCAL_ACTION_PERFORM_HINT:
                switch(pResult->hint)
                {
                case AD_HINT_DELETE_FIRST:
					pResult->deleteByHint = true;
					result = Delete(pResult->first);
                    if (!result)
						pResult->deleteByHint = false;
					return result;
                case AD_HINT_DELETE_SECOND:
                    //return Delete(pResult->second);
					pResult->deleteByHint = true;
					result = Delete(pResult->second);
                    if (!result)
						pResult->deleteByHint = false;
					return result;
                case AD_HINT_RENAME_FIRST_TO_SECOND:
                    return Rename(pResult->first, pResult->second);
                case AD_HINT_RENAME_SECOND_TO_FIRST:
                    return Rename(pResult->second, pResult->first);
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

	//private Удаляет изображение
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

	//public
    bool TUndoRedoEngine::RenameCurrent(adRenameCurrentType renameCurrentType, const TString & newFileName)
    {
        TResult *pResult = m_pCurrent->results[m_pCurrent->currentIndex];
		// Получаем информацию о файле, который переименовываем
        TImageInfo *pImageInfo = renameCurrentType == AD_RENAME_CURRENT_FIRST ? pResult->first : pResult->second;
        return Rename(pImageInfo, newFileName);
    }

	//public Переименовывает файл с заданной группой и индексом.
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

	//public Переносим файлы из текущей группы в заданную директорию.
	bool TUndoRedoEngine::MoveCurrentGroup(const TString & directory)
    {
		bool isMoving = false;

		if(!IsDirectoryExists(directory.c_str()))
            return false;

		// Сохраняем сотояние
        TUndoRedoChange *pOldChange = m_pCurrent->change;
        m_pCurrent->change = new TUndoRedoChange();

        TResult *pResult = m_pCurrent->results[m_pCurrent->currentIndex];
		TImageGroupPtr pImageGroup = m_pCurrent->groups.Get(pResult->group, false);
		if(pImageGroup == NULL)
			return false;

		// Проходимся по списку изображений в группе.
		for (size_t i = 0; i < pImageGroup->images.size(); i++)
		{
			TPath * path = &pImageGroup->images[i]->path;
			if (path->GetDirectory() != directory)
			{
				TString target = CreatePath(directory, path->GetName());
				if (IsFileExists(target.c_str()))
					target = GetSimilarPath(TPath(target));

				//if (Rename(pImageGroup->images[i], target))
				if(::MoveFileEx(path->Original().c_str(), target.c_str(), MOVEFILE_COPY_ALLOWED) != FALSE)
				{
					m_pCurrent->change->renamedImages.push_back(TRename(pImageGroup->images[i], path->Original(), target));
					m_pStatus->RenameImage(1);
					m_pMistakeStorage->Rename(pImageGroup->images[i], target);
					pImageGroup->images[i]->Rename(target);
					isMoving = true;
				}
			}
		}

		// Если ничего сделано не было.
        if(!isMoving)
        {
            delete m_pCurrent->change;
            m_pCurrent->change = pOldChange;
            return false;
        }

        m_pUndoDeque->push_back(m_pCurrent->Clone());

        m_pCurrent->UpdateGroups();
        m_pCurrent->UpdateHints(m_pOptions, false, m_pStatus);

        m_pCurrent->change = NULL;

        if(pOldChange)
            delete pOldChange;

        ClearRedo();
        AdjustUndoDequeSize(m_pOptions->advanced.undoQueueSize);

        return true;
    }

	//public Переименовавает файлы в группе как переданное имя файла.
	bool TUndoRedoEngine::RenameCurrentGroupAs(const TString & fileName)
    {
		bool isRenaming = false;

		// если новое имя пустое
        if(fileName == TString())
            return false;

		// Сохраняем сотояние
        TUndoRedoChange *pOldChange = m_pCurrent->change;
        m_pCurrent->change = new TUndoRedoChange();

        TResult *pResult = m_pCurrent->results[m_pCurrent->currentIndex];
		TImageGroupPtr pImageGroup = m_pCurrent->groups.Get(pResult->group, false);
		if(pImageGroup == NULL)
			return false;

		for (size_t i = 0; i < pImageGroup->images.size(); i++)
		{
			TPath* path = &pImageGroup->images[i]->path;
			if (path->GetName(false) != fileName)
			{
				TString target = CreatePath(path->GetDirectory(), fileName + path->GetExtension());
				if (IsFileExists(target.c_str()))
					target = GetSimilarPath(TPath(target), *path); //разименовываем указатель
				if(::MoveFileEx(path->Original().c_str(), target.c_str(), MOVEFILE_COPY_ALLOWED) != FALSE)
				{
					m_pCurrent->change->renamedImages.push_back(TRename(pImageGroup->images[i], path->Original(), target));
					m_pStatus->RenameImage(1);
					m_pMistakeStorage->Rename(pImageGroup->images[i], target);
					pImageGroup->images[i]->Rename(target);
					isRenaming = true;
				}
			}
		}

		// Если ничего сделано не было.
        if(!isRenaming)
        {
            delete m_pCurrent->change;
            m_pCurrent->change = pOldChange;
            return false;
        }

        m_pUndoDeque->push_back(m_pCurrent->Clone());

        m_pCurrent->UpdateGroups();
        m_pCurrent->UpdateHints(m_pOptions, false, m_pStatus);

        m_pCurrent->change = NULL;

        if(pOldChange)
            delete pOldChange;

        ClearRedo();
        AdjustUndoDequeSize(m_pOptions->advanced.undoQueueSize);

        return true;
    }
}
