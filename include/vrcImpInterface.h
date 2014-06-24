/*! 
 *********************************************************************
 *  @file     vrcImpInterface.h
 *  @brief    Basic interfaces implementation
 *
 * This file contains the infrastructure needed to implement COM objects
 * and their interfaces. 
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
#ifndef IMPINTERFACE_H_INCLUDED
#define IMPINTERFACE_H_INCLUDED

#include "vrcConfig.h"
#include "vrcTracer.h"
#include "vrcLock.h"

#pragma warning(disable:4355)
#pragma warning(disable:4290)	// C++ Exception Specification ignored

VRCOM_BEGIN

class ModuleBase;


/*!
 *  @class CThreaded 
 *  @brief Synchronized base class
 *
 *  Parent class for objects wich need to protect concurent access to members.
 *	This class uses internally CGenericLock
 *
 *	@see CGenericLock
 */
class VRCOM_EXP CThreaded
{
public:
	CThreaded();
	virtual ~CThreaded();

	// MTA locking support
	void Lock() const { m_pLock->Lock(); }
	void Unlock() const { m_pLock->Unlock(); }

	static bool m_bMultiThreaded;

	static CGenericLock* CreateLockObject();

private:
	mutable CGenericLock*	m_pLock;
};

/*!
 *  @class CImpGeneric 
 *  @brief CImpInterface template virtual base class
 *
 *  This class defines the signature for a interface implementation class.
 *   
 *  @see CThreaded
 *	@see CTracerBase
 *	@see CImpInterface
 */
class VRCOM_EXP CImpGeneric : virtual public CThreaded, 
							  public CTracerBase
{
public:
	CImpGeneric() {}
	virtual ~CImpGeneric() {}

	virtual bool HasIID(REFIID riid) const = 0;
	virtual bool GetSupportErrorInfo() const { return false; }
	virtual void Init(ModuleBase* pModule) {}

protected:
	virtual IUnknown* GetOuterUnknown() const = 0;
};

/*!
 *  @class CCheckIID
 *  @brief operator class for comparing IIDs
 *
 *  @see CImpInterface
 */
template <class INT>
struct CCheckIID : public std::unary_function<INT, bool>
{
	bool operator ()(REFIID iid) const { return (iid == __uuidof(INT)) ? true : false; }
};

/*!
 *  @class CCheckIID2
 *  @brief operator class for comparing IIDs
 *
 *  @see CImpInterface
 */
template <class INT1, class INT2>
struct CCheckIID2 : public std::unary_function<INT, bool>
{
	bool operator ()(REFIID iid) const { return ((iid == __uuidof(INT1)) || (iid == __uuidof(INT2))) ? true : false; }
};

/*!
 *  @class CCheckIID3
 *  @brief operator class for comparing IIDs
 *
 *  @see CImpInterface
 */
template <class INT1, class INT2, class INT3>
struct CCheckIID3 : public std::unary_function<INT, bool>
{
	bool operator ()(REFIID iid) const { return ((iid == __uuidof(INT1)) || (iid == __uuidof(INT2)) || (iid == __uuidof(INT3))) ? true : false; }
};

/*!
 *  @class CImpInterface
 *  @brief Single nested interface implementation
 *  @param INT The implemented interface's type
 *  @param HOLDER Outer class type
 *  @param IIDHOLDER IID comparing class
 *  
 *  Interface implementations are nested into a CImpInnerUnknown or a CComObject and
 *  inherit from this class. It implements IUnknown interface.
 *  
 *  @see CImpInnerUnknown
 *  @see CComObject
 */
template <class INT, class HOLDER, class IIDHOLDER =CCheckIID<INT> >
class CImpInterface :	public INT,
						public CImpGeneric
{
public:
	typedef IIDHOLDER _iidHolder;

	explicit CImpInterface(HOLDER* pHolder) : m_pHolder(pHolder) 
	{
		// this class is designed to be nested !
		assert(pHolder != NULL);
	}

	virtual bool HasIID(REFIID riid) const 
	{ 
		static _iidHolder iidHolder;
		return iidHolder(riid); 
	}
	HOLDER* GetHolder() const { return m_pHolder; }

	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) 
	{ 
#ifdef _DEBUG
		LPOLESTR pszInterface = FindInterfaceName(riid);
	#ifdef _UNICODE
		Trace(TRACE_QI, _T("QueryInterface %s\n"), pszInterface);
	#else
		Trace(TRACE_QI, _T("QueryInterface %S\n"), pszInterface);
	#endif
		CoTaskMemFree(pszInterface);
#endif
		return GetOuterUnknown()->QueryInterface(riid, ppvObject); 
	}
	STDMETHODIMP_(ULONG) AddRef() 
	{ 
		Trace(TRACE_REFCOUNT, _T("AddRef -> "));
		return GetOuterUnknown()->AddRef(); 
	}
	STDMETHODIMP_(ULONG) Release() 
	{ 
		Trace(TRACE_REFCOUNT, _T("Release -> "));
		return GetOuterUnknown()->Release(); 
	}

