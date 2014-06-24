/*! 
 *********************************************************************
 *  @file     Factory.cpp
 *  @brief    Class factory classes
 *
 * 
 *
 *********************************************************************
 * Copyright (C) 1999-2002 Valentin Rusu
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *********************************************************************
 * You can reach the author by e-mail at vcom@rusu.info
 *********************************************************************
 */

#include "stdafx.h"

#include "vrcTracer.h"
#include "vrcFactory.h"
#include "vrcComSrv.h"
#include "vrcRegKey.h"

using namespace vrcom;

CFactoryBase::CFactoryList CFactoryBase::m_GlobalFactoryList;
CFactoryBase::RegisterList CFactoryBase::m_RegisterList;

CFactoryBase::CFactoryBase() : 
	m_cRefs(1), m_pModule(NULL)
{
	// trick: prevent object destruction
	AddRef();
	m_GlobalFactoryList.push_back(this);
}

HRESULT CFactoryBase::QueryInterface(REFIID riid, void** ppvObject)
{
	HRESULT hr = E_NOINTERFACE;
	*ppvObject  = NULL;
	if (riid == IID_IUnknown)
	{
		AddRef();
		*ppvObject = dynamic_cast<IUnknown*>(this);
		hr = S_OK;
	}
	else
	if (riid == IID_IClassFactory)
	{
		AddRef();
		*ppvObject = dynamic_cast<IClassFactory*>(this);
		hr = S_OK;
	}
	return hr;
}

HRESULT CFactoryBase::GetClassObject(ModuleBase* pModule, REFCLSID clsid, REFIID riid, void** ppvObject)
{
	HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;
	CFactoryList::iterator pos = m_GlobalFactoryList.begin();
	while (pos != m_GlobalFactoryList.end())
	{
		CFactoryBase* pFactory = *pos++;
		if (*pFactory->GetCLSID() == clsid)
		{
			pFactory->SetModule(pModule);
			hr = pFactory->QueryInterface(riid, ppvObject);
			break;
		}
	}
	return hr;
}

