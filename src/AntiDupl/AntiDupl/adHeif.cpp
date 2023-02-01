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
#include "adLogger.h"

//Install vcpkg to get libheif see https://github.com/microsoft/vcpkg
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
			::GlobalUnlock(hGlobal);

			return ((filetype_check == heif_filetype_yes_supported) || (filetype_check == heif_filetype_maybe));
		}
        return false;
    }

	THeif* THeif::Load(HGLOBAL hGlobal)
	{
		AD_FUNCTION_PERFORMANCE_TEST

		THeif* pHeif = NULL;
		if (hGlobal)
		{
			uint8_t* data = (uint8_t*)::GlobalLock(hGlobal);
			size_t data_size = ::GlobalSize(hGlobal);
			
			struct heif_error heif_error = heif_init(NULL);
			if (heif_error.code != heif_error_Ok)
			{
#ifdef AD_LOGGER_ENABLE
				AD_LOG(heif_error.message);
#endif//AD_LOGGER_ENABLE
				return NULL;
			}

			heif_context* heif_ctx = heif_context_alloc();

			assert(heif_ctx);

			heif_error = heif_context_read_from_memory_without_copy(heif_ctx, data, data_size, nullptr);

			int numCPU = SimdCpuInfo(SimdCpuInfoCores);
			heif_context_set_max_decoding_threads(heif_ctx, numCPU);

			::GlobalUnlock(hGlobal);

			if (heif_error.code == heif_error_Ok)
			{  
				heif_image_handle* heif_handle;
				heif_error = heif_context_get_primary_image_handle(heif_ctx, &heif_handle);
				if (heif_error.code == heif_error_Ok)
				{
					struct heif_image* heif_img;
					struct heif_decoding_options* decode_options;
					decode_options = heif_decoding_options_alloc();
					decode_options->ignore_transformations = 0;

					int img_has_alpha = heif_image_handle_has_alpha_channel(heif_handle);

					heif_error = heif_decode_image(heif_handle, &heif_img, heif_colorspace_RGB, img_has_alpha ? heif_chroma_interleaved_RGBA : heif_chroma_interleaved_RGB, decode_options);

					heif_decoding_options_free(decode_options);

					if (heif_error.code == heif_error_Ok)
					{

						size_t img_width = heif_image_handle_get_width(heif_handle);
						size_t img_height = heif_image_handle_get_height(heif_handle);

						// Get interleaved RGB plane
						int img_stride_RGB = 0;
						const uint8_t* pData_RGB = heif_image_get_plane_readonly(heif_img, heif_channel_interleaved, &img_stride_RGB);
						TView* pView_RGB = new TView(img_width, img_height, img_stride_RGB, TView::Rgb24, NULL);

						AD_PERFORMANCE_TEST_SET_SIZE(img_height * img_stride_RGB)

						if (pView_RGB)
						{
							memcpy(pView_RGB->data, pData_RGB, img_height * img_stride_RGB);
							pHeif = new THeif();
							pHeif->m_pView = pView_RGB;
							pHeif->m_format = TImage::Heif;
						}
						else
						{
							delete pView_RGB;
						}
						heif_image_release(heif_img);
					}
				}
				heif_image_handle_release(heif_handle);
			}

			if (heif_error.code != heif_error_Ok)
			{ 
#ifdef AD_LOGGER_ENABLE
				AD_LOG(heif_error.message);
#endif//AD_LOGGER_ENABLE
				heif_deinit();
				return NULL;
			}

			heif_context_free(heif_ctx);
			heif_deinit();
			return pHeif;
        }
		return NULL;
	}
}