protected:
	virtual IUnknown* GetOuterUnknown() const { return m_pHolder->GetOuterUnknown(); }

private:
	HOLDER*	m_pHolder;
};

/*!
 * Property assignement
 *
 *
 * @param dest : left operand
 * @param src : right operand
 *
 * @return void 
 */
template <typename DST, typename SRC>
inline void CopyProperty(DST& dest, const SRC& src)
{ 
	dest = src; 
}


/*!
 * This specilization copies a _bstr_t into a new BSTR
 *
 * @param dest :
 * @param src : 
 *
 * @return template <>  : 
 */
template <> inline void CopyProperty(BSTR& dest, const _bstr_t& src)
{
	dest = src.copy();
}


/*!
 * This specialization copies a _bstr_t into a null terminated string
 *
 * @param ppDest : 
 * @param src : 
 *
 * @return template <>  : 
 */
template <> inline void CopyProperty(unsigned char*& ppDest, const _bstr_t& src)
{
	char* pSrc = src;
	ppDest = reinterpret_cast<unsigned char*>(CoTaskMemAlloc(strlen(pSrc) +1));
	if (NULL == ppDest)
		::_com_issue_error(E_OUTOFMEMORY);
	strcpy((char*)ppDest, pSrc);
}


/*!
 *  @class Property
 *  @brief Thread safe property management
 *  
 *  This class must be used when implementing COM object properties.
 *  
 *  @see CopyProperty
 */
template <typename T>
class Property
{
	typedef Property<T> _MyType;
public:
	Property()
	{
		m_pLock = CThreaded::CreateLockObject();
	}
	virtual ~Property()
	{
		if (m_pLock)
			delete m_pLock, m_pLock = NULL;
	}

	__inline Lock() const throw() { m_pLock->Lock(); }
	__inline Unlock() const throw() { m_pLock->Unlock(); }


	template <typename S, class INT>
	HRESULT Put(const S& that, const INT* pInterface) throw()
	{
		_ASSERTE(pInterface != NULL);
		Lock();
		try
		{
			CopyProperty<T, S>(m_Value, that);
		}
		catch (_com_error e)
		{
			Unlock();
			return pInterface->VerifyError(e.Error());
		}
		catch (...)
		{
			Unlock();
			return pInterface->VerifyError(E_FAIL);
		}
		Unlock();
		return S_OK;
	}

	template <typename D, typename INT>
	HRESULT Get(D* pDest, const INT* pInterface) const throw()
	{
		_ASSERTE(pInterface != NULL);
		// don't bother if pDest is buggy !
		if (NULL == pDest)
			return E_POINTER;
		//*pDest = NULL;

		Lock();
		try
		{
			CopyProperty<D, T>(*pDest, m_Value);
		}
		catch (_com_error e)
		{
			Unlock();
			return pInterface->VerifyError(e.Error());
		}
		catch (...)
		{
			Unlock();
			return pInterface->VerifyError(E_FAIL);
		}
		Unlock();
		return S_OK;
	}

	_MyType& operator = (const T& that) 
	{ 
		m_pLock->Lock();
		try
		{
			m_Value = that; 
		}
		catch (...)
		{
			m_pLock->Unlock();
			throw;
		}
		m_pLock->Unlock();
		return *this;
	}
	operator T () const 
	{
		T result;
		m_pLock->Lock();
		try
		{
			result = m_Value;
		}
		catch (...)
		{
			m_pLock->Unlock();
			throw;
		}
		m_pLock->Unlock();
		return result;
	}

	operator const T& () const
	{
		return m_Value;
	}

private:
	T	m_Value;
	CGenericLock*	m_pLock;
};


/*!
 *  @class CImpCompInterface 
 *  @brief Interface implementation for a coclass
 *  @param INT is the type of the implemented interface
 *  @param COMP is the type of the component implementation
 *  @param IIDHOLDER is the IID comparing class
 *  
 *  Use this template when implementing component interfaces with error handling
 *  
 *  @see 
 */
template <class INT, class COMP, class IIDHOLDER =CCheckIID<INT> >
class CImpCompInterface : public CImpInterface<INT, COMP, IIDHOLDER>
{
public:
	CImpCompInterface(COMP* pComponent) : 
	  CImpInterface<INT, COMP, IIDHOLDER>(pComponent)
	{
		pComponent->AddInterface(this);
	}

	COMP* GetComponent() const { return static_cast<COMP*>(GetHolder()); }

	// the component default is to respond to ISupportErrorInfo
	virtual bool GetSupportErrorInfo() const { return true;	}

