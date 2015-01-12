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
#ifndef __adLogger_h__
#define __adLogger_h__

#include "adThreads.h"

namespace ad
{
    class TLogger
    {
        class TOut;
        class TStdOut;
        class TFileOut;

        TOut *m_pOut;
        TCriticalSection *m_pCS;
        bool m_threadIdAnnotation;

    public:
        TLogger();
        ~TLogger();

        void ResetOut();
        void SetStdOut();
        void SetFileOut(const TChar *name, bool append);

        void SetThreadIdAnnotation(bool enable);

        void Write(const TChar* message);

        void Flush();

        static TLogger s_logger;

	private:
		static std::string CurrentDateTimeString();
	};
}

#ifdef AD_LOGGER_ENABLE
#define AD_LOG(message) \
{ \
	ad::TStringStream ___ss; \
	___ss << message; \
	ad::TLogger::s_logger.Write(___ss.str().c_str()); \
}
#else//AD_LOGGER_ENABLE
#define AD_LOG(message)
#endif//AD_LOGGER_ENABLE

#endif//__adLogger_h__