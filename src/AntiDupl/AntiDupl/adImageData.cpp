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
#include "adOptions.h"
#include "adImageData.h"
#include "adIO.h"

namespace ad
{
	TImageData::TImageData(size_t reducedImageSize)
	{
		Init();
		SetData(reducedImageSize);
	}

	TImageData::TImageData(const TFileInfo& fileInfo, size_t reducedImageSize) 
		: TImageInfo(fileInfo) 
	{
		Init();
		SetData(reducedImageSize);
	}

	TImageData::TImageData(const TImageData& imageData) 
	{
		Init(); 
		*this = imageData;
	}

	void TImageData::Init()
	{
		ratio = 0;
		valid = false;
		index = AD_IS_NOT_EXIST;
		defect = AD_DEFECT_UNDEFINE;
		crc32 = 0;
		data = NULL;
		m_owner = false;
		hGlobal = NULL;
	}

	void TImageData::SetData(size_t reducedImageSize)
	{
		if(data == NULL || data->side != reducedImageSize)
		{
			if(m_owner && data != NULL)
			{
				delete data;
				m_owner = false;
			}
			data = new TPixelData(reducedImageSize);
			m_owner = true;
		}
	}

	TImageData::~TImageData()
	{
		if(m_owner)
		{
			delete data;
		}
		FreeGlobal();
	}

	TImageData& TImageData::operator = (const TImageData& imageData)
	{
		*(static_cast<TImageInfo*>(this)) = imageData;
		ratio = imageData.ratio;
		valid = imageData.valid;
		defect = imageData.defect;
		crc32 = imageData.crc32;
		if(m_owner && imageData.data->side != data->side)
		{
			delete data;
			m_owner = false;
		}
		if(!m_owner)
		{
			data = new TPixelData(*imageData.data);
			m_owner = true;
		}
		else
		{
			memcpy(data->fast, imageData.data->fast, data->full);
		}
		return *this;
	}

	bool TImageData::PixelDataFillingNeed(TOptions *pOptions) const
	{
		return (pOptions->check.checkOnEquality == TRUE || pOptions->check.checkOnDefect == TRUE) && 
			!data->filled && type != AD_IMAGE_NONE;
	}

	bool TImageData::DefectCheckingNeed(TOptions *pOptions) const
	{
		return pOptions->check.checkOnDefect == TRUE && defect == AD_DEFECT_UNDEFINE;
	}

	void TImageData::FillOther(TOptions *pOptions)
	{
		if(type > AD_IMAGE_NONE)
		{
			data->FillFast(pOptions->GetIgnoreWidthFrame());

			const int resolution = pOptions->advanced.ratioResolution;
			if(width > height)
			{
				ratio = height*resolution/width - resolution;
			}
			else
			{
				ratio = resolution - width*resolution/height;
			}

			valid = pOptions->validPaths.IsHasSubPath(path) != AD_IS_NOT_EXIST || 
				pOptions->validPaths.IsHasPath(path) != AD_IS_NOT_EXIST;

			index = std::min(pOptions->searchPaths.IsHasSubPath(path), pOptions->searchPaths.IsHasPath(path));
		}
	}

	void TImageData::Turn(TUInt8 *pBuffer)
	{
		std::swap(width, height);
		ratio = -ratio;
		data->Turn(pBuffer);
	}

	void TImageData::Mirror(TUInt8 *pBuffer)
	{
		data->Mirror(pBuffer);
	}

	void TImageData::FreeGlobal()
	{
		if(hGlobal)
		{
			::GlobalFree(hGlobal);
			hGlobal = NULL;
		}
	}

	bool TImageData::NeedToSave() const 
	{
		return (data != NULL && data->filled) || defect != AD_DEFECT_UNDEFINE;
	}
}