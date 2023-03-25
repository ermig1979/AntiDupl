/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2023 Yermalayeu Ihar,
*               2013-2023 Borisov Dmitry.
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

#include "adJxl.h"
#include "adPerformance.h"
#include "adLogger.h"

#include <jxl/decode.h>
#include <jxl/decode_cxx.h>
#include <jxl/resizable_parallel_runner.h>
#include <jxl/resizable_parallel_runner_cxx.h>

namespace ad
{
	bool TJxl::Supported(HGLOBAL hGlobal)
	{
		if (hGlobal)
		{
			uint8_t* data = (uint8_t*)::GlobalLock(hGlobal);
			size_t data_size = ::GlobalSize(hGlobal);

			JxlSignature signature = JxlSignatureCheck(data, data_size);
			::GlobalUnlock(hGlobal);

			if (signature == JXL_SIG_NOT_ENOUGH_BYTES || signature == JXL_SIG_INVALID)
				return false;
			else
				return true;

		}
		return false;
	}

	TJxl* TJxl::Load(HGLOBAL hGlobal)
	{
		AD_FUNCTION_PERFORMANCE_TEST

		TJxl* pJxl = NULL;
		if (hGlobal)
		{
			uint8_t* data = (uint8_t*)::GlobalLock(hGlobal);
			size_t data_size = ::GlobalSize(hGlobal);

			auto runner = JxlResizableParallelRunnerMake(nullptr);

			auto decoder = JxlDecoderMake(nullptr);
			if (JXL_DEC_SUCCESS !=
				JxlDecoderSubscribeEvents(decoder.get(), JXL_DEC_BASIC_INFO |
					JXL_DEC_COLOR_ENCODING |
					JXL_DEC_FULL_IMAGE))
			{
#ifdef AD_LOGGER_ENABLE
				AD_LOG("JxlDecoderSubscribeEvents failed\n);
#endif//AD_LOGGER_ENABLE
				return NULL;
			}

			if (JXL_DEC_SUCCESS != JxlDecoderSetParallelRunner(decoder.get(),
				JxlResizableParallelRunner,
				runner.get()))
			{
#ifdef AD_LOGGER_ENABLE
				AD_LOG("JxlDecoderSetParallelRunner failed\n);
#endif//AD_LOGGER_ENABLE
				return NULL;
			}

			JxlBasicInfo info;
			JxlPixelFormat format = { 4, JXL_TYPE_UINT8, JXL_NATIVE_ENDIAN, 0 };
			size_t xsize = 0, ysize = 0;
			std::vector<uint8_t> pixels;

			JxlDecoderSetInput(decoder.get(), data, data_size);
			JxlDecoderCloseInput(decoder.get());

			for (;;)
			{
				JxlDecoderStatus status = JxlDecoderProcessInput(decoder.get());

				if (status == JXL_DEC_ERROR)
				{
#ifdef AD_LOGGER_ENABLE
					AD_LOG("Decoder error\n");
#endif//AD_LOGGER_ENABLE
					return NULL;
				}
				else if (status == JXL_DEC_NEED_MORE_INPUT)
				{
#ifdef AD_LOGGER_ENABLE
					AD_LOG("Error, already provided all input\n");
#endif//AD_LOGGER_ENABLE
					return NULL;
				}
				else if (status == JXL_DEC_BASIC_INFO)
				{
					if (JXL_DEC_SUCCESS != JxlDecoderGetBasicInfo(decoder.get(), &info))
					{
#ifdef AD_LOGGER_ENABLE
						AD_LOG("JxlDecoderGetBasicInfo failed\n);
#endif//AD_LOGGER_ENABLE
						return NULL;
					}
					xsize = info.xsize;
					ysize = info.ysize;
					JxlResizableParallelRunnerSetThreads(
						runner.get(),
						JxlResizableParallelRunnerSuggestThreads(info.xsize, info.ysize));
				}
				else if (status == JXL_DEC_COLOR_ENCODING)
				{
					// Get the ICC color profile of the pixel data
					size_t icc_size;
					if (JXL_DEC_SUCCESS !=
						JxlDecoderGetICCProfileSize(
							decoder.get(), &format, JXL_COLOR_PROFILE_TARGET_DATA, &icc_size))
					{
#ifdef AD_LOGGER_ENABLE
						AD_LOG("JxlDecoderGetICCProfileSize failed\n);
#endif//AD_LOGGER_ENABLE
						return NULL;
					}
					//icc_profile->resize(icc_size);
					//if (JXL_DEC_SUCCESS != JxlDecoderGetColorAsICCProfile(
					//	decoder.get(), &format,
					//	JXL_COLOR_PROFILE_TARGET_DATA,
					//	icc_profile->data(), icc_profile->size())) {
					//	return NULL;
					//}
				}
				else if (status == JXL_DEC_NEED_IMAGE_OUT_BUFFER)
				{
					size_t buffer_size;
					if (JXL_DEC_SUCCESS !=
						JxlDecoderImageOutBufferSize(decoder.get(), &format, &buffer_size))
					{
#ifdef AD_LOGGER_ENABLE
						AD_LOG("JxlDecoderImageOutBufferSize failed\n);
#endif//AD_LOGGER_ENABLE
						return NULL;
					}
					if (buffer_size != xsize * ysize * 4) {
#ifdef AD_LOGGER_ENABLE
						AD_LOG("Invalid out buffer size");
#endif//AD_LOGGER_ENABLE
						return NULL;
					}
					pixels.resize(xsize * ysize * 4);
					void* pixels_buffer = (void*)pixels.data();
					size_t pixels_buffer_size = pixels.size() * sizeof(uint8_t);
					if (JXL_DEC_SUCCESS != JxlDecoderSetImageOutBuffer(decoder.get(), &format,
						pixels_buffer,
						pixels_buffer_size)) {
#ifdef AD_LOGGER_ENABLE
						AD_LOG("JxlDecoderSetImageOutBuffer failed\n");
#endif//AD_LOGGER_ENABLE
						return NULL;
					}
				}
				else if (status == JXL_DEC_FULL_IMAGE) {
					// Nothing to do. Do not yet return. If the image is an animation, more
					// full frames may be decoded. This example only keeps the last one.
				}
				else if (status == JXL_DEC_SUCCESS) {
					// All decoding successfully finished.
					// It's not required to call JxlDecoderReleaseInput(dec.get()) here since
					// the decoder will be destroyed.
					break;
				}
				else {
					fprintf(stderr, "Unknown decoder status\n");
#ifdef AD_LOGGER_ENABLE
					AD_LOG("Unknown decoder status\n");
#endif//AD_LOGGER_ENABLE
					return NULL;
				}
			}

			TView* pView_RGBA = new TView(xsize, ysize, xsize * 4, TView::Rgba32, NULL);
			AD_PERFORMANCE_TEST_SET_SIZE(ysize * xsize * 4)
			if (pView_RGBA)
			{
				memcpy(pView_RGBA->data, pixels.data(), ysize * xsize * 4);
				pJxl = new TJxl();
				pJxl->m_pView = pView_RGBA;
				pJxl->m_format = TImage::Jxl;
			}
			else
			{
				delete pView_RGBA;
			}
			return pJxl;
		}
		return NULL;
	}
}