/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar, 2013-2018 Borisov Dmitry.
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
#include <windows.h>
#include <gdiplus.h>
#pragma comment (lib, "GDIPlus.lib")

#include "adPerformance.h"
#include "adGdiplus.h"
#include "adImageExif.h"

namespace ad
{
    bool GetEncoderClsid(const ::GUID & formatGuid, ::CLSID* pClsid)
    {
        UINT num = 0, size = 0, index = -1;
        if(Gdiplus::GetImageEncodersSize(&num, &size) == Gdiplus::Ok && size && num)
        {
            Gdiplus::ImageCodecInfo * pImageCodecInfo = (Gdiplus::ImageCodecInfo*)malloc(size);
            if(pImageCodecInfo)
            {
                if(Gdiplus::GetImageEncoders(num, size, pImageCodecInfo) == Gdiplus::Ok)
                {
                    for(UINT i = 0; i < num; ++i)
                    {
                        if(pImageCodecInfo[i].FormatID == formatGuid)
                        {
                            *pClsid = pImageCodecInfo[i].Clsid;
                            free(pImageCodecInfo);
                            return true;
                        }    
                    }
                }
                free(pImageCodecInfo);
            }
        }
        return false;
    }

    static TImage::TFormat GetFormat(const GUID & guid)
    {
        if(guid == Gdiplus::ImageFormatBMP)
            return TImage::Bmp;
        else if(guid == Gdiplus::ImageFormatGIF)
            return TImage::Gif;
        else if(guid == Gdiplus::ImageFormatJPEG)
            return TImage::Jpeg;
        else if(guid == Gdiplus::ImageFormatPNG)
            return TImage::Png;
        else if(guid == Gdiplus::ImageFormatTIFF)
            return TImage::Tiff;
        else if(guid == Gdiplus::ImageFormatEMF)
            return TImage::Emf;
        else if(guid == Gdiplus::ImageFormatWMF)
            return TImage::Wmf;
        else if(guid == Gdiplus::ImageFormatEXIF)
            return TImage::Exif;
        else if(guid == Gdiplus::ImageFormatIcon)
            return TImage::Icon;
        else
            return TImage::None;
    }

    static TImage::TFormat GetFormat(Gdiplus::Bitmap *pBitmap)
    {
        GUID guid;
        if(pBitmap && pBitmap->GetRawFormat(&guid) == Gdiplus::Ok)
            return GetFormat(guid);
        else
            return TImage::None;
    }

    static bool EnableCustomConversion(Gdiplus::Bitmap *pBitmap)
    {
        Gdiplus::PixelFormat pixelFormat = pBitmap->GetPixelFormat();
        return 
            pixelFormat == PixelFormat32bppARGB ||
            pixelFormat == PixelFormat24bppRGB;
    }

    static bool CustomConversion(Gdiplus::BitmapData *pBitmapData, TView *pView)
    {
        UINT width = pBitmapData->Width;
        UINT height = pBitmapData->Height;
        unsigned char *pSrc = (unsigned char *)pBitmapData->Scan0;
        unsigned char *pDst = pView->data;
        if(pBitmapData->PixelFormat == PixelFormat32bppARGB)
        {
            for(UINT row = 0; row < height; ++row)
            {
                memcpy(pDst, pSrc, pView->width*4);
                pDst += pView->stride;
                pSrc += pBitmapData->Stride;
            }
        }
        else if(pBitmapData->PixelFormat == PixelFormat24bppRGB)
        {
            SimdBgrToBgra(pSrc, width, height, pBitmapData->Stride, pDst, pView->stride, 0xFF);
        }
        else
        {
            return false;
        }
        return true;
    }

    static TView* GetView(Gdiplus::Bitmap *pBitmap)
    {
        TView * pView = NULL;
        UINT width = pBitmap->GetWidth();
        UINT height = pBitmap->GetHeight();
        if(EnableCustomConversion(pBitmap))
        {
            Gdiplus::Rect rect(0, 0, width, height);
            Gdiplus::BitmapData bitmapData;
            if(pBitmap->LockBits(&rect, 0, pBitmap->GetPixelFormat(), &bitmapData) == Gdiplus::Ok)
            {
				pView = new TView(width, height, TView::Bgra32, NULL);
                if(pView)
                {
                    if(pView->data == NULL || !CustomConversion(&bitmapData, pView))
                    {
                        delete pView;
                        pView = NULL;
                    }
                }
                pBitmap->UnlockBits(&bitmapData);
            }
        }
        else
        {
            pView = new TView(width, height, TView::Bgra32, NULL);
            if(pView)
            {
                Gdiplus::Bitmap dst(width, height, (int)pView->stride, PixelFormat32bppARGB, pView->data);
                Gdiplus::Graphics graphics(&dst); 
                if(pView->data == NULL || graphics.DrawImage(pBitmap, 0, 0, width, height) != Gdiplus::Ok)
                {
                    delete pView;
                    pView = NULL;
                }
            }
        }
        return pView;
    }

