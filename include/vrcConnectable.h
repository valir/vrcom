/*! 
 *********************************************************************
 *  @file     vrcConnectable.h
 *  @brief    Connection points implementation classes
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
#ifndef CONNECTABLE_H_INCLUDED
#define CONNECTABLE_H_INCLUDED

#include "vrcConfig.h"
#include "vrcObject.h"
#include "vrcConnectionPointBase.h"

VRCOM_BEGIN

/*!
 *  @class CImpEnumConnectionPoints 
 *  @brief IEnumConnectionPoints implementation
 *  
 *  
 *  @see CImpConnectionPointContainer
 */
template <class CONT>
class CImpEnumConnectionPoints : public IEnumConnectionPoints
{
public:
	CImpEnumConnectionPoints() : m_pContainer(NULL) {}

	void SetContainer(CONT* pCont) 
	{ 
		assert(pCont != NULL);
		m_pContainer = pCont; 
		Reset();
	}

	// IEnumConnectionPoints
	STDMETHODIMP Next(ULONG cConnections, LPCONNECTIONPOINT* ppCP, ULONG* pcFetched)
	{
		if ((cConnections >1) && (NULL == pcFetched))
			return E_POINTER;
		if (NULL == ppCP)
			return E_POINTER;

		if (pcFetched)
			*pcFetched = 0;

//		memset(*ppCP, 0, sizeof(LPCONNECTIONPOINT)*cConnections);

		HRESULT hr = S_FALSE;
		const CONT::CNestedList& list = m_pContainer->GetNestedList();
		LPCONNECTIONPOINT	pCP = *ppCP;
		while (cConnections-- && (m_pos != list.end()))
		{
			if (FAILED((dynamic_cast<IUnknown*>(*m_pos++))->QueryInterface(IID_IConnectionPoint, reinterpret_cast<void**>(&pCP))))
			{
				// release the already obtained interfaces 
				while (pCP != *ppCP)
					(pCP--)->Release();
				return E_UNEXPECTED;
			}
			pCP->AddRef();
			if (pcFetched)
				(*pcFetched)++;
			pCP++;
			hr = S_OK;
		}
		return hr;
	}
	STDMETHODIMP Skip(ULONG cConnections)
	{
		while (cConnections-- && (m_pos != m_pContainer->GetNestedList().end()))
		{
			m_pos++;
		}
		return S_OK;
	}
	STDMETHODIMP Reset()
	{
		m_pos = m_pContainer->GetNestedList().begin();
		return S_OK;
	}
	STDMETHODIMP Clone(IEnumConnectionPoints** ppEnum)
	{
		typedef CImpEnumConnectionPoints<CONT> EnumType;
		CComObject<EnumType, CImpEnumConnectionPoints<CONT>::CheckIID>* pEnum = NULL;
		try
		{
			pEnum = pEnum->CreateInstance();
			((EnumType*)(*pEnum))->SetContainer(m_pContainer);
			((EnumType*)(*pEnum))->m_pos = m_pos;
		}
		catch (_com_error e)
		{
			return e.Error();
		}
		return pEnum->QueryInterface(IID_IEnumConnectionPoints, reinterpret_cast<void**>(ppEnum));
	}

	class CheckIID
	{
	public:
		bool operator () (REFIID riid) const { return (riid == IID_IEnumConnectionPoints) ? true : false; }
	};
private:
	CONT*	m_pContainer;
	CONT::CNestedList::const_iterator	m_pos;
};

/*!
 *  @class CImpConnectionPointContainer 
 *  @brief IConnectionPointContainer implementation
 *  
 *  
 *  @see CImpEnumConnectionPoints
 */
template <class COMP>
class CImpConnectionPointContainer : public CImpInnerUnknown<IConnectionPointContainer>
{
	typedef CImpInnerUnknown<IConnectionPointContainer> _Base;
public:
	CImpConnectionPointContainer(IUnknown* pUnkOuter) :
	  _Base(NULL)
	{
	}

