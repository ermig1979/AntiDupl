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
#ifndef __adPerformance_h__
#define __adPerformance_h__

#include "adThreads.h"
#include "adStrings.h"

namespace ad
{
	double Time();

	//-------------------------------------------------------------------------

    class TPerformanceMeasurer
    {
        TString	m_decription;
        double m_start;
        int m_count;
        double m_total;
        double m_min;
        double m_max;
        bool m_entered;

        long long m_size;

    public:
        TPerformanceMeasurer(const TString& decription = TEXT("Unnamed"));

        void Enter();
        void Leave(size_t size = 1);

        double Average() const;
        TString Statistic() const;

        TString Description() const { return m_decription; }

        void Combine(const TPerformanceMeasurer & other);
    };

    //-------------------------------------------------------------------------

    class TScopedPerformanceMeasurer
    {
        TPerformanceMeasurer * m_pPm;
        size_t m_size;
    public:

        TScopedPerformanceMeasurer(TPerformanceMeasurer & pm): m_pPm(&pm), m_size(1)
        {
            if (m_pPm)
                m_pPm->Enter();
        }

        TScopedPerformanceMeasurer(TPerformanceMeasurer * pPm): m_pPm(pPm), m_size(1)
        {
            if (m_pPm)
                m_pPm->Enter();
        }

        ~TScopedPerformanceMeasurer()
        {
            if (m_pPm)
                m_pPm->Leave(m_size);
        }

        void SetSize(size_t size) { m_size = size; }
    };

    //-------------------------------------------------------------------------

    class TPerformanceMeasurerStorage
    {
        TCriticalSection *m_pCS;

        typedef std::map<std::string, TPerformanceMeasurer*> TPmMap;
        typedef std::map<unsigned int, TPmMap*> TThreadMap;
        TThreadMap *m_pMap;

        static void Free(TPmMap *pPmMap);
    public:
        TPerformanceMeasurerStorage();
        ~TPerformanceMeasurerStorage();

        TPerformanceMeasurer* Get(const std::string & name);

        TString Statistic() const;

        static TPerformanceMeasurerStorage s_storage;
    };
}

#ifdef AD_PERFORMANCE_TEST_ENABLE
#define AD_PERFORMANCE_TEST(decription) ad::TScopedPerformanceMeasurer ___spm(*(ad::TPerformanceMeasurerStorage::s_storage.Get(decription)));
#define AD_FUNCTION_PERFORMANCE_TEST AD_PERFORMANCE_TEST(__FUNCTION__)
#define AD_PERFORMANCE_TEST_SET_SIZE(size) ___spm.SetSize(size);
#else//AD_PERFORMANCE_TEST_ENABLE
#define AD_PERFORMANCE_TEST(decription)
#define AD_FUNCTION_PERFORMANCE_TEST
#define AD_PERFORMANCE_TEST_SET_SIZE(size)
#endif//AD_PERFORMANCE_TEST_ENABLE

#endif//__adPerformance_h__