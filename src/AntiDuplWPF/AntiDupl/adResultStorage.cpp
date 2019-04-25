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
#include <algorithm>
#include <list>
#include <vector>
#include <map>

#include <math.h>

#include "adThreads.h"
#include "adFileUtils.h"
#include "adImageInfoStorage.h"
#include "adStatus.h"
#include "adOptions.h"
#include "adEngine.h"
#include "adResult.h"
#include "adHintSetter.h"
#include "adDuplResultFilter.h"
#include "adUndoRedoTypes.h"
#include "adResultStorage.h"
#include "adFileStream.h"

namespace ad
{
	const char RESULT_CONTROL_BYTES[] = "adr";

    //-------------------------------------------------------------------------
    TResultStorage::TResultStorage(TEngine *pEngine)
        :m_pOptions(pEngine->Options()),
        m_pStatus(pEngine->Status()),
        m_nextId(0)
    {
        m_pCriticalSection = new TCriticalSection();
        m_pImageInfoStorage = new TImageInfoStorage(pEngine);
        m_pDuplResultFilter = new TDuplResultFilter();
        //m_pUndoRedoEngine = new TUndoRedoEngine(pEngine, m_pImageInfoStorage);
		m_pCurrent = new TUndoRedoStage();
    }

    TResultStorage::~TResultStorage()
    {
        Clear();
        delete m_pCriticalSection;
        //delete m_pUndoRedoEngine;
        delete m_pImageInfoStorage;
        delete m_pDuplResultFilter;
		delete m_pCurrent;
    }

    bool TResultStorage::AddDuplImagePair(const TImageInfoPtr first, 
        const TImageInfoPtr second, 
        double difference, 
        TTransformType transform)
    {
        TCriticalSection::TLocker locker(m_pCriticalSection);

        if(m_pCurrent->results.size() >= (size_t)m_pOptions->advanced.resultCountMax)
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
        if(m_pOptions->compare.transformedImage && m_pDuplResultFilter->AlreadyHas(pResult))
        {
            delete pResult;
            return false;
        }
        m_pCurrent->results.push_back(pResult);
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

        if(m_pCurrent->results.size() >= (size_t)m_pOptions->advanced.resultCountMax)
        {
            m_pStatus->Stop();
            return false;
        }

        TResultPtr pResult = new TResult();
        pResult->type = AD_RESULT_DEFECT_IMAGE;
        pResult->first = m_pImageInfoStorage->Add(info);
        pResult->second = m_pImageInfoStorage->GetStub();
        pResult->defect = defect;
        m_pCurrent->results.push_back(pResult);
        pResult->id = m_nextId++;
        pResult->first->links++;
        m_pStatus->AddDefectImage();
        return true;
    }

    void TResultStorage::Clear()
    {
        m_pCurrent->Clear();
        //m_pUndoRedoEngine->Clear();
        m_pImageInfoStorage->Clear();
        m_pDuplResultFilter->Clear();
        m_nextId = 0;
    }

    void TResultStorage::Sort(TSortType sortType, bool increasing)
    {
        m_pCurrent->Sort(sortType, increasing);
    }

    void TResultStorage::Refresh()
    {
        //m_pUndoRedoEngine->Clear();
        m_pCurrent->RemoveInvalid(m_pStatus);
		m_pCurrent->RemoveSkipped(m_pStatus, m_pOptions); //удаляет из результатов пропускаемые
        m_pCurrent->SetGroups(m_pStatus); //очищает внутреннее хранилище групп
		m_pCurrent->UpdateGroups(); //обновляет группы из результатов
        m_pCurrent->UpdateHints(m_pOptions, true);
    }

    adError TResultStorage::Export(adSizePtr pStartFrom, adResultPtrA pResult, adSizePtr pResultSize) const
    {
        return m_pCurrent->Export(pStartFrom, pResult, pResultSize);
    }

    adError TResultStorage::Export(adSizePtr pStartFrom, adResultPtrW pResult, adSizePtr pResultSize) const
    {
        return m_pCurrent->Export(pStartFrom, pResult, pResultSize);
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
			TResultPtrVector &results = m_pCurrent->results;
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
						m_pStatus->AddDefectImage();
						results.push_back(new TResult(result));
						results.back()->id = m_nextId++;
						results.back()->first->links++;
						if(results.back()->first->group == AD_UNDEFINED)
							results.back()->first->group = results.back()->group;
					}
				}
				if(result.type == AD_RESULT_DUPL_IMAGE_PAIR)
				{
					result.first = m_pImageInfoStorage->Get((size_t)result.first);
					result.second = m_pImageInfoStorage->Get((size_t)result.second);
					if(!check || (result.first->Actual() && result.second->Actual()))
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
			m_pCurrent->UpdateGroups();
			m_pCurrent->UpdateHints(m_pOptions, true);
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

			TResultPtrVector &results = m_pCurrent->results;
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
            TResultPtrVector &results = m_pCurrent->results;
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
            /*switch (actionEnableType)
            {
            case AD_ACTION_ENABLE_UNDO:
                return m_pUndoRedoEngine->UndoEnable();
            case AD_ACTION_ENABLE_REDO:
                return m_pUndoRedoEngine->RedoEnable();
            }*/
        }

        return false;
    }

	unsigned int * TResultStorage::GetResultSize() const
	{
		return (unsigned int *)m_pCurrent->results.size();
	}

	void TResultStorage::GetResult(adResultPtrW pResult, const unsigned int & index) const
	{
		m_pCurrent->results[index]->Export(pResult);
	}

}