	virtual void Init(ModuleBase* pModule)
	{
		(static_cast<COMP*>(this))->SetupConnectionPoints();
		_Base::Init(pModule);
	}

	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)
	{
		if (riid == IID_IConnectionPointContainer)
		{
			IUnknown* pUnk = dynamic_cast<IUnknown*>(this);
			pUnk->AddRef();
			*ppvObject = pUnk;
			return S_OK;
		}
		else
			return _Base::QueryInterface(riid, ppvObject);
	}
	// IConnectionPointContainer implementation
	STDMETHODIMP FindConnectionPoint(REFIID riid, IConnectionPoint** ppCP)
	{
		const CNestedList& list = GetNestedList();
		CNestedList::const_iterator pos = list.begin();
		while (pos != list.end())
		{
			CImpConnectionPointBase* pCP = dynamic_cast<CImpConnectionPointBase*>(*pos++);
			if (pCP == NULL)
				continue;
			if (riid == pCP->GetConnectionIID())
			{
				*ppCP = dynamic_cast<IConnectionPoint*>(pCP);
				(*ppCP)->AddRef();
				return S_OK;
			}
		}
		return CONNECT_E_NOCONNECTION;
	}
	STDMETHODIMP EnumConnectionPoints(IEnumConnectionPoints** ppEnum)
	{
		typedef CImpEnumConnectionPoints<CImpConnectionPointContainer<COMP> > EnumType;
		CComObject<EnumType, EnumType::CheckIID>* pEnum = NULL;
		try
		{
			pEnum = pEnum->CreateInstance();
			((EnumType*)(*pEnum))->SetContainer(this);
		}
		catch (_com_error e)
		{
			return e.Error();
		}
		return pEnum->QueryInterface(IID_IEnumConnectionPoints, reinterpret_cast<void**>(ppEnum));
	}
};

/*!
 *  @class CImpEnumConnections 
 *  @brief IEnumConnections implementation
 *  
 *  
 *  @see 
 */
template <class POINT>
class CImpEnumConnections : public IEnumConnections
{
public:
	CImpEnumConnections() : m_pPoint(NULL) {}

	void SetPoint(const POINT* pPoint) 
	{ 
		assert(pPoint != NULL);
		m_pPoint = pPoint; 
		Reset();
	}

	STDMETHODIMP Next(ULONG cConnections, LPCONNECTDATA rgcd, ULONG* pcFetched)
	{
		if ((cConnections >1) && (NULL == pcFetched))
			return E_POINTER;
		if (rgcd == NULL)
			return E_POINTER;

		if (pcFetched)
			*pcFetched = NULL;
		
		HRESULT hr = S_FALSE;
		LPCONNECTDATA	pcd = rgcd;
		const POINT::CConnectionList& list = m_pPoint->GetConnectionList();
		while (cConnections-- && (m_pos != list.end()))
		{
			pcd->dwCookie = reinterpret_cast<DWORD>(m_pos._Mynode());
			pcd->pUnk = *m_pos++;
			pcd++->pUnk->AddRef();
			if (pcFetched)
				(*pcFetched)++;
			hr = S_OK;
		}
		return hr;
	}
	STDMETHODIMP Skip(ULONG cConnections)
	{
		while (cConnections && (m_pos != m_pPoint->GetConnectionList().end()))
			m_pos++;
		return S_OK;
	}
	STDMETHODIMP Reset()
	{
		m_pos = m_pPoint->GetConnectionList().begin();
		return S_OK;
	}
	STDMETHODIMP Clone(IEnumConnections** ppEnum)
	{
		typedef CImpEnumConnections<POINT> EnumType;
		CComObject<EnumType, EnumType::CheckIID>* pEnum = NULL;
		try
		{
			pEnum = pEnum->CreateInstance();
			((EnumType*)(*pEnum))->SetPoint(m_pPoint);
			((EnumType*)(*pEnum))->m_pos = m_pos;
		}
		catch (_com_error e)
		{
			return e.Error();
		}
		return pEnum->QueryInterface(IID_IEnumConnections, reinterpret_cast<void**>(ppEnum));
	}

	class CheckIID
	{
	public:
		bool operator () (REFIID riid) const { return (riid == IID_IEnumConnections) ? true : false; }
	};
private:
	POINT::CConnectionList::const_iterator m_pos;
	const POINT* m_pPoint;
};

/*!
 *  @class CImpConnectionPoint 
 *  @brief IConnectionPoint implementation
 *  
 *  
 *  @see CImpConnectionPointBase
 */
