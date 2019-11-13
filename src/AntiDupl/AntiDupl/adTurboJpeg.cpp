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
#include "TurboJpeg/inc/turbojpeg.h"

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

        TView * Decompress(const unsigned char * data, size_t size)
        {
            int subsamp, colorspace, width, height, flags = 0;
            if(::tjDecompressHeader3(_handle, data, (unsigned long)size, &width, &height, &subsamp, &colorspace) != 0)
                return NULL;
            if (width == 0 || height == 0)
                return NULL;
            TView * pView = new TView(width, height, TView::Bgra32, NULL, 4);
            if (::tjDecompress2(_handle, data, size, pView->data, width, 0, height, ::TJPF_RGBA, flags) != 0 && ::tjGetErrorCode(_handle) != ::TJERR_WARNING)
            {
                //int code = ::tjGetErrorCode(_handle);
                //const char * str = ::tjGetErrorStr2(_handle);
                delete pView;
                pView = NULL;
            }
            return pView;
        }

    private:
        ::tjhandle _handle;
    };

    thread_local TurboJpeg turboJpeg;

    TTurboJpeg * TTurboJpeg::Load(HGLOBAL hGlobal)
    {
        if (hGlobal)
        {
            const unsigned char * data = (unsigned char*)::GlobalLock(hGlobal);
            size_t size = ::GlobalSize(hGlobal);
            TTurboJpeg * pTurboJpeg = NULL;
            TView * pView = turboJpeg.Decompress(data, size);
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