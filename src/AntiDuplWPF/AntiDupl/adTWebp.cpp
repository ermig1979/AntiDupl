#include "adTWebp.h"
#include "adPerformance.h"

#include "libwebp\src\webp\decode.h"
//#include "libwebp\imageio\webpdec.h"

#pragma comment( lib, "libwebp.lib" )

//#include "libwebp\src\dec\webp_dec.c"
//#include "libwebp\src\dec\buffer_dec.c"
//#include "libwebp\src\utils\rescaler_utils.c"

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
			WebPDecodeBGRAInto(data, data_size, pView->data, pView->DataSize(), pView->stride);


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