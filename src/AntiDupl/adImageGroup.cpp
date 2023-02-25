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
#include "adResult.h"
#include "adStatus.h"
#include "adHintSetter.h"
#include "adImageGroup.h"

namespace ad
{
	TImageGroup::TImageGroup(size_t id_)
		: id(id_)
		, invalidHint(false)
	{
	}

	TImageGroup::TImageGroup(const TImageGroup & imageGroup)
		: id(imageGroup.id)
		, invalidHint(imageGroup.invalidHint)
	{
	}

	// Копируем из списка результатов информацию об изображениях в наш контейнер images.
	void TImageGroup::UpdateImages()
	{
		TImageInfoPtrSet buffer;
		for(TResultPtrList::iterator it = results.begin(); it != results.end(); ++it)
		{
			TResultPtr pResult = *it;
			buffer.insert(pResult->first);
			if(pResult->type == AD_RESULT_DUPL_IMAGE_PAIR)
				buffer.insert(pResult->second);
		}
		images.assign(buffer.begin(), buffer.end());
	}

	// Экспортируем в группу для экспорта из dll.
	bool TImageGroup::Export(adGroupPtr pGroup) const
	{
		if(pGroup == NULL)
			return false;

		pGroup->id = id;
		pGroup->size = images.size();

		return true;
	}
	//-------------------------------------------------------------------------
	TImageGroupStorage::TImageGroupStorage()
	{
	}

	TImageGroupStorage::~TImageGroupStorage()
	{
		Clear();
	}

	// Очищаем карту
	void TImageGroupStorage::Clear()
	{
		for(TMap::iterator it = m_map.begin(); it != m_map.end(); ++it)
			delete it->second;
		m_map.clear();
	}

	// Получаем или создаем группу с переданным ID
	TImageGroupPtr TImageGroupStorage::Get(size_t id, bool create)
	{
		TMap::iterator it = m_map.find(id);
		// Если не находим в карте группы с данным ID
		if(it == m_map.end())
		{
			TImageGroupPtr pImageGroup = NULL;
			if(create)
			{
				pImageGroup = new TImageGroup(id);
				m_map[id] = pImageGroup;
			}
			return pImageGroup;
		}
		else
			return it->second;
	}

	// Удаляем группу с данным ID из карты.
	void TImageGroupStorage::Erase(size_t id)
	{
		TMap::iterator it = m_map.find(id);
		if(it != m_map.end())
		{
			delete it->second;
			m_map.erase(it);
		}
	}

	// Копируем содержимое карты в вектор.
	void TImageGroupStorage::UpdateVector()
	{
		m_vector.clear();
		m_vector.reserve(m_map.size());
		for(TMap::iterator it = m_map.begin(); it != m_map.end(); ++it)
		{
			TImageGroupPtr pImageGroup = it->second;
			m_vector.push_back(pImageGroup);
		}
	}

	// Копируем из переданного хранилиша в карту
	void TImageGroupStorage::Assign(const TImageGroupStorage & storage)
	{
		Clear();
		for(TMap::const_iterator it = storage.m_map.begin(); it != storage.m_map.end(); ++it)
			m_map[it->first] = new TImageGroup(*it->second);
	}

	// Устанавливает в переданном списке результатов группы из внутреннего хранилища групп и очищает его.
	void TImageGroupStorage::Set(TResultPtrVector & results, TStatus * pStatus)
	{
		pStatus->SetProgress(0, 0);
		size_t current = 0, total = results.size(), groupId = 0;
		Clear();
		// Идем по списку результатов
		for(TResultPtrVector::iterator it = results.begin(); it != results.end(); ++it)
		{
			pStatus->SetProgress(current++, total);
			TResultPtr pResult = *it;
			if(pResult->group == AD_UNDEFINED)
			{
				if(pResult->type == AD_RESULT_DUPL_IMAGE_PAIR)
				{
					// Если обе группы не определены, находим ее с хранилише и заполняем значения.
					if(pResult->first->group == AD_UNDEFINED && pResult->second->group == AD_UNDEFINED)
					{
						TImageGroup * pGroup = Get(groupId++);
						pResult->group = pGroup->id;
						pResult->first->group = pGroup->id;
						pResult->second->group = pGroup->id;
					}
					else
					{
						// Если первая группа не определена, копируем значения из второй
						if(pResult->first->group == AD_UNDEFINED)
						{
							pResult->group = pResult->second->group;
							pResult->first->group = pResult->second->group;
						}
						else if(pResult->second->group == AD_UNDEFINED)
						{
							pResult->group = pResult->first->group;
							pResult->second->group = pResult->first->group;
						}
						else if(pResult->first->group == pResult->second->group)
						{
							pResult->group = pResult->first->group;
						}
						else
						{
							TImageGroup * src = Get(std::min(pResult->first->group, pResult->second->group));
							TImageGroup * dst = Get(std::max(pResult->first->group, pResult->second->group));
							for(TResultPtrList::iterator srcIt = src->results.begin(); srcIt != src->results.end(); ++srcIt)
							{
								(*srcIt)->group = dst->id;
								(*srcIt)->first->group = dst->id;
								if((*srcIt)->type == AD_RESULT_DUPL_IMAGE_PAIR)
									(*srcIt)->second->group = dst->id;
								dst->results.push_back(*srcIt);
							}
							Erase(src->id);
							pResult->group = dst->id;
						}
					}
				}
				else
				{
					if(pResult->first->group == AD_UNDEFINED)
					{
						TImageGroup * pGroup = Get(groupId++);
						pResult->group = pGroup->id;
						pResult->first->group = pGroup->id;
					}
					else
					{
						pResult->group = pResult->first->group;
					}
				}
				Get(pResult->group)->results.push_back(pResult);
			}
		}

		for(TMap::iterator groupIt = m_map.begin(); groupIt != m_map.end(); ++groupIt)
		{
			TImageGroupPtr pImageGroup = groupIt->second;
			pImageGroup->UpdateImages();
		}

		SetGroupSize(results);

		UpdateVector();

		pStatus->Reset();
	}

