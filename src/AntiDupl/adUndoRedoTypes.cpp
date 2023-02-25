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

#include "adFileUtils.h"
#include "adResult.h"
#include "adOptions.h"
#include "adStatus.h"
#include "adMistakeStorage.h"
#include "adUndoRedoTypes.h"

namespace ad
{
    TUndoRedoStage::TUndoRedoStage()
        :currentIndex(AD_UNDEFINED),
        change(NULL)
    {
    }

    TUndoRedoStage::~TUndoRedoStage()
    {
        Clear();
    }

    void TUndoRedoStage::Clear()
    {
        for(TResultPtrVector::iterator it = results.begin(); it != results.end(); it++)
            delete *it;
        results.clear();
        currentIndex = AD_UNDEFINED;
        if(change)
        {
            delete change;
            change = NULL;
        }
    }

    TUndoRedoStage* TUndoRedoStage::Clone()
    {
       TUndoRedoStage *pUndoRedoStage = new TUndoRedoStage();
       pUndoRedoStage->results.resize(results.size());
       for(size_t i = 0; i < results.size(); ++i)
            pUndoRedoStage->results[i] = new TResult(*results[i]);
       pUndoRedoStage->groups.Assign(groups);
       pUndoRedoStage->currentIndex = currentIndex;
       pUndoRedoStage->change = change;
       return pUndoRedoStage;
    }

    void TUndoRedoStage::Sort(TSortType sortType, bool increasing)
    {
        for(TResultPtrVector::iterator it = results.begin(); it != results.end(); ++it)
        {
            TResultPtr pResult = *it;
            if(pResult->type == AD_RESULT_DUPL_IMAGE_PAIR && 
                !TResult::ImageInfoLesser(pResult->first, pResult->second, sortType, increasing))
            {
                pResult->Swap();
            }
        }
        std::sort(results.begin(), results.end(), TResultPtrLesser(sortType, increasing));
        UpdateCurrentIndex();
    }

    adError TUndoRedoStage::SetCurrent(adSize newCurrentIndex)
    {
        if(newCurrentIndex != AD_UNDEFINED && newCurrentIndex >= results.size())
            return AD_ERROR_INVALID_INDEX;

        if(currentIndex != newCurrentIndex)
        {
            if(currentIndex != AD_UNDEFINED)
                results[currentIndex]->current = false;
            currentIndex = newCurrentIndex;
            if(currentIndex != AD_UNDEFINED)
                results[currentIndex]->current = true;
        }

        return AD_OK;
    }

	// Возврашает текущий индекс
    adError TUndoRedoStage::GetCurrent(adSizePtr pCurrentIndex)
    {
        if(pCurrentIndex == NULL)
            return AD_ERROR_INVALID_POINTER;

        *pCurrentIndex = currentIndex;

        return AD_OK;
    }

    adError TUndoRedoStage::SetSelection(adSizePtr pStartFrom, adSize size, adBool value)
    {
        if(pStartFrom == NULL)
            return AD_ERROR_INVALID_POINTER;
        if(results.size() < *pStartFrom)
        {
            *pStartFrom = results.size();
            return AD_ERROR_INVALID_START_POSITION;
        }

        size_t end = std::min(*pStartFrom + size, results.size());
        for(size_t i = *pStartFrom; i < end; i++)
            results[i]->selected = value != FALSE;

        return AD_OK;
    }

    adError TUndoRedoStage::GetSelection(adSizePtr pStartFrom, adBoolPtr pSelection, adSizePtr pSelectionSize)
    {
        if(pSelection == NULL || pSelectionSize == NULL || pStartFrom == NULL)
            return AD_ERROR_INVALID_POINTER;
        if(results.size() < *pStartFrom)
        {
            *pStartFrom = results.size();
            return AD_ERROR_INVALID_START_POSITION;
        }

        *pSelectionSize = std::min(results.size() - *pStartFrom, *pSelectionSize);
        for(size_t i = 0; i < *pSelectionSize; i++)
            pSelection[i] = results[*pStartFrom + i]->selected ? TRUE : FALSE;

        return AD_OK;
    }

