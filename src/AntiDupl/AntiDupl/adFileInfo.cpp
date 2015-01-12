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
#include "adIO.h"
#include "adFileInfo.h"

namespace ad
{
	TFileInfo::TFileInfo()
		: size(0)
		, time(0)
		, hash(0)
		, m_actual(AD_UNDEFINED)
	{
	}

    TFileInfo::TFileInfo(const TString& path_)
        :path(path_),
        size(0),
        time(0),
        hash(path.GetCrc32()),
        m_actual(AD_UNDEFINED)
    {
        WIN32_FILE_ATTRIBUTE_DATA fileAttributeData;
        if(GetFileAttributesEx(path_.c_str(), GetFileExInfoStandard,
            &fileAttributeData))
        {
            size = TUInt64(fileAttributeData.nFileSizeLow) + 
                TUInt64(fileAttributeData.nFileSizeHigh)*0x100000000; 
            time = *(TUInt64*)&fileAttributeData.ftLastWriteTime;
            m_actual = 1;
        }
        else
        {
            m_actual = 0;
        }
    }


    TFileInfo::TFileInfo(const TString& path_, TUInt64 size_, TUInt64 time_)
        :path(path_),
        size(size_),
        time(time_),
        hash(path.GetCrc32()),
        m_actual(AD_UNDEFINED)
    {
    }

    TFileInfo& TFileInfo::operator = (const TFileInfo& fileInfo)
    {
        path = fileInfo.path;
        size = fileInfo.size;
        time = fileInfo.time;
        hash = fileInfo.hash;
        m_actual = fileInfo.m_actual;
        return *this;
    }

    bool TFileInfo::operator==(const TFileInfo &fileInfo) const
    {
        return
            hash == fileInfo.hash &&
            (TPath::EqualByPath(path, fileInfo.path) &&
            size == fileInfo.size &&
            time == fileInfo.time);
    }

    bool TFileInfo::Actual(bool update)
    {
        if(update || m_actual == AD_UNDEFINED)
        {
            m_actual = (*this == TFileInfo(path.Original())) ? 1 : 0;
        }
        return m_actual > 0;
    }

    void TFileInfo::Rename(const TString& newPath)
    {
        path = newPath;
        hash = path.GetCrc32();
    }
}

