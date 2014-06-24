/*!
 *********************************************************************
 *  @file     ComSrv.cpp
 *  @brief    Server module implementation
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


#include "vrcTypeInfo.h"
#include <vrcComSrv.h>
#include "vrcLock.h"

using namespace vrcom;

#pragma warning(disable:4355)

ModuleBase::_TypeInfoList ModuleBase::m_TypeInfoList;

ModuleBase::ModuleBase(bool bDLL, LPCTSTR pszApplication) :
	m_bDll(bDLL),
	m_bInitialized(false),
	m_bMultiThreaded(true),
	m_cbLocks(0),
	m_cObjects(0),
	m_hModule(NULL),
	m_strApplication(pszApplication)
{
#ifdef _DEBUG
	m_nObj = 0;
#endif
}

ModuleBase::~ModuleBase()
{
}

void ModuleBase::Init(bool bThreaded, HMODULE hMod)
{
#ifdef _DEBUG
	::GetModuleFileName(hMod, m_szModule, sizeof(m_szModule)/sizeof(m_szModule[0]));
	Trace(TRACE_MODULE, _T("Init(0x%lX : %s);\n"), hMod, m_szModule);
#endif
	m_hModule = hMod;
	if (!m_bInitialized)
	{
		m_bMultiThreaded = bThreaded;
		// initialize all type infos
		_TypeInfoList::iterator pti = m_TypeInfoList.begin();
		while (pti != m_TypeInfoList.end())
			(*pti++)->Init();
		m_bInitialized = true;
	}
	if (!m_bDll)
	{
		ITypeLibPtr spTypeLib;
		try
		{
			spTypeLib = GetTypeLib(REGKIND_REGISTER);
		}
		catch (_com_error& )
		{
			Trace(TRACE_MODULE, _T("Init WARNING module has no type library!\n"));
		}
		if (spTypeLib != NULL)
			_THROW_HR(CFactoryBase::RegisterClassObjects(this, spTypeLib));
	}
}

void ModuleBase::Unload()
{
#ifdef _DEBUG
	Trace(TRACE_MODULE, _T("Unload [0x%lX : %s]\n"), m_hModule, m_szModule);
#endif
	if (!m_bDll)
		_THROW_HR(CFactoryBase::UnRegisterClassObjects());
}

HRESULT ModuleBase::RegisterAll()
{
	return CFactoryBase::RegisterAll(
		m_strApplication, 
		GetTypeLib(REGKIND_REGISTER),
		m_hModule,
		m_bDll);
}

HRESULT ModuleBase::UnregisterAll()
{
	return CFactoryBase::UnregisterAll(
		m_strApplication, 
		GetTypeLib(),
		m_bDll);
}

void ModuleBase::LockServer(BOOL fLock)
{  
	if (fLock)
		::InterlockedIncrement(&m_cbLocks);
	else
		::InterlockedDecrement(&m_cbLocks);
	Trace(TRACE_SRVLOCK, _T("Lock %d\n"), m_cbLocks);
}

void ModuleBase::ObjectsUp(IUnknown* pObj)
{
	LONG lCount = ::InterlockedIncrement(&m_cObjects);
#ifdef _DEBUG
	Trace(TRACE_SRVLOCK, _T("[%s] ObjectsUp %d\n"), ::PathFindFileName(m_szModule), lCount);
#endif
/*
#ifdef _DEBUG
	m_objMapLock.Lock();
	m_objMap[pObj] = ++m_nObj;
	_objMap::iterator pos = m_objMap.begin();
	while (pos != m_objMap.end())
	{
		Trace(TRACE_SRVLOCK, _T("[%s] ObjectsUp : map 0x%X = %ld\n"), ::PathFindFileName(m_szModule), (*pos).first, (*pos).second);
		pos++;
	}
	m_objMapLock.Unlock();
#endif
*/
}

void ModuleBase::ObjectsDown(IUnknown* pObj)
{
	LONG lCount = ::InterlockedDecrement(&m_cObjects);
#ifdef _DEBUG
	Trace(TRACE_SRVLOCK, _T("[%s] ObjectsDown %d\n"), ::PathFindFileName(m_szModule), lCount);
#endif
/*
#ifdef _DEBUG
	m_objMapLock.Lock();
	m_objMap.erase(pObj);
	_objMap::iterator pos = m_objMap.begin();
	while (pos != m_objMap.end())
	{
		Trace(TRACE_SRVLOCK, _T("[%s] ObjectsDown : map 0x%X = %ld\n"), ::PathFindFileName(m_szModule), (*pos).first, (*pos).second);
		pos++;
	}
	m_objMapLock.Unlock();
#endif
*/
}

HRESULT ModuleBase::GetClassObject(REFCLSID clsid, REFIID riid, void** ppvObject)
{
	return CFactoryBase::GetClassObject(this, clsid, riid, ppvObject);
}

void ModuleBase::AddTypeInfo(CTypeInfoBase* pTypeInfo)
{ 
	m_TypeInfoList.push_back(pTypeInfo); 
}

ITypeLibPtr ModuleBase::GetTypeLib(REGKIND regKind)
{
	if (m_spTypeLib == NULL)
	{
		// try to load current type-library
		TCHAR szModule[_MAX_PATH];
		::GetModuleFileName(m_hModule, szModule, _COUNT_OF(szModule));

		_THROW_HR(::LoadTypeLibEx(_bstr_t(szModule), regKind, &m_spTypeLib));
	}
	return m_spTypeLib;
}

