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
#include "adPsd.h"

namespace ad
{
    namespace Psd
    {
        const int CHANNEL_COUNT_MAX = 56;
        const int CHANNEL_MAX = 255;

        enum TMode
        {
            BitmapMode = 0,
            GrayscaleMode = 1,
            IndexedMode = 2,
            RGBMode = 3,
            CMYKMode = 4,
            MultichannelMode = 7,
            DuotoneMode = 8,
            LabMode = 9
        };

        enum TCompression
        {
            UndefinedCompression,
            NoCompression,
            RLECompression
        };

        enum TColorspace
        {
            UndefinedColorspace,
            RGBColorspace,
            GRAYColorspace,
            LabColorspace,
            CMYKColorspace,
        };        

        template <class T> __forceinline T ReadValue(const unsigned char * & pixels)
        {
            return InvertByteOrder(((T*)(pixels += sizeof(T)))[-1]);
        }

        __forceinline unsigned short ReadShort(const unsigned char * & pixels)
        {
            return ReadValue<unsigned short>(pixels);
        }

        __forceinline unsigned long ReadLong(const unsigned char * & pixels)
        {
            return ReadValue<unsigned long>(pixels);
        }

        __forceinline unsigned char ScaleShortToChar(unsigned short value)
        {
            return (unsigned char)((value + 128)/257);
        }

        __forceinline void UpdatePixelsAndCount(size_t depth, unsigned char compactPixel, unsigned char *& pixels, size_t &count)
        {
            switch (depth)
            {
            case 1:
                *pixels ++= (compactPixel >> 7) & 0x01 ? 0U : 255U;
                *pixels ++= (compactPixel >> 6) & 0x01 ? 0U : 255U;
                *pixels ++= (compactPixel >> 5) & 0x01 ? 0U : 255U;
                *pixels ++= (compactPixel >> 4) & 0x01 ? 0U : 255U;
                *pixels ++= (compactPixel >> 3) & 0x01 ? 0U : 255U;
                *pixels ++= (compactPixel >> 2) & 0x01 ? 0U : 255U;
                *pixels ++= (compactPixel >> 1) & 0x01 ? 0U : 255U;
                *pixels ++= (compactPixel >> 0) & 0x01 ? 0U : 255U;
                count += 8;
                break;
            case 2:
                *pixels ++= (compactPixel >> 6) & 0x03;
                *pixels ++= (compactPixel >> 4) & 0x03;
                *pixels ++= (compactPixel >> 2) & 0x03;
                *pixels ++= (compactPixel & 0x03) & 0x03;
                count += 4;
                break;
            case 4:
                *pixels ++= (compactPixel >> 4) & 0xff;
                *pixels ++= (compactPixel & 0x0f) & 0xff;
                count += 2;
                break;
            default:
                *pixels ++= compactPixel;
                count++;
                break;
            }
        }

        size_t DecodePixels(const unsigned char *compactPixels, size_t compactPixelsCount, size_t depth, unsigned char *pixels, size_t pixelsCount)
        {
            size_t packets = compactPixelsCount;
            size_t count = 0;
            for (; packets > 1 && count < pixelsCount;)
            {
                size_t length = (*compactPixels++);
                packets--;
                if (length == 128)
                    continue;
                if (length > 128)
                {
                    length = 256 - length + 1;
                    unsigned char compactPixel = (*compactPixels++);
                    packets--;
                    for (size_t j = 0; j < length; j++)
                        UpdatePixelsAndCount(depth, compactPixel, pixels, count);
                    continue;
                }
                length++;
                for (size_t j = 0; j < length; j++)
                {
                    UpdatePixelsAndCount(depth, *compactPixels, pixels, count);
                    compactPixels++;
                }
            }
            return count;
        }

        struct TInfo
        {
            unsigned short channels; 
            unsigned short version;
            size_t height;             
            size_t width;
            unsigned short depth;
            TMode mode;
            TColorspace colorspace;
            TCompression compression;
        };

