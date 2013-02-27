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
#ifndef __adImageDataStorage_h__
#define __adImageDataStorage_h__

#include <map>

#include "adImageMultiData.h"

namespace ad
{
    class TEngine;
    //-------------------------------------------------------------------------
    class TImageDataStorage
    {
    public:
        TImageDataStorage(TEngine *pEngine);
        ~TImageDataStorage() {Clear();}

        TImageDataPtr Get(const TFileInfo& fileInfo);

        adError Load(const TChar *fileName, bool check);
        adError Save(const TChar *fileName) const;

        void Clear();

    private:
        typedef std::multimap<TUInt32, TImageMultiDataPtr> TStorage;
        typedef std::pair<TUInt32, TImageMultiDataPtr> TRecord;

        TStorage::iterator Find(const TFileInfo& fileInfo);
        TStorage::iterator Insert(TImageMultiData* pImageMultiData);

        TStorage m_storage;
        TStatus *m_pStatus;
        TOptions *m_pOptions;
    };
    //-------------------------------------------------------------------------
}
#endif //__adImageDataStorage_h__
