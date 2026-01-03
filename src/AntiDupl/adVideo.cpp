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
#include "adVideo.h"

extern "C" {
	#include <libavformat/avformat.h>
	#include <libavcodec/avcodec.h>
	#include <libswscale/swscale.h>
	#include <libavutil/imgutils.h>
	#include <libavutil/display.h>
	#include <libavutil/frame.h>
}

namespace ad
{
	// Memory reader for ffmpeg AVIO
	struct FfmpegMemoryReader
	{
		const uint8_t* data;
		size_t size;
		size_t pos;
	};

	static int ff_read_packet(void* opaque, uint8_t* buf, int buf_size)
	{
		FfmpegMemoryReader* r = (FfmpegMemoryReader*)opaque;
		if (!r || r->pos >= r->size)
			return AVERROR_EOF;
		size_t to_read = r->size - r->pos;
		if (to_read > (size_t)buf_size) to_read = (size_t)buf_size;
		memcpy(buf, r->data + r->pos, to_read);
		r->pos += to_read;
		return (int)to_read;
	}

	static int64_t ff_seek(void* opaque, int64_t offset, int whence)
	{
		FfmpegMemoryReader* r = (FfmpegMemoryReader*)opaque;
		if (!r) return -1;
		if (whence == AVSEEK_SIZE) return (int64_t)r->size;
		size_t newpos = 0;
		switch (whence)
		{
		case SEEK_SET: newpos = (size_t)offset; break;
		case SEEK_CUR: newpos = r->pos + (size_t)offset; break;
		case SEEK_END: newpos = r->size + (size_t)offset; break;
		default: return -1;
		}
		if (newpos > r->size) return -1;
		r->pos = newpos;
		return (int64_t)r->pos;
	}

	int is_image_codec(AVCodecID id) {
		switch (id) {
		case AV_CODEC_ID_PNG:
		case AV_CODEC_ID_JPEG2000:
		case AV_CODEC_ID_MJPEG:
		case AV_CODEC_ID_BMP:
		case AV_CODEC_ID_GIF:
		case AV_CODEC_ID_TIFF:
			return true;
		default:
			return false;
		}
	}

	bool TVideo::Supported(HGLOBAL hGlobal)
	{
		if (!hGlobal)
			return false;

		bool result = false;
		uint8_t* data = (uint8_t*)::GlobalLock(hGlobal);
		if (!data)
			return false;

		size_t data_size = ::GlobalSize(hGlobal);

		AVIOContext* avio_ctx = nullptr;
		AVFormatContext* fmt_ctx = nullptr;
		uint8_t* avio_buffer = nullptr;
		FfmpegMemoryReader reader;
		reader.data = data;
		reader.size = data_size;
		reader.pos = 0;

		const int avio_buf_size = 4096;
		avio_buffer = (uint8_t*)av_malloc(avio_buf_size);
		if (avio_buffer)
		{
			avio_ctx = avio_alloc_context(avio_buffer, avio_buf_size, 0, &reader, ff_read_packet, NULL, ff_seek);
			if (avio_ctx)
			{
				fmt_ctx = avformat_alloc_context();
				fmt_ctx->pb = avio_ctx;
				// Prevent avformat_open_input from trying to seek using the underlying IO beyond our callbacks
				fmt_ctx->flags |= AVFMT_FLAG_CUSTOM_IO;

				if (avformat_open_input(&fmt_ctx, NULL, NULL, NULL) == 0)
				{
					if (avformat_find_stream_info(fmt_ctx, NULL) >= 0)
					{
						for (unsigned i = 0; i < fmt_ctx->nb_streams; ++i)
						{
							if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
							{
								if (is_image_codec(fmt_ctx->streams[i]->codecpar->codec_id) == false)
								{
									result = true;
									break;
								}
							}
						}
					}
				}
				avformat_close_input(&fmt_ctx);
			}
		}

		// free avio_buffer (avio_ctx owns the buffer pointer but avio_free_context does not free it automatically)
		if (avio_ctx)
		{
			// avio_ctx->buffer was provided by av_malloc above; avio_free_context will free internal structures but not the buffer itself
			av_freep(&avio_ctx->buffer);
			avio_context_free(&avio_ctx);
		}
		::GlobalUnlock(hGlobal);
		return result;
	}

