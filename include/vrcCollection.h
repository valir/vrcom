/*! 
 *********************************************************************
 *  @file     vrcCollection.h
 *  @brief    COM Collection classes.
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
#ifndef COLLECTION_H_INCLUDED
#define COLLECTION_H_INCLUDED

#include "vrcConfig.h"
#include "vrcObject.h"

VRCOM_BEGIN

#pragma warning(disable:4355)

/*!
 *  @class CImpEnum 
 *  @brief 
 *  
 *	CImpEnum implement the IEnumVARIANT interface for the collection objects
 *  @param COLLOBJ	the collection implementation object
 *
 * Note: this class is not thread-safe : there is no need for it to be
 *       because it is always used by a single thread into the client side
 *  
 * @see CImpCollection
 */
template <class COLLOBJ>
class CImpEnum : public IEnumVARIANT
{
public:

    /*!
     * Default constructor
     *
     */
	CImpEnum() 
	{
	}


    /*!
     * Receives the collection implementation object
     *
     * @param pParent the pointer to the implementation object
     */
	void SetCollection(COLLOBJ* pParent) 
	{ 
		m_pParent = pParent;
		m_Elements = pParent->GetElements(); 
		Reset();
	}

    /*!
     * This accessor returns IID_IEnumVARIANT.
     *
     */
	virtual REFIID GetUUID() const { return IID_IEnumVARIANT; }


    /*!
     * Implements IEnumVARIANT::Next. For more information see MS Platform SDK.
     *
     * @param celt			number of elements to return
     * @param rgvar			pointer to the VARIANT array where elements should be stored
     * @param pceltFetched	if not NULL, it will hold the number of elements effectively returned
     *
     * @return HRESULT
     */
	STDMETHODIMP Next(unsigned long celt, VARIANT* rgvar, unsigned long* pceltFetched)
	{
		if (pceltFetched)
			*pceltFetched = 0;
		else
			if (celt != 1)
				return E_POINTER;
		
		VARIANT* pv = rgvar;
		for (unsigned long l =0; l<celt; l++, pv++)
			::VariantInit(pv);

		HRESULT hr = S_FALSE;
		pv = rgvar;
		while (celt-- && (m_pos != m_Elements.end()))
		{
			_variant_t v(*pv);
			m_pParent->CopyIntoVariant(v, *m_pos++);
			*pv++ = v.Detach();
			if (pceltFetched)
				(*pceltFetched)++;
			hr = S_OK;
		}

		return hr;
	}

    /*!
     * Implements IEnumVARIANT::Skip
     *
     * @param celt the number of elements to skip
     *
     * @return HRESULT
     */
	STDMETHODIMP Skip(unsigned long celt)
	{
		if ((m_pos + celt) < m_Elements.end())
		{
			m_pos += celt;
			return S_OK;
		}
		return S_FALSE;
	}

    /*!
     * Implements IEnumVARIANT::Reset
     *
     * @return HRESULT 
     */
	STDMETHODIMP Reset()
	{
		m_pos = m_Elements.begin();
		return S_OK;
	}

    /*!
     * Implements IEnumVARIANT::Clone
     *
     * @param ppEnum pointer where IEnumVARIANT pointer of the clone will be stored
     *
     * @return HRESULT
     */
	STDMETHODIMP Clone(IEnumVARIANT** ppEnum)
	{
		typedef CImpEnum<COLLOBJ> CImpEnumType;
		CComObject<CImpEnumType, CheckIID>* pClone = NULL;

		try
		{
			pClone = pClone->CreateInstance(_Module::GetSingleton());
			((CImpEnumType*)(*pClone))->m_Elements = m_Elements;
			((CImpEnumType*)(*pClone))->m_pos = m_pos;
		}
		catch (_com_error e)
		{
			return e.Error();
		}
		return pClone->QueryInterface(IID_IEnumVARIANT, reinterpret_cast<void**>(ppEnum));
	}

protected:

	/*!
	 *  @class CheckIID
	 *  @brief Helper class for IID comparison.
	 *  
	 *  
	 *  @see Clone
	 */
	class CheckIID
	{
	public:
		bool operator() (REFIID riid) const { return (riid == IID_IEnumVARIANT) ? true : false; }
	};

private:
	COLLOBJ::container_type		m_Elements;
	COLLOBJ::container_type::iterator	m_pos;
	COLLOBJ*					m_pParent;
};

