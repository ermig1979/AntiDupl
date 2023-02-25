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
#ifndef __adEngine_h__
#define __adEngine_h__

#include "adStrings.h"

namespace ad
{
    struct TOptions;
    struct TImageData;
    typedef TImageData* TImageDataPtr;
    typedef std::list<TImageDataPtr> TImageDataPtrs;

	class TInit;
    class TStatus;
    class TResultStorage;
    class TImageDataStorage;
    class TMistakeStorage;
    class TImageDataPtrList;
    class TCompareManager;
    class TCollectManager;
    class TSearcher;
    class TRecycleBin;
	class TCriticalSection;

    typedef TImageData *TImageDataPtr; 
    //-------------------------------------------------------------------------
    class TEngine
    {
        friend class TImageComparer;
    public:
        TEngine(const TString & userPath); 
        ~TEngine();

        void Search();

        const TString & UserPath() const { return _userPath; }
        TStatus* Status() {return m_pStatus;}
        TOptions* Options() {return m_pOptions;};
        TImageDataStorage* ImageDataStorage() {return m_pImageDataStorage;}
        TMistakeStorage* MistakeStorage() {return m_pMistakeStorage;}
        TResultStorage* Result() {return m_pResult;}
        TCriticalSection* CriticalSection() {return m_pCriticalSection;}
        TRecycleBin* RecycleBin() {return m_pRecycleBin;}

    private:
        TString _userPath;
        TImageDataPtrs *m_pImageDataPtrs;
        TCompareManager *m_pCompareManager;
        TCollectManager *m_pCollectManager;
        TStatus *m_pStatus;
        TOptions *m_pOptions;
        TResultStorage *m_pResult;
        TImageDataStorage *m_pImageDataStorage;
        TMistakeStorage *m_pMistakeStorage;
        TCriticalSection *m_pCriticalSection;
        TInit *m_pInit;
        TSearcher *m_pSearcher;
        TRecycleBin *m_pRecycleBin;
    };
    //-------------------------------------------------------------------------
}

#endif//__adSearch_h__ 