	// Обновляем содержимое хранилиша групп в соответсвие с переданными результатами.
	void TImageGroupStorage::Update(TResultPtrVector & results)
	{
		// Очишаем результаты для групп в хранилише
		for(TMap::iterator groupIt = m_map.begin(); groupIt != m_map.end(); ++groupIt)
		{
			TImageGroupPtr pImageGroup = groupIt->second;
			pImageGroup->results.clear();
		}

		// заполняем результаты в группах
		for(TResultPtrVector::iterator resultIt = results.begin(); resultIt != results.end(); ++resultIt)
		{
			TResultPtr pResult = *resultIt;
			TImageGroupPtr pImageGroup = Get(pResult->group);
			pImageGroup->results.push_back(pResult);
		}

		for(TMap::iterator groupIt = m_map.begin(); groupIt != m_map.end(); ++groupIt)
		{
			TImageGroupPtr pImageGroup = groupIt->second;
			if(pImageGroup->results.empty())
			{
				groupIt = m_map.erase(groupIt);
				if(groupIt == m_map.end())
					break;
			}
			else
				pImageGroup->UpdateImages();
		}

		SetGroupSize(results);

		UpdateVector();
	}

	void TImageGroupStorage::UpdateHints(TOptions *pOptions, bool force, TStatus * pStatus)
	//void TImageGroupStorage::UpdateHints(TOptions *pOptions, bool force)
	{
		pStatus->SetProgress(0, 0);
		size_t current = 0, total = m_map.size();
		//THintSetter hintSetter(pOptions);
		//m_hintSetter_pointer = GetHintSetterPointer(pOptions);
		THintSetter *hintSetter_pointer = THintSetterStorage::GetHintSetterPointer(pOptions);
		//hintSetter_pointer->Load();
		for(TMap::iterator groupIt = m_map.begin(); groupIt != m_map.end(); ++groupIt)
		{
			pStatus->SetProgress(current++, total);
			TImageGroupPtr pImageGroup = groupIt->second;
			if(force || pImageGroup->invalidHint)
			{
				TResultPtrList & results = pImageGroup->results;
				for(TResultPtrList::iterator resultIt = results.begin(); resultIt != results.end(); ++resultIt)
				{
					TResultPtr pResult = *resultIt;
					hintSetter_pointer->Execute(pResult, results.size() == 1);
				}
			}
			pImageGroup->invalidHint = false;
		}
		pStatus->Reset();
	}
	
	void TImageGroupStorage::SetGroupSize(TResultPtrVector & results)
	{
		for(TResultPtrVector::iterator it = results.begin(); it != results.end(); ++it)
		{
			TResultPtr pResult = *it;
			if(pResult->type == AD_RESULT_DUPL_IMAGE_PAIR)
			{
				TImageGroup * pGroup = Get(pResult->group, false);
				if (pGroup)
					pResult->groupSize = pGroup->images.size();
			}
			else if(pResult->type == AD_RESULT_DEFECT_IMAGE)
				pResult->groupSize = 1;
		}
	}

	adError TImageGroupStorage::Export(adSizePtr pStartFrom, adGroupPtr pGroup, adSizePtr pGroupSize) const
	{
		if(pGroup == NULL || pGroupSize == NULL || pStartFrom == NULL)
			return AD_ERROR_INVALID_POINTER;
		if(m_vector.size() < *pStartFrom)
		{
			*pStartFrom = m_vector.size();
			return AD_ERROR_INVALID_START_POSITION;
		}

		adGroupPtr p = pGroup;
		*pGroupSize = std::min(m_vector.size() - *pStartFrom, *pGroupSize);
		for(size_t i = *pStartFrom, n = *pStartFrom + *pGroupSize; i < n; ++i)
		{
			m_vector[i]->Export(p);
			p++;
		}

		return AD_OK;
	}