	TVideo* TVideo::Load(HGLOBAL hGlobal)
	{
		TVideo* pVideo = NULL;
		if (!hGlobal)
			return NULL;

		uint8_t* data = (uint8_t*)::GlobalLock(hGlobal);
		if (!data)
			return NULL;

		size_t data_size = ::GlobalSize(hGlobal);

		AVFormatContext* fmt_ctx = nullptr;
		AVIOContext* avio_ctx = nullptr;
		uint8_t* avio_buffer = nullptr;
		FfmpegMemoryReader reader;
		reader.data = data;
		reader.size = data_size;
		reader.pos = 0;

		const int avio_buf_size = 4096;
		avio_buffer = (uint8_t*)av_malloc(avio_buf_size);
		if (!avio_buffer)
		{
			::GlobalUnlock(hGlobal);
			return NULL;
		}

		avio_ctx = avio_alloc_context(avio_buffer, avio_buf_size, 0, &reader, ff_read_packet, NULL, ff_seek);
		if (!avio_ctx)
		{
			av_free(avio_buffer);
			::GlobalUnlock(hGlobal);
			return NULL;
		}

		fmt_ctx = avformat_alloc_context();
		fmt_ctx->pb = avio_ctx;
		fmt_ctx->flags |= AVFMT_FLAG_CUSTOM_IO;

		AVCodecContext* codec_ctx = NULL;
		struct SwsContext* sws_ctx = NULL;
		AVFrame* frame = NULL;
		AVFrame* rgb_frame = NULL;
		AVPacket* pkt = NULL;
		uint8_t* rgb_buffer = NULL;

		// RAII cleanup to avoid goto-based cleanup
		struct Cleanup
		{
			HGLOBAL hGlobal;
			AVIOContext** avio_ctx_ptr;
			uint8_t** avio_buffer_ptr;
			AVFormatContext** fmt_ctx_ptr;
			AVCodecContext** codec_ctx_ptr;
			SwsContext** sws_ctx_ptr;
			AVFrame** frame_ptr;
			AVFrame** rgb_frame_ptr;
			AVPacket** pkt_ptr;
			uint8_t** rgb_buffer_ptr;

			~Cleanup()
			{
				if (avio_ctx_ptr && *avio_ctx_ptr)
				{
					av_freep(&(*avio_ctx_ptr)->buffer);
					avio_context_free(avio_ctx_ptr);
				}
				else if (avio_buffer_ptr && *avio_buffer_ptr)
				{
					av_free(*avio_buffer_ptr);
					*avio_buffer_ptr = nullptr;
				}

				if (rgb_buffer_ptr && *rgb_buffer_ptr)
				{
					av_free(*rgb_buffer_ptr);
					*rgb_buffer_ptr = nullptr;
				}
				if (sws_ctx_ptr && *sws_ctx_ptr)
				{
					sws_freeContext(*sws_ctx_ptr);
					*sws_ctx_ptr = nullptr;
				}
				if (pkt_ptr && *pkt_ptr)
				{
					av_packet_free(pkt_ptr);
				}
				if (frame_ptr && *frame_ptr)
				{
					av_frame_free(frame_ptr);
				}
				if (rgb_frame_ptr && *rgb_frame_ptr)
				{
					av_frame_free(rgb_frame_ptr);
				}
				if (codec_ctx_ptr && *codec_ctx_ptr)
				{
					avcodec_free_context(codec_ctx_ptr);
				}
				if (fmt_ctx_ptr && *fmt_ctx_ptr)
				{
					avformat_close_input(fmt_ctx_ptr);
				}
				if (hGlobal)
					::GlobalUnlock(hGlobal);
			}
		} cleanup{ hGlobal, &avio_ctx, &avio_buffer, &fmt_ctx, &codec_ctx, &sws_ctx, &frame, &rgb_frame, &pkt, &rgb_buffer };

		// open input and find stream info
		if (avformat_open_input(&fmt_ctx, NULL, NULL, NULL) < 0)
			return NULL;
		if (avformat_find_stream_info(fmt_ctx, NULL) < 0)
			return NULL;

		int video_stream_index = -1;
		for (unsigned i = 0; i < fmt_ctx->nb_streams; ++i)
		{
			if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				video_stream_index = (int)i;
				break;
			}
		}
		if (video_stream_index < 0)
			return NULL;

