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
#include <process.h>

#include "adThreads.h"

namespace ad
{
	//-------------------------------------------------------------------------
	unsigned __stdcall TThread::Task(void *pThis)
	{
		TThread *pThread = (TThread*)pThis;
		switch(pThread->m_type)
		{
		case TASK:
			pThread->m_pTask->Work();
			break;
		case FUNCTION:
			pThread->m_pFunction(pThread->m_pData);
			break;
		case METHOD:
			(pThread->m_pObject->*pThread->m_pMethod)();
			break;
		}
		return 0;
	}

	void TThread::CreateThread()
	{
		m_handle = (HANDLE)_beginthreadex(NULL, 0, TThread::Task, this, CREATE_SUSPENDED, &m_id);
	}

	TThread::TThread(TTask *pTask)
		:m_type(TASK), m_pTask(pTask)
	{
		CreateThread();
	}

	TThread::TThread(void (*pFunction)(void*), void *pData)
		:m_type(FUNCTION),
		m_pFunction(pFunction),
		m_pData(pData)
	{
		CreateThread();
	}

	TThread::~TThread()
	{
		if(m_handle)
		{
			if(Active())
				Terminate();
			CloseHandle(m_handle);
		}
	}

	bool TThread::Resume()
	{
		if(m_handle)
		{
			DWORD oldSuspendCount = ResumeThread(m_handle);
			if(oldSuspendCount != -1)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	bool TThread::Suspend()
	{
		if(m_handle)
		{
			DWORD oldSuspendCount = SuspendThread(m_handle);
			if(oldSuspendCount != -1)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	bool TThread::Terminate(unsigned int exitCode, unsigned int waitTime)
	{
		if(m_handle)
		{
			if(m_type == TASK && m_pTask && m_pTask->Status())
				m_pTask->Status()->Stop();

			DWORD waitResult = WaitForSingleObject(m_handle, (DWORD)waitTime);
			switch (waitResult) 
			{
			case WAIT_OBJECT_0:
				return true;
			case WAIT_TIMEOUT: 
			case WAIT_ABANDONED:
				{
					BOOL result = TerminateThread(m_handle, (DWORD)exitCode);
					if(result == TRUE)
						return true;
					else
						return false;
				}
			default:
				return false;
			}
		}
		else
			return false;
	}

	bool TThread::Active() const
	{
		if(m_handle)
		{
			DWORD exitCode;
			BOOL result;
			result = GetExitCodeThread(m_handle, &exitCode);
			if(result == TRUE && exitCode == STILL_ACTIVE)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	bool TThread::SetPriority(int priority)
	{
		if(m_handle)
		{
			BOOL result;
			result = SetThreadPriority(m_handle, priority);
			if(result == TRUE)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	void TThread::Exit(unsigned int exitCode)
	{
		_endthreadex(exitCode);
	}

	unsigned int TThread::CurrentId()
	{
		return ::GetCurrentThreadId();
	}
	//-------------------------------------------------------------------------
}
