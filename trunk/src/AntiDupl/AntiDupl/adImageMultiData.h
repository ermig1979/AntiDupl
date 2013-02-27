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
#ifndef __adImageMultiData_h__
#define __adImageMultiData_h__

#include <list>

#include "adImageData.h"

namespace ad
{
    struct TImageMultiData : public TImageData
    {
        TImageMultiData(const TString& path_ = TString()) : TImageData(path_) {}
        TImageMultiData(const TString& path_, TUInt64 size_, TUInt64 time_) : TImageData(path_, size_, time_) {}
        TImageMultiData(const TFileInfo &fileInfo) : TImageData(fileInfo) {}
        virtual ~TImageMultiData() {Clear();}

        bool SetData(size_t reducedImageSize);

        bool Load(HANDLE hIn);
        bool Save(HANDLE hOut) const;

        size_t FilledDataSize() const;

    private:
        typedef std::list<TPixelDataPtr> TPixelDataPtrs;

        TPixelDataPtr GetData(size_t reducedImageSize);
        void Clear();

        TPixelDataPtrs m_data;
    };

    typedef TImageMultiData* TImageMultiDataPtr;
}
#endif //__adImageMultiData_h__