		AVCodecParameters* codecpar = fmt_ctx->streams[video_stream_index]->codecpar;
		const AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);
		if (!codec)
			return NULL;

		codec_ctx = avcodec_alloc_context3(codec);
		if (!codec_ctx)
			return NULL;

		if (avcodec_parameters_to_context(codec_ctx, codecpar) < 0)
			return NULL;
		if (avcodec_open2(codec_ctx, codec, NULL) < 0)
			return NULL;

		frame = av_frame_alloc();
		rgb_frame = av_frame_alloc();
		pkt = av_packet_alloc();
		if (!frame || !rgb_frame || !pkt)
			return NULL;

		int got_frame = 0;
		while (av_read_frame(fmt_ctx, pkt) >= 0)
		{
			if (pkt->stream_index == video_stream_index)
			{
				if (avcodec_send_packet(codec_ctx, pkt) == 0)
				{
					while (avcodec_receive_frame(codec_ctx, frame) == 0)
					{
						got_frame = 1;
						break;
					}
				}
			}
			av_packet_unref(pkt);
			if (got_frame) break;
		}
		if (!got_frame)
			return NULL;

		int img_width = frame->width;
		int img_height = frame->height;
		int img_stride_RGB = img_width * 3;
		int rgb_buf_size = av_image_get_buffer_size(AV_PIX_FMT_RGB24, img_width, img_height, 1);
		rgb_buffer = (uint8_t*)av_malloc((size_t)rgb_buf_size);
		if (!rgb_buffer)
			return NULL;

		av_image_fill_arrays(rgb_frame->data, rgb_frame->linesize, rgb_buffer, AV_PIX_FMT_RGB24, img_width, img_height, 1);

		sws_ctx = sws_getContext(img_width, img_height, (AVPixelFormat)frame->format,
			img_width, img_height, AV_PIX_FMT_RGB24,
			SWS_BILINEAR, NULL, NULL, NULL);
		if (!sws_ctx)
			return NULL;

		sws_scale(sws_ctx, frame->data, frame->linesize, 0, img_height, rgb_frame->data, rgb_frame->linesize);

		// Determine rotation from metadata/side-data
		double angle = 0.0;
		// Try metadata "rotate"
		AVDictionaryEntry* rotate_tag = av_dict_get(fmt_ctx->streams[video_stream_index]->metadata, "rotate", NULL, 0);
		if (rotate_tag)
		{
			angle = atof(rotate_tag->value);
		}
		else
		{
			AVFrameSideData* sd = av_frame_get_side_data(frame, AV_FRAME_DATA_DISPLAYMATRIX);
			if (sd) {
				angle = av_display_rotation_get((int32_t*)sd->data);
			}
		}
		// normalize
		int rotation = ((int)angle % 360 + 360) % 360;

		// Create TView and copy RGB data
		TView* pView_RGB = new TView(img_width, img_height, img_stride_RGB, TView::Rgb24, NULL);
		if (pView_RGB)
		{
			memcpy(pView_RGB->data, rgb_frame->data[0], img_height * img_stride_RGB);
			pVideo = new TVideo();
			pVideo->m_pView = pView_RGB;
			pVideo->m_format = TImage::Video;

			// Use simdlib TransformSize to determine destination size after rotation
			::SimdTransformType simdTransform = SimdTransformRotate0;
			TView* pView_RGB_rotated;
			if (rotation == 90)
			{
				pView_RGB_rotated = new TView(img_height, img_height, img_height*3, TView::Rgb24, NULL);
				simdTransform = SimdTransformRotate90;
			}
			else if (rotation == 180)
			{
				pView_RGB_rotated = new TView(img_width, img_height, img_stride_RGB, TView::Rgb24, NULL);
				simdTransform = SimdTransformRotate180;
			}
			else if (rotation == 270)
			{
				pView_RGB_rotated = new TView(img_height, img_width, img_height*3, TView::Rgb24, NULL);
				simdTransform = SimdTransformRotate270;
			}
			
			if (rotation > 0)
			{
				Simd::TransformImage(*pVideo->m_pView, simdTransform, *pView_RGB_rotated);
				pVideo->m_pView = pView_RGB_rotated;
			}
		}
		else
		{
			// If allocation failed, ensure we don't leak (pView_RGB is nullptr so delete is safe no-op)
			delete pView_RGB;
		}

		// cleanup destructor will run here (unlocks global, frees ffmpeg resources)
		return pVideo;
	}
}