/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2013-2018 Borisov Dmitry.
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

#include "adImageExif.h"

/**
Property of image.
*/
namespace ad
{
	TImageExif::TImageExif()
		: isEmpty(true)
	{
	}

	// Ёкспортируем в структуры дл¤ передачи из dll
	bool TImageExif::Export(adExifInfoA * pExifInfo) const
    {
        if(pExifInfo == NULL)
            return false;

		return equipMake.CopyTo(pExifInfo->equipMake, MAX_EXIF_SIZE);
    }

	bool TImageExif::Export(adExifInfoW * pExifInfo) const
    {
        if(pExifInfo == NULL)
            return false;

		pExifInfo->isEmpty = this->isEmpty ? TRUE : FALSE;
		imageDescription.CopyTo(pExifInfo->imageDescription, MAX_EXIF_SIZE);
		equipMake.CopyTo(pExifInfo->equipMake, MAX_EXIF_SIZE);
		equipModel.CopyTo(pExifInfo->equipModel, MAX_EXIF_SIZE);
		softwareUsed.CopyTo(pExifInfo->softwareUsed, MAX_EXIF_SIZE);
		dateTime.CopyTo(pExifInfo->dateTime, MAX_EXIF_SIZE);
		artist.CopyTo(pExifInfo->artist, MAX_EXIF_SIZE);
		userComment.CopyTo(pExifInfo->userComment, MAX_EXIF_SIZE);
		return true;
    }

	TImageExif& TImageExif::operator = (const TImageExif& exifInfo)
    {
		isEmpty = exifInfo.isEmpty;
		imageDescription = exifInfo.imageDescription;
		equipMake = exifInfo.equipMake;
		equipModel = exifInfo.equipModel;
		softwareUsed = exifInfo.softwareUsed;
		dateTime = exifInfo.dateTime;
		artist = exifInfo.artist;
		userComment = exifInfo.userComment;

        return *this;
    }

}