template <class CONT, class SINK>
class CImpConnectionPoint : public CImpCompInterface<IConnectionPoint, CONT>,
							public CImpConnectionPointBase
{
public:
	typedef CImpCompInterface<IConnectionPoint, CONT> _base_type;

	CImpConnectionPoint(CONT* pContainer) :
	  _base_type(pContainer)
	{
	}
	~CImpConnectionPoint()
	{
		// release all hold interfaces
		CConnectionList::iterator it = m_ConnectionList.begin();
		while (it != m_ConnectionList.end())
		{
			(*it++)->Release();
		}
	}

	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)
	{
		return _base_type::QueryInterface(riid, ppvObject);
	}

	virtual REFIID GetConnectionIID() const { return __uuidof(SINK); }

	template <typename P1>
	void FireEvent(HRESULT (__stdcall SINK::*pfn)(P1), P1 p1)
	{
		CConnectionList::const_iterator it = m_ConnectionList.begin();
		while (it != m_ConnectionList.end())
		{
			SINK* pSink = *it++;
			(pSink->*pfn)(p1);
		}
	}

	template <typename P1, typename P2>
	void FireEvent(HRESULT (__stdcall SINK::*pfn)(P1), P1 p1, P2 p2)
	{
		CConnectionList::const_iterator it = m_ConnectionList.begin();
		while (it != m_ConnectionList.end())
		{
			SINK* pSink = *it++;
			(pSink->*pfn)(p1, p2);
		}
	}

	template <typename P1, typename P2, typename P3>
	void FireEvent(HRESULT (__stdcall SINK::*pfn)(P1), P1 p1, P2 p2, P3 p3)
	{
		CConnectionList::const_iterator it = m_ConnectionList.begin();
		while (it != m_ConnectionList.end())
		{
			SINK* pSink = *it++;
			(pSink->*pfn)(p1, p2, p3);
		}
	}

	template <class EV>
	bool FireTestEvent(EV& ev) const
	{
		bool bTest = false;
		CConnectionList::const_iterator it = m_ConnectionList.begin();
		while (it != m_ConnectionList.end())
		{
			SINK* pSink = *it++;
			bTest |= ev(pSink);
		}
		return bTest;
	}

	STDMETHODIMP GetConnectionInterface(IID* pIID)
	{
		if (NULL == pIID)
			return E_POINTER;
		*pIID = __uuidof(SINK);
		return S_OK;
	}
	STDMETHODIMP GetConnectionPointContainer(IConnectionPointContainer** ppCPC)
	{
		if (NULL == ppCPC)
			return E_POINTER;
		// we must do an AddRef : it's simpler to call QueryInterface on it
		return GetComponent()->QueryInterface(IID_IConnectionPointContainer, reinterpret_cast<void**>(ppCPC));
	}
	STDMETHODIMP Advise(IUnknown* pUnkSink, DWORD* pdwCookie)
	{
		if ((NULL == pUnkSink) || (NULL == pdwCookie))
			return E_POINTER;
		HRESULT hr;
		// check that the pointer designates a correct sink
		SINK* pSink = NULL;
		IID iid;
		GetConnectionInterface(&iid);
		hr = pUnkSink->QueryInterface(iid, reinterpret_cast<void**>(&pSink));
		if (hr == E_NOINTERFACE)
			return CONNECT_E_CANNOTCONNECT;
		if (FAILED(hr))
			return CONNECT_E_CANNOTCONNECT;

		// finally, store the pointer to that sink
		Lock();
		m_ConnectionList.push_front(pSink);
		*pdwCookie = reinterpret_cast<DWORD>(m_ConnectionList.begin()._Mynode());
		Unlock();
		return S_OK;
	}
	STDMETHODIMP Unadvise(DWORD dwCookie)
	{
		if (dwCookie == 0)
			return CONNECT_E_NOCONNECTION;
		Lock();
		try
		{
			CConnectionList::iterator	it(reinterpret_cast<CConnectionList::_Nodeptr>(dwCookie));
			(*it)->Release();
			m_ConnectionList.erase(it);
		}
		catch (...)
		{
			Unlock();
			return CONNECT_E_NOCONNECTION;
		}
		Unlock();
		return S_OK;
	}
	STDMETHODIMP EnumConnections(IEnumConnections** ppEnum)
	{
		typedef CImpEnumConnections<CImpConnectionPoint<CONT, SINK> > EnumType;
		CComObject<EnumType, EnumType::CheckIID>* pEnum = NULL;
		try
		{
			pEnum = pEnum->CreateInstance();
			((EnumType*)(*pEnum))->SetPoint(this);
		}
		catch (_com_error e)
		{
			return e.Error();
		}
		return pEnum->QueryInterface(IID_IEnumConnections, reinterpret_cast<void**>(ppEnum));
	}

	typedef std::list<SINK*> CConnectionList;
	const CConnectionList& GetConnectionList() const { return m_ConnectionList; }

private:
	CConnectionList m_ConnectionList;
};




VRCOM_END
#endif // CONNECTABLE_H_INCLUDED