        struct TPixel
        {
            unsigned char blue;
            unsigned char green;
            unsigned char red;
            unsigned char alpha;

            void Negate()
            {
                blue = 255 - blue;
                green = 255 - green;
                red = 255 - red;
            }
        };

        struct TImage
        {
            TInfo info;
            std::vector<TPixel> data;

            bool Load(IStream* pStream);

        private:
            size_t ReadOffset(IStream* pStream);
            size_t ReadSize(IStream* pStream);

            bool LoadInfo(IStream* pStream);
            bool LoadData(size_t type, const size_t *offsets, IStream *pStream);
        };

        __forceinline size_t TImage::ReadOffset(IStream* pStream)
        {
            if (info.version == 1)
                return ReadBigEndianShort(pStream);
            return ReadBigEndianLong(pStream);
        }

        __forceinline size_t TImage::ReadSize(IStream* pStream)
        {
            if (info.version == 1)
                return ReadBigEndianLong(pStream);
            return (size_t)ReadBigEndianLongLong(pStream);
        }

        bool TImage::LoadInfo(IStream* pStream)
        {
            unsigned char signature[4];
            ReadBuffer(pStream, signature, 4);
            info.version = ReadBigEndianShort(pStream);
            if (memcmp(signature,"8BPS", 4) != 0 || ((info.version != 1) && (info.version != 2)))
                return false;
            ReadStubBuffer(pStream, 6);
            info.channels = ReadBigEndianShort(pStream);
            if (info.channels > CHANNEL_COUNT_MAX)
                return false;
            info.height = ReadBigEndianLong(pStream);
            info.width = ReadBigEndianLong(pStream);
            if ((info.version == 1) && ((info.height > 30000) || (info.width > 30000)))
                return false;
            info.depth = ReadBigEndianShort(pStream);
            if ((info.depth != 1) && (info.depth != 8) && (info.depth != 16))
                return false;
            info.mode = (TMode)ReadBigEndianShort(pStream);
            switch(info.mode)
            {
            case LabMode:
                info.colorspace = LabColorspace;
                break;
            case CMYKMode:
                info.colorspace = CMYKColorspace;
                break;
            case RGBMode:
                info.colorspace = RGBColorspace;
                break;
            case BitmapMode:
            case GrayscaleMode:
            case DuotoneMode:
                info.colorspace = GRAYColorspace;
                break;
            default:
                info.colorspace = UndefinedColorspace;
                break;
            }
            info.compression = NoCompression;
            return true;
        }

        bool TImage::LoadData(size_t type, const size_t *offsets, IStream *pStream)
        {
            AD_FUNCTION_PERFORMANCE_TEST
            size_t packet_size = 1;
            if (info.depth > 8)
                packet_size++;
            std::vector<unsigned char> pixels((info.width + 256)*packet_size, 0);

            std::vector<unsigned char> compact_pixels;
            if (info.compression == RLECompression)
            {
                size_t length = 0;
                for (size_t y = 0; y < info.height; y++)
                {
                    if(length < offsets[y])
                        length = offsets[y];
                }
                compact_pixels.resize(length, 0);
            }
            for(size_t y = 0; y < info.height; y++)
            {        
                ULONG count = 0;  
                if (info.depth == 1)
                {

                    size_t length = (info.width + 7)/8;
                    if (info.compression != RLECompression)
                        pStream->Read(&pixels[0], (ULONG)length, &count);
                    else
                    {
                        pStream->Read(&compact_pixels[0], (ULONG)offsets[y], &count);
                        if ((size_t)count != offsets[y])
                            break;
                        count = (ULONG)DecodePixels(&compact_pixels[0], offsets[y], (size_t)123456, &pixels[0], length);
                    }
                    if ((size_t)count < length)
                        break;
                }
                else
                {
                    size_t length = packet_size*info.width;
                    if (info.compression != RLECompression)
                        pStream->Read(&pixels[0], (ULONG)length, &count);
                    else
                    {
                        pStream->Read(&compact_pixels[0], (ULONG)offsets[y], &count);
                        if ((size_t)count != offsets[y])
                            break;
                        count = (ULONG)DecodePixels(&compact_pixels[0], offsets[y], info.depth, &pixels[0], length);
                    }
                    if ((size_t)count < length)
                        break;
                }

                TPixel *q = &data[y*info.width];
                const unsigned char *p = &pixels[0];
                for (size_t x = 0; x < info.width; x++)
                {
                    unsigned char pixel;
                    if (packet_size == 1)
                        pixel = *p++;
                    else
                        pixel = ScaleShortToChar(ReadShort(p));
                    switch (type)
                    {
                    case 0:
                        q->red = pixel;
                        if (info.channels == 1)
                        {
                            q->green = q->red;
                            q->blue = q->red;
                            q->alpha = CHANNEL_MAX;
                        }
                        break;
                    case 1:
                        q->green = pixel;
                        break;
                    case 2:
                        q->blue = pixel;
                        if (info.channels == 3)
                            q->alpha = CHANNEL_MAX;
                        break;
                    case 3:
                        q->alpha = pixel;
                        break;
                    case 4:
                        q->alpha = pixel;
                        break;
                    default:
                        break;
                    }
                    q++;
                }
            }
            return true;
        }