	__inline HRESULT VerifyError(HRESULT hr) const
	{
		if (FAILED(hr) && GetSupportErrorInfo())
		{
			VerifyFailError(hr);
		}
		return hr;
	}


private:
	void VerifyFailError(HRESULT hr) const
	{
		Trace(TRACE_ERRORS, _T("VerifyResult : preparing IErrorInfo on hr=%lX !\n"), hr);

		IErrorInfoPtr		spErrInfo;
		ICreateErrorInfoPtr spCreateErr;

		if (SUCCEEDED(::CreateErrorInfo(&spCreateErr)))
		{
			spCreateErr->SetGUID(__uuidof(INT));
			
			LPOLESTR lpsz;
			ProgIDFromCLSID(GetComponent()->GetCLSID(), &lpsz);
			spCreateErr->SetSource(lpsz);
			CoTaskMemFree(lpsz);

			_com_error e(hr);
			spCreateErr->SetDescription((LPOLESTR)e.ErrorMessage());
			try
			{
				spErrInfo = spCreateErr;
				SetErrorInfo(0, spErrInfo);
			}
			catch (...)
			{
				Trace(TRACE_ERRORS, _T("Fatal exception while preparing ErrorInfo\n"));
			}
		}
		else
			Trace(TRACE_ERRORS, _T("CreateErrorInfo failed\n"));

		// enfin, si l'objet travaille sous MTS, annule sa transaction
		/*
		if (pThis->m_spObjectContext)
			pThis->m_spObjectContext->SetAbort();
		*/
	}
};

/*!
 *  @class CImpDualInterface 
 *  @brief Dual interface implementation for a coclass
 *  @param INT is the implemented interface type
 *  @param COMP is the coclass implementation type
 *  @param IIDHOLDER is the IID comparing operator class
 *  
 *  Use this class to implement a IDispatch descendant interface. It implements
 *  all IDispatch members.
 *  
 *  @see 
 */
template <class INT, class COMP, class IIDHOLDER =CCheckIID<INT> >
class CImpDualInterface : public CImpCompInterface<INT, COMP, IIDHOLDER>
{
	typedef CImpCompInterface<INT, COMP, IIDHOLDER> _Base;
public:
	CImpDualInterface(COMP* pComponent) : _Base(pComponent) {}

	virtual void Init(ModuleBase* pModule)
	{
		pModule->GetTypeLib()->GetTypeInfoOfGuid(__uuidof(INT), &m_pTypeInfo);
		_ASSERTE(m_pTypeInfo != NULL);
	}

	virtual bool HasIID(REFIID riid) const 
	{ 
		// force the recognition of IID_IDispatch here 
		if (riid == IID_IDispatch)
			return true;
		return _Base::HasIID(riid);
	}

	// IDispatch standard MTA implementation
	STDMETHODIMP GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid)
	{
		return ::DispGetIDsOfNames(m_pTypeInfo, rgszNames, cNames, rgdispid);
	}
	STDMETHODIMP Invoke(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS * pdispparams, VARIANT * pvarResult, EXCEPINFO * pexcepinfo, UINT * puArgErr)
	{
		return DispInvoke(
			this, m_pTypeInfo,
			dispidMember, wFlags, pdispparams,
			pvarResult, pexcepinfo, puArgErr);
	}
	STDMETHODIMP GetTypeInfoCount(UINT FAR* pctinfo)
	{
		*pctinfo = 1;
		return NOERROR;
	}
	STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo FAR* FAR* ppTInfo)
	{
		*ppTInfo = NULL;

		if(iTInfo != 0)
			return ResultFromScode(DISP_E_BADINDEX);

		m_pTypeInfo->AddRef();        // AddRef and return pointer to cached
										// typeinfo for this object.
		*ppTInfo = m_pTypeInfo;

		return NOERROR;
	}


private:
	ITypeInfoPtr	m_pTypeInfo;
};

/*!
 *  @class CImpInnerUnknown 
 *  @brief COM object (coclass) implementation
 *  @param BASEINT is the base interface type - typically IUnknown
 *  
 *  This class is used as a base class for COM object implementations. It is defined
 *  with BASEINT as a parameter to avoid multiple-inheritance problems.
 *  
 *  @see 
 */
