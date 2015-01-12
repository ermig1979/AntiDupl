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
#ifndef __adIO_h__
#define __adIO_h__

#include "adConfig.h"

namespace ad
{
	inline bool SaveFormatVersion(HANDLE hOut, const std::string & format, TUInt32 version)
	{
		DWORD byte_was_written;
		BOOL result;

		result = WriteFile(hOut, format.c_str(), (DWORD)format.size(), &byte_was_written, NULL);
		if(result != TRUE || byte_was_written < (DWORD)format.size())
			return false;

		result = WriteFile(hOut, &version, (DWORD)sizeof(version), 
			&byte_was_written, NULL);
		if(result != TRUE || byte_was_written < (DWORD)sizeof(version))
			return false;

		return true;
	}

	inline bool LoadFormatVersion(HANDLE hIn, const std::string & format, TUInt32 version)
	{
		DWORD byte_was_read;
		BOOL result;

		std::string current_format = format;
		result = ReadFile(hIn, (void*)current_format.c_str(), (DWORD)current_format.size(), 
			&byte_was_read, NULL); 
		if(result != TRUE || byte_was_read < (DWORD)current_format.size() || current_format != format)
			return false;

		TUInt32 current_version = version;
		result = ReadFile(hIn, &current_version, sizeof(TUInt32), &byte_was_read, NULL); 
		if(result != TRUE || byte_was_read < sizeof(TUInt32) || current_version != version)
			return false;

		return true;
	}

    inline unsigned short InvertByteOrder(unsigned short value)
    {
        return
            ((value & 0xFF00) >> 8) |
            ((value & 0x00FF) << 8);
    }

    inline unsigned long InvertByteOrder(unsigned long value)
    {
        return
            ((value & 0xFF000000) >> 24) |
            ((value & 0x00FF0000) >> 8) |
            ((value & 0x0000FF00) << 8) |
            ((value & 0x000000FF) << 24);
    }

    inline unsigned long long InvertByteOrder(unsigned long long value)
    {
        return
            ((value & 0xFF00000000000000) >> 56) |
            ((value & 0x00FF000000000000) >> 40) |
            ((value & 0x0000FF0000000000) >> 24) |
            ((value & 0x000000FF00000000) >> 8) |
            ((value & 0x00000000FF000000) << 8) |
            ((value & 0x0000000000FF0000) << 24) |
            ((value & 0x000000000000FF00) << 40) |
            ((value & 0x00000000000000FF) << 56);
    }

    template <class T> 
    inline T ReadValue(IStream *pStream)
    {
        T value;
        ULONG count;
        if(pStream->Read(&value, sizeof(T), &count) == S_OK && count == sizeof(T))
        {
            return value;
        }
        else
        {
            std::stringstream ss;
            ss << "Can't read " << sizeof(T) << " bytes from stream!";
            throw std::exception(ss.str().c_str());
        }
        return (T)0;    
    }

    inline unsigned char ReadByte(IStream *pStream)
    {
        return ReadValue<unsigned char>(pStream);
    }

    inline unsigned short ReadBigEndianShort(IStream *pStream)
    {
        return InvertByteOrder(ReadValue<unsigned short>(pStream));
    }

    inline unsigned long ReadBigEndianLong(IStream *pStream)
    {
        return InvertByteOrder(ReadValue<unsigned long>(pStream));
    }

    inline unsigned long long ReadBigEndianLongLong(IStream *pStream)
    {
        return InvertByteOrder(ReadValue<unsigned long long>(pStream));
    }

	inline unsigned short ReadLittleEndianShort(IStream *pStream)
	{
		return ReadValue<unsigned short>(pStream);
	}

	inline unsigned long ReadLittleEndianLong(IStream *pStream)
	{
		return ReadValue<unsigned long>(pStream);
	}

	inline unsigned long long ReadLittleEndianLongLong(IStream *pStream)
	{
		return ReadValue<unsigned long long>(pStream);
	}

    inline void ReadBuffer(IStream *pStream, unsigned char *buffer, size_t size)
    {
        if(size > 0)
        {
            ULONG count;
            if(pStream->Read(buffer, (ULONG)size, &count) != S_OK || count != (ULONG)size)
            {
                std::stringstream ss;
                ss << "Can't read " << size << " bytes from stream!";
                throw std::exception(ss.str().c_str());
            }
        }
    }

    inline void ReadStubBuffer(IStream *pStream, size_t size)
    {
        if(size > 0)
        {
            LARGE_INTEGER shift;
            shift.QuadPart = size;
            if(pStream->Seek(shift, STREAM_SEEK_CUR, NULL) != S_OK)
                throw std::exception("Can't read stub buffer!");
        }
    }

    inline unsigned long long GetCurrentPosition(IStream *pStream)
    {
        ULARGE_INTEGER position;
        LARGE_INTEGER shift = {0};
        if(pStream->Seek(shift, STREAM_SEEK_CUR, &position) != S_OK)
            throw std::exception("Can't get current stream position!");
        return position.QuadPart;
    }

    inline void SetCurrentPosition(IStream *pStream, long long position)
    {
        LARGE_INTEGER shift;
        shift.QuadPart = position;
        if(pStream->Seek(shift, STREAM_SEEK_CUR, NULL) != S_OK)
            throw std::exception("Can't get current stream position!");
    }
}

#endif//__adIO_h__
