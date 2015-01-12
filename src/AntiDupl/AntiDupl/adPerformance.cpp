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
#include <limits>
#include <iomanip>

#include "adPerformance.h"

namespace ad
{
	static double GetFrequency()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return double(frequency.QuadPart);
	}

	double g_frequency = GetFrequency();

	double Time()
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		return double(counter.QuadPart)/g_frequency;
	}

	//-------------------------------------------------------------------------

    TPerformanceMeasurer::TPerformanceMeasurer(const TString& decription)
        :m_decription(decription),
        m_count(0),
        m_total(0),
        m_entered(false),
        m_min(std::numeric_limits<double>::max()),
        m_max(std::numeric_limits<double>::min()),
        m_size(0)
    {
    }

    void TPerformanceMeasurer::Enter()
    {
        if (!m_entered)
        {
            m_entered = true;
            m_start = Time();
        }
    }

    void TPerformanceMeasurer::Leave(size_t size)
    {
        if (m_entered)
        {
            m_entered = false;
            double difference = double(Time() - m_start);
            m_total += difference;
            m_min = std::min(m_min, difference);
            m_max = std::max(m_max, difference);
           ++m_count;
           m_size += std::max<size_t>(1, size);
        }		
    }

    double TPerformanceMeasurer::Average() const
    {
        return m_count ? (m_total / m_count) : 0;
    }

    TString TPerformanceMeasurer::Statistic() const
    {
        TStringStream ss;
        ss << m_decription << TEXT(": ");
        ss << std::setprecision(0) << std::fixed << m_total*1000 << TEXT(" ms");
        ss << TEXT(" / ") << m_count << TEXT(" = ");
        ss << std::setprecision(2) << std::fixed << Average()*1000.0 << TEXT(" ms");
        ss << std::setprecision(2) << TEXT(" {min=") << m_min*1000.0 << TEXT("; max=") << m_max*1000.0 << TEXT("}");
        if(m_size > (long long)m_count)
        {
            double size = double(m_size);
            ss << std::setprecision(3) << TEXT(" [<s>=") << size/m_count*0.001 << TEXT(" kb; <t>=") << m_total/size*1000000000 << TEXT(" ns]");
        }
        return ss.str();
    }

    void TPerformanceMeasurer::Combine(const TPerformanceMeasurer & other)
    {
        m_count += other.m_count;
        m_total += other.m_total;
        m_min = std::min(m_min, other.m_min);
        m_max = std::max(m_max, other.m_max);
        m_size += other.m_size;
    }

    //-------------------------------------------------------------------------


    TPerformanceMeasurerStorage::TPerformanceMeasurerStorage()
        :m_pCS(NULL), 
        m_pMap(NULL)
    {
        m_pCS = new TCriticalSection();
        m_pMap = new TThreadMap();
    }

    TPerformanceMeasurerStorage::~TPerformanceMeasurerStorage()
    {
        for(TThreadMap::iterator threadIt = m_pMap->begin(); threadIt != m_pMap->end(); ++threadIt)
            Free(threadIt->second);
        delete m_pMap;
        delete m_pCS;
    }

    void TPerformanceMeasurerStorage::Free(TPmMap *pPmMap)
    {
        for(TPmMap::iterator pmIt = pPmMap->begin(); pmIt != pPmMap->end(); ++pmIt)
            delete pmIt->second;
        delete pPmMap;
    }

    TPerformanceMeasurer* TPerformanceMeasurerStorage::Get(const std::string & name)
    {
        TCriticalSection::TLocker locker(m_pCS);

        unsigned int threadId = TThread::CurrentId();
        TPmMap *pPmMap = NULL;
        TThreadMap::iterator threadIt = m_pMap->find(threadId);
        if(threadIt == m_pMap->end())
        {
            pPmMap = new TPmMap();
            (*m_pMap)[threadId] = pPmMap;
        }
        else
            pPmMap = threadIt->second;

        TPerformanceMeasurer *pPm = NULL;
        TPmMap::iterator pmIt = pPmMap->find(name);
        if(pmIt == pPmMap->end())
        {
            pPm = new TPerformanceMeasurer(name);
            (*pPmMap)[name] = pPm;
       }
        else
            pPm = pmIt->second;
        return pPm;
    }

    TString TPerformanceMeasurerStorage::Statistic() const
    {
        TCriticalSection::TLocker locker(m_pCS);
        TPmMap *pTotal = new TPmMap();
        for(TThreadMap::iterator threadIt = m_pMap->begin(); threadIt != m_pMap->end(); ++threadIt)
        {
            TPmMap *pPmMap = threadIt->second;
            for(TPmMap::iterator pmIt = pPmMap->begin(); pmIt != pPmMap->end(); ++pmIt)
            {
                TPerformanceMeasurer *pPm = NULL;
                TPmMap::iterator totalIt = pTotal->find(pmIt->first);
                if(totalIt == pTotal->end())
                {
                    pPm = new TPerformanceMeasurer(pmIt->second->Description().c_str());
                    (*pTotal)[pmIt->first] = pPm;
                }
                else
                    pPm = totalIt->second;

                pPm->Combine(*pmIt->second);
            }
        }

        std::vector<TString> statistics;
        for(TPmMap::iterator totalIt = pTotal->begin(); totalIt != pTotal->end(); ++totalIt)
            statistics.push_back(totalIt->second->Statistic());
        Free(pTotal);

        std::sort(statistics.begin(), statistics.end());

        TStringStream statistic;
        for(size_t i = 0;  i < statistics.size(); ++i)
            statistic << statistics[i] << std::endl;
        return statistic.str();
    }

    TPerformanceMeasurerStorage TPerformanceMeasurerStorage::s_storage = TPerformanceMeasurerStorage();
}