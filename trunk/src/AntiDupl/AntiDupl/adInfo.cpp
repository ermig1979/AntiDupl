/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2013 Yermalayeu Ihar.
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
#define OPJ_STATIC
#include "LibOpenJpeg/openjpeg.h"

#include "adExternal.h"
#include "adInfo.h"

namespace ad
{
	static void ParseVersion(const char * string, adVersionPtr pVersion)
	{
		std::string buffer(string);
		for(size_t i = 0; i < buffer.size(); ++i)
		{
			if(buffer[i] == '.')
				buffer[i] = ' ';
		}
		std::stringstream stream(buffer);
		stream >> pVersion->major;
		stream >> pVersion->minor;
		stream >> pVersion->release;
	}

	bool GetVersion(adVersionType versionType, adVersionPtr pVersion)
	{
		switch(versionType)
		{
		case AD_VERSION_TYPE_ANTIDUPL:
			ParseVersion(ad::VERSION, pVersion);
			pVersion->revision = ad::REVISION;
			break;
		case AD_VERSION_TYPE_SIMD:
			pVersion->major = AD_UNDEFINED;
			pVersion->minor = AD_UNDEFINED;
			pVersion->release = AD_UNDEFINED;
			pVersion->revision = Simd::VERSION;
			break;
		case AD_VERSION_TYPE_OPENJPEG:
			ParseVersion(opj_version(), pVersion);
			pVersion->revision = AD_UNDEFINED;
			break;
		default:
			return false;
		}
		return true;
	}
}

