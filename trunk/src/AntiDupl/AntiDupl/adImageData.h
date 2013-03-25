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
		TUInt32 crc32;
		TPixelDataPtr data;
		HGLOBAL hGlobal; 

		TImageData(const TString& path_ = TString()) : TImageInfo(path_) {Init();}
		TImageData(const TString& path_, TUInt64 size_, TUInt64 time_) : TImageInfo(path_, size_, time_) {Init();}
		TImageData(const TFileInfo& fileInfo) : TImageInfo(fileInfo) {Init();}
		TImageData(const TImageData& imageData) :TImageInfo(imageData) {Init(); *this = imageData;};
		virtual ~TImageData();

		TImageData& operator = (const TImageData& imageData);

		bool PixelDataFillingNeed(TOptions *pOptions) const;
		bool DefectCheckingNeed(TOptions *pOptions) const;

		void FillOther(TOptions *pOptions);

		void Turn(TUInt8 *pBuffer);
		void Mirror(TUInt8 *pBuffer);

		void FreeGlobal();

		bool Load(HANDLE hIn);
		bool Save(HANDLE hOut) const;

		bool NeedToSave() const;

		bool SetData(size_t reducedImageSize);

	private:
		void Init();

		bool m_owner;
	};
	typedef TImageData* TImageDataPtr;
	typedef std::list<TImageDataPtr> TImageDataPtrs;
}

#endif/*__adImageData_h__*/