    adError TUndoRedoStage::Export(adSizePtr pStartFrom, adResultPtrA pResult, adSizePtr pResultSize) const
    {
        if(pResult == NULL || pResultSize == NULL || pStartFrom == NULL)
            return AD_ERROR_INVALID_POINTER;
        if(results.size() < *pStartFrom)
        {
            *pStartFrom = results.size();
            return AD_ERROR_INVALID_START_POSITION;
        }

        adResultPtrA p = pResult;
        *pResultSize = std::min(results.size() - *pStartFrom, *pResultSize);
        for(size_t i = *pStartFrom, n = *pStartFrom + *pResultSize; i < n; ++i)
        {
            results[i]->Export(p);
            p++;
        }

        return AD_OK;
    }

	// Эскпортириет список результатов из хранилища.
    adError TUndoRedoStage::Export(adSizePtr pStartFrom, adResultPtrW pResult, adSizePtr pResultSize) const
    {
        if(pResult == NULL || pResultSize == NULL || pStartFrom == NULL)
            return AD_ERROR_INVALID_POINTER;
        if(results.size() < *pStartFrom)
        {
            *pStartFrom = results.size();
            return AD_ERROR_INVALID_START_POSITION;
        }

        adResultPtrW p = pResult;
        *pResultSize = std::min(results.size() - *pStartFrom, *pResultSize);
        for(size_t i = *pStartFrom, n = *pStartFrom + *pResultSize; i < n; ++i)
        {
            results[i]->Export(p);
            p++;
        }

        return AD_OK;
    }

    void TUndoRedoStage::RemoveInvalid(TStatus *pStatus, TMistakeStorage *pMistakeStorage)
    {
        class TValidator
        {
            TMistakeStorage *m_pMistakeStorage;
        public:
            TValidator(TMistakeStorage *pMistakeStorage) :m_pMistakeStorage(pMistakeStorage) {}
            bool operator()(TImageInfo* a) const //DEFECT
            {
                return 
                    !a->removed && 
                    IsFileExists(a->path.Original().c_str()) &&
                    !m_pMistakeStorage->IsHas(a);
            }
            bool operator()(TImageInfo* a, TImageInfo* b) const //DUPL_IMAGE_PAIR
            {
                return 
                    (!a->removed) && (!b->removed) &&
                    IsFileExists(a->path.Original().c_str()) && IsFileExists(b->path.Original().c_str()) &&
                    !m_pMistakeStorage->IsHas(a, b);
            }
        };

        RemoveInvalid(TValidator(pMistakeStorage), pStatus, true);
    }

    void TUndoRedoStage::RemoveDeleted(TStatus *pStatus)
    {
        class TValidator
        {
        public:
            bool operator()(TImageInfo* a) const {return !a->removed;}
            bool operator()(TImageInfo* a, TImageInfo* b) const 
            {return (!a->removed) && (!b->removed);}
        };

        RemoveInvalid(TValidator(), pStatus);
    }

    void TUndoRedoStage::RemoveMistaken(TStatus *pStatus, TMistakeStorage *pMistakeStorage)
    {
        class TValidator
        {
            TMistakeStorage *m_pMistakeStorage;
        public:
            TValidator(TMistakeStorage *pMistakeStorage) :m_pMistakeStorage(pMistakeStorage) {}
            bool operator()(TImageInfo* a) const {return !m_pMistakeStorage->IsHas(a);}
            bool operator()(TImageInfo* a, TImageInfo* b) const 
            {return !m_pMistakeStorage->IsHas(a, b);}
        };

        RemoveInvalid(TValidator(pMistakeStorage), pStatus);
    }