HRESULT CFactoryBase::RegisterAll(LPCTSTR pszApplication, 
								  ITypeLibPtr spLib, 
								  HMODULE hModule, 
								  bool bDll,
								  LPCTSTR pszAppID)
{
	HRESULT hr = S_OK;

	try
	{
		TCHAR szModule[_MAX_PATH];
		::GetModuleFileName(hModule, szModule, _COUNT_OF(szModule));

		// try to load current type-library
		ITypeLibPtr	spTypeLib = spLib;
		TLIBATTR* pLibAttr = NULL;
		_THROW_HR(spTypeLib->GetLibAttr(&pLibAttr));
		TLIBATTR LibAttr = *pLibAttr;
		spTypeLib->ReleaseTLibAttr(pLibAttr);

		OLECHAR szTypeLib[48];
		::StringFromGUID2(LibAttr.guid, szTypeLib, _COUNT_OF(szTypeLib));
		_bstr_t strTypeLib(szTypeLib);
		
		CRegKey	hkcr(HKEY_CLASSES_ROOT);
		CRegKey clsid(hkcr, _T("CLSID"));
		CRegKey appid(hkcr, _T("AppID"));
		if (pszAppID && !bDll)
		{
			appid.CreateKey(pszAppID);
			appid.CreateKey(::PathFindFileName(szModule));
		}

		CFactoryList::iterator pos = m_GlobalFactoryList.begin();
		while (pos != m_GlobalFactoryList.end())
		{
			CFactoryBase* pFactory = *pos++;

			// attempt to obtain this coclass description
			ITypeInfoPtr	spCompInfo;
			_THROW_HR(spTypeLib->GetTypeInfoOfGuid(*pFactory->GetCLSID(), &spCompInfo));

			// check that we are dealing with a coclass
			TYPEATTR* pattr = NULL;
			_THROW_HR(spCompInfo->GetTypeAttr(&pattr));
			_ASSERTE(pattr->typekind = TKIND_COCLASS);
			TYPEATTR attr = *pattr;
			spCompInfo->ReleaseTypeAttr(pattr);

			UINT index = 0;
			_THROW_HR(spCompInfo->GetContainingTypeLib(&spTypeLib, &index));
			BSTR pstrDocString = NULL;
			BSTR pstrName = NULL;
			_THROW_HR(spTypeLib->GetDocumentation(index, &pstrName, &pstrDocString, NULL, NULL));
			_bstr_t strDocString(pstrDocString, false);
			_bstr_t strName(pstrName, false);

			// finally, register the component
			TCHAR szProgId[40];
			_sntprintf(szProgId, _COUNT_OF(szProgId), _T("%s.%s"), pszApplication, (LPCTSTR)strName);

			TCHAR szCurrProgId[40];
			if (attr.wMajorVerNum)
			{
				if (attr.wMinorVerNum)
					_sntprintf(szCurrProgId, _COUNT_OF(szCurrProgId), _T("%s.%d.%d"), szProgId, attr.wMajorVerNum, attr.wMinorVerNum);
				else
					_sntprintf(szCurrProgId, _COUNT_OF(szCurrProgId), _T("%s.%d"), szProgId, attr.wMajorVerNum);
			}

			OLECHAR szClsid[48];
			::StringFromGUID2(attr.guid, szClsid, _COUNT_OF(szClsid));
			_bstr_t strClsid(szClsid);

			CRegKey keyCurrProgId = hkcr.CreateKey(szCurrProgId);
			keyCurrProgId.SetValue(NULL, strDocString);
			keyCurrProgId.CreateKey(_T("CLSID")).SetValue(NULL, strClsid);

			CRegKey keyProgId = hkcr.CreateKey(szProgId);
			keyProgId.SetValue(NULL, strDocString);
			keyProgId.CreateKey(_T("CLSID")).SetValue(NULL, strClsid);
			keyProgId.CreateKey(_T("CurVer")).SetValue(NULL, szCurrProgId);

			CRegKey keyCLSID = clsid.CreateKey(strClsid);
			keyCLSID.SetValue(NULL, strDocString);
			if (pszAppID)
				keyCLSID.SetValue(_T("AppID"), pszAppID);
			keyCLSID.CreateKey(_T("ProgID")).SetValue(NULL, szCurrProgId);
			keyCLSID.CreateKey(_T("VersionIndependentProgID")).SetValue(NULL, szProgId);
			keyCLSID.CreateKey(_T("TypeLib")).SetValue(NULL, strTypeLib);
			CRegKey keySrv = bDll ? keyCLSID.CreateKey(_T("InprocServer32")) : keyCLSID.CreateKey(_T("LocalServer32"));
			keySrv.SetValue(NULL, szModule);
			keySrv.SetValue(_T("ThreadingModel"), _T("Both"));

			// finalement, fait tout opération d'enrégistrement spécifique
			pFactory->OnRegister(strClsid, szModule);
		}
	}
	catch (_com_error e)
	{
		return e.Error();
	}
	catch (...)
	{
		_RPT0(_CRT_ERROR, _T("Unknown exception during RegisterAll"));
		return E_FAIL;
	}
	return hr;
}

