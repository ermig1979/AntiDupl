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
#ifndef __adDataCollector_h__
#define __adDataCollector_h__

#include <list>

#include "adConfig.h"

namespace ad
{
    struct TImageData;
    struct TOptions;
    struct TPixelData; 
    class TEngine;
    class TResultStorage;
    //-------------------------------------------------------------------------
    class TDataCollector
    {
        TOptions *m_pOptions;
        TResultStorage *m_pResult;
        std::vector<TView*> m_pGrayBuffers;

    public:
        TDataCollector(TEngine *pEngine);
        ~TDataCollector();

        void Fill(TImageData* pImageData);

    private:
        void FillPixelData(TImageData* pImageData);
        void CheckOnDefect(TImageData* pImageData);
        void SetCrc32c(TImageData* pImageData);
		double GetBlockiness(const TView & gray);
		double GetBlockiness(const std::vector<unsigned int> & sums);
	};
}
#endif//__adDataCollector_h__ 