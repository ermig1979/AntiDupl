/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2019 Yermalayeu Ihar,
*               2013-2019 Borisov Dmitry.
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
#include "adWebp.h"
#include "adPerformance.h"

#include "libwebp\src\webp\decode.h"

namespace ad
{
	bool TWebp::Supported(HGLOBAL hGlobal)
    {
        if(hGlobal)
        {
		  	uint8_t *data = (uint8_t *)::GlobalLock(hGlobal);
			size_t data_size = ::GlobalSize(hGlobal);

			int result;
			int width;
			int height;
			result = WebPGetInfo(data, data_size, &width, &height);
            ::GlobalUnlock(hGlobal);
			return result;
		}
        return false;
    }

	TWebp* TWebp::Load(HGLOBAL hGlobal)
	{
		if(hGlobal)
        {	
			uint8_t *data = (uint8_t *)::GlobalLock(hGlobal);
			size_t data_size = ::GlobalSize(hGlobal);

			//int width;
			//int height;
			//uint8_t* webpData = NULL;
			//webpData = WebPDecodeBGRA(data, data_size, &width, &height);

			WebPBitstreamFeatures webPBitstreamFeatures;

			VP8StatusCode code = WebPGetFeatures(data, data_size, &webPBitstreamFeatures);


			TView *pView;
			pView = new TView(webPBitstreamFeatures.width, webPBitstreamFeatures.height, webPBitstreamFeatures.width * TView::PixelSize(TView::Bgra32),
				TView::Bgra32, NULL);

			pView->Recreate(webPBitstreamFeatures.width, webPBitstreamFeatures.height, TView::Bgra32);
			WebPDecodeBGRAInto(data, data_size, pView->data, pView->DataSize(), (int)pView->stride);


			::GlobalUnlock(hGlobal);

			//memcpy(pView->data, &webpData, TView::PixelSize(TView::Bgra32) * width);
			//WebPFree(webpData);
			if(pView)
            {
                TWebp* pWebp = new TWebp();
                pWebp->m_pView = pView;
                pWebp->m_format = TImage::Webp;
                return pWebp;
            }



            //TView *pView = Load((unsigned char*)::GlobalLock(hGlobal), ::GlobalSize(hGlobal));
            //::GlobalUnlock(hGlobal);
            
        }
        return NULL;
	}

}