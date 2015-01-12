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
#include "adPerformance.h"
#include "adIO.h"
#include "adDds.h"

namespace ad
{
	namespace Dds
	{
		const size_t DDSD_CAPS = 0x00000001;
		const size_t DDSD_HEIGHT = 0x00000002;
		const size_t DDSD_WIDTH = 0x00000004;
		const size_t DDSD_PITCH = 0x00000008;
		const size_t DDSD_PIXELFORMAT = 0x00001000;
		const size_t DDSD_MIPMAPCOUNT = 0x00020000;
		const size_t DDSD_LINEARSIZE = 0x00080000;
		const size_t DDSD_DEPTH = 0x00800000;

		const size_t DDPF_ALPHAPIXELS = 0x00000001;
		const size_t DDPF_FOURCC = 0x00000004;
		const size_t DDPF_RGB = 0x00000040;

		const size_t FOURCC_DXT1 = 0x31545844;
		const size_t FOURCC_DXT3 = 0x33545844;
		const size_t FOURCC_DXT5 = 0x35545844;

		const size_t DDSCAPS_COMPLEX = 0x00000008;
		const size_t DDSCAPS_TEXTURE = 0x00001000;
		const size_t DDSCAPS_MIPMAP = 0x00400000;

		const size_t DDSCAPS2_CUBEMAP = 0x00000200;
		const size_t DDSCAPS2_CUBEMAP_POSITIVEX = 0x00000400;
		const size_t DDSCAPS2_CUBEMAP_NEGATIVEX = 0x00000800;
		const size_t DDSCAPS2_CUBEMAP_POSITIVEY = 0x00001000;
		const size_t DDSCAPS2_CUBEMAP_NEGATIVEY = 0x00002000;
		const size_t DDSCAPS2_CUBEMAP_POSITIVEZ = 0x00004000;
		const size_t DDSCAPS2_CUBEMAP_NEGATIVEZ = 0x00008000;
		const size_t DDSCAPS2_VOLUME = 0x00200000;

		AD_INLINE unsigned char RedFrom565(unsigned short x)
		{
			unsigned char bits = unsigned char((x & 0xF800) >> 11); 
			return bits << 3 | bits >> 2;
		}

		AD_INLINE unsigned char GreenFrom565(unsigned short x)
		{
			unsigned char bits = unsigned char((x & 0x07E0) >> 5); 
			return bits << 2 | bits >> 4;
		}

		AD_INLINE unsigned char BlueFrom565(unsigned short x)
		{
			unsigned char bits = unsigned char(x & 0x001F); 
			return bits << 3 | bits >> 2;
		}

		AD_INLINE size_t Div2(size_t x)
		{
			return x > 1 ? (x >> 1) : 1;
		}

		struct TColors
		{
			unsigned char r[4];
			unsigned char g[4];
			unsigned char b[4];
			unsigned char a[4];

			TColors(unsigned short c0, unsigned short c1, bool ignoreAlpha);
		};

		TColors::TColors(unsigned short c0, unsigned short c1, bool ignoreAlpha)
		{
			a[0] = a[1] = a[2] = a[3] = 0;

			r[0] = RedFrom565(c0);
			g[0] = GreenFrom565(c0);
			b[0] = BlueFrom565(c0);

			r[1] = RedFrom565(c1);
			g[1] = GreenFrom565(c1);
			b[1] = BlueFrom565(c1);

			if (ignoreAlpha || c0 > c1)
			{
				r[2] = (unsigned char) ((2 * r[0] + r[1]) / 3);
				g[2] = (unsigned char) ((2 * g[0] + g[1]) / 3);
				b[2] = (unsigned char) ((2 * b[0] + b[1]) / 3);

				r[3] = (unsigned char) ((r[0] + 2 * r[1]) / 3);
				g[3] = (unsigned char) ((g[0] + 2 * g[1]) / 3);
				b[3] = (unsigned char) ((b[0] + 2 * b[1]) / 3);
			}
			else
			{
				r[2] = (unsigned char) ((r[0] + r[1]) / 2);
				g[2] = (unsigned char) ((g[0] + g[1]) / 2);
				b[2] = (unsigned char) ((b[0] + b[1]) / 2);

				r[3] = g[3] = b[3] = 0;
				a[3] = 255;
			}
		}

		struct TPixelFormat
		{
			size_t flags;
			size_t fourcc;
			size_t rgbBitCount;
			size_t redBitMask;
			size_t greenBitMask;
			size_t blueBitMask;
			size_t alphaBitMask;
		};

