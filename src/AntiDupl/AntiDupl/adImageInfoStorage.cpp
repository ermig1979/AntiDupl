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
#include "adIO.h"
#include "adEngine.h"
#include "adStatus.h"
#include "adImageInfoStorage.h"
#include "adFileStream.h"

namespace ad
{
    TImageInfoStorage::TImageInfoStorage(TEngine *pEngine)
        :m_pStatus(pEngine->Status())
    {
    }

    TImageInfoPtr TImageInfoStorage::Add(const TImageInfoPtr pImageInfo)
    {
        TAddMap::iterator it = m_addMap.find(pImageInfo); 
        if(it == m_addMap.end())
        {
            TImageInfoPtr &p = m_addMap[pImageInfo];
            p = new TImageInfo(*pImageInfo);
            m_mainList.push_back(p);
            return p;
        }
        return it->second;
    }

    TImageInfoPtr TImageInfoStorage::Get(size_t index) const 
    {
        return (index < m_loadVector.size() ? m_loadVector[index] : NULL);
    }

	void TImageInfoStorage::Load(const TInputFileStream & inputFile, bool check)
	{
		Clear();

		size_t size = inputFile.LoadSizeChecked(SIZE_CHECK_LIMIT);
		m_loadVector.reserve(size);
		TImageInfo imageInfo;
		for(size_t i = 0; i < size; i++)
		{
			inputFile.Load(imageInfo);
			TImageInfo *pImageInfo = new TImageInfo(imageInfo);
			m_loadVector.push_back(pImageInfo);
			m_mainList.push_back(pImageInfo);
			m_pStatus->SetProgress(i, size);
			if(m_pStatus->Stopped())
				return;
		}

		for(size_t i = 0; i < size; i++)
		{
			TImageInfo *pImageInfo = m_loadVector[i];
			pImageInfo->removed = check && !pImageInfo->Actual();
			m_pStatus->SetProgress(i, size);
			if(m_pStatus->Stopped())
				return;
		}
	}

	void TImageInfoStorage::Save(const TOutputFileStream & outputFile) const
	{
		size_t size = m_mainList.size();
		outputFile.SaveSize(size);
		size_t index = 0;
		for(TMainList::const_iterator it = m_mainList.begin(); it != m_mainList.end(); ++it, ++index)
		{
			(*it)->index = index;
			outputFile.Save(**it);
			m_pStatus->SetProgress(index, size);
			if(m_pStatus->Stopped())
				return;
		}
	}

    void TImageInfoStorage::Clear()
    {
        for(TMainList::iterator it = m_mainList.begin(); it != m_mainList.end(); ++it)
            delete *it;
        m_mainList.clear();
        m_loadVector.clear();
        m_addMap.clear();
    }

    void TImageInfoStorage::RemoveUnlinked()
    {
        for(TMainList::iterator it = m_mainList.begin(); it != m_mainList.end();)
        {
            if((*it)->links == 0)
            {
                delete *it;
                it = m_mainList.erase(it);
            }
            else
                ++it;
        }
    }
}