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
#ifndef __adImages_h__
#define __adImages_h__

#include "adImage.h"

namespace ad
{
    struct TOptions;

    //-------------------------------------------------------------------------

    adError LoadBitmap(const TString& fileName, adBitmapPtr pBitmap, const TOptions * pOptions);

    // Aspect-preserving decode: the image is fitted into maxW x maxH without upscaling
    // (target sizes are multiples of 8, minimum 8 — same formula as the original
    // SuperPoint/LightGlue preprocessing) and placed at the top-left corner of the
    // caller's maxW x maxH ARGB32 canvas (stride = maxW*4); the rest of the canvas
    // is zeroed. Pure addition: LoadBitmap (stretch) keeps its semantics.
    adError LoadBitmapLetterbox(const TString& fileName, adUInt32 maxW, adUInt32 maxH, adBitmapPtr pBitmap, const TOptions * pOptions);

    // Расширенные варианты декодирования для deep-пайплайна (DINOv2):
    // возвращают те же растры, что и базовые функции, но дополнительно сообщают
    // метаданные нативного изображения, которые ядро и так знает после TImage::Load
    // (без повторного чтения файла).
    //
    // pNativeWidth/pNativeHeight — опциональные (NULL = не вычислять): нативные
    // размеры изображения (до какого-либо ресайза).
    adError LoadBitmapEx(const TString & fileName, adBitmapPtr pBitmap, const TOptions * pOptions,
                         adUInt32 * pNativeWidth, adUInt32 * pNativeHeight);

    // То же, что LoadBitmapLetterbox, плюс опциональные метрики качества файла,
    // вычисляемые на нативном сером растре теми же алгоритмами, что в классическом
    // пайплайне (TDataCollector::FillPixelData): ComputeBlockiness + TBlurringDetector.
    // pNativeWidth/pNativeHeight/pBlurring/pBlockiness — опциональные (NULL = не вычислять).
    adError LoadBitmapLetterboxMetrics(const TString & fileName, adUInt32 maxW, adUInt32 maxH, adBitmapPtr pBitmap, const TOptions * pOptions,
                                       adUInt32 * pNativeWidth, adUInt32 * pNativeHeight,
                                       double * pBlurring, double * pBlockiness);
}
#endif//__adImages_h__

