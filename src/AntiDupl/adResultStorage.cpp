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
#include <algorithm>
#include <list>
#include <vector>
#include <map>

#include <math.h>

#include "adThreads.h"
#include "adFileUtils.h"
#include "adImageInfoStorage.h"
#include "adMistakeStorage.h"
#include "adStatus.h"
#include "adOptions.h"
#include "adEngine.h"
#include "adResult.h"
#include "adHintSetter.h"
#include "adDuplResultFilter.h"
#include "adUndoRedoTypes.h"
#include "adUndoRedoEngine.h"
#include "adResultStorage.h"
#include "adFileStream.h"

namespace ad
{
	const char RESULT_CONTROL_BYTES[] = "adr";

    //-------------------------------------------------------------------------
    TResultStorage::TResultStorage(TEngine *pEngine)
        :m_pOptions(pEngine->Options()),
        m_pStatus(pEngine->Status()),
        m_pMistakeStorage(pEngine->MistakeStorage()),
        m_nextId(0)
    {
        m_pCriticalSection = new TCriticalSection();
        m_pImageInfoStorage = new TImageInfoStorage(pEngine);
        m_pDuplResultFilter = new TDuplResultFilter();
        m_pUndoRedoEngine = new TUndoRedoEngine(pEngine, m_pImageInfoStorage);
    }

    TResultStorage::~TResultStorage()
    {
        Clear();
        delete m_pCriticalSection;
        delete m_pUndoRedoEngine;
        delete m_pImageInfoStorage;
        delete m_pDuplResultFilter;
    }

    bool TResultStorage::AddDuplImagePair(const TImageInfoPtr first, 
        const TImageInfoPtr second, 
        double difference, 
        TTransformType transform)
    {
        TCriticalSection::TLocker locker(m_pCriticalSection);

        if(m_pUndoRedoEngine->Current()->results.size() >= (size_t)m_pOptions->advanced.resultCountMax)
        {
            m_pStatus->Stop();
            return false;
        }

        TResultPtr pResult = new TResult();
        pResult->type = AD_RESULT_DUPL_IMAGE_PAIR;
        pResult->first = m_pImageInfoStorage->Add(first);
        pResult->second = m_pImageInfoStorage->Add(second);
        pResult->difference = difference;
        pResult->transform = transform;
        if((m_pOptions->advanced.mistakeDataBase == TRUE && m_pMistakeStorage->IsHas(pResult->first, pResult->second)) ||
           (m_pOptions->compare.transformedImage == TRUE && m_pDuplResultFilter->AlreadyHas(pResult)))
        {
            delete pResult;
            return false;
        }
        m_pUndoRedoEngine->Current()->results.push_back(pResult);
        pResult->id = m_nextId++;
        pResult->first->links++;
        pResult->second->links++;
        m_pStatus->AddDuplImagePair();
        return true;
    }

    bool TResultStorage::AddDefectImage(const TImageInfoPtr info, 
        TDefectType defect)
    {
        TCriticalSection::TLocker locker(m_pCriticalSection);

        if(m_pUndoRedoEngine->Current()->results.size() >= (size_t)m_pOptions->advanced.resultCountMax)
        {
            m_pStatus->Stop();
            return false;
        }

        TResultPtr pResult = new TResult();
        pResult->type = AD_RESULT_DEFECT_IMAGE;
        pResult->first = m_pImageInfoStorage->Add(info);
        pResult->second = m_pImageInfoStorage->GetStub();
        pResult->defect = defect;
        if(m_pOptions->advanced.mistakeDataBase == TRUE && 
           m_pMistakeStorage->IsHas(pResult->first))
        {
            delete pResult;
            return false;
        }
        m_pUndoRedoEngine->Current()->results.push_back(pResult);
        pResult->id = m_nextId++;
        pResult->first->links++;
        m_pStatus->AddDefectImage();
        return true;
    }

    void TResultStorage::Clear()
    {
        m_pUndoRedoEngine->Current()->Clear();
        m_pUndoRedoEngine->Clear();
        m_pImageInfoStorage->Clear();
        m_pDuplResultFilter->Clear();
        m_nextId = 0;
    }

    void TResultStorage::Sort(TSortType sortType, bool increasing)
    {
        m_pUndoRedoEngine->Current()->Sort(sortType, increasing);
    }

    void TResultStorage::SetGroup()
    {
        m_pUndoRedoEngine->Current()->SetGroups(m_pStatus);
    }

