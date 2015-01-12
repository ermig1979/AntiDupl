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
#ifndef __adMistakeStorage_h__
#define __adMistakeStorage_h__

#include <set>

#include "adConfig.h"

namespace ad
{
    class TEngine;
    class TStatus;

    struct TImageInfo;

    typedef TImageInfo* TImageInfoPtr;
    
    //------------------------------------------------------------------------
    class TMistakeStorage
    {
    public:
        TMistakeStorage(TEngine *pEngine);
        ~TMistakeStorage() {Clear();}

        adError Save(const TChar *fileName);
        adError Load(const TChar *fileName, bool check);

        void Add(const TImageInfoPtr first, const TImageInfoPtr second);
        void Add(const TImageInfoPtr single);

        void Erase(const TImageInfoPtr first, const TImageInfoPtr second);
        void Erase(const TImageInfoPtr single);

        bool IsHas(const TImageInfoPtr first, const TImageInfoPtr second) const;
        bool IsHas(const TImageInfoPtr single) const;

        void Clear();
        
        void Rename(const TImageInfoPtr pImageInfo, const TString& newPath);

    private:
        struct TImageInfoPtrLesser
        {
            bool operator () (TImageInfoPtr first, TImageInfoPtr second) const;
        };
        typedef std::multiset<TImageInfoPtr, TImageInfoPtrLesser> TImageInfoPtrMultiSet;
        TImageInfoPtrMultiSet m_singles;

        typedef std::pair<TImageInfoPtr, TImageInfoPtr> TImageInfoPtrPair;
        struct TImageInfoPtrPairLesser
        {
            bool operator () (const TImageInfoPtrPair& first, const TImageInfoPtrPair& second) const;
        };
        typedef std::multiset<TImageInfoPtrPair, TImageInfoPtrPairLesser> TImageInfoPtrPairMultiSet;
        TImageInfoPtrPairMultiSet m_pairs;
        
        TImageInfoPtrPairMultiSet::const_iterator Find(const TImageInfoPtr first, const TImageInfoPtr second) const;
        TImageInfoPtrPairMultiSet::iterator Find(const TImageInfoPtr first, const TImageInfoPtr second);
        TImageInfoPtrMultiSet::const_iterator Find(const TImageInfoPtr single) const;
        TImageInfoPtrMultiSet::iterator Find(const TImageInfoPtr single);
        
        TStatus *m_pStatus;
    };
}
#endif //__adMistakeStorage_h__
