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

#ifndef __adImageExif_h__
#define __adImageExif_h__

#include "AntiDupl.h"
#include "adStrings.h"

namespace ad
{
	/*
	Property of image.
	*/
	struct TImageExif
    {
		// False, если заполнено хоть одно значение.
		bool isEmpty;

		TString imageDescription;

		// Камера, изготовитель
		// manufacturer of the equipment.
		TString equipMake;

		/// model name or model number of the equipment.
		// Камера, модель
		TString equipModel;

		TString softwareUsed;
		TString dateTime;
		TString artist;
		TString userComment;
		
		TImageExif();
		bool Export(adExifInfoA *) const;
		bool Export(adExifInfoW *) const;
		TImageExif& TImageExif::operator = (const TImageExif& exifInfo);
	};

	typedef TImageExif* TImageExifPtr;
}

#endif//__adImageExif_h__ 