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
#ifndef __adFileInfo_h__
#define __adFileInfo_h__

#include "adPath.h"

namespace ad
{
    struct TFileInfo
    {
        TPath path;
        TUInt64 size;
        TUInt64 time;
        TUInt32 hash;

        TFileInfo(const TString& path_ = TString());
        TFileInfo(const TString& path_, TUInt64 size_, TUInt64 time_);
        TFileInfo(const TFileInfo& fileInfo) {*this = fileInfo;};
  
        TFileInfo& operator = (const TFileInfo& fileInfo);

        bool operator==(const TFileInfo &fileInfo) const;
        inline bool operator!=(const TFileInfo &fileInfo) const {return !(*this == fileInfo);}
        inline bool operator>(const TFileInfo &fileInfo) const {return TPath::BiggerByPath(path, fileInfo.path);}
        inline bool operator<(const TFileInfo &fileInfo) const {return TPath::LesserByPath(path, fileInfo.path);}

        bool Load(HANDLE hIn);
        bool Save(HANDLE hOut) const;

        bool Actual(bool update = false);

        void Rename(const TString& newPath);

    private:
        int m_actual;
    };
}

#endif/*__adFileInfo_h__*/