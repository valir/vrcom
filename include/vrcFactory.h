/*! 
 *********************************************************************
 *  @file     vrcFactory.h
 *  @brief    COM class factories
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
#ifndef FACTORY_H_INCLUDED
#define FACTORY_H_INCLUDED

#include "vrcConfig.h"
#include "vrcImpInterface.h"

VRCOM_BEGIN

// some tricks to get std::list<CFactoryBase*> exported
class CFactoryBase;
#pragma warning(disable:4231)
VRCOM_EXP_TEMPLATE template class VRCOM_EXP std::vector<CFactoryBase*>;
VRCOM_EXP_TEMPLATE template class VRCOM_EXP std::vector<DWORD>;
#pragma warning(default:4231)

/*!
 *  @class CFactoryBase
 *  @brief Class for internal use.
 *  
 *
 *  @see 
 */
class VRCOM_EXP CFactoryBase : public IClassFactory,
								public CTracerBase
{
public:
	CFactoryBase();

	virtual const CLSID* GetCLSID() const = 0;
	__inline void SetModule(ModuleBase* pModule) 
	{ 
#ifdef _DEBUG
		if (m_pModule && (m_pModule != pModule))
			_RPTF0(_CRT_WARN, "WARNING: redefining module\n");
#endif
		m_pModule = pModule; 
	}

	// IUnknown
	STDMETHODIMP_(ULONG) AddRef() 
	{
		return m_cRefs; 
	}
	STDMETHODIMP_(ULONG) Release()
	{
		return m_cRefs;
	}
	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject);

	static HRESULT GetClassObject(ModuleBase* pModule, REFCLSID clsid, REFIID riid, void** ppvObject);
	static HRESULT RegisterAll(LPCTSTR pszApplication, ITypeLibPtr spLib, HMODULE hMod, bool bDll, LPCTSTR pszAppID =NULL);
	static HRESULT UnregisterAll(LPCTSTR pszApplication, ITypeLibPtr spLib, bool bDll, LPCTSTR pszAppID =NULL);
	static HRESULT RegisterClassObjects(ModuleBase* pModule, ITypeLibPtr spLib);
	static HRESULT UnRegisterClassObjects();

protected:
	ModuleBase*	m_pModule;

	virtual void OnRegister(LPCTSTR pszClsId, LPCTSTR pszModule);
	virtual void OnUnregister(LPCTSTR pszClsId);

private:
	typedef std::vector<CFactoryBase*> CFactoryList;
	typedef std::vector<DWORD> RegisterList;

	LONG		m_cRefs;
	
	static CFactoryList	m_GlobalFactoryList;
	static RegisterList	m_RegisterList;
};


/*!
 *  @class CCompFactory 
 *  @brief COM component class factory
 *  
 *  The class factory is not aggregatable (there is no need to do that)
 *  
 *  @see CFactoryBase
 */
template <class COMP, const CLSID* pclsid>
class CCompFactory : public CFactoryBase
{
public:
	CCompFactory()
	{
	}
	~CCompFactory()
	{
	}

	virtual const CLSID* GetCLSID() const { return pclsid; }

	STDMETHODIMP CreateInstance(IUnknown* pOuter, REFIID riid, void** ppvObject)
	{
		Trace(TRACE_FACTORY, _T("CreateInstance\n"));
		HRESULT hr = E_FAIL;
		if (NULL != pOuter && riid != IID_IUnknown)
			hr = CLASS_E_NOAGGREGATION;
		else
		{
			COMP* pComp = NULL;
			try
			{
				pComp = COMP::CreateInstance(m_pModule, pOuter);
				hr = pComp->QueryInterface(riid, ppvObject);
				pComp->Release(); // if QI failed, then this will cause the objects deletion
				/*
				if (FAILED(hr))
				{
					delete pComp;
					// the destructor has called
					// CComSrv::GetInstance().ObjectsDown();
				}
				*/
			}
			catch (_com_error e)
			{
				Trace(TRACE_FACTORY, _T("CreateInstance : _com_error catched %s\n"), e.ErrorMessage());
				if (pComp)
					delete pComp;
				hr = e.Error();
			}
		}
		return hr;
	}
	STDMETHODIMP LockServer(BOOL fLock)
	{
		m_pModule->LockServer(fLock);
		return S_OK;
	}

protected:
	virtual void OnRegister(LPCTSTR pszClsId, LPCTSTR pszModule) { COMP::OnRegister(pszClsId, pszModule); }
	virtual void OnUnregister(LPCTSTR pszClsId) { COMP::OnUnregister(pszClsId); }
};

/*!
 *  @class CComponentBase 
 *  @brief 
 *
 *  component that can be instantiated only into the server, by the server
 *  
 *  
 *  @see 
 */
template <class COMP, class BASE =CImpInnerUnknown<IUnknown> >
class CComponentBase : public BASE
{
public:
	explicit CComponentBase(IUnknown* pOuter) : BASE(pOuter) {}

	static COMP* CreateInstance(ModuleBase* pModule, IUnknown* pOuter =NULL) throw (_com_error)
	{
		COMP* pComp = new COMP(pOuter);
		// the constructor has called 
		// CComSrv::GetInstance().ObjectsUp();
		if (NULL == pComp)
			_THROW_HR(E_OUTOFMEMORY);
		pComp->Init(pModule);
		pComp->AddRef();
		return pComp;
	}
};

/*!
 *  @class CComponent 
 *  @brief 
 *  
 *	coclass component (COMP) factory inherits this
 *  
 *  @see 
 */
template <class COMP, const CLSID* CLSCOMP, class BASE =CImpInnerUnknown<IUnknown> >
class CComponent : public CComponentBase<COMP, BASE>
{
	typedef CComponentBase<COMP, BASE> _Base;
public:
	typedef CCompFactory<COMP, CLSCOMP> _Factory;

	explicit CComponent(IUnknown* pOuter) : _Base(pOuter) 
	{
		// BUGBUG : keep this line to provoque m_Factory member's instantiation !
		m_Factory.GetCLSID();
	}

	static const CLSID& GetCLSID() { return *CLSCOMP; }
	static void OnRegister(LPCTSTR pszClsId, LPCTSTR pszModule) { /* rien */ }
	static void OnUnregister(LPCTSTR pszClsId) { /* rien */ }

private:
	static _Factory	m_Factory;
};

template <class COMP, const CLSID* CLSCOMP, class BASE>
CCompFactory<COMP, CLSCOMP> CComponent<COMP, CLSCOMP, BASE>::m_Factory;

template <class COMP>
class ComponentRef
{
public:
	ComponentRef() : m_comp(NULL) {}
	ComponentRef(COMP* comp) : m_comp(comp) { if (m_comp) m_comp->AddRef(); }
	ComponentRef(const ComponentRef& that) : m_comp(that.m_comp) { if (m_comp) m_comp->AddRef(); }
	~ComponentRef() { if (m_comp) m_comp->Release(); m_comp = NULL; }

	ComponentRef& operator = (const ComponentRef& that) {
		if (m_comp)
			m_comp->Release();
		m_comp = that.m_comp;
		if (m_comp)
			m_comp->AddRef();
		return *this;
	}

	COMP* operator -> () const { return m_comp; }

	operator COMP* () const { return m_comp; }

private:
	COMP*	m_comp;
};


VRCOM_END
#endif // FACTORY_H_INCLUDED