HRESULT CFactoryBase::UnregisterAll(LPCTSTR pszApplication, 
									ITypeLibPtr spLib, 
									bool bDll,
									LPCTSTR pszAppID)
{
	HRESULT hr = S_OK;

	try
	{
		// try to load current type-library
		ITypeLibPtr	spTypeLib = spLib;
		TLIBATTR* pLibAttr = NULL;
		_THROW_HR(spTypeLib->GetLibAttr(&pLibAttr));
		TLIBATTR LibAttr = *pLibAttr;
		spTypeLib->ReleaseTLibAttr(pLibAttr);

		_THROW_HR(::UnRegisterTypeLib(LibAttr.guid, LibAttr.wMajorVerNum, LibAttr.wMinorVerNum, LibAttr.lcid, LibAttr.syskind));

		OLECHAR szTypeLib[48];
		::StringFromGUID2(LibAttr.guid, szTypeLib, _COUNT_OF(szTypeLib));
		
		CRegKey	hkcr(HKEY_CLASSES_ROOT);
		CRegKey clsid(hkcr, _T("CLSID"));
		CRegKey appid(hkcr, _T("AppID"));

		appid.DestroyKey(pszApplication);

		CFactoryList::iterator pos = m_GlobalFactoryList.begin();
		while (pos != m_GlobalFactoryList.end())
		{
			CFactoryBase* pFactory = *pos++;

			// attempt to obtain this coclass description
			ITypeInfoPtr	spCompInfo;
			_THROW_HR(spTypeLib->GetTypeInfoOfGuid(*pFactory->GetCLSID(), &spCompInfo));

			// check that we are dealing with a coclass
			TYPEATTR* pattr = NULL;
			_THROW_HR(spCompInfo->GetTypeAttr(&pattr));
			_ASSERTE(pattr->typekind = TKIND_COCLASS);
			TYPEATTR attr = *pattr;
			spCompInfo->ReleaseTypeAttr(pattr);

			UINT index = 0;
			BSTR pstrDocString = NULL;
			BSTR pstrName = NULL;
			_THROW_HR(spCompInfo->GetContainingTypeLib(&spTypeLib, &index));
			_THROW_HR(spTypeLib->GetDocumentation(index, &pstrName, &pstrDocString, NULL, NULL));
			_bstr_t strName(pstrName, false);
			_bstr_t strDocString(pstrDocString, false);

			// finally, register the component
			TCHAR szProgId[40];
			_sntprintf(szProgId, _COUNT_OF(szProgId), _T("%s.%s"), pszApplication, strName);

			TCHAR szCurrProgId[40];
			if (attr.wMajorVerNum)
			{
				if (attr.wMinorVerNum)
					_sntprintf(szCurrProgId, _COUNT_OF(szCurrProgId), _T("%s.%d.%d"), szProgId, attr.wMajorVerNum, attr.wMinorVerNum);
				else
					_sntprintf(szCurrProgId, _COUNT_OF(szCurrProgId), _T("%s.%d"), szProgId, attr.wMajorVerNum);
			}

			OLECHAR szClsid[48];
			::StringFromGUID2(attr.guid, szClsid, _COUNT_OF(szClsid));
			_bstr_t strClsid(szClsid);

			hkcr.DestroyKey(szCurrProgId);
			hkcr.DestroyKey(szProgId);
			clsid.DestroyKey(strClsid);

			// finalement, fait tout opération de nettoyage spécifique
			pFactory->OnUnregister(strClsid);
		}
	}
	catch (_com_error e)
	{
		return e.Error();
	}
	catch (...)
	{
		_RPT0(_CRT_ERROR, _T("Unknown exception during RegisterAll"));
		return E_FAIL;
	}
	return hr;
}

HRESULT CFactoryBase::RegisterClassObjects(ModuleBase* pModule, ITypeLibPtr spLib)
{
	HRESULT hr = S_OK;
	try
	{
		// try to load current type-library
		ITypeLibPtr	spTypeLib = spLib;

		CFactoryList::iterator pos = m_GlobalFactoryList.begin();
		while (pos != m_GlobalFactoryList.end())
		{
			CFactoryBase* pFactory = *pos++;
			pFactory->SetModule(pModule);

			// attempt to obtain this coclass description
			ITypeInfoPtr	spCompInfo;
			_THROW_HR(spTypeLib->GetTypeInfoOfGuid(*pFactory->GetCLSID(), &spCompInfo));

			// check that we are dealing with a coclass
			TYPEATTR* pattr = NULL;
			_THROW_HR(spCompInfo->GetTypeAttr(&pattr));
			_ASSERTE(pattr->typekind = TKIND_COCLASS);
			spCompInfo->ReleaseTypeAttr(pattr);

			IUnknownPtr spUnkFactory(pFactory);

			DWORD dwRegisterCode = 0;
			_THROW_HR(::CoRegisterClassObject(
				*pFactory->GetCLSID(),
				spUnkFactory,
				CLSCTX_SERVER,
				REGCLS_MULTIPLEUSE,
				&dwRegisterCode));

			m_RegisterList.push_back(dwRegisterCode);
		}
	}
	catch (_com_error e)
	{
		
		return e.Error();
	}
	catch (...)
	{
		_RPT0(_CRT_ERROR, _T("Unknown exception during RegisterClassObjects"));
		return E_FAIL;
	}
	return hr;
}

HRESULT CFactoryBase::UnRegisterClassObjects()
{
	HRESULT hr = S_OK;
	try
	{
		RegisterList::iterator pos = m_RegisterList.begin();
		while (pos != m_RegisterList.end())
			_THROW_HR(::CoRevokeClassObject(*pos++));
	}
	catch (_com_error e)
	{
		return e.Error();
	}
	catch (...)
	{
		_RPT0(_CRT_ERROR, _T("Unknown exception during RegisterClassObjects"));
		return E_FAIL;
	}
	return hr;
}

void CFactoryBase::OnRegister(LPCTSTR, LPCTSTR pszModule)
{
	/* rien */
}

void CFactoryBase::OnUnregister(LPCTSTR)
{
	/* rien */
}
