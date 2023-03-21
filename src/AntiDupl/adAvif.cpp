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

#include "adAvif.h"
#include "adPerformance.h"
#include "adLogger.h"

#include <avif/avif.h>

namespace ad
{
	bool TAvif::Supported(HGLOBAL hGlobal)
	{
		if (hGlobal)
		{
			uint8_t* data = (uint8_t*)::GlobalLock(hGlobal);
			size_t data_size = ::GlobalSize(hGlobal);

			const auto avif_data = avifROData{ data, data_size };
			bool filetype_supported = avifPeekCompatibleFileType(&avif_data);

			::GlobalUnlock(hGlobal);

			return filetype_supported;
		}
		return false;
	}

	TAvif* TAvif::Load(HGLOBAL hGlobal)
	{
		return NULL;
	}
}