/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2015 Yermalayeu Ihar, 2013-2015 Borisov Dmitry.
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

#include "adDump.h"

#ifdef AD_DUMP_ENABLE

#include <Dbghelp.h>
#include <strsafe.h>

namespace ad
{
    const DWORD STATUS_FATAL_APP_EXIT = 0x40000015;

    typedef BOOL (*MiniDumpWriteDumpPtr)(
        HANDLE hProcess,
        DWORD ProcessId,
        HANDLE hFile,
        MINIDUMP_TYPE DumpType,
        const PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, 
        const PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, 
        const PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

    MiniDumpWriteDumpPtr miniDumpWriteDump = NULL;

    TCHAR * GetProgramPath(TCHAR *buffer, int maxlen)
    {
        assert(buffer != NULL);
        int len;
        if ((len = GetModuleFileName(NULL, buffer, maxlen)) == 0)
            return NULL;
        if (len == maxlen)
            return NULL;
        while (buffer[--len] != '\\');
        buffer[len] = '\0';
        return buffer;
    }

    void MakeMinidump(EXCEPTION_POINTERS* e)
    {
        TCHAR tszFileName[MAX_PATH] = {0};
        TCHAR tszPath[MAX_PATH] = {0};
        SYSTEMTIME stTime = {0};
        GetSystemTime(&stTime);
        GetProgramPath(tszPath, MAX_PATH);

        StringCbPrintf(tszFileName, 
            _countof(tszFileName), 
            _T("%s\\%s_%4d%02d%02d_%02d%02d%02d.dmp"), 
            tszPath, 
            _T("CrashDump"),  
            stTime.wYear, 
            stTime.wMonth, 
            stTime.wDay, 
            stTime.wHour, 
            stTime.wMinute, 
            stTime.wSecond);

        HANDLE hFile = CreateFile(tszFileName, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        if(hFile == INVALID_HANDLE_VALUE)
            return;

        MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
        exceptionInfo.ThreadId = GetCurrentThreadId();
        exceptionInfo.ExceptionPointers = e;
        exceptionInfo.ClientPointers = FALSE;

        miniDumpWriteDump(
            GetCurrentProcess(),
            GetCurrentProcessId(),
            hFile,
            MINIDUMP_TYPE(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory | MiniDumpWithFullMemory),
            e ? &exceptionInfo : NULL,
            NULL,
            NULL);

        if(hFile)
        {
            CloseHandle(hFile);
            hFile = NULL;
        }
        return; 
    }

    LONG CALLBACK UnhandledException(EXCEPTION_POINTERS* info)
    {
        DWORD exception_code = info->ExceptionRecord->ExceptionCode;
        void *address = info->ExceptionRecord->ExceptionAddress;
        if (exception_code == STATUS_FATAL_APP_EXIT) 
        {// abort()
            MessageBox(NULL, TEXT("Program aborted."), 
                TEXT("Crash report"), MB_OK | MB_ICONERROR);

            EXCEPTION_EXECUTE_HANDLER; // handle dump by caller of abort() better
        }

        if ((exception_code != EXCEPTION_ACCESS_VIOLATION &&
            exception_code != EXCEPTION_ARRAY_BOUNDS_EXCEEDED &&
            exception_code != EXCEPTION_INT_DIVIDE_BY_ZERO &&
            exception_code != EXCEPTION_PRIV_INSTRUCTION &&
            exception_code != EXCEPTION_ILLEGAL_INSTRUCTION)
            )
            return EXCEPTION_CONTINUE_SEARCH; // no chance. keep eventlogs.

        MakeMinidump(info);

        MessageBox(NULL, TEXT("Program crashed."), 
            TEXT("Crash report"), MB_OK | MB_ICONERROR);
        return EXCEPTION_EXECUTE_HANDLER; // terminate now. prevent eventlogs.
    }

    void DumpInit()
    {
        if(miniDumpWriteDump == NULL)
        {
            HMODULE module = ::LoadLibraryA("DbgHelp.dll");
            if(module != NULL)
            {
                miniDumpWriteDump = (MiniDumpWriteDumpPtr)::GetProcAddress(module, "MiniDumpWriteDump");
            }
        }

        if(miniDumpWriteDump != NULL)
        {
            ::SetUnhandledExceptionFilter(UnhandledException);
        }
    }
}

#else

namespace ad
{
    void DumpInit()
    {

    }
}

#endif
