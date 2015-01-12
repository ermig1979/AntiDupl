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
#include "adTga.h"

namespace ad
{
	namespace Tga
	{
		const unsigned char TGAColormap = 1;
		const unsigned char TGARGB = 2;
		const unsigned char TGAMonochrome  = 3;
		const unsigned char TGARLEColormap = 9;
		const unsigned char TGARLERGB = 10;
		const unsigned char TGARLEMonochrome = 11;

		AD_INLINE unsigned char Scale5BitToChar(unsigned char value)
		{
			return value << 3 | value >> 2;
		}

		struct TInfo
		{
			unsigned char idLength;
			unsigned char colorMapType;
			unsigned char imageType;
			unsigned short colorMapIndex;
			unsigned short colorMapLength;
			unsigned char colorMapSize;
			unsigned short xOrigin;
			unsigned short yOrigin;
			unsigned short width;
			unsigned short height;
			unsigned char bitsPerPixel;
			unsigned char attributes;
		};

		struct TPixel
		{
			unsigned char blue;
			unsigned char green;
			unsigned char red;
			unsigned char alpha;
		};

		struct TImage
		{
			TInfo info;
			std::string comment;
			std::vector<TPixel> colors;
			std::vector<TPixel> data;

			bool Load(IStream* pStream);
			bool LoadInfo(IStream* pStream);
		private:
			bool LoadColorMap(IStream* pStream);
			bool LoadData(IStream* pStream);
		};

		bool TImage::Load(IStream* pStream)
		{
			if(!LoadInfo(pStream))
				return false;

			comment.resize(info.idLength, 0);
			ReadBuffer(pStream, (unsigned char*)comment.c_str(), info.idLength);

			if ((info.imageType == TGAColormap) ||
				(info.imageType == TGAMonochrome) ||
				(info.imageType == TGARLEColormap) ||
				(info.imageType == TGARLEMonochrome))
			{
				if (info.colorMapType != 0)
					colors.resize(info.colorMapLength);
				else
					colors.resize(size_t(1) << info.bitsPerPixel);
			}

			if(info.colorMapType != 0 && !LoadColorMap(pStream))
				return false;

			data.resize(size_t(info.height)*info.width);
			if(!LoadData(pStream))
				return false;

			return true;
		}

		bool TImage::LoadInfo(IStream* pStream)
		{
			info.idLength = ReadByte(pStream);
			info.colorMapType = ReadByte(pStream);
			info.imageType = ReadByte(pStream);
			if (((info.imageType != TGAColormap) && (info.imageType != TGARGB) && (info.imageType != TGAMonochrome) &&
				(info.imageType != TGARLEColormap) && (info.imageType != TGARLERGB) && (info.imageType != TGARLEMonochrome)) ||
				(((info.imageType == TGAColormap) || (info.imageType == TGARLEColormap)) && (info.colorMapType == 0)))
				return false;

			info.colorMapIndex = ReadLittleEndianShort(pStream);
			info.colorMapLength = ReadLittleEndianShort(pStream);
			info.colorMapSize = ReadByte(pStream);
			info.xOrigin = ReadLittleEndianShort(pStream);
			info.yOrigin = ReadLittleEndianShort(pStream);
			info.width = ReadLittleEndianShort(pStream);
			info.height = ReadLittleEndianShort(pStream);
			info.bitsPerPixel = ReadByte(pStream);
			info.attributes = ReadByte(pStream);
			if ((((info.bitsPerPixel <= 1) || (info.bitsPerPixel >= 17)) &&
				(info.bitsPerPixel != 24) && (info.bitsPerPixel != 32)))
				return false;

			return true;
		}

		bool TImage::LoadColorMap(IStream* pStream)
		{
			for (size_t i = 0; i < colors.size(); i++)
			{
				TPixel & p = colors[i];
				switch (info.colorMapSize)
				{
				case 8:
				default:
					{
						unsigned char gray =  ReadByte(pStream);
						p.red = gray;
						p.green = gray;
						p.blue = gray;
						p.alpha = 0xFF;
						break;
					}
				case 15:
				case 16:
					{
						unsigned char j = ReadByte(pStream);
						unsigned char k = ReadByte(pStream);
						p.red = Scale5BitToChar((k & 0x7c) >> 2);
						p.green = Scale5BitToChar(((k & 0x03) << 3) | ((j & 0xe0) >> 5));
						p.blue = Scale5BitToChar(j & 0x1f);
						break;
					}
				case 24:
				case 32:
					{
						p.blue = ReadByte(pStream);
						p.green = ReadByte(pStream);
						p.red = ReadByte(pStream);
						break;
					}
				}
			}
			return true;
		}