		struct TInfo
		{
			size_t flags;
			size_t height;
			size_t width;
			size_t pitchOrLinearSize;
			size_t depth;
			size_t mimMapCount;
			size_t ddscaps1;
			size_t ddscaps2;
			TPixelFormat pixelFormat;
		};

		struct TPixel
		{
			unsigned char blue;
			unsigned char green;
			unsigned char red;
			unsigned char alpha;
		};

		struct TView			
		{
			size_t width;
			size_t height;
			size_t stride;
			TPixel *data;
			TPixel & At(size_t x, size_t y) {return data[y*stride + x];}
		};

		struct TImage
		{
			TInfo info;
			std::vector<TPixel> data;

			bool Load(IStream* pStream);

		private:
			bool LoadInfo(IStream * pStream);

			bool ReadDXT1(IStream * pStream, size_t n);
			bool ReadDXT3(IStream * pStream, size_t n);
			bool ReadDXT5(IStream * pStream, size_t n);
			bool ReadUncompressedRGB(IStream *pStream, size_t n);
			bool ReadUncompressedRGBA(IStream *pStream, size_t n);

			TView GetView(size_t x, size_t y, size_t width, size_t height, size_t n);

			void SkipDXTMipmaps(IStream *pStream, int texel_size) const;
			void SkipRGBMipmaps(IStream *pStream, int pixel_size) const;
		};

		bool TImage::Load(IStream* pStream)
		{
			bool cubemap = false, volume = false;

			if(!LoadInfo(pStream))
				return false;

			if (info.ddscaps2 & DDSCAPS2_CUBEMAP)
				cubemap = true;

			if (info.ddscaps2 & DDSCAPS2_VOLUME && info.depth > 0)
				volume = true;

			size_t num_images = 1;
			if (cubemap)
			{
				num_images = 0;
				if (info.ddscaps2 & DDSCAPS2_CUBEMAP_POSITIVEX) num_images++;
				if (info.ddscaps2 & DDSCAPS2_CUBEMAP_NEGATIVEX) num_images++;
				if (info.ddscaps2 & DDSCAPS2_CUBEMAP_POSITIVEY) num_images++;
				if (info.ddscaps2 & DDSCAPS2_CUBEMAP_NEGATIVEY) num_images++;
				if (info.ddscaps2 & DDSCAPS2_CUBEMAP_POSITIVEZ) num_images++;
				if (info.ddscaps2 & DDSCAPS2_CUBEMAP_NEGATIVEZ) num_images++;
			}

			if (volume)
				num_images = info.depth;

			data.resize(info.width*info.height*num_images);

			for (size_t n = 0; n < num_images; n++)
			{
				if (info.pixelFormat.flags & DDPF_RGB)
				{
					if (info.pixelFormat.flags & DDPF_ALPHAPIXELS)
						ReadUncompressedRGBA(pStream, n);
					else
						ReadUncompressedRGB(pStream, n);
				}
				else if (info.pixelFormat.flags & DDPF_FOURCC)
				{
					switch (info.pixelFormat.fourcc)
					{
					case FOURCC_DXT1:
							ReadDXT1(pStream, n);
							break;
					case FOURCC_DXT3:
							ReadDXT3(pStream, n);
							break;
					case FOURCC_DXT5:
							ReadDXT5(pStream, n);
							break;
					default:
						return false;
					}
				}
				else
				{
					return false;
				}
			}

			return true;
		}
		
		bool TImage::LoadInfo(IStream *pStream)
		{
			ReadStubBuffer(pStream, 4);
			size_t headerSize = ReadLittleEndianLong(pStream);
			if (headerSize != 124)
				return false;

			info.flags = ReadLittleEndianLong(pStream);
			size_t required = (size_t) (DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT);
			if ((info.flags & required) != required)
				return false;

			info.height = ReadLittleEndianLong(pStream);
			info.width = ReadLittleEndianLong(pStream);
			info.pitchOrLinearSize = ReadLittleEndianLong(pStream);
			info.depth = ReadLittleEndianLong(pStream);
			info.mimMapCount = ReadLittleEndianLong(pStream);

			ReadStubBuffer(pStream, 44);
			headerSize = ReadLittleEndianLong(pStream);
			if (headerSize != 32)
				return false;

			info.pixelFormat.flags = ReadLittleEndianLong(pStream);
			info.pixelFormat.fourcc = ReadLittleEndianLong(pStream);
			info.pixelFormat.rgbBitCount = ReadLittleEndianLong(pStream);
			info.pixelFormat.redBitMask = ReadLittleEndianLong(pStream);
			info.pixelFormat.greenBitMask =  ReadLittleEndianLong(pStream);
			info.pixelFormat.blueBitMask = ReadLittleEndianLong(pStream);
			info.pixelFormat.alphaBitMask = ReadLittleEndianLong(pStream);

			info.ddscaps1 = ReadLittleEndianLong(pStream);
			info.ddscaps2 = ReadLittleEndianLong(pStream);

			ReadStubBuffer(pStream, 12);

			return true;
		}

