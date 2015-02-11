/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2015 Yermalayeu Ihar, 2015 Borisov Dmitry.
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

#include "adFileStream.h"
#include "adPath.h"
#include "adFileInfo.h"
#include "adImageInfo.h"
#include "adImageData.h"
#include "adFileUtils.h"
#include "adResult.h"

namespace ad
{
	TFileStream::TFileStream(const TChar * fileName, const char * format)
		: m_fileName(fileName)
		, m_format(format)
		, m_hFile(INVALID_HANDLE_VALUE)
		, m_version(FILE_VERSION)
	{
	}

	TFileStream::~TFileStream()
	{
		if(m_hFile != INVALID_HANDLE_VALUE)
			::CloseHandle(m_hFile);
	}

	//-------------------------------------------------------------------------

	TInputFileStream::TInputFileStream(const TChar * fileName, const char * format_)
		:TFileStream(fileName, format_)
	{
		if(!IsFileExists(fileName))
			throw TException(AD_ERROR_FILE_IS_NOT_EXIST);

		m_hFile = CreateFile(m_fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE)
			throw TException(AD_ERROR_CANT_OPEN_FILE);

		try
		{
			std::string format = m_format;
			Load((void*)format.c_str(), m_format.size());
			if(format != m_format)
				throw TException(AD_ERROR_INVALID_FILE_FORMAT);

			m_version = LoadChecked<TUInt32>(1, FILE_VERSION);
		}
		catch (TException e)
		{
			::CloseHandle(m_hFile);
			throw TException(e.Error);
		}
	}

	size_t TInputFileStream::LoadSize() const 
	{ 
		return (size_t)Load<TUInt64>(); 
	}

	void TInputFileStream::LoadSize(size_t & size) const 
	{ 
		size = LoadSize(); 
	}

	size_t TInputFileStream::LoadSizeChecked(size_t sizeMax) const
	{
		return (size_t)LoadChecked<TUInt64>(0, sizeMax); 
	}

	void TInputFileStream::LoadSizeChecked(size_t & size, size_t sizeMax) const
	{
		size = LoadSizeChecked(sizeMax); 
	}

	void TInputFileStream::Load(void * buffer, size_t size) const
	{
		DWORD byte_was_read; 
		BOOL result = ReadFile(m_hFile, buffer, (DWORD)size, &byte_was_read, NULL); 
		if(result != TRUE || byte_was_read < (DWORD)size) 
			throw TException(AD_ERROR_CANT_READ_FILE);
	}

	void TInputFileStream::Load(TString & string) const
	{
		size_t size = LoadSizeChecked(SIZE_CHECK_LIMIT);
		string.resize(size, 0);
		Load((void*)string.c_str(), size*sizeof(TChar));
	}

	void TInputFileStream::Load(TPath & path) const
	{
		TString string;
		Load(string);
		if(!TPath::Valid(string.size()))
			throw TException(AD_ERROR_INVALID_FILE_FORMAT);
		path = string;
	}

	void TInputFileStream::Load(TFileInfo & fileInfo) const
	{
		fileInfo = TFileInfo();
		Load(fileInfo.path);
		Load(fileInfo.size);
		Load(fileInfo.time);
		Load(fileInfo.hash);
		if(fileInfo.hash != fileInfo.path.GetCrc32())
			throw TException(AD_ERROR_INVALID_FILE_FORMAT);
	}

	// Загружаем данные изображения
	void TInputFileStream::Load(TImageInfo & imageInfo) const
	{
		//imageInfo = TImageInfo();
		Load((TFileInfo&)imageInfo);
		Load(imageInfo.type);
		Load(imageInfo.width);
		Load(imageInfo.height);
		if(m_version > 1)
			Load(imageInfo.blockiness); 
		if(m_version > 2)
			Load(imageInfo.blurring); 
		if(m_version > 3)
			LoadExif(imageInfo.imageExif);
	}

	// Считываем эскиз изображения
	void TInputFileStream::Load(TPixelData & pixelData) const
	{
		size_t side = LoadSize();
		if(side != pixelData.side)
			throw TException(AD_ERROR_INVALID_FILE_FORMAT);
		Load(pixelData.main, pixelData.size);
		if(m_version > 3)
		{
			Load(pixelData.average);
			Load(pixelData.varianceSquare);
		}
	}

