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
#ifndef __adImageInfoStorage_h__
#define __adImageInfoStorage_h__

#include <vector>
#include <map>

#include "adImageInfo.h"

namespace ad
{
    class TEngine;
    class TStatus;

	class TInputFileStream;
	class TOutputFileStream;

    typedef std::list<TImageInfoPtr> TImageInfoPtrList;
    //-------------------------------------------------------------------------
    class TImageInfoStorage
    {
    public:
        TImageInfoStorage(TEngine *pEngine);
        ~TImageInfoStorage() {Clear();}

        TImageInfoPtr Add(const TImageInfoPtr pImageInfo);

        TImageInfoPtr Get(size_t index) const;
        TImageInfoPtr GetStub() {return &m_stub;};

		void Load(const TInputFileStream & inputFile, bool check);
		void Save(const TOutputFileStream & outputFile) const;

        void Clear();

        void RemoveUnlinked();

    private:
        typedef std::map<TImageInfoPtr, TImageInfoPtr> TAddMap;
        typedef std::vector<TImageInfoPtr> TLoadVector;
        typedef TImageInfoPtrList TMainList;

         TAddMap m_addMap;
         TLoadVector m_loadVector;
         TMainList m_mainList;
         TImageInfo m_stub;
         TStatus *m_pStatus;
    };
    //-------------------------------------------------------------------------
}
#endif //__adImageDataStorage_h__