		bool TImage::ReadDXT1(IStream *pStream, size_t n)
		{
			for (size_t y = 0; y < info.height; y += 4)
			{
				for (size_t x = 0; x < info.width; x += 4)
				{
					TView view = GetView(x, y, 4, 4, n);

					unsigned short c0 = ReadLittleEndianShort(pStream);
					unsigned short c1 = ReadLittleEndianShort(pStream);
					size_t bits = ReadLittleEndianLong(pStream);

					TColors colors(c0, c1, false);

					for (size_t j = 0; j < view.height; j++)
					{
						for (size_t i = 0; i < view.width; i++)
						{
							size_t code = (unsigned char) ((bits >> ((j*4+i)*2)) & 0x3);
							TPixel & p = view.At(i, j);
							p.red = colors.r[code];
							p.green = colors.g[code];
							p.blue = colors.b[code];
							p.alpha = colors.a[code];
						}
					}
				}
			}
			SkipDXTMipmaps(pStream, 8);
			return true;
		}

		bool TImage::ReadDXT3(IStream *pStream, size_t n)	
		{
			for (size_t y = 0; y < info.height; y += 4)
			{
				for (size_t x = 0; x < info.width; x += 4)
				{
					TView view = GetView(x, y, 4, 4, n);

					size_t a0 = ReadLittleEndianLong(pStream);
					size_t a1 = ReadLittleEndianLong(pStream);

					unsigned short c0 = ReadLittleEndianShort(pStream);
					unsigned short c1 = ReadLittleEndianShort(pStream);
					size_t bits = ReadLittleEndianLong(pStream);

					TColors colors(c0, c1, true);

					for (size_t j = 0; j < view.height; j++)
					{
						for (size_t i = 0; i < view.width; i++)
						{
							size_t code = (bits >> ((4*j+i)*2)) & 0x3;
							TPixel & p = view.At(i, j);
							p.red = colors.r[code];
							p.green = colors.g[code];
							p.blue = colors.b[code];
							if (j < 2)
								p.alpha = 17U * (unsigned char) ((a0 >> (4*(4*j+i))) & 0xf);
							else
								p.alpha = 17U * (unsigned char) ((a1 >> (4*(4*(j-2)+i))) & 0xf);

						}
					}
				}
			}
			SkipDXTMipmaps(pStream, 16);
			return true;
		}

		bool TImage::ReadDXT5(IStream *pStream, size_t n)	
		{
			for (size_t y = 0; y < info.height; y += 4)
			{
				for (size_t x = 0; x < info.width; x += 4)
				{
					TView view = GetView(x, y, 4, 4, n);

					unsigned char a0 = ReadByte(pStream);
					unsigned char a1 = ReadByte(pStream);

					long long alpha_bits = (long long)ReadLittleEndianLong(pStream) | 
						((long long)ReadLittleEndianShort(pStream) << 32);

					unsigned short c0 = ReadLittleEndianShort(pStream);
					unsigned short c1 = ReadLittleEndianShort(pStream);
					size_t bits = ReadLittleEndianLong(pStream);

					TColors colors(c0, c1, true);

					for (size_t j = 0; j < view.height; j++)
					{
						for (size_t i = 0; i < view.width; i++)
						{
							size_t code = (unsigned char) ((bits >> ((j*4+i)*2)) & 0x3);
							TPixel & p = view.At(i, j);
							p.red = colors.r[code];
							p.green = colors.g[code];
							p.blue = colors.b[code];

							size_t alpha_code = (size_t) (alpha_bits >> (3*(4*j+i))) & 0x7;
							if (alpha_code == 0)
								p.alpha = a0;
							else if (alpha_code == 1)
								p.alpha = a1;
							else if (a0 > a1)
								p.alpha = (unsigned char)(((8-alpha_code) * a0 + (alpha_code-1) * a1) / 7);
							else if (alpha_code == 6)
								p.alpha = (unsigned char) alpha_code;
							else if (alpha_code == 7)
								p.alpha = 255;
							else
								p.alpha = (unsigned char)(((6-alpha_code) * a0 + (alpha_code-1) * a1) / 5);	
						}
					}
				}
			}
			SkipDXTMipmaps(pStream, 16);
			return true;
		}