		bool TImage::LoadData(IStream* pStream)
		{
			size_t base = 0;
			size_t flag = 0;
			bool skip = false;
			size_t real = 0;
			unsigned char runlength = 0;
			size_t offset = 0;
			for (size_t y = 0; y < (size_t)info.height; y++)
			{
				size_t real = offset;
				if (((unsigned char) (info.attributes & 0x20) >> 5) == 0)
					real = info.height - y - 1;
				TPixel * p = &data[real*info.width];

				for (size_t x = 0; x < (size_t)info.width; x++)
				{
					if ((info.imageType == TGARLEColormap) || (info.imageType == TGARLERGB) || (info.imageType == TGARLEMonochrome))
					{
						if (runlength != 0)
						{
							runlength--;
							skip = flag != 0;
						}
						else
						{
							runlength = ReadByte(pStream);
							flag = runlength & 0x80;
							if (flag != 0)
								runlength -= 128;
							skip = false;
						}
					}
					if (skip == false)
					{
						switch (info.bitsPerPixel)
						{
						case 8:
						default:
							{
								size_t index = ReadByte(pStream);
								if (info.colorMapType != 0)
									*p = colors[index];
								else
								{
									p->red = (unsigned char)index;
									p->green = (unsigned char)index;
									p->blue = (unsigned char)index;
								}
								break;
							}
						case 15:
						case 16:
							{
								unsigned char j = ReadByte(pStream);
								unsigned char k = ReadByte(pStream);
								p->red = Scale5BitToChar((k & 0x7c) >> 2);
								p->green = Scale5BitToChar(((k & 0x03) << 3) | ((j & 0xe0) >> 5));
								p->blue = Scale5BitToChar(j & 0x1f);
								p->alpha = ((info.attributes & 0x0FU) && (k & 0x80) == 0) ? 0xFF : 0x00;
								break;
							}
						case 24:
							{
								p->red = ReadByte(pStream);
								p->green = ReadByte(pStream);
								p->blue = ReadByte(pStream);
								p->alpha = 0xFF;
								break;
							}
						case 32:
							{
								p->red = ReadByte(pStream);
								p->green = ReadByte(pStream);
								p->blue = ReadByte(pStream);
								p->alpha = ReadByte(pStream);
								break;
							}
						}
					}
					p++;
				}
				if (((unsigned char) (info.attributes & 0xc0) >> 6) == 4)
					offset += 4;
				else if (((unsigned char) (info.attributes & 0xc0) >> 6) == 2)
					offset += 2;
				else
					offset++;
				if (offset >= info.height)
				{
					base++;
					offset = base;
				}
			}
			return true;
		}
	}

	TTga* TTga::Load(HGLOBAL hGlobal)
	{
		AD_FUNCTION_PERFORMANCE_TEST

		TTga* pTga = NULL;
		if(hGlobal)
		{
			IStream* pStream = NULL;
			if(::CreateStreamOnHGlobal(hGlobal, FALSE, &pStream) == S_OK)
			{
				Tga::TImage image;
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
								Tga::TPixel & src = image.data[row*width + col];
								unsigned char * dst = pView->data + row*pView->stride + col*4;
								dst[0] = src.blue;
								dst[1] = src.green;
								dst[2] = src.red;
								dst[3] = 0xFF;
							}
						}
						pTga= new TTga();
						pTga->m_pView = pView;
						pTga->m_format = TImage::Tga;
						AD_PERFORMANCE_TEST_SET_SIZE(height*width)
					}
				}
				pStream->Release();
			}
		}
		return pTga;
	}

	bool TTga::Supported(HGLOBAL hGlobal)
	{
		if(hGlobal)
		{
			IStream* pStream = NULL;
			if(::CreateStreamOnHGlobal(hGlobal, FALSE, &pStream) == S_OK)
			{
				Tga::TImage image;
				bool result = false;
				try
				{
					result = image.LoadInfo(pStream);
				}
				catch (...)
				{
				}
				pStream->Release();
				return result;
			}
		}
		return false;
	}
}