/*!
 *  @class CImpCollection 
 *  @brief Static collection implementation.
 *  
 *  This class can be used to implement the members of your COM guidelines compliant collection interfaces.
 *  The guidelines state that a (static) collection interface should be dual and should have the following members :
 *  <CODE><BR>[propget, id(DISPID_NEWENUM), helpstring("Element count")] HRESULT Count([out, retval] long *pVal);<BR>
 *  [propget, restricted, helpstring("property _NewEnum")] HRESULT _NewEnum([out, retval] IUnknown* *pVal);</CODE>
 *  
 *  @param CONT	container implementation class
 *  @param COMP COM component class
 *  @param ITEM element class
 *  @param BASE base collection interface
 *
 *  @see CImpEnum
 */
template <class CONT, class COMP, typename ITEM, class BASE>
class CImpCollection : public CImpDualInterface<BASE, COMP>
{
public:
	typedef CONT container_type;

	CImpCollection(COMP* pOuter, CONT& rElementList) : 
	  m_ElementList(rElementList),
	  CImpDualInterface<BASE, COMP>(pOuter)
	{
	}
	~CImpCollection()
	{
	}

	CONT& GetElements() { return m_ElementList; }

	STDMETHODIMP get_Count(long* pCount) 
	{ 
		Lock();
		*pCount = m_ElementList.size(); 
		Unlock();
		return S_OK;
	}
	STDMETHODIMP get__NewEnum(IUnknown** pUnkEnum)
	{
		typedef CImpEnum<CImpCollection<CONT, COMP, ITEM, BASE> > CImpEnumType;

		CComObject<CImpEnumType, CImpEnumType::CheckIID>* pEnumObj = NULL;
		// we will aggregate this object, since it is enumerating our data
		try
		{
			pEnumObj = pEnumObj->CreateInstance(GetComponent()->GetModule());
			((CImpEnumType*)(*pEnumObj))->SetCollection(this);
		}
		catch (_com_error e)
		{
			return e.Error();
		}
		return pEnumObj->QueryInterface(IID_IUnknown, reinterpret_cast<void**>(pUnkEnum));
	}
	STDMETHODIMP get_Item(VARIANT varIndex, ITEM* pItem)
	{
		return _Item(varIndex, pItem);
	}

	virtual LPCTSTR GetTracingAlias() const { return _T("CImpCollection"); }
	virtual void CopyIntoVariant(_variant_t& v, container_type::value_type& elem) =0;


protected:
	template <typename ITEM> HRESULT _Item(VARIANT varIndex, ITEM* ppItem)
	{
		if (ppItem == NULL)
			return E_POINTER;
		*ppItem = NULL;
		long index = CheckIndex(varIndex);
		if (index < 0)
			return E_INVALIDARG;
		Lock();
		*ppItem = m_ElementList[index];
		(*ppItem)->AddRef();
		Unlock();
		return S_OK;
	}

	long CheckIndex(VARIANT& varIndex)
	{
		_variant_t v;
		long index = 0;
		if (SUCCEEDED(::VariantChangeType(&v, &varIndex, 0, VT_I4)))
			index = V_I4(&v);

		// the third commandement states that indexing with Count starts with 1
		if ((--index <0) || (index >= m_ElementList.size()))
			return -1;
		return index;
	}
protected:
	CONT&	m_ElementList;
};

/*****************************************************************************
 *	_DynamicCollection implementation
	[
		uuid(fa64f204-f9c8-11d2-8460-0060b0541558),
		helpstring("Dynamic collection"),
		object, restricted
	]
	interface _DynamicCollection : _Collection
	{
	};
 *****************************************************************************/
/*!
 *  @class CImpDynamicCollection 
 *  @brief Dynamic collection implementation.
 *  
 *  This class can be used to implement the members of your COM guidelines compliant collection interfaces.
 *  The guidelines state that a (static) collection interface should be dual and should have the following members :
 *  <CODE><BR>[helpstring("Add an include directory")] HRESULT Add([in] VARIANT Dir);<BR>
 *  [helpstring("Remove a directory from the list")] HRESULT Remove([in] VARIANT Index);</CODE>
 *
 *  Also, please note that a dynamic collection is also a static collection (CImpCollection).
 *
 *  @param CONT container implementation class
 *  @param COMP COM component class
 *  @param ITEM element class
 *  @param BASE base collection interface
 *
 *  @see CImpCollection
 */
