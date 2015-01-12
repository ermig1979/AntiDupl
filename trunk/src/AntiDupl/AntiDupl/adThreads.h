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
#ifndef __adThreads_h__
#define __adThreads_h__

#include "adConfig.h"

namespace ad
{
	class TThread
	{
	public:
		class TStatus
		{
		protected:
			volatile bool m_stopped;
		public:
			TStatus() :m_stopped(false) {};

			virtual void Reset() {m_stopped = false;};
			virtual void Stop() {m_stopped = true;};
			bool Stopped() const {return m_stopped;};
		};

		class TTask
		{
			TStatus *m_pStatus;
		public:
			TTask(TStatus *pStatus) :m_pStatus(pStatus) {};
			virtual ~TTask() {};

			TStatus* Status() const {return m_pStatus;} 
			virtual void Work() = 0;
		};

	private:
		static unsigned __stdcall Task(void *pThis);
		void CreateThread();

		unsigned int m_id;
		HANDLE m_handle;

		enum TType
		{
			TASK,
			FUNCTION,
			METHOD
		} m_type;

		TTask *m_pTask;

		void (*m_pFunction)(void*);
		void *m_pData;

		class TStubObject {};
		TStubObject *m_pObject;
		void (TStubObject::*m_pMethod)();

	public:
		TThread(TTask *pTask);
		TThread(void (*pFunction)(void*), void *pData);

		template <class T>
		TThread(T *pObject, void(T::*pMethod)())
			:m_type(METHOD),
			m_pObject((TStubObject*)pObject),
			m_pMethod(*((void (TStubObject::**)())&pMethod))
		{
			CreateThread();
		}

		~TThread();

		bool Resume();
		bool Suspend();
		bool Terminate(unsigned int exitCode = 1, unsigned int waitTime = 100);

		HANDLE Handle() const {return m_handle;};
		unsigned int Id() const {return m_id;};

		bool Active() const;
		bool SetPriority(int priority);

		static void Exit(unsigned int exitCode);
		static unsigned int CurrentId();
	};
	//-------------------------------------------------------------------------
	class TCriticalSection
	{
		CRITICAL_SECTION m_CS;
	public:
		class TLocker
		{
			TCriticalSection *m_pCS;
		public:
			TLocker(TCriticalSection *pCS) :m_pCS(pCS) {m_pCS->Enter();};
			~TLocker() {m_pCS->Leave();};
		};

		TCriticalSection() {InitializeCriticalSection(&m_CS);};
		~TCriticalSection() {DeleteCriticalSection(&m_CS);};

		void Enter() {EnterCriticalSection(&m_CS);};
		void Leave() {LeaveCriticalSection(&m_CS);};
	};
}

#endif//__adThreads_h__