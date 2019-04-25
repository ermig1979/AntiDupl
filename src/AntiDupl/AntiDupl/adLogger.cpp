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
#include <time.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "adStrings.h"
#include "adIO.h"
#include "adPerformance.h"
#include "adLogger.h"

namespace ad
{
    class TLogger::TOut
    {
    public:
        virtual void Write(const TChar* message) = 0;

        virtual void Flush() = 0;
    };

    //-------------------------------------------------------------------------

    class TLogger::TStdOut : public TLogger::TOut
    {
    public:
        virtual void Write(const TChar* message)
        {
            std::cout << message;
        }

        virtual void Flush()
        {
            std::cout.flush();
        }
    };

    //-------------------------------------------------------------------------

    class TLogger::TFileOut : public TLogger::TOut
    {
        std::ofstream m_ofstream; 
    public:
        TFileOut(const TChar *name, bool append)
            :m_ofstream(name, (append ? std::ios_base::app : std::ios_base::out))
        {
            if(append)
            {
                m_ofstream << std::endl << "Session started at: " << CurrentDateTimeString() << std::endl << std::endl;
            }
        }

        ~TFileOut()
        {
            if(m_ofstream.is_open())
            {
                m_ofstream.close();
            }
        }

        virtual void Write(const TChar* message)
        {
            if(m_ofstream.is_open())
            {
                m_ofstream << TString(message).ToString();
            }
        }

        virtual void Flush()
        {
            if(m_ofstream.is_open())
            {
                m_ofstream.flush();
            }
        }
    };

    //-------------------------------------------------------------------------

    TLogger::TLogger()
        :m_pOut(NULL),
        m_threadIdAnnotation(true),
        m_pCS(NULL)
    {
        m_pCS = new TCriticalSection();
    }

    TLogger::~TLogger()
    {
        ResetOut();
        delete m_pCS;
    }

    void TLogger::ResetOut()
    {
        TCriticalSection::TLocker locker(m_pCS);
        if(m_pOut)
        {
            m_pOut->Flush();
            delete m_pOut;
            m_pOut = NULL;
        }
    }

    void TLogger::SetStdOut()
    {
        ResetOut();
        TCriticalSection::TLocker locker(m_pCS);
        m_pOut = new TStdOut();
    }

    void TLogger::SetFileOut(const TChar *name, bool append)
    {
        ResetOut();
        TCriticalSection::TLocker locker(m_pCS);
        m_pOut = new TFileOut(name, append);
    }

    void TLogger::SetThreadIdAnnotation(bool enable)
    {
        TCriticalSection::TLocker locker(m_pCS);
        m_threadIdAnnotation = enable;
    }

    void TLogger::Write(const TChar* message)
    {
        TCriticalSection::TLocker locker(m_pCS);
        if(m_pOut)
        {
            TStringStream ss;

            if(m_threadIdAnnotation)
            {
                ss << TEXT("[") << TThread::CurrentId() << TEXT("] ");
            }

            ss << message;

            m_pOut->Write((ss.str() + TEXT("\n")).c_str());
        }
    }

    TLogger TLogger::s_logger = TLogger();

	std::string TLogger::CurrentDateTimeString()
	{
		std::stringstream ss;
		time_t t_;
		if(time(&t_))
		{
			tm t;
			if(localtime_s(&t, &t_) == 0)
			{
				ss << 1900 + t.tm_year << ".";
				ss << (t.tm_mon < 9 ? "0" : "") << t.tm_mon + 1 << "."; 
				ss << (t.tm_mday < 10 ? "0" : "") << t.tm_mday << " ";
				ss << (t.tm_hour < 10 ? "0" : "") << t.tm_hour << ":"; 
				ss << (t.tm_min < 10 ? "0" : "") << t.tm_min << ":";
				ss << (t.tm_sec < 10 ? "0" : "") << t.tm_sec; 
			}
		}
		return ss.str();
	}
}