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
#include "adImage.h"
#include "adFileUtils.h"
#include "adImageUtils.h"
#include "adPerformance.h"

namespace ad
{
    adError LoadBitmap(const TString& fileName, adBitmapPtr pBitmap)
    {
        if(pBitmap == NULL)
            return AD_ERROR_INVALID_POINTER;

        if(pBitmap->width == 0 || pBitmap->height == 0 || pBitmap->stride == 0 || 
            pBitmap->format <= AD_PIXEL_FORMAT_NONE || pBitmap->format >= AD_PIXEL_FORMAT_SIZE || 
            pBitmap->data == NULL)
            return AD_ERROR_INVALID_BITMAP;

        if(!IsFileExists(fileName.c_str()))
            return AD_ERROR_FILE_IS_NOT_EXIST;

        adError result = AD_ERROR_UNKNOWN;

        HGLOBAL hGlobal = LoadFileToMemory(fileName.c_str());
        if(hGlobal)
        {
            TImage *pImage = TImage::Load(hGlobal);
            if(pImage)
            {
                TView::Format format = TView::None;
                switch(pBitmap->format)
                {
                case AD_PIXEL_FORMAT_ARGB32:
                    format = TView::Bgra32;
                    break;
                }
                if(format)
                {
                    TView view(pBitmap->width, pBitmap->height, pBitmap->stride, format, pBitmap->data);
                    Simd::ResizeBilinear(*pImage->View(), view);
                    result = AD_OK;
                }
                delete pImage;
            }
            else
                result = AD_ERROR_CANT_LOAD_IMAGE;
            ::GlobalFree(hGlobal);
        }
        else
            result = AD_ERROR_CANT_OPEN_FILE;

        return result;
    }
}