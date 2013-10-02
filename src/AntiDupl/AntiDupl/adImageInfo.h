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
#ifndef __adImageInfo_h__
#define __adImageInfo_h__

#include "adConfig.h"
#include "adFileInfo.h"

namespace ad
{
    struct TImageInfo : public TFileInfo
    {
        TImageType type;
        TUInt32 width;
        TUInt32 height;
		double blockiness;
		double blurring;

        size_t index;

		size_t group;
        int links;
        bool removed;
		bool selected;
        
		TImageInfo() {Init();}
        TImageInfo(const TString& path_) : TFileInfo(path_) {Init();}
        TImageInfo(const TString& path_, TUInt64 size_, TUInt64 time_) : TFileInfo(path_, size_, time_) {Init();}
        TImageInfo(const TFileInfo& fileInfo) : TFileInfo(fileInfo) {Init();}
        TImageInfo(const TImageInfo& imageInfo) {Init(); *this = imageInfo;};
        virtual ~TImageInfo();

        TImageInfo& operator = (const TImageInfo& imageInfo);

        bool Export(adImageInfoPtrA pImageInfo) const;
        bool Export(adImageInfoPtrW pImageInfo) const;

		TUInt32 Area() const {return width*height;}
private:
        void Init();
    };

    typedef TImageInfo* TImageInfoPtr;
}

#endif/*__adImageInfo_h__*/