	void TInputFileStream::Load(TImageData & imageData) const
	{
		Load((TImageInfo&)imageData);
		Load(imageData.defect);
		Load(imageData.crc32c);
		Load(imageData.data->filled);
		if(imageData.data->filled)
			Load(*imageData.data);
	}

	void TInputFileStream::Load(TResult & result) const
	{
		Load(result.type);
		result.first = (TImageInfoPtr)LoadSize();
		result.second = (TImageInfoPtr)LoadSize();
		Load(result.defect);
		Load(result.difference);
		Load(result.transform);
		LoadSizeChecked(result.group, SIZE_CHECK_LIMIT);
		Load(result.hint);
	}

	// Загружаем Exif
	void TInputFileStream::LoadExif(TImageExif * imageExif) const
	{
		Load(imageExif->isEmpty);
		Load(imageExif->imageDescription);
		Load(imageExif->equipMake);
		Load(imageExif->equipModel);
		Load(imageExif->softwareUsed);
		Load(imageExif->dateTime);
		Load(imageExif->artist);
		Load(imageExif->userComment);
	}

	//-------------------------------------------------------------------------

	TOutputFileStream::TOutputFileStream(const TChar * fileName, const char * format)
		:TFileStream(fileName, format)
	{
		m_hFile = CreateFile(m_fileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE)
			throw TException(AD_ERROR_CANT_CREATE_FILE);

		try
		{
			Save(m_format.c_str(), m_format.size());

			Save(m_version);
		}
		catch (TException e)
		{
			::CloseHandle(m_hFile);
			throw TException(e.Error);
		}
	}

	void TOutputFileStream::SaveSize(size_t size) const 
	{ 
		Save<TUInt64>(size); 
	}

	// Сохраняем Exif
	void TOutputFileStream::SaveExif(const TImageExif * imageExif) const
	{
		Save(imageExif->isEmpty);
		Save(imageExif->imageDescription);
		Save(imageExif->equipMake);
		Save(imageExif->equipModel);
		Save(imageExif->softwareUsed);
		Save(imageExif->dateTime);
		Save(imageExif->artist);
		Save(imageExif->userComment);
	}

	// Записываем данные любого типа заданого размера
	void TOutputFileStream::Save(const void * buffer, size_t size) const
	{
		DWORD byte_was_write; 
		BOOL result = WriteFile(m_hFile, buffer, (DWORD)size, &byte_was_write, NULL); 
		if(result != TRUE || byte_was_write < (DWORD)size) 
			throw TException(AD_ERROR_CANT_WRITE_FILE);
	}

	void TOutputFileStream::Save(const TString & string) const
	{
		SaveSize(string.size());
		Save(string.c_str(), sizeof(TChar)*string.size());
	}

	void TOutputFileStream::Save(const TPath & path) const
	{
		Save(path.Original());
	}

	void TOutputFileStream::Save(const TFileInfo & fileInfo) const
	{
		Save(fileInfo.path);
		Save(fileInfo.size);
		Save(fileInfo.time);
		Save(fileInfo.hash);
	}

	// Сохраняем данные изображения
	void TOutputFileStream::Save(const TImageInfo & imageInfo) const
	{
		Save((const TFileInfo&)imageInfo);
		Save(imageInfo.type);
		Save(imageInfo.width);
		Save(imageInfo.height);
		Save(imageInfo.blockiness); 
		Save(imageInfo.blurring); 
		SaveExif(imageInfo.imageExif);
	}

	// Сохраняем пиксели изображения
	void TOutputFileStream::Save(const TPixelData & pixelData) const
	{
		// сначала сохраняем размер сторны квадрата эскиза
		SaveSize(pixelData.side);
		// записываем эскиз
		Save(pixelData.main, pixelData.size);
		Save(pixelData.average);
		Save(pixelData.varianceSquare);
	}

	// Сохранение в потоке изображения
	void TOutputFileStream::Save(const TImageData & imageData) const
	{
		Save((const TImageInfo&)imageData);
		Save(imageData.defect);
		Save(imageData.crc32c);
		Save(imageData.data->filled);
		if(imageData.data->filled)
			Save(*imageData.data);
	}

	void TOutputFileStream::Save(const TResult & result) const
	{
		Save(result.type);
		SaveSize(result.first->index);
		SaveSize(result.second->index);
		Save(result.defect);
		Save(result.difference);
		Save(result.transform);
		SaveSize(result.group);
		Save(result.hint);
	}
}