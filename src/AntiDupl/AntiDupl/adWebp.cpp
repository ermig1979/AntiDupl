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

#include "webp\decode.h"

namespace ad
{
	bool TWebp::Supported(HGLOBAL hGlobal)
    {
        if(hGlobal)
        {
		  	uint8_t *data = (uint8_t *)::GlobalLock(hGlobal);
			size_t data_size = ::GlobalSize(hGlobal);
			WebPBitstreamFeatures features;
			VP8StatusCode code = WebPGetFeatures(data, data_size, &features);
            ::GlobalUnlock(hGlobal);
			int64_t unpacked_size = int64_t(features.height) * int64_t(features.width) * 4;
			return code == VP8_STATUS_OK && unpacked_size > 0 && unpacked_size < INT_MAX;
		}
        return false;
    }

	TWebp* TWebp::Load(HGLOBAL hGlobal)
	{
		TWebp* pWebp = NULL;
		if(hGlobal)
        {	
			uint8_t *data = (uint8_t *)::GlobalLock(hGlobal);
			size_t data_size = ::GlobalSize(hGlobal);
			WebPBitstreamFeatures features;
			if (WebPGetFeatures(data, data_size, &features) == VP8_STATUS_OK)
			{
				TView * pView = new TView(features.width, features.height, TView::Bgra32);
				if (pView)
				{
					pView->Recreate(features.width, features.height, TView::Bgra32);
					if (WebPDecodeBGRAInto(data, data_size, pView->data, pView->DataSize(), (int)pView->stride))
					{
						pWebp = new TWebp();
						pWebp->m_pView = pView;
						pWebp->m_format = TImage::Webp;
					}
					else
						delete pView;
				}
			}
			::GlobalUnlock(hGlobal);
        }
        return pWebp;
	}
}