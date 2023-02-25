/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2013-2018 Borisov Dmitry.
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
#include "adStatisticsOfDeleting.h"
#include "adResult.h"
#include "adFileUtils.h"

#include <iomanip>

namespace ad
{
	TStatisticsOfDeleting::TStatisticsOfDeleting(const TString & statisticsPath)
		: m_pCriticalSection(NULL)
		, m_ofstream(statisticsPath.ToString().c_str(), std::ios_base::app)
	{
		//TCriticalSection::TLocker locker(m_pCS);
		//m_path = TString(GetApplicationDirectory() + TEXT("\\Statistics.txt"));
		m_pCriticalSection = new TCriticalSection();
		//m_ofstream = new std::ofstream(TString(GetApplicationDirectory() + TEXT("\\AntiDupl.log")).c_str(), std::ios_base::app);
	}

	TStatisticsOfDeleting::~TStatisticsOfDeleting(void)
	{
		if(m_ofstream.is_open())
		{
			m_ofstream.close();
		}
	}

	void TStatisticsOfDeleting::Write(TResult * result)
	{
		if(!result->deleteByHint &&
			result->difference != 0.0 &&
			(result->first->removed && !result->second->removed ||
			 !result->first->removed && result->second->removed))
		{
			TCriticalSection::TLocker locker(m_pCriticalSection);

			/*TStringStream ss;

			ss << std::setprecision(3) << std::fixed << result->difference << TEXT("\t");
			ss << std::setprecision(3) << std::fixed << result->first->Area() << TEXT("\t") 
			<< result->first->size << TEXT("\t") 
				<< result->first->blockiness << TEXT("\t") << result->first->blurring << TEXT("\t");

			ss << result->second->Area() << TEXT("\t") << result->second->size << TEXT("\t") 
				<< result->second->blockiness << TEXT("\t") << result->second->blurring; //<< std::endl;

			if(m_ofstream.is_open())
            {
                m_ofstream << (ss.str() + TEXT("\n")).c_str();
				m_ofstream.flush();
            }*/

			if(m_ofstream.is_open())
            {
				m_ofstream << result->difference << "\t"
					<< result->first->Area() << "\t" << result->first->size << "\t" 
					<< result->first->blockiness << "\t"  << result->first->blurring << "\t" 
					<< result->second->Area() << "\t" << result->second->size << "\t"  
					<< result->second->blockiness << "\t" << result->second->blurring << "\t"
					// если удаляем первую то 0
					<< (result->first->removed ? 0 : 1)
					<< std::endl;

				
				//m_ofstream << "2_" << std::setprecision(3) << std::fixed << result->difference << "\t"
				//	<< result->first->Area() << std::endl;
			}
		}
	}

}