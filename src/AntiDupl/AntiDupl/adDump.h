#ifndef __adDump_h__
#define __adDump_h__

#include "adConfig.h"
#include <Dbghelp.h>
#pragma comment(lib, "DbgHelp")
#include <strsafe.h>

#define STATUS_FATAL_APP_EXIT 0x40000015

LONG CALLBACK UnhandledException(EXCEPTION_POINTERS* e);

#endif//__adDump_h__ 