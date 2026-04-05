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
#include "adPerformance.h"
#include "adIO.h"
#include "adTurboJpeg.h"

#ifdef AD_TURBO_JPEG_ENABLE
#include "turbojpeg.h"

FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void)
{
    return _iob;
}

namespace ad
{
    struct TurboJpeg
    {
        TurboJpeg()
        {
            _handle = ::tjInitDecompress();
        }

        ~TurboJpeg()
        {
            ::tjDestroy(_handle);
        }

        TView * Decompress(const unsigned char * data, size_t size, int targetSize = 0)
        {
            int subsamp, colorspace, width, height, flags = 0;
            if(::tjDecompressHeader3(_handle, data, (unsigned long)size, &width, &height, &subsamp, &colorspace) != 0)
                return NULL;
            if (width == 0 || height == 0)
                return NULL;

            // Scaled decode для ускорения (если targetSize > 0)
            int scaledWidth = width;
            int scaledHeight = height;
            
            if (targetSize > 0 && (width > targetSize || height > targetSize)) {
                // libjpeg-turbo поддерживает только определённые scaling factors
                // Выбираем ближайший: 1/2, 1/4, 1/8
                int denom = 1;
                if (width / 8 >= targetSize && height / 8 >= targetSize) denom = 8;
                else if (width / 4 >= targetSize && height / 4 >= targetSize) denom = 4;
                else if (width / 2 >= targetSize && height / 2 >= targetSize) denom = 2;

                if (denom > 1) {
                    scaledWidth = width / denom;
                    scaledHeight = height / denom;
                    flags |= TJFLAG_FASTUPSAMPLE;
                }
            }

            TView * pView = new TView(scaledWidth, scaledHeight, TView::Bgra32, NULL, 4);
            if (::tjDecompress2(_handle, data, size, pView->data, scaledWidth, 0, scaledHeight, ::TJPF_RGBA, flags) != 0 && ::tjGetErrorCode(_handle) != ::TJERR_WARNING)
            {
                delete pView;
                pView = NULL;
            }
            return pView;
        }

    private:
        ::tjhandle _handle;
    };

    thread_local TurboJpeg turboJpeg;

    TTurboJpeg * TTurboJpeg::Load(HGLOBAL hGlobal, int targetSize)
    {
        if (hGlobal)
        {
            const unsigned char * data = (unsigned char*)::GlobalLock(hGlobal);
            size_t size = ::GlobalSize(hGlobal);
            TTurboJpeg * pTurboJpeg = NULL;
            TView * pView = turboJpeg.Decompress(data, size, targetSize);
            if (pView)
            {
                pTurboJpeg = new TTurboJpeg();
                pTurboJpeg->m_format = TImage::Jpeg;
                pTurboJpeg->m_pView = pView;
            }
            ::GlobalUnlock(hGlobal);
            return pTurboJpeg;
        }
        return NULL;
    }

    bool TTurboJpeg::Supported(HGLOBAL hGlobal)
    {
        if (hGlobal)
        {
            const unsigned char * data = (unsigned char*)::GlobalLock(hGlobal);
            size_t size = ::GlobalSize(hGlobal);
            bool supported = (size >= 4 && data[0] == 0xFF && data[1] == 0xD8 && data[2] == 0xFF && data[3] == 0xE0);
            ::GlobalUnlock(hGlobal);
            return supported;
        }
        return false;
    }
}
#endif//AD_TURBO_JPEG_ENABLE