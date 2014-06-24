/*! 
 *********************************************************************
 *  @file     vrcObject.h
 *  @brief    No coclass COM object class
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
#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include "vrcConfig.h"
#include "vrcImpInterface.h"


VRCOM_BEGIN

#pragma warning(disable:4355)
#pragma warning(disable:4290)

/*!
 *  @class CComObject
 *  @brief Use this class to create a simple, runtime, COM object.
 *  
 *	an aggregatable COM object
 *  
 *  @see 
 */
template <class INT, class IIDHOLDER =CCheckIID<INT> >
class CComObject :	public IUnknown,
					public CThreaded,
					public CTracerBase
{
public:
	explicit CComObject(IUnknown* pOuter =NULL) :
		m_pOuter(pOuter ? pOuter : this),
		m_cRefs(0),
		m_INT(this),
		m_pModule(NULL)
	{
		Trace(TRACE_OBJECT, _T("Creating %s\n"), (pOuter == NULL) ? _T("SINGLE") : _T("AGGREGATED"));
	}
	~CComObject()
	{
		GetModule()->ObjectsDown(this);
	}

	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)
	{
		if (ppvObject == NULL)
			return E_POINTER;
		*ppvObject = NULL;

		if (riid == IID_IUnknown)
		{
			*ppvObject = m_pOuter;
		}
		else
		if (m_INT.HasIID(riid))
		{
			*ppvObject = dynamic_cast<IUnknown*>(&m_INT);
		}

		if (*ppvObject)
		{
			(static_cast<IUnknown*>(*ppvObject))->AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	STDMETHODIMP_(ULONG) AddRef()
	{
		::InterlockedIncrement(&m_cRefs);
		Trace(TRACE_OBJECT, _T("AddRef : %d\n"), m_cRefs);
		return m_cRefs;
	}

	STDMETHODIMP_(ULONG) Release()
	{
		assert(m_cRefs != 0);
		Lock();
		--m_cRefs;
		Trace(TRACE_OBJECT, _T("Release : %d\n"), m_cRefs);
		if (0 == m_cRefs)
		{
			Unlock();
			delete this;
			return 0;
		}
		Unlock();
		return m_cRefs;
	}

	virtual void Init(ModuleBase* pModule) 
	{
		m_pModule = pModule;
		// since this object is created into our server, then it must be notified
		GetModule()->ObjectsUp(this);
	}

	// access method to interface's implementation
	operator INT* () { return &m_INT; }

	IUnknown* GetOuterUnknown() const { return m_pOuter; }
	ModuleBase* GetModule() const { return m_pModule; }

	static CComObject<INT, IIDHOLDER>* CreateInstance(ModuleBase* pModule, IUnknown* pOuter =NULL) throw (_com_error)
	{
		CComObject<INT, IIDHOLDER>* pObj = new CComObject<INT, IIDHOLDER>(pOuter);
		if (NULL == pObj)
		{
			_com_raise_error(E_OUTOFMEMORY);
		}
		
		try
		{
			pObj->Init(pModule);
		}
		catch (...)
		{
			delete pObj;
			throw;
		}
		return pObj;
	}

private:

	IUnknown*		m_pOuter;
	LONG			m_cRefs;
	ModuleBase*		m_pModule;
	CImpInterface<INT, CComObject<INT, IIDHOLDER>, IIDHOLDER >	m_INT;
};

#pragma warning(default:4355)

VRCOM_END
#endif // OBJECT_H_INCLUDED