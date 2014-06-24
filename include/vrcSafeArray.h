/*! 
 *********************************************************************
 *  @file     vrcSafeArray.h
 *  @brief    Ole automation safe array manipulation
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
#ifndef SAFEARRAY_H_INCLUDED
#define SAFEARRAY_H_INCLUDED

#include "vrcConfig.h"

#pragma warning(disable:4290)	// warning C4290: C++ Exception Specification ignored

VRCOM_BEGIN

//using namespace std;

/*!
 *  @class SafeArray
 *  @brief Encapsulate the SAFEARRAY automation type
 *  
 *  
 *  @see 
 */
template <class T, VARTYPE VT>
class SafeArray
{
public:
	SafeArray(SAFEARRAY* pArray) throw(_com_error) : m_bOwnArray(false)
	{
		Attach(pArray);
	}
	SafeArray(const VARIANT& var) throw (_com_error) : m_bOwnArray(false), m_pBounds(NULL)
	{
		_ASSERTE((V_VT(&var) & VT_ARRAY) == VT_ARRAY);
		Attach(V_ARRAY(&var));
	}
	SafeArray(unsigned int cDims =1) throw (_com_error) : m_bOwnArray(true)
	{
		// if this asserts, please use the constructor which takes the supplemental arg
		_ASSERTE((VT != VT_RECORD) && (VT != VT_DISPATCH) && (VT != VT_UNKNOWN));
		_ASSERTE(cDims == 1);	// single dimmension arrays, please
		for (unsigned int i=0; i<cDims; i++)
			ZeroMemory(&m_rgBounds[i], sizeof(SAFEARRAYBOUND));
		if (NULL == (m_pArray = ::SafeArrayCreate(VT, cDims, m_rgBounds)))
			_THROW_HR(E_OUTOFMEMORY);
	}
	SafeArray(IRecordInfo* pRecInfo, unsigned int cDims =1) throw (_com_error) : m_bOwnArray(true)
	{
		_ASSERTE(pRecInfo != NULL);
		_ASSERTE(cDims == 1);	// single dimmension arrays, please
		for (unsigned int i=0; i<cDims; i++)
			ZeroMemory(&m_rgBounds[i], sizeof(SAFEARRAYBOUND));
		if (NULL == (m_pArray = ::SafeArrayCreateEx(VT, cDims, m_rgBounds, pRecInfo)))
			_THROW_HR(E_OUTOFMEMORY);
	}
	~SafeArray()
	{
		if (m_bOwnArray)
			_THROW_HR(::SafeArrayDestroy(m_pArray));
		m_pArray = NULL;
	}

	inline T __fastcall GetAt(long lIndex) const throw (_com_error)
	{
		T* pData;
		_THROW_HR(::SafeArrayPtrOfIndex(m_pArray, &lIndex, reinterpret_cast<void HUGEP**>(&pData)));
		return *pData;
	}
	T operator [] (long lIndex) const  throw (_com_error) { return GetAt(lIndex); }
	operator SAFEARRAY* () const { return m_pArray; }
	SAFEARRAY* Detach()
	{
		_ASSERTE(m_pArray != NULL);
		SAFEARRAY* pArray = m_pArray;
		m_pArray = NULL;
		return pArray;
	}
	void Attach(SAFEARRAY* pArray) throw (_com_error)
	{
		_ASSERTE(pArray != NULL);
		_ASSERTE(1 == ::SafeArrayGetDim(pArray));	// only one-dimmensionnal arrays, please
		
		VARTYPE pType;
		_THROW_HR(::SafeArrayGetVartype(pArray, &pType));
		if (pType != VT)
			_THROW_HR(E_INVALIDARG);

		m_pArray = pArray;
		m_rgBounds[0].cElements = UBound() - LBound() +1;
		m_rgBounds[0].lLbound = LBound();
	}
	operator _variant_t () const throw (_com_error)
	{
		VARTYPE vt;
		_THROW_HR(::SafeArrayGetVartype(m_pArray, &vt));
		_variant_t varResult;
		V_VT(&varResult) = VT_ARRAY | VT_BYREF | vt;
		V_ARRAYREF(&varResult) = const_cast<SAFEARRAY**>(&m_pArray);
		return varResult;
	}
	__inline long UBound(int nDim =1) const  throw (_com_error)
	{
		long lBound;
		_THROW_HR(::SafeArrayGetUBound(m_pArray, nDim, &lBound));
		return lBound;
	}
	__inline long LBound(int nDim = 1) const  throw (_com_error)
	{
		long lBound;
		_THROW_HR(::SafeArrayGetLBound(m_pArray, nDim, &lBound));
		return lBound;
	}
	inline void SetAt(long lIndex, T val) throw(_com_error)
	{
		// ici, il y a encore un peu de travail à faire ...
		if (lIndex > UBound())
		{
			++(m_rgBounds[0].cElements);
			_THROW_HR(::SafeArrayRedim(m_pArray, m_rgBounds));
		}
		lIndex = UBound();
		_THROW_HR(::SafeArrayPutElement(m_pArray, &lIndex, &val));
	}
	inline void Append(T val) throw (_com_error) {	SetAt(UBound() +1, val);	}

private:
	SAFEARRAY*		m_pArray;
	SAFEARRAYBOUND	m_rgBounds[1];	// this version supports only one-dimmensionnale arrays
	bool			m_bOwnArray;
};


/*!
 *  @class safearray
 *  @brief Another SAFEARRAY encapsulation, but STL style
 *  
 *  this class is not yet finished
 *  
 *  @see 
 */
template <typename T, class A=allocator<T> >
class safearray
{
public:
	typedef A					allocator_type;
	typedef A::size_type		size_type;
    typedef A::difference_type	difference_type;
    typedef A::reference		reference;
    typedef A::const_reference	const_reference;
    typedef A::value_type		value_type;
    typedef A::pointer			iterator;
    typedef A::const_pointer	const_iterator;
    
	typedef std::reverse_iterator<iterator>		reverse_iterator;
    
	typedef std::reverse_iterator<const_iterator>		const_reverse_iterator;
    
	explicit safearray(const A& al = A()) : m_pArray(NULL) {};
    explicit safearray(size_type n, const T& v = T(), const A& al = A());
    safearray(const safearray& x);
    safearray(const_iterator first, const_iterator last, const A& al = A());
    void reserve(size_type n)
	{

	}
    size_type capacity() const;
    iterator begin();
    const_iterator begin() const;
    iterator end();
    iterator end() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    void resize(size_type n, T x = T());
    size_type size() const;
    size_type max_size() const;
    bool empty() const;
    A get_allocator() const;
    reference at(size_type pos);
    const_reference at(size_type pos) const;
    reference operator[](size_type pos);
    const_reference operator[](size_type pos);
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    void push_back(const T& x);
    void pop_back();
    void assign(const_iterator first, const_iterator last);
    void assign(size_type n, const T& x = T());
    iterator insert(iterator it, const T& x = T());
    void insert(iterator it, size_type n, const T& x);
    void insert(iterator it,
        const_iterator first, const_iterator last);
    iterator erase(iterator it);
    iterator erase(iterator first, iterator last);
    void clear();
    void swap(safearray x);
protected:
    A				allocator;
	SAFEARRAY*		pArray;
	SAFEARRAYBOUND	rgBounds[1];	
};

VRCOM_END

#pragma warning(default:4290)	// warning C4290: C++ Exception Specification ignored

#endif // SAFEARRAY_H_INCLUDED