    void TResultStorage::SetHint()
    {
        m_pUndoRedoEngine->Current()->UpdateHints(m_pOptions, true, m_pStatus);
    }

    adError TResultStorage::ApplyTo(adLocalActionType localActionType, adTargetType targetType)
    {
        if(localActionType < 0 && localActionType >= AD_LOCAL_ACTION_SIZE)
            return AD_ERROR_INVALID_LOCAL_ACTION_TYPE;

        if(targetType < 0 && targetType >= AD_TARGET_SIZE)
            return AD_ERROR_INVALID_TARGET_TYPE;

		// Запрешена замена выделнных
        /*if(targetType == AD_TARGET_SELECTED &&
           (localActionType == AD_LOCAL_ACTION_RENAME_FIRST_TO_SECOND || localActionType == AD_LOCAL_ACTION_RENAME_SECOND_TO_FIRST))
            return AD_ERROR_INVALID_PARAMETER_COMBINATION;*/

        return m_pUndoRedoEngine->ApplyTo(localActionType, targetType) ? AD_OK : AD_ERROR_ZERO_TARGET;
    }

    adError TResultStorage::RenameCurrent(adRenameCurrentType renameCurrentType, const TString& newFileName)
    {
        if(renameCurrentType < 0 && renameCurrentType >= AD_RENAME_CURRENT_SIZE)
            return AD_ERROR_INVALID_RENAME_CURRENT_TYPE;

        TUndoRedoStage *pCurrent = m_pUndoRedoEngine->Current();
        if(pCurrent->currentIndex >= pCurrent->results.size())
            return AD_ERROR_ZERO_TARGET;

        TResult *pResult = pCurrent->results[pCurrent->currentIndex];
        if(pResult->type == AD_RESULT_DEFECT_IMAGE && 
            renameCurrentType == AD_RENAME_CURRENT_SECOND)
            return AD_ERROR_ZERO_TARGET;

        if(IsFileExists(newFileName.c_str()) || 
            !IsDirectoryExists(GetFileDirectory(newFileName).c_str()))
            return AD_ERROR_INVALID_FILE_NAME;

        return m_pUndoRedoEngine->RenameCurrent(renameCurrentType, newFileName) ? AD_OK : AD_ERROR_UNKNOWN;
    }

	adError TResultStorage::MoveCurrentGroup(const TString& directory)
    {
        TUndoRedoStage *pCurrent = m_pUndoRedoEngine->Current();
        if(pCurrent->currentIndex >= pCurrent->results.size())
            return AD_ERROR_ZERO_TARGET;

        TResult *pResult = pCurrent->results[pCurrent->currentIndex];
		// Дефектные изображения не перемещаем.
        if(pResult->type == AD_RESULT_DEFECT_IMAGE)
            return AD_ERROR_ZERO_TARGET;

		if(!IsDirectoryExists(directory.c_str()))
            return AD_ERROR_DIRECTORY_IS_NOT_EXIST;

        return m_pUndoRedoEngine->MoveCurrentGroup(directory) ? AD_OK : AD_ERROR_UNKNOWN;
    }

	adError TResultStorage::RenameCurrentGroupAs(const TString& fileName)
    {
        TUndoRedoStage *pCurrent = m_pUndoRedoEngine->Current();
        if(pCurrent->currentIndex >= pCurrent->results.size())
            return AD_ERROR_ZERO_TARGET;

        TResult *pResult = pCurrent->results[pCurrent->currentIndex];
		// Дефектные изображения не переименовываем.
        if(pResult->type == AD_RESULT_DEFECT_IMAGE)
            return AD_ERROR_ZERO_TARGET;

        return m_pUndoRedoEngine->RenameCurrentGroupAs(fileName) ? AD_OK : AD_ERROR_UNKNOWN;
    }


    void TResultStorage::Refresh()
    {
        m_pUndoRedoEngine->Clear();
        m_pUndoRedoEngine->Current()->RemoveInvalid(m_pStatus, m_pMistakeStorage);
		m_pUndoRedoEngine->Current()->RemoveSkipped(m_pStatus, m_pOptions); //удаляет из результатов пропускаемые
        m_pUndoRedoEngine->Current()->SetGroups(m_pStatus); //очищает внутреннее хранилище групп
		m_pUndoRedoEngine->Current()->UpdateGroups(); //обновляет группы из результатов
        m_pUndoRedoEngine->Current()->UpdateHints(m_pOptions, true, m_pStatus);
    }