template <class CONT, class COMP, typename ITEM, class BASE>
class CImpDynamicCollection : public CImpCollection<CONT, COMP, ITEM, BASE>
{
public:
	CImpDynamicCollection(COMP* pOuter, CONT& rElementList) : 
	  CImpCollection<CONT, COMP, ITEM, BASE>(pOuter, rElementList) {}

	STDMETHODIMP Add(VARIANT varNew)
	{
		if (V_VT(&varNew) == VT_EMPTY)
			return E_INVALIDARG;
		Lock();
		_variant_t v(varNew);
		CONT::value_type elem = v;
		m_ElementList.push_back(elem);
		Unlock();
		return S_OK;
	}
	STDMETHODIMP Add(ITEM newItem)
	{
		CHECK_POINTER(newItem);
		Lock();
		m_ElementList.push_back(newItem);
		Unlock();
		return S_OK;
	}
	STDMETHODIMP Add(IDispatch* pNewItem)
	{
		CHECK_POINTER(pNewItem);
		Lock();
		m_ElementList.push_back(pNewItem);
		Unlock();
		return S_OK;
	}
	STDMETHODIMP Remove(VARIANT varIndex)
	{
		long index = CheckIndex(varIndex);
		if (index < 0)
			return E_INVALIDARG;
		Lock();
		m_ElementList.erase(m_ElementList.begin() + index);
		Unlock();
		return S_OK;
	}
	
	virtual LPCTSTR GetTracingAlias() const { return _T("CImpDynamicCollection"); }
};

/*! @def VRCOM_BEGIN_DYNAMIC_COLLECTION
 * @brief Starts collection interface implementation
 *
 * Use this macro inside COM component class declaration if this component implements
 * a collection compliant interface. It expands to a CImpDynamicCollection nested class declaration.
 * Use VRCOM_END_DYNAMIC_COLLECTION to end nested class declaration.
 *
 * @param impName	the name you want your nested class to have
 * @param intName	the dynamic collection interface
 * @param intElem	the collection element type, as given into the IDL
 * @param Cont		the container implementation class 
 * @param Comp		the component class where this class is nested (outer class)
 * @param Elems		the name of the outer class collection member
 *
 * @see CImpDynamicCollection, VRCOM_END_DYNAMIC_COLLECTION
 */
/*! @def VRCOM_END_DYNAMIC_COLLECTION
 * @brief Ends collection interface implementation
 *
 * Use this macro inside COM component class declaration, in conjunction with VRCOM_BEGIN_DYNAMIC_COLLECTION.
 *
 * @param impName	the same as the one you used for VRCOM_BEGIN_DYNAMIC_COLLECTION
 * 
 * @see VRCOM_BEGIN_DYNAMIC_COLLECTION, CImpDynamicCollection
 */
/*! \def VRCOM_COPY_OBJECT
 * Helper macro used to copy a collection element into a variant
 */
#define VRCOM_BEGIN_DYNAMIC_COLLECTION(impName, intName, intElem, Cont, Comp, Elems) \
private: \
	class impName : public CImpDynamicCollection<Cont, Comp, intElem, intName> \
	{ \
		typedef CImpDynamicCollection<Cont, Comp, intElem, intName> _Base; \
	public: \
		impName() : \
			_Base( \
				(Comp*)((char*)this - offsetof(Comp, m_ ## impName)), \
				((Comp*)((char*)this - offsetof(Comp, m_ ## impName)))-> ##Elems   ) {}

#define VRCOM_END_DYNAMIC_COLLECTION(impName) \
	}; \
	impName	m_ ##impName; \
	friend class impName; 

#define VRCOM_COPY_OBJECT() \
	virtual void CopyIntoVariant(_variant_t& v, container_type::value_type& elem) \
	{ \
		v = (IDispatch*)elem; \
	} 

#pragma warning(default:4355)

VRCOM_END
#endif // COLLECTION_H_INCLUDED