		bool TImage::ReadUncompressedRGB(IStream *pStream, size_t n)
		{
			for (size_t y = 0; y < info.height; y++)
			{
				TView view = GetView(0, y, info.width, 1, n);
				for (size_t x = 0; x < info.width; x++)
				{
					TPixel & p = view.At(x, 0);
					p.blue = ReadByte(pStream);
					p.green = ReadByte(pStream);
					p.red = ReadByte(pStream);
					if (info.pixelFormat.rgbBitCount == 32)
						ReadByte(pStream);
					p.alpha = 0xFF;
				}
			}
			SkipRGBMipmaps(pStream, 3);
			return true;
		}

		bool TImage::ReadUncompressedRGBA(IStream *pStream, size_t n)
		{
			for (size_t y = 0; y < info.height; y++)
			{
				TView view = GetView(0, y, info.width, 1, n);
				for (size_t x = 0; x < info.width; x++)
				{
					TPixel & p = view.At(x, 0);
					p.blue = ReadByte(pStream);
					p.green = ReadByte(pStream);
					p.red = ReadByte(pStream);
					p.alpha = ReadByte(pStream);
				}
			}
			SkipRGBMipmaps(pStream, 4);
			return true;
		}

		TView TImage::GetView(size_t x, size_t y, size_t width, size_t height, size_t n)
		{
			TView view;
			view.width = Simd::Min(width, info.width - x);
			view.height = Simd::Min(height, info.height - y);
			view.stride = info.width;
			view.data = (&data[0]) + y*info.width + x + n*info.width*info.height;
			return view;
		}

		void TImage::SkipDXTMipmaps(IStream *pStream, int texel_size) const
		{
			if (info.ddscaps1 & DDSCAPS_MIPMAP && (info.ddscaps1 & DDSCAPS_TEXTURE || info.ddscaps2 & DDSCAPS2_CUBEMAP))
			{
				size_t w = Div2(info.width);
				size_t h = Div2(info.height);
				for (size_t i = 1; (i < (size_t) info.mimMapCount) && w && h; i++)
				{
					size_t offset = (size_t) ((w + 3) / 4) * ((h + 3) / 4) * texel_size;
					ReadStubBuffer(pStream, offset);
					w = Div2(w);
					h = Div2(h);
				}
			}
		}

		void TImage::SkipRGBMipmaps(IStream *pStream, int pixel_size) const
		{
			if (info.ddscaps1 & DDSCAPS_MIPMAP && (info.ddscaps1 & DDSCAPS_TEXTURE || info.ddscaps2 & DDSCAPS2_CUBEMAP))
			{
				size_t w = Div2(info.width);
				size_t h = Div2(info.height);

				for (size_t i = 1; (i < (size_t) info.mimMapCount) && w && h; i++)
				{
					size_t offset = (size_t) w * h * pixel_size;
					ReadStubBuffer(pStream, offset);
					w = Div2(w);
					h = Div2(h);
				}
			}
		}
	}

	TDds* TDds::Load(HGLOBAL hGlobal)
	{
		AD_FUNCTION_PERFORMANCE_TEST

		TDds* pDds = NULL;
		if(hGlobal)
		{
			IStream* pStream = NULL;
			if(::CreateStreamOnHGlobal(hGlobal, FALSE, &pStream) == S_OK)
			{
				Dds::TImage image;
				bool result = false;
				try
				{
					result = image.Load(pStream);
				}
				catch (...)
				{
				}
				if(result)
				{
					size_t width = image.info.width;
					size_t height = image.info.height;
					TView *pView = new TView(width, height, width*TView::PixelSize(TView::Bgra32), TView::Bgra32, NULL); 
					if(pView)
					{
						for(size_t row = 0; row < height; ++row)
						{
							for(size_t col = 0; col < width; ++col)
							{
								Dds::TPixel & src = image.data[row*width + col];
								unsigned char * dst = pView->data + row*pView->stride + col*4;
								dst[0] = src.blue;
								dst[1] = src.green;
								dst[2] = src.red;
								dst[3] = 0xFF;
							}
						}
						pDds= new TDds();
						pDds->m_pView = pView;
						pDds->m_format = TImage::Dds;
						AD_PERFORMANCE_TEST_SET_SIZE(height*width)
					}
				}
				pStream->Release();
			}
		}
		return pDds;
	}

	bool TDds::Supported(HGLOBAL hGlobal)
	{
		if(hGlobal)
		{
			unsigned char *data = (unsigned char*)::GlobalLock(hGlobal);
			size_t size = ::GlobalSize(hGlobal);
			bool supported = (size >= 4 && memcmp(data, "DDS ", 4) == 0);
			::GlobalUnlock(hGlobal);
			return supported;
		}
		return false;
	}
}