    bool TResultStorage::Undo()
    {
        return m_pUndoRedoEngine->Undo();
    }

    bool TResultStorage::Redo()
    {
        return m_pUndoRedoEngine->Redo();
    }

    adError TResultStorage::SetCurrent(adSize currentIndex)
    {
        return m_pUndoRedoEngine->Current()->SetCurrent(currentIndex);
    }

    adError TResultStorage::GetCurrent(adSizePtr pCurrentIndex)
    {
        return m_pUndoRedoEngine->Current()->GetCurrent(pCurrentIndex);
    }

    adError TResultStorage::SetSelection(adSizePtr pStartFrom, adSize size, adBool value)
    {
        return m_pUndoRedoEngine->Current()->SetSelection(pStartFrom, size, value);
    }

    adError TResultStorage::GetSelection(adSizePtr pStartFrom, adBoolPtr pSelection, adSizePtr pSelectionSize)
    {
        return m_pUndoRedoEngine->Current()->GetSelection(pStartFrom, pSelection, pSelectionSize);
    }

    adError TResultStorage::Export(adSizePtr pStartFrom, adResultPtrA pResult, adSizePtr pResultSize) const
    {
        return m_pUndoRedoEngine->Current()->Export(pStartFrom, pResult, pResultSize);
    }

    adError TResultStorage::Export(adSizePtr pStartFrom, adResultPtrW pResult, adSizePtr pResultSize) const
    {
        return m_pUndoRedoEngine->Current()->Export(pStartFrom, pResult, pResultSize);
    }

	adError TResultStorage::Export(adSizePtr pStartFrom, adGroupPtr pGroup, adSizePtr pGroupSize) const
	{
		return m_pUndoRedoEngine->Current()->groups.Export(pStartFrom, pGroup, pGroupSize);
	}

	adError TResultStorage::Export(adSize groupId, adSizePtr pStartFrom, adImageInfoPtrA pImageInfo, adSizePtr pImageInfoSize) const
	{
		return m_pUndoRedoEngine->Current()->groups.Export(groupId, pStartFrom, pImageInfo, pImageInfoSize);
	}

	// Вызывается при запросе информации о размерах групп.
	adError TResultStorage::Export(adSize groupId, adSizePtr pStartFrom, adImageInfoPtrW pImageInfo, adSizePtr pImageInfoSize) const
	{
		return m_pUndoRedoEngine->Current()->groups.Export(groupId, pStartFrom, pImageInfo, pImageInfoSize);
	}

	adError TResultStorage::SetSelection(adSize groupId, adSize index, adSelectionType selectionType)
	{
		return m_pUndoRedoEngine->Current()->groups.SetSelection(groupId, index, selectionType);
	}

	adError TResultStorage::GetSelection(adSize groupId, adSizePtr pStartFrom, adBoolPtr pSelection, adSizePtr pSelectionSize) const
	{
		return m_pUndoRedoEngine->Current()->groups.GetSelection(groupId, pStartFrom, pSelection, pSelectionSize);
	}
	
	// Переименовывает файл с заданной группой и индексом в списке результатов.
    adError TResultStorage::Rename(adSize groupId, adSize index, const TString & newFileName)
    {
        TImageGroupPtr pImageGroup = m_pUndoRedoEngine->Current()->groups.Get(groupId, false);
        if(pImageGroup == NULL)
            return AD_ERROR_INVALID_GROUP_ID;

        TImageInfoPtrVector & images = pImageGroup->images;
        if(index >= images.size())
            return AD_ERROR_INVALID_INDEX;

        if(IsFileExists(newFileName.c_str()) || 
            !IsDirectoryExists(GetFileDirectory(newFileName).c_str()))
            return AD_ERROR_INVALID_FILE_NAME;

        return m_pUndoRedoEngine->Rename(groupId, index, newFileName) ? AD_OK : AD_ERROR_UNKNOWN;
    }