        bool TImage::Load(IStream* pStream)
        {
            AD_FUNCTION_PERFORMANCE_TEST
            if(!LoadInfo(pStream))
                return false;
            size_t length = ReadBigEndianLong(pStream);
            if (length)
                ReadStubBuffer(pStream, length);
            length = ReadBigEndianLong(pStream);
            if (length)
                ReadStubBuffer(pStream, length);
            length = ReadSize(pStream);
            if(length == 8)
            {
                length = ReadBigEndianLong(pStream);
                length = ReadBigEndianLong(pStream);
            }
            if(length)
                ReadStubBuffer(pStream, length);
            std::vector<size_t> offsets(1);
            unsigned short compression = ReadBigEndianShort(pStream);
            if (compression == 1)
            {
                info.compression = RLECompression;
                offsets.resize(info.height*info.channels);
                for (size_t i = 0; i < (size_t)(info.height*info.channels); i++)
                    offsets[i] = ReadOffset(pStream);
            }
            data.resize(info.width*info.height);
            AD_PERFORMANCE_TEST_SET_SIZE(info.width*info.height)
            for (size_t i = 0; i < (size_t)info.channels; i++)
            {
                if(!LoadData(i, &offsets[0] + i*info.height, pStream))
                    return false;
            }
            if (info.colorspace == CMYKColorspace)
            {
                for(size_t i = 0; i < data.size(); ++i)
                    data[i].Negate();
            }
            return true;
        }
    }

    TPsd* TPsd::Load(HGLOBAL hGlobal)
    {
        AD_FUNCTION_PERFORMANCE_TEST
        TPsd* pPsd = NULL;
        if(hGlobal)
        {
            IStream* pStream = NULL;
            if(::CreateStreamOnHGlobal(hGlobal, FALSE, &pStream) == S_OK)
            {
                Psd::TImage image;
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
                            memcpy(pView->data + row*pView->stride, &image.data[0] + row*width, 4*width);
                        pPsd = new TPsd();
                        pPsd->m_pView = pView;
                        pPsd->m_format = TImage::Psd;
                        AD_PERFORMANCE_TEST_SET_SIZE(height*width)
                    }
                }
                pStream->Release();
            }
        }
        return pPsd;
    }

    bool TPsd::Supported(HGLOBAL hGlobal)
    {
        if(hGlobal)
        {
            unsigned char *data = (unsigned char*)::GlobalLock(hGlobal);
            size_t size = ::GlobalSize(hGlobal);
            bool supported = (size >= 4 && memcmp(data, "8BPS", 4) == 0);
            ::GlobalUnlock(hGlobal);
            return supported;
        }
        return false;
    }
}
