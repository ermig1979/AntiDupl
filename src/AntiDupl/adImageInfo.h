/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2023 Yermalayeu Ihar,
*               2013-2018 Borisov Dmitry.
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
#include "adPath.h"
#include "adImageExif.h"

namespace ad
{
    struct TImageInfo
    {
        TPath path;
        TUInt64 size;
        TUInt64 time;
        TUInt32 hash;
        TImageType type;
        TUInt32 width;
        TUInt32 height;
		double blockiness;
		double blurring;
		TImageExif imageExif;

        size_t index;

		size_t group;
        int links;
        bool removed;
		bool selected;
        
        TImageInfo();
        TImageInfo(const TString& path_);
        TImageInfo(const TString& path_, TUInt64 size_, TUInt64 time_);
        TImageInfo(const TImageInfo& imageInfo);
        virtual ~TImageInfo();

        TImageInfo& operator = (const TImageInfo& imageInfo);

        bool operator==(const TImageInfo& imageInfo) const;
        inline bool operator!=(const TImageInfo& imageInfo) const { return !(*this == imageInfo); }
        inline bool operator>(const TImageInfo& imageInfo) const { return TPath::BiggerByPath(path, imageInfo.path); }
        inline bool operator<(const TImageInfo& imageInfo) const { return TPath::LesserByPath(path, imageInfo.path); }

        bool Actual(bool update = false);

        void Rename(const TString& newPath);

        bool Export(adImageInfoPtrA pImageInfo) const;
        bool Export(adImageInfoPtrW pImageInfo) const;

		TUInt32 Area() const {return width*height;}
private:
        int _actual;

        void Init();
    };

    typedef TImageInfo* TImageInfoPtr;
}

#endif/*__adImageInfo_h__*/