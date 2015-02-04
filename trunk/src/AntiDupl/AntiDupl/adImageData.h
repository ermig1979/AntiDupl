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
#ifndef __adImageData_h__
#define __adImageData_h__

#include "adConfig.h"
#include "adImageInfo.h"
#include "adPixelData.h"

namespace ad
{
	struct TOptions;
	//-------------------------------------------------------------------------
	struct TImageData : public TImageInfo
	{
		TInt32 ratio; // Ratio between height and width of image;
		bool valid; // The Image lie in 'valid' directory;
		size_t index; // Index of the path where this image were found;
		TDefectType defect;
		TUInt32 crc32c;
		TPixelDataPtr data;
		HGLOBAL hGlobal; 

		TImageData(size_t reducedImageSize);
		TImageData(const TFileInfo& fileInfo, size_t reducedImageSize);
		TImageData(const TImageData& imageData);
		virtual ~TImageData();

		TImageData& operator = (const TImageData& imageData);

		bool PixelDataFillingNeed(const TOptions *pOptions) const;
		bool DefectCheckingNeed(const TOptions *pOptions) const;

		void FillOther(TOptions *pOptions);

		TDefectType GetDefect(const TOptions *pOptions) const;

		void Turn(TUInt8 *pBuffer);
		void Mirror(TUInt8 *pBuffer);

		void FreeGlobal();

		bool NeedToSave() const;

	private:
		void Init();
		void SetData(size_t reducedImageSize);

		bool m_owner; // если владеет данными TPixelDataPtr data (заполнены)
	};
	typedef TImageData* TImageDataPtr;
	typedef std::list<TImageDataPtr> TImageDataPtrs;
}

#endif/*__adImageData_h__*/