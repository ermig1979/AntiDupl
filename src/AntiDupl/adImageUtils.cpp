/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar.
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
#include "adDataCollector.h"
#include "adBlurringDetector.h"

namespace ad
{
    // Декод файла в нативное изображение (TImage::Load); NULL = файл не открылся/не декодировался.
    static TImage * DecodeImage(const TString & fileName, const TOptions * pOptions)
    {
        HGLOBAL hGlobal = LoadFileToMemory(fileName.c_str());
        if(!hGlobal)
            return NULL;

        TImage *pImage = TImage::Load(hGlobal, pOptions);
        ::GlobalFree(hGlobal);
        return pImage;
    }

    // Конвертация нативного изображения в буфер вызывающего (формат/размер — по pBitmap).
    // Поведение 1-в-1 с оригинальной реализацией LoadBitmap.
    static bool ConvertImageToBitmap(const TImage * pImage, adBitmapPtr pBitmap)
    {
        TView::Format format = TView::None;
        switch(pBitmap->format)
        {
        case AD_PIXEL_FORMAT_ARGB32:
            format = TView::Bgra32;
            break;
        }
        if(!format)
            return false;

        TView view(pBitmap->width, pBitmap->height, pBitmap->stride, format, pBitmap->data);
        if (pImage->View()->format == TView::Format::Rgb24)
        {
            int img_stride = (pImage->View()->width) * TView::PixelSize(TView::Bgra32);;
            TView Bgra(pImage->View()->width, pImage->View()->height, img_stride, TView::Bgra32, NULL);
            Simd::RgbToBgra(*pImage->View(), Bgra);
            Simd::Resize(Bgra, view);
        }
        else if (pImage->View()->format == TView::Format::Rgba32)
        {
            int img_stride = (pImage->View()->width) * TView::PixelSize(TView::Bgra32);;
            TView Bgra(pImage->View()->width, pImage->View()->height, TView::Bgra32, NULL);
            Simd::RgbaToBgra(*pImage->View(), Bgra);
            Simd::Resize(Bgra, view);
        }
        else
        {
            Simd::Resize(*pImage->View(), view);
        }
        return true;
    }

    // Вписывание нативного изображения в латербокс-канвас pBitmap (maxW x maxH):
    // масштаб без апскейла, размер кратен 8 (минимум 8), канвас зачёрнен,
    // изображение — в левый верхний угол. Поведение 1-в-1 с оригинальной
    // реализацией LoadBitmapLetterbox.
    static void FitAndLetterbox(const TView & nativeView, adUInt32 maxW, adUInt32 maxH, adBitmapPtr pBitmap)
    {
        // Fit into maxW x maxH without upscaling; target sizes are multiples
        // of 8 (minimum 8) — same formula as the original preprocessing.
        float scale = 1.0f;
        if(nativeView.width > maxW)
            scale = (float)maxW / nativeView.width;
        if(nativeView.height > maxH)
            scale = (float)maxH / nativeView.height < scale ? (float)maxH / nativeView.height : scale;
        int w = (int)((float)nativeView.width * scale / 8) * 8;
        int h = (int)((float)nativeView.height * scale / 8) * 8;
        if(w < 8) w = 8;
        if(h < 8) h = 8;

        TView dst(w, h, (ptrdiff_t)w * 4, TView::Bgra32, NULL);
        if (nativeView.format == TView::Format::Rgb24)
        {
            int img_stride = (int)nativeView.width * TView::PixelSize(TView::Bgra32);
            TView Bgra(nativeView.width, nativeView.height, img_stride, TView::Bgra32, NULL);
            Simd::RgbToBgra(nativeView, Bgra);
            Simd::Resize(Bgra, dst);
        }
        else if (nativeView.format == TView::Format::Rgba32)
        {
            TView Bgra(nativeView.width, nativeView.height, TView::Bgra32, NULL);
            Simd::RgbaToBgra(nativeView, Bgra);
            Simd::Resize(Bgra, dst);
        }
        else
        {
            Simd::Resize(nativeView, dst);
        }

        // Zero the canvas and copy the fitted image to the top-left corner.
        adUInt8 * canvas = pBitmap->data;
        const adUInt8 * src = dst.data;
        memset(canvas, 0, (size_t)maxH * pBitmap->stride);
        for(int y = 0; y < h; y++)
            memcpy(canvas + (size_t)y * pBitmap->stride, src + (size_t)y * w * 4, (size_t)w * 4);
    }