	void FullExifStruct(TString *structProperty, const Gdiplus::PropertyItem *gdiProp, TImageExif *imageExif)
	{
		if (gdiProp->length > SIZE_CHECK_LIMIT)
			return;
		TString str = TString((char*)gdiProp->value);
		str.Trim();
		if (str.length() > 0)
		{
			structProperty->assign(str);
			imageExif->isEmpty = false;
		}
	}

	static void GetExifProperty(Gdiplus::Bitmap * pBitmap, TImageExif *pImageExif)
	{
		//Exif data
		UINT  size = 0;
		UINT  count = 0;

		pBitmap->GetPropertySize(&size, &count);
		// GetAllPropertyItems returns an array of PropertyItem objects.
		// Allocate a buffer large enough to receive that array.
		Gdiplus::PropertyItem* pPropBuffer =(Gdiplus::PropertyItem*)malloc(size);
		// Get the array of PropertyItem objects.
		if (pBitmap->GetAllPropertyItems(size, count, pPropBuffer) == Gdiplus::Ok)
		{
			// For each PropertyItem in the array, display the id, type, and length.
			for(UINT j = 0; j < count; ++j)
			{
				Gdiplus::PropertyItem  &it = pPropBuffer[j] ;
				if (!it.length || !it.value)
					continue;

				switch(it.id)
				{
				case PropertyTagImageDescription:
					FullExifStruct(&pImageExif->imageDescription, &it, pImageExif);
					break;
				case PropertyTagEquipMake:
					FullExifStruct(&pImageExif->equipMake, &it, pImageExif);
					break;
				case PropertyTagEquipModel:
					FullExifStruct(&pImageExif->equipModel, &it, pImageExif);
					break;
				case PropertyTagSoftwareUsed:
					FullExifStruct(&pImageExif->softwareUsed, &it, pImageExif);
					break;
				case PropertyTagDateTime:
					FullExifStruct(&pImageExif->dateTime, &it, pImageExif);
					break;
				case PropertyTagArtist:
					FullExifStruct(&pImageExif->artist, &it, pImageExif);
					break;
				case PropertyTagExifUserComment:
					FullExifStruct(&pImageExif->userComment, &it, pImageExif);
					break;
				}
			}
		}

		free(pPropBuffer);
	}

	// Загрузка изображения с помощью GDI.
    TGdiplus* TGdiplus::Load(HGLOBAL hGlobal)
    {
        AD_FUNCTION_PERFORMANCE_TEST

        TView *pView = NULL;
        TImage::TFormat format = TImage::None;
		TImageExif *imageExif = new TImageExif();; //указатель

        if(hGlobal)
        {
            IStream* pStream = NULL;
            if(::CreateStreamOnHGlobal(hGlobal, FALSE, &pStream) == S_OK)
            {
                try
				{
					Gdiplus::Bitmap *pBitmap = Gdiplus::Bitmap::FromStream(pStream);

                    if(pBitmap && pBitmap->GetWidth() && pBitmap->GetHeight())
                    {
                        pView = GetView(pBitmap);
                        if(pView)
						{
                            format = GetFormat(pBitmap);
							GetExifProperty(pBitmap, imageExif);
						}
                    }
                    AD_PERFORMANCE_TEST_SET_SIZE(pBitmap->GetHeight()*pBitmap->GetWidth())
                        delete pBitmap;
                }
                catch(...)
                {
                }
                pStream->Release();
            }
        }

		// Если получен вид SIMD
        if(pView)
        {
            TGdiplus* pGdiplus = new TGdiplus();
			pGdiplus->m_exifInfo = *imageExif; //// вывод значения содержащегося в переменной через указатель, операцией разименования указателя
			if(imageExif)
				delete imageExif;
            pGdiplus->m_pView = pView;
            pGdiplus->m_format = format;
            return pGdiplus;
        }

		if(imageExif)
			delete imageExif;

        return NULL;
    }

    bool TGdiplus::Save(const TView *pView, const TChar * fileName, TImage::TFormat format)
    {
        Gdiplus::PixelFormat pixelFormat;
        switch(pView->format)
        {
        case TView::Bgra32:
            pixelFormat = PixelFormat32bppARGB;
            break;
        default:
            return false;
        }

        ::GUID imageFormatId;
        switch(format)
        {
        case TImage::Png:
            imageFormatId = Gdiplus::ImageFormatPNG;
            break;
        default:
            return false;
        }

        ::CLSID codecClsid;
        if(GetEncoderClsid(imageFormatId, &codecClsid))
        {
            Gdiplus::Bitmap bitmap((INT)pView->width, (INT)pView->height, (INT)pView->stride, pixelFormat, pView->data);
            return bitmap.Save(TString(fileName).ToWString().c_str(), &codecClsid, 0) == Gdiplus::Ok;
        }
        return false;
    }
}
