/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2013 Yermalayeu Ihar.
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
#include "adImageMultiData.h"

namespace ad
{
    bool TImageMultiData::SetData(size_t reducedImageSize)
    {
        TPixelDataPtr pData = GetData(reducedImageSize);
        if(pData == NULL)
        {
            pData = new TPixelData(reducedImageSize);
            m_data.push_back(pData);
        }
        data = pData;
        return data->filled;
    }

    TPixelDataPtr TImageMultiData::GetData(size_t reducedImageSize)
    {
        for(TPixelDataPtrs::const_iterator it = m_data.begin(); it != m_data.end(); it++)
            if((*it)->side == reducedImageSize)
                return *it;
        return NULL;
    }

    void TImageMultiData::Clear()
    {
        for(TPixelDataPtrs::const_iterator it = m_data.begin(); it != m_data.end(); it++)
            delete *it;
        m_data.clear();
    }

    bool TImageMultiData::Load(HANDLE hIn)
    {
        if(!(static_cast<TImageInfo*>(this))->Load(hIn))
            return false;

        READ_VALUE_FROM_FILE(hIn, defect);
        READ_VALUE_FROM_FILE(hIn, crc32);

        size_t size;
        READ_SIZE_FROM_FILE(hIn, size);

        Clear();
        for(size_t i = 0; i < size; i++)
        {
            TPixelDataPtr data = TPixelData::Load(hIn);
            if(data)
            {
                if(GetData(data->side) == NULL)
                    m_data.push_back(data);
                else
                    delete data;
            }
            else
            {
                Clear();
                return false;
            }
        }

        return true;
    }

    bool TImageMultiData::Save(HANDLE hOut) const
    {
        if(!(static_cast<const TImageInfo*>(this))->Save(hOut))
            return false;

        WRITE_VALUE_TO_FILE(hOut, defect);
        WRITE_VALUE_TO_FILE(hOut, crc32);

        size_t size = FilledDataSize();
        WRITE_SIZE_TO_FILE(hOut, size);

        for(TPixelDataPtrs::const_iterator it = m_data.begin(); it != m_data.end(); it++)
        {
            if((*it)->filled)
            {
                if(!(*it)->Save(hOut))
                    return false;
            }
        }

        return true;
    }

    size_t TImageMultiData::FilledDataSize() const
    {
        size_t size = 0;
        for(TPixelDataPtrs::const_iterator it = m_data.begin(); it != m_data.end(); it++)
        {
            if((*it)->filled)
                size++;
        }
        return size;
    }
}