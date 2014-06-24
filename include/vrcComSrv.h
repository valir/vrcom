/*! 
 *********************************************************************
 *  @file     vrcComSrv.h
 *  @brief    COM server module declaration
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
#ifndef COMSRV_H_INCLUDED
#define COMSRV_H_INCLUDED

#include "vrcConfig.h"
#include "vrcTracer.h"
#include "vrcFactory.h"
#include <dpSingleton.h>
#include "vrcOleInit.h"

using namespace DP;

VRCOM_BEGIN

class CGenericLock;
class CTypeInfoBase;

#pragma warning(disable:4275)
#pragma warning(disable:4251)
#pragma warning(disable:4786)
#pragma warning(disable:4290)
#pragma warning(disable:4355)

/*!
 *  @class ModuleBase
 *  @brief COM server module base class
 *  
 *  The "module" corresponds to a physical package - EXE or DLL - housing
 *  one or more COM components. It has attached at least one type library
 *  and it handles automatic registration / unregistration.
 *  
 *  @see Module
 */
class VRCOM_EXP ModuleBase : public CTracerBase
{
	typedef std::list<CTypeInfoBase*> _TypeInfoList;
public:
	ModuleBase(bool bDLL, LPCTSTR pszApplication);
	virtual ~ModuleBase();

	void Init(bool bThreaded, HMODULE hMod);
	void Unload();

	HRESULT GetClassObject(REFCLSID clsid, REFIID riid, void** ppvObject);
	__inline HRESULT CanUnloadNow() { return (m_cbLocks && m_cObjects) ? S_FALSE : S_OK; }
	void LockServer(BOOL fLock);
	void ObjectsUp(IUnknown* pObj);
	void ObjectsDown(IUnknown* pObj);

	HRESULT RegisterAll();
	HRESULT UnregisterAll();
	__inline HMODULE GetHandle() const {return m_hModule; }

	// internal support
	static void AddTypeInfo(CTypeInfoBase* pTypeInfo);
	ITypeLibPtr GetTypeLib(REGKIND regKind =REGKIND_NONE);

private:
	bool			m_bInitialized;
	bool			m_bDll;
	bool			m_bMultiThreaded;
	_bstr_t			m_strApplication;
	LONG			m_cbLocks;
	LONG			m_cObjects;
	HMODULE			m_hModule;
	ITypeLibPtr		m_spTypeLib;

	static _TypeInfoList	m_TypeInfoList;

#ifdef _DEBUG
	typedef std::map<IUnknown*, LONG> _objMap;
	TCHAR			m_szModule[_MAX_PATH];
	_objMap			m_objMap;
	LONG			m_nObj;
	CMTALock		m_objMapLock;
#endif
};

/*!
 *  @class Module 
 *  @brief Physical module class
 *  
 *  
 *  
 *  @param MODEL	defines threading model, as of COINIT enum from MS Platform SDK
 *  @param bDLL		true if the module is a DLL or false for an EXE
 *
 *  @see 
 */
template <COINIT MODEL, bool bDLL>
class Module : public DP::SingletonClass<Module>, public ModuleBase
{
	typedef DP::SingletonClass<Module> _Base;
public:
	Module(LPCTSTR pszApplication) : 
		_Base(this),
		ModuleBase(bDLL, pszApplication)
	{
		CThreaded::m_bMultiThreaded = (MODEL & COINIT_APARTMENTTHREADED) != COINIT_APARTMENTTHREADED;
	}

	void Init(HMODULE hMod)
	{
		ModuleBase::Init((MODEL & COINIT_MULTITHREADED) == COINIT_MULTITHREADED, hMod);
	}


private:
	OleInit<MODEL>	m_OleInit;
};

#pragma warning(default:4275)
#pragma warning(default:4251)

VRCOM_END
#endif // COMSRV_H_INCLUDED
