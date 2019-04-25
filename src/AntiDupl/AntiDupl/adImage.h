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
#ifndef __adImage_h__
#define __adImage_h__

#include "adStrings.h"
#include "adImageExif.h"

namespace ad
{
    class TImage
    {
    public:
        enum TFormat
        {
            None = 0,
            Bmp = 1,
            Gif = 2,
            Jpeg = 3,
            Png = 4,
            Tiff = 5,
            Emf = 6,
            Wmf = 7,
            Exif = 8,
            Icon = 9,
            Jp2 = 10,
            Psd = 11,
			Dds = 12,
			Tga = 13,
            FormatSize
        };

        virtual ~TImage();

		TImageExif ImageExif() 
		{
			return m_exifInfo; 
		}

        TFormat Format() const {return m_format;}
        TView* View() const {return m_pView;}
        
        static TStrings Extensions(TFormat format);
        static TImage* Load(HGLOBAL hGlobal);
        static TImage* Load(const TChar * fileName);

    protected:
        TImage();

        void FreeView();

        TView *m_pView;
        TFormat m_format;
		TImageExif m_exifInfo;
    };
}

#endif//__adImage_h__