template <class BASEINT =IUnknown>
class CImpInnerUnknown : 
						 public CThreaded,
						 public CTracerBase,
						 public BASEINT // see VC++ 6.0 BUG ID Q168385 into MSDN
{
public:
	explicit CImpInnerUnknown(IUnknown* pOuter) :
		m_cRefs(0), m_pOuter(NULL), m_SupportErrorInfo(this), m_pModule(NULL)
	{
		if (pOuter)
			Trace(TRACE_OBJECT, _T("is aggregated\n"));
		m_pOuter = pOuter ? pOuter : this;
	}
	~CImpInnerUnknown()
	{
		GetModule()->ObjectsDown(this);
	}

	virtual void Init(ModuleBase* pModule)
	{
		m_pModule = pModule;
		GetModule()->ObjectsUp(this);
		CNestedList::iterator pos = m_NestedList.begin();
		while (pos != m_NestedList.end())
		{
			(*pos++)->Init(pModule);
		}
	}

	// IUnknown
	STDMETHODIMP_(ULONG) AddRef() 
	{
		::InterlockedIncrement(&m_cRefs);
		Trace(TRACE_REFCOUNT, _T("AddRef  -> %d\n"), m_cRefs);
		return m_cRefs; 
	}
	STDMETHODIMP_(ULONG) Release()
	{
		assert(m_cRefs != 0);
		Lock();
		--m_cRefs;
		Trace(TRACE_REFCOUNT, _T("Release -> %d\n"), m_cRefs);
		if (0 == m_cRefs)
		{
			Trace(TRACE_REFCOUNT, _T("Deleting...\n"));
			Unlock();
			DeleteThis();
			// this makes sense since 'this' is deleted !
			return 0;
		}
		Unlock();
		return m_cRefs;
	}
	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)
	{
		if (ppvObject == NULL)
			return E_POINTER;

		HRESULT hr = E_NOINTERFACE;
		*ppvObject  = NULL;
		if (riid == IID_IUnknown)
		{
			*ppvObject = m_pOuter;
			AddRef();
			hr = S_OK;
		}
		else
		if (riid == __uuidof(BASEINT))
		{
			*ppvObject = dynamic_cast<IUnknown*>(this);
			AddRef();
			hr = S_OK;
		}
		else
		{
			CNestedList::iterator pos = m_NestedList.begin();
			while (pos != m_NestedList.end())
			{
				CImpGeneric* pImpl = *pos++;
				if (pImpl->HasIID(riid))
				{
					IUnknown* pUnkImpl = dynamic_cast<IUnknown*>(pImpl);
					*ppvObject = pUnkImpl;
					pUnkImpl->AddRef();
					hr = S_OK;
					break;
				}
			}
		}
#ifdef _DEBUG
		LPOLESTR pszInterface = FindInterfaceName(riid);
	#ifdef _UNICODE
		Trace(TRACE_QI, _T("QueryInterface %s : %s\n"), pszInterface, SUCCEEDED(hr) ? _T("OK") : _T("Not implemented"));
	#else
		Trace(TRACE_QI, _T("QueryInterface %S : %s\n"), pszInterface, SUCCEEDED(hr) ? _T("OK") : _T("Not implemented"));
	#endif
		CoTaskMemFree(pszInterface);
#endif
		return hr;
	}
	template <class INT>
	void QueryInterface(INT** ppInt) throw (_com_error)
	{
		_THROW_HR(QueryInterface(__uuidof(INT), reinterpret_cast<void**>(ppInt)));
	}

	// COM error handling
	class CImpSupportErrorInfo : public CImpCompInterface<ISupportErrorInfo, CImpInnerUnknown<BASEINT> >
	{
	public:
		CImpSupportErrorInfo(CImpInnerUnknown<BASEINT>* pComp) :
		  CImpCompInterface<ISupportErrorInfo, CImpInnerUnknown<BASEINT> > (pComp) {}

		STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid)
		{
			CImpInnerUnknown<BASEINT>::CNestedList::iterator pos = GetComponent()->m_NestedList.begin();
			while (pos != GetComponent()->m_NestedList.end())
			{
				CImpGeneric* pImpl = *pos++;
				if (pImpl->HasIID(riid) && 
					pImpl->GetSupportErrorInfo())
				{
					return true;
				}
			}
			return false;
		}
	};

	friend class CImpSupportErrorInfo;


	// internal stuff
	typedef std::list<CImpGeneric*> CNestedList;
	IUnknown* GetOuterUnknown() const { return m_pOuter; }
	const CNestedList& GetNestedList() const { return m_NestedList; }
	ModuleBase* GetModule() const { return m_pModule; }

	void AddInterface(CImpGeneric* pImpl)
	{
		CImpGeneric* pGenericImpl = dynamic_cast<CImpGeneric*>(pImpl);
		m_NestedList.push_back(pImpl);
	}

	virtual void DeleteThis()
	{
		delete this;
	}


private:
	ModuleBase*					m_pModule;
	LONG						m_cRefs;
	IUnknown*					m_pOuter;
	CNestedList					m_NestedList;
	CImpSupportErrorInfo		m_SupportErrorInfo;
};

#pragma warning(default:4355)


VRCOM_END
#endif // IMPINTERFACE_H_INCLUDED