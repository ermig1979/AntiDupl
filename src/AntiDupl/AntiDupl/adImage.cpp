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
#include "adFileUtils.h"
#include "adImage.h"
#include "adGdiplus.h"
#include "adOpenJpeg.h"
#include "adPsd.h"
#include "adDds.h"
#include "adTga.h"

namespace ad
{
    const size_t EXTENSION_MAX_COUNT = 9;
    const TChar *s_FormatExtensions[TImage::FormatSize][EXTENSION_MAX_COUNT + 1] = 
    {
        {0           , 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0},// None = 0
        {TEXT("BMP") , TEXT("DIB") , TEXT("RLE") , 0           , 0           , 0           , 0           , 0           , 0           , 0},// Bmp = 1
        {TEXT("GIF") , 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0},// Gif = 2
        {TEXT("JPEG"), TEXT("JFIF"), TEXT("JPG") , TEXT("JPE") , TEXT("JIFF"), TEXT("JIF") , TEXT("J")   , TEXT("JNG") , TEXT("JFF") , 0},// Jpeg = 3
        {TEXT("PNG") , 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0},// Png = 4
        {TEXT("TIF") , TEXT("TIFF"), 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0},// Tiff = 5
        {TEXT("EMF") , TEXT("EMZ") , 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0},// Emf = 6
        {TEXT("WMF") , 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0},// Wmf = 7
        {TEXT("EXIF"), 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0},// Exif = 8
        {TEXT("ICON"), TEXT("ICO") , TEXT("ICN") , 0           , 0           , 0           , 0           , 0           , 0           , 0},// Icon = 9
        {TEXT("JP2") , TEXT("J2K") , TEXT("J2C") , TEXT("JPC") , TEXT("JPF") , TEXT("JPX") , 0           , 0           , 0           , 0},// Jp2 = 10
        {TEXT("PSD") , 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0},// Psd = 11
		{TEXT("DDS") , 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0},// Dds = 12
		{TEXT("TGA") , TEXT("TPIC"), 0           , 0           , 0           , 0           , 0           , 0           , 0           , 0},// Tga = 13
    };
    //-------------------------------------------------------------------------

    TImage::TImage()
        :m_pView(NULL),
        m_format(None)
    {
    }

    TImage::~TImage()
    {
        FreeView();
    }

    void TImage::FreeView()
    {
        if(m_pView)
            delete m_pView;
        m_pView = NULL;
    }

    TStrings TImage::Extensions(TImage::TFormat format)
    {
        TStrings extensions;
        if(format > None && format < FormatSize)
        {
            extensions.reserve(EXTENSION_MAX_COUNT);
            for(size_t i = 0; s_FormatExtensions[format][i]; ++i)
                extensions.push_back(s_FormatExtensions[format][i]);
        }
        return extensions;
    }

	// Вызывается из adDataCollector.cpp
    TImage* TImage::Load(HGLOBAL hGlobal)
    {
        if(TOpenJpeg::Supported(hGlobal))
            return TOpenJpeg::Load(hGlobal);
        else if(TPsd::Supported(hGlobal))
            return TPsd::Load(hGlobal);
		else if(TDds::Supported(hGlobal))
			return TDds::Load(hGlobal);
		else if(TTga::Supported(hGlobal))
			return TTga::Load(hGlobal);
        else
            return TGdiplus::Load(hGlobal);
    }
    
    TImage* TImage::Load(const TChar * fileName)
    {
        TImage *pImage = NULL;
        HGLOBAL hGlobal = LoadFileToMemory(fileName);
        if(hGlobal)
        {
            pImage = Load(hGlobal);
            ::GlobalFree(hGlobal);
        }
        return pImage;
    }
}