    // Метрики качества файла на нативном сером растре — те же алгоритмы, что в
    // классическом пайплайне (TDataCollector::FillPixelData): серая конверсия
    // (Rgb/Rgba/Bgra -> Gray), ComputeBlockiness, TBlurringDetector.
    static void ComputeNativeMetrics(const TView & nativeView, double * pBlurring, double * pBlockiness)
    {
        if(!pBlurring && !pBlockiness)
            return;

        TView gray(nativeView.width, nativeView.height, TView::Gray8, NULL);
        if (nativeView.format == TView::Format::Rgb24)
            Simd::RgbToGray(nativeView, gray);
        else if (nativeView.format == TView::Format::Rgba32)
            Simd::RgbaToGray(nativeView, gray);
        else
            Simd::BgraToGray(nativeView, gray);

        if(pBlockiness)
            *pBlockiness = ComputeBlockiness(gray);
        if(pBlurring)
        {
            TBlurringDetector detector;
            *pBlurring = detector.Detect(gray);
        }
    }

    adError LoadBitmap(const TString & fileName, adBitmapPtr pBitmap, const TOptions * pOptions)
    {
        return LoadBitmapEx(fileName, pBitmap, pOptions, NULL, NULL);
    }

    adError LoadBitmapEx(const TString & fileName, adBitmapPtr pBitmap, const TOptions * pOptions,
                         adUInt32 * pNativeWidth, adUInt32 * pNativeHeight)
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

        TImage *pImage = DecodeImage(fileName, pOptions);
        if(pImage)
        {
            if(pNativeWidth)
                *pNativeWidth = (adUInt32)pImage->View()->width;
            if(pNativeHeight)
                *pNativeHeight = (adUInt32)pImage->View()->height;

            if(ConvertImageToBitmap(pImage, pBitmap))
                result = AD_OK;
            delete pImage;
        }
        else
            result = AD_ERROR_CANT_LOAD_IMAGE;

        return result;
    }

    adError LoadBitmapLetterbox(const TString & fileName, adUInt32 maxW, adUInt32 maxH, adBitmapPtr pBitmap, const TOptions * pOptions)
    {
        return LoadBitmapLetterboxMetrics(fileName, maxW, maxH, pBitmap, pOptions, NULL, NULL, NULL, NULL);
    }

    adError LoadBitmapLetterboxMetrics(const TString & fileName, adUInt32 maxW, adUInt32 maxH, adBitmapPtr pBitmap, const TOptions * pOptions,
                                       adUInt32 * pNativeWidth, adUInt32 * pNativeHeight,
                                       double * pBlurring, double * pBlockiness)
    {
        if(pBitmap == NULL)
            return AD_ERROR_INVALID_POINTER;

        if(maxW == 0 || maxH == 0 || pBitmap->width != maxW || pBitmap->height != maxH ||
            pBitmap->stride != (adInt32)(maxW * 4) ||
            pBitmap->format != AD_PIXEL_FORMAT_ARGB32 || pBitmap->data == NULL)
            return AD_ERROR_INVALID_BITMAP;

        if(!IsFileExists(fileName.c_str()))
            return AD_ERROR_FILE_IS_NOT_EXIST;

        adError result = AD_ERROR_UNKNOWN;

        TImage *pImage = DecodeImage(fileName, pOptions);
        if(pImage)
        {
            TView *pView = pImage->View();
            if(pNativeWidth)
                *pNativeWidth = (adUInt32)pView->width;
            if(pNativeHeight)
                *pNativeHeight = (adUInt32)pView->height;

            // Метрики — на нативном растре (до ресайза/канваса): канвас 800x600 и
            // чёрная рамка исказили бы блочность (8x8) и детектор размытия.
            ComputeNativeMetrics(*pView, pBlurring, pBlockiness);

            FitAndLetterbox(*pView, maxW, maxH, pBitmap);
            result = AD_OK;
            delete pImage;
        }
        else
            result = AD_ERROR_CANT_LOAD_IMAGE;

        return result;
    }
}