    adError TResultStorage::Load(const TChar* fileName, bool check)
    {
		adError error = AD_OK;
		m_pStatus->Reset();
		m_pStatus->ClearStatistic();
		Clear();
		try
		{
			TInputFileStream inputFile(fileName, RESULT_CONTROL_BYTES);

			m_pImageInfoStorage->Load(inputFile, check);

			size_t size = inputFile.LoadSizeChecked(SIZE_CHECK_LIMIT);
			TResultPtrVector &results = m_pUndoRedoEngine->Current()->results;
			results.reserve(size);
			TResult result;
			for(size_t i = 0; i < size; i++)
			{
				m_pStatus->SetProgress(i, size);
				inputFile.Load(result);
				if(result.type == AD_RESULT_DEFECT_IMAGE)
				{
					result.first = m_pImageInfoStorage->Get((size_t)result.first);
					result.second = m_pImageInfoStorage->GetStub();
					if(!check || result.first->Actual())
					{
						if(m_pOptions->advanced.mistakeDataBase == FALSE ||
							!m_pMistakeStorage->IsHas(result.first))
						{
							m_pStatus->AddDefectImage();
							results.push_back(new TResult(result));
							results.back()->id = m_nextId++;
							results.back()->first->links++;
							if(results.back()->first->group == AD_UNDEFINED)
								results.back()->first->group = results.back()->group;
						}
					}
				}
				if(result.type == AD_RESULT_DUPL_IMAGE_PAIR)
				{
					result.first = m_pImageInfoStorage->Get((size_t)result.first);
					result.second = m_pImageInfoStorage->Get((size_t)result.second);
					if(!check || (result.first->Actual() && result.second->Actual()))
					{
						if(m_pOptions->advanced.mistakeDataBase == FALSE ||
							!m_pMistakeStorage->IsHas(result.first, result.second))
						{
							m_pStatus->AddDuplImagePair();
							results.push_back(new TResult(result));
							results.back()->id = m_nextId++;
							results.back()->first->links++;
							results.back()->second->links++;
							if(results.back()->first->group == AD_UNDEFINED)
								results.back()->first->group = results.back()->group;
							if(results.back()->second->group == AD_UNDEFINED)
								results.back()->second->group = results.back()->group;
						}
					}
				}
			}
			m_pUndoRedoEngine->Current()->UpdateGroups();
			m_pUndoRedoEngine->Current()->UpdateHints(m_pOptions, true, m_pStatus);
		}
		catch (TException e)
		{
			m_pStatus->ClearStatistic();
			Clear();
			error = e.Error;
		}
		m_pStatus->Reset();		
        return error;
    }

    adError TResultStorage::Save(const TChar* fileName) const
    {
		try
		{
			m_pStatus->Reset();
			TOutputFileStream outputFile(fileName, RESULT_CONTROL_BYTES);

			m_pImageInfoStorage->Save(outputFile);

			TResultPtrVector &results = m_pUndoRedoEngine->Current()->results;
			outputFile.SaveSize(results.size());

			m_pStatus->SetProgress(0, 0);
			size_t i = 0;
			for(TResultPtrVector::iterator it = results.begin(); it != results.end(); it++, i++)
			{
				outputFile.Save(**it);
				m_pStatus->SetProgress(i, results.size());
				if(m_pStatus->Stopped())
					break;
			}
			m_pStatus->Reset();
		}
		catch (TException e)
		{
			m_pStatus->Reset();
			return e.Error;
		}
        return AD_OK;
    }

    bool TResultStorage::CanApply(adActionEnableType actionEnableType) const
    {
        if(actionEnableType <= AD_ACTION_ENABLE_PEFORM_HINT)
        {
            TResultPtrVector &results = m_pUndoRedoEngine->Current()->results;
            for (size_t i = 0; i < results.size(); i++)
            {
                const TResult &result = *results[i];
                if (result.selected)
                {
                    switch (actionEnableType)
                    {
                    case AD_ACTION_ENABLE_ANY:
                        return true;
                        break;
                    case AD_ACTION_ENABLE_DEFECT:
                        if (result.type == AD_RESULT_DEFECT_IMAGE)
                            return true;
                        break;
                    case AD_ACTION_ENABLE_DUPL_PAIR:
                        if (result.type == AD_RESULT_DUPL_IMAGE_PAIR)
                            return true;
                        break;
                    case AD_ACTION_ENABLE_PEFORM_HINT:
                        if (result.hint != AD_HINT_NONE)
                            return true;
                        break;
                    }
                }
            }
        }
        else
        {
            switch (actionEnableType)
            {
            case AD_ACTION_ENABLE_UNDO:
                return m_pUndoRedoEngine->UndoEnable();
            case AD_ACTION_ENABLE_REDO:
                return m_pUndoRedoEngine->RedoEnable();
            }
        }

        return false;
    }
    //-------------------------------------------------------------------------
}
