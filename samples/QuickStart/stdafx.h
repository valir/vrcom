#ifndef STDAFX_H_INCLUDED
#define STDAFX_H_INCLUDED

#define _WIN32_DCOM

#pragma warning(disable:4786)

#include <windows.h>
#include <tchar.h>
#include <objbase.h>
#include <comdef.h>
#include <comutil.h>
#include <stdio.h>
#include <crtdbg.h>
#include <shlobj.h>
#include <shlguid.h>
#include <map>

#define NO_ACTIVEX_CONTAINER /*! réduction des choses incluses */
#define VRCOM_THREADING_MODEL COINIT_MULTITHREADED
#define VRCOM_DLL
#include <vrcom.h>		// les classes de templates COM de vrusu
using namespace VRCOM;

#include <dpException.h>
#include <wtkEventLog.h>

typedef WTK::EventStrings<Exception>	_EventStrings;
typedef WTK::EventLog<Exception>		_EventLog;

#define DECLARE_EVENT_STRINGS() \
	_EventStrings eventStrings; \
	eventStrings += GetLastError(); \
	eventStrings += (LPCTSTR)NULL; \
	eventStrings += _T(__FILE__); \
	eventStrings += __LINE__

#define REPORT_EVENT_STRINGS(id) \
	_EventLog::GetSingleton()->Report(id, eventStrings)

#define REPORT_EXCEPTION(pException) \
	DECLARE_EVENT_STRINGS(); \
	REPORT_EVENT_STRINGS(WRN_REPORTING_EXCEPTION); \
	_EventLog::GetSingleton()->ReportException(pException); \
	delete pException;

#define REPORT_COM_ERROR(e) \
	_EventLog::GetSingleton()->ReportComError(e, ERR_COM_ERROR, (LPCTSTR)_bstr_t(gszComErrorFileName), glComErrorFileLine)

#define REPORT_UNKNOWN_EXCEPTION() \
	DECLARE_EVENT_STRINGS(); \
	REPORT_EVENT_STRINGS(ERR_FATAL_EXCEPTION);


#endif // STDAFX_H_INCLUDED
