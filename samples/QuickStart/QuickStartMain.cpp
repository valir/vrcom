#include "stdafx.h"

#define APP_NAME _T("QuickStart")

using namespace VRCOM;

_Module leModule(APP_NAME);

BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD dwReason, LPVOID)
{
	static _EventLog leEventLog(APP_NAME);

	BOOL bResult = TRUE;
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		leModule.Init(hInstDll);
		// déactivation des traces VRCOM
		//CTracerBase::m_dwTraceFlags = 0;
		// préparation des erreurs
		break;

	case DLL_PROCESS_DETACH:
		leModule.Unload();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;

	default:
		bResult = FALSE;
	}
	return bResult;
}


STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	return leModule.GetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow()
{
	return leModule.CanUnloadNow();
}

STDAPI DllRegisterServer()
{
	try
	{
		_EventLog::GetSingleton()->RegisterSource(leModule.GetHandle());
	}
	catch (...)
	{
		return E_FAIL;
	}
	return leModule.RegisterAll();
}

STDAPI DllUnregisterServer()
{
	try
	{
		_EventLog::GetSingleton()->UnRegisterSource();
	}
	catch (...)
	{
		return E_FAIL;
	}
	return leModule.UnregisterAll();
}