	adError TImageGroupStorage::Export(adSize groupId, adSizePtr pStartFrom, adImageInfoPtrA pImageInfo, adSizePtr pImageInfoSize) const
	{
		if(pImageInfo == NULL || pImageInfoSize == NULL || pStartFrom == NULL)
			return AD_ERROR_INVALID_POINTER;

		const TImageGroupPtr pImageGroup = ((TImageGroupStorage*)this)->Get(groupId, false);
		if(pImageGroup == NULL)
			return AD_ERROR_INVALID_GROUP_ID;

		const TImageInfoPtrVector & images = pImageGroup->images;
		if(images.size() < *pStartFrom)
		{
			*pStartFrom = images.size();
			return AD_ERROR_INVALID_START_POSITION;
		}

		adImageInfoPtrA p = pImageInfo;
		*pImageInfoSize = std::min(images.size() - *pStartFrom, *pImageInfoSize);
		for(size_t i = *pStartFrom, n = *pStartFrom + *pImageInfoSize; i < n; ++i)
		{
			images[i]->Export(p);
			p++;
		}

		return AD_OK;
	}

	adError TImageGroupStorage::Export(adSize groupId, adSizePtr pStartFrom, adImageInfoPtrW pImageInfo, adSizePtr pImageInfoSize) const
	{
		if(pImageInfo == NULL || pImageInfoSize == NULL || pStartFrom == NULL)
			return AD_ERROR_INVALID_POINTER;

		const TImageGroupPtr pImageGroup = ((TImageGroupStorage*)this)->Get(groupId, false);
		if(pImageGroup == NULL)
			return AD_ERROR_INVALID_GROUP_ID;

		const TImageInfoPtrVector & images = pImageGroup->images;
		if(images.size() < *pStartFrom)
		{
			*pStartFrom = images.size();
			return AD_ERROR_INVALID_START_POSITION;
		}

		adImageInfoPtrW p = pImageInfo;
		*pImageInfoSize = std::min(images.size() - *pStartFrom, *pImageInfoSize);
		for(size_t i = *pStartFrom, n = *pStartFrom + *pImageInfoSize; i < n; ++i)
		{
			images[i]->Export(p);
			p++;
		}

		return AD_OK;
	}

	adError TImageGroupStorage::SetSelection(adSize groupId, adSize index, adSelectionType selectionType)
	{
		if(selectionType < AD_SELECTION_SELECT_CURRENT || selectionType >= AD_SELECTION_SIZE)            
			return AD_ERROR_INVALID_SELECTION_TYPE;

		TImageGroupPtr pImageGroup = ((TImageGroupStorage*)this)->Get(groupId, false);
		if(pImageGroup == NULL)
			return AD_ERROR_INVALID_GROUP_ID;

		TImageInfoPtrVector & images = pImageGroup->images;

		switch(selectionType)
		{
		case AD_SELECTION_SELECT_CURRENT:
			if(index >= images.size())
				return AD_ERROR_INVALID_INDEX;
			images[index]->selected = true;
			break;
		case AD_SELECTION_UNSELECT_CURRENT:
			if(index >= images.size())
				return AD_ERROR_INVALID_INDEX;
			images[index]->selected = false;
			break;
		case AD_SELECTION_SELECT_ALL:
			for(size_t i = 0; i < images.size(); ++i)
				images[i]->selected = true;
			break;
		case AD_SELECTION_UNSELECT_ALL:
			for(size_t i = 0; i < images.size(); ++i)
				images[i]->selected = false;
			break;
		case AD_SELECTION_SELECT_ALL_BUT_THIS:
			if(index >= images.size())
				return AD_ERROR_INVALID_INDEX;
			for(size_t i = 0; i < images.size(); ++i)
				images[i]->selected = true;
			images[index]->selected = false;
			break;
		default:
			return AD_ERROR_INVALID_SELECTION_TYPE;
		}

		return AD_OK;
	}

	adError TImageGroupStorage::GetSelection(adSize groupId, adSizePtr pStartFrom, adBoolPtr pSelection, adSizePtr pSelectionSize) const
	{
		if(pSelection == NULL || pSelectionSize == NULL || pStartFrom == NULL)
			return AD_ERROR_INVALID_POINTER;

		const TImageGroupPtr pImageGroup = ((TImageGroupStorage*)this)->Get(groupId, false);
		if(pImageGroup == NULL)
			return AD_ERROR_INVALID_GROUP_ID;

		const TImageInfoPtrVector & images = pImageGroup->images;
		if(images.size() < *pStartFrom)
		{
			*pStartFrom = images.size();
			return AD_ERROR_INVALID_START_POSITION;
		}

		adBoolPtr p = pSelection;
		*pSelectionSize = std::min(images.size() - *pStartFrom, *pSelectionSize);
		for(size_t i = *pStartFrom, n = *pStartFrom + *pSelectionSize; i < n; ++i)
		{
			*p = (images[i]->selected ? TRUE : FALSE);
			p++;
		}

		return AD_OK;
	}
}
