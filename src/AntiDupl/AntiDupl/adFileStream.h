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
#ifndef __adFileStream_h__
#define __adFileStream_h__

#include "adConfig.h"
#include "adStrings.h"
#include "adException.h"

namespace ad
{
	class TFileStream
	{
	protected:
		TString m_fileName;
		std::string m_format;
		TUInt32 m_version;
		HANDLE m_hFile;

	public:
		TFileStream(const TChar * fileName, const char * format);
		virtual ~TFileStream();

		TUInt32 Version() const {return m_version;}
		TString FileName() const {return m_fileName;}
		TString Format() const {return m_format;}
	};

	//-------------------------------------------------------------------------

	class TInputFileStream : public TFileStream
	{
	public:
		TInputFileStream(const TChar * fileName, const char * format);

		void Load(void * buffer, size_t size) const;

		template <class T> T Load() const
		{
			T t;
			Load(&t, sizeof(T));
			return t;
		}

		template <class T> void Load(T & t) const 
		{	
			Load(&t, sizeof(T)); 
		}

		template <class T> T LoadChecked(T tMin, T tMax) const
		{
			T t = Load<T>();
			if(t < tMin || t > tMax)
				throw TException(AD_ERROR_INVALID_FILE_FORMAT);
			return t;
		}

		template <class T> void LoadChecked(T & t, T tMin, T tMax) const 
		{	
			t = LoadChecked(tMin, tMax); 
		}

		template <class T> void Check(const T & t) const 
		{	
			LoadChecked(t, t); 
		}

		size_t LoadSize() const;
		void LoadSize(size_t & size) const;
		size_t LoadSizeChecked(size_t sizeMax) const;
		void LoadSizeChecked(size_t & size, size_t sizeMax) const;

		void Load(TString & string) const;
		void Load(class TPath & path) const;
		void Load(struct TFileInfo & fileInfo) const;
		void Load(struct TImageInfo & imageInfo) const;
		void Load(struct TPixelData & pixelData) const;
		void Load(struct TImageData & imageData) const;
		void Load(struct TResult & result) const;
	};

	//-------------------------------------------------------------------------

	class TOutputFileStream : public TFileStream
	{
	public:
		TOutputFileStream(const TChar * fileName, const char * format);

		void Save(const void * buffer, size_t size) const;

		template <class T> void Save(T t) const 
		{ 
			Save(&t, sizeof(T)); 
		}

		void SaveSize(size_t size) const;

		void Save(const TString & string) const;
		void Save(const class TPath & path) const;
		void Save(const struct TFileInfo & fileInfo) const;
		void Save(const struct TImageInfo & imageInfo) const;
		void Save(const struct TPixelData & pixelData) const;
		void Save(const struct TImageData & imageData) const;
		void Save(const struct TResult & result) const;
	};
}

#endif//__adFileStream_h__ 