	void TUndoRedoStage::RemoveSkipped(TStatus *pStatus, TOptions *pOptions)
	{
		class TValidator
        {
			TOptions *m_pOptions;
        public:
			TValidator(TOptions *pOptions) :m_pOptions(pOptions) {}
            bool operator()(TImageInfo* a) const //DEFECT
			{
				return !((m_pOptions->ignorePaths.IsHasPath(a->path.Original()) != AD_IS_NOT_EXIST) ||
						 (m_pOptions->ignorePaths.IsHasPath(a->path.GetDirectory()) != AD_IS_NOT_EXIST));
			} 
            bool operator()(TImageInfo* a, TImageInfo* b) const  //DUPL_IMAGE_PAIR true - оставлять, false- удалять
            {	// != AD_IS_NOT_EXIST - есть в каталоге пропуска (удалить), == AD_IS_NOT_EXIST - нету в каталоге пропуска
				return !((m_pOptions->ignorePaths.IsHasPath(a->path.Original()) != AD_IS_NOT_EXIST) ||
					   (m_pOptions->ignorePaths.IsHasPath(a->path.GetDirectory()) != AD_IS_NOT_EXIST) ||
					   (m_pOptions->ignorePaths.IsHasPath(b->path.Original()) != AD_IS_NOT_EXIST) ||
					   (m_pOptions->ignorePaths.IsHasPath(b->path.GetDirectory()) != AD_IS_NOT_EXIST));
			}
        };

        RemoveInvalid(TValidator(pOptions), pStatus);
	}

    void TUndoRedoStage::SetGroups(TStatus *pStatus)
    {
		groups.Set(results, pStatus);
    }

    void TUndoRedoStage::UpdateGroups()
    {
		groups.Update(results);
    }

    void TUndoRedoStage::UpdateHints(TOptions *pOptions, bool force, TStatus *pStatus)
    {
		groups.UpdateHints(pOptions, force, pStatus);
    }

    void TUndoRedoStage::UpdateCurrentIndex()
    {
        currentIndex = AD_UNDEFINED;
        for(size_t i = 0; i < results.size(); i++)
        {
            if(results[i]->current)
            {
                currentIndex = i;
                break;
            }
        }
    }

	// Удаление неправильных результатов из списка результатов и сохранение состояния.
    template <class TValidator> void TUndoRedoStage::RemoveInvalid(const TValidator &validator, TStatus *pStatus, bool canCancel)
    {
        pStatus->SetProgress(0, 0);
        if(!pStatus->Stopped())
        {
            bool searchValidCurrent = false;
            ptrdiff_t removedDefects = 0;
            ptrdiff_t removedDuplPair = 0;
            TResultPtrList validList;
            for(size_t i = 0; i < results.size() && (!canCancel || !pStatus->Stopped()); i++)
            {
                pStatus->SetProgress(i, results.size());

                if(i == currentIndex)
                    searchValidCurrent = true;
                TResultPtr pResult = results[i];

                switch(pResult->type)
                {
                case AD_RESULT_DEFECT_IMAGE:
                    if(validator(pResult->first))
                    {
                        validList.push_back(pResult);
                        if(searchValidCurrent)
                        {
                            currentIndex = validList.size() - 1;
                            validList.back()->current = true;
                            validList.back()->selected = true;
                            searchValidCurrent = false;
                        }
                    }
                    else
                    {
                        if(change)
                            change->removedResults.push_back(pResult);
                        groups.Get(pResult->group)->invalidHint = true;
                        removedDefects++;
                    }
                    break;
                case AD_RESULT_DUPL_IMAGE_PAIR:
                    if(validator(pResult->first, pResult->second)) //если хоть одна удалена
                    {
                        validList.push_back(pResult);
                        if(searchValidCurrent)
                        {
                            currentIndex = validList.size() - 1;
                            validList.back()->current = true;
                            validList.back()->selected = true;
                            searchValidCurrent = false;
                        }
                    }
                    else
                    {
                        if(change)
                            change->removedResults.push_back(pResult);
                        groups.Get(pResult->group)->invalidHint = true;
                        removedDuplPair++;
                    }
                    break;
                }
            }

            if(!canCancel || !pStatus->Stopped())
            {
                if(searchValidCurrent)
                {
                    if(validList.size())
                    {
                        currentIndex = validList.size() - 1;
                        validList.back()->current = true;
                        validList.back()->selected = true;
                    }
                    else
                        currentIndex = AD_UNDEFINED;
                }

                pStatus->AddDefectImage(-removedDefects);
                pStatus->AddDuplImagePair(-removedDuplPair);

                results.assign(validList.begin(), validList.end());
            }
        }
        pStatus->Reset();
    }
}
