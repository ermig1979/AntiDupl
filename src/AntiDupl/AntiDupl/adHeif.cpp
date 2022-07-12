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
#include "adHeif.h"
#include "adPerformance.h"

#include "libwebp\src\webp\decode.h"
#include "libheif\heif.h"

namespace ad
{
	bool THeif::Supported(HGLOBAL hGlobal)
    {
        if(hGlobal)
        {
		  	uint8_t *data = (uint8_t *)::GlobalLock(hGlobal);
			size_t data_size = ::GlobalSize(hGlobal);
			heif_filetype_result filetype_check = heif_check_filetype(data, data_size);

			return (filetype_check == heif_filetype_yes_supported);
		}
        return false;
    }

	THeif* THeif::Load(HGLOBAL hGlobal)
	{
		THeif* pHeif = NULL;
		if (hGlobal)
		{
			uint8_t* data = (uint8_t*)::GlobalLock(hGlobal);
			size_t data_size = ::GlobalSize(hGlobal);
			
			heif_context* ctx = heif_context_alloc();
			heif_error heif_error = heif_context_read_from_memory_without_copy(ctx, data, data_size, nullptr);

			if (heif_error.code == heif_error_Ok)
			{  
				heif_image_handle* handle;
				heif_error = heif_context_get_primary_image_handle(ctx, &handle);
				if (heif_error.code == heif_error_Ok)
				{
					heif_image* img;
					struct heif_decoding_options* decode_options;
					decode_options = heif_decoding_options_alloc();
					decode_options->ignore_transformations = 1;
					heif_error = heif_decode_image(handle, &img, heif_colorspace_RGB, heif_chroma_interleaved_RGBA, decode_options);
					heif_decoding_options_free(decode_options);
					if (heif_error.code == heif_error_Ok)
					{
						size_t img_width = heif_image_handle_get_width(handle);
						size_t img_height = heif_image_handle_get_height(handle);
						int img_stride;
						uint8_t* pData = heif_image_get_plane(img, heif_channel_interleaved, &img_stride);
						size_t profile_size = heif_image_handle_get_raw_color_profile_size(handle);
						if (profile_size > 0) {
							uint8_t* profile_data = static_cast<uint8_t*>(malloc(profile_size));
							heif_image_handle_get_raw_color_profile(handle, profile_data);
							free(profile_data);
						}
						TView* pView = new TView(img_width, img_height, img_stride, TView::Bgra32, pData);
						if (pView)
						{
							pHeif = new THeif();
							pHeif->m_pView = pView;
							pHeif->m_format = TImage::Heif;
						}
						else
							delete pView;
					}
				}
				//int has_alpha = heif_image_handle_has_alpha_channel(handle);
				heif_image_handle_release(handle);
			}
			::GlobalUnlock(hGlobal);
        }
        return pHeif;
	}
}