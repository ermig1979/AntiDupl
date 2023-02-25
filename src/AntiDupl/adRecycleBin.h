/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar.
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
#ifndef __adRecycleBin_h__
#define __adRecycleBin_h__

#include "adStrings.h"

namespace ad
{
    struct TOptions;
    struct TImageInfo;

    class TStatus;
    class TEngine;

    //-------------------------------------------------------------------------
    class TRecycleBin
    {
        struct TRecord
        {
            TString oldPath;
            TString newPath;
            DWORD attribute;
        };

        typedef std::map<TImageInfo*, TRecord*> TMap;
        typedef std::list<TRecord*> TList;

        TOptions *m_pOptions;
        TStatus *m_pStatus;
        TMap m_map;
        TList m_temporary;        
        int m_counter;
    
    public:
        TRecycleBin(TEngine *pEngine);
        ~TRecycleBin();

        bool Delete(TImageInfo *pImageInfo);
        bool Restore(TImageInfo *pImageInfo);
        bool Free(TImageInfo *pImageInfo);
        void FreeAll();
        bool ClearTemporaryFiles(bool permanent);

    private:
        TString GetUniquePath(const TPath &path);
        bool ClearTemporaryFile(TRecord *pRecord, bool permanent);
    };
}

#endif/*__adRecycleBin_h__*/