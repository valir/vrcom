/*! 
 *********************************************************************
 *  @file     vrcUDT.h
 *  @brief    Ole automation User Defined Type manipulation
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
#ifndef UDT_H_INCLUDED
#define UDT_H_INCLUDED

#include "vrcConfig.h"
#include "vrcSafeArray.h"

#pragma warning(push)
#pragma warning(disable:4290)	// 

VRCOM_BEGIN


/*!
 *  @class UDT
 *  @brief Ole automation User Defined Type manipulation
 *  
 *  
 *  @see 
 */
template <typename T>
class UDT
{
public:
	UDT(ITypeLib* pTypeLib =NULL) throw (_com_error)
	{
		if (NULL == pTypeLib)
			pTypeLib = CComSrv::GetTypeLib();
		ITypeInfoPtr spTypeInfo;
		_THROW_HR(pTypeLib->GetTypeInfoOfGuid(__uuidof(T), &spTypeInfo));
		_THROW_HR(::GetRecordInfoFromTypeInfo(spTypeInfo, &m_spRecordInfo));
	}
	UDT(REFGUID rGuidTypeLib, ULONG uVerMajor, ULONG uVerMinor)
	{
		_ASSERTE(0);
	}

	operator IRecordInfo* () 
	{ 
		m_spRecordInfo.AddRef();
		return m_spRecordInfo; 
	}
	IRecordInfo* operator -> () { return m_spRecordInfo; }


private:
	IRecordInfoPtr	m_spRecordInfo;
};

/*!
 *  @class UDTInst
 *  @brief UDT instance
 *  
 *  
 *  @see UDT
 */
template <class T>
class UDTInst
{
public:
	UDTInst() throw (_com_error)
	{
		if (NULL == (m_pInst = m_Udt->RecordCreate()))
			_THROW_HR(E_OUTOFMEMORY);
	}
	~UDTInst()
	{
		if (m_pInst)
			_THROW_HR(m_Udt->RecordDestroy(m_pInst));
		m_pInst = NULL;
	}
	
	void CopyInto(T* pDest)
	{
		_THROW_HR(m_Udt->RecordCopy(m_pInst, pDest));
	}
	void CopyInto(_variant_t& var)
	{
		var.Clear();
		V_VT(&var) = VT_RECORD;
		V_RECORDINFO(&var) = m_Udt;
		if (NULL == (V_RECORD(&var) = m_Udt->RecordCreate()))
			_THROW_HR(E_OUTOFMEMORY);
		CopyInto(V_RECORD(&var));
	}

	UDTInst<T>& operator = (T& right)
	{
		_THROW_HR(m_Udt->RecordCopy(&right, m_pInst));
	}

	T* operator -> () { return &m_Inst; }
	operator const T& () const { return m_Inst; }
	T* Detach()
	{
		T* pInst = m_pInst;
		m_pInst = NULL;
		return pInst;
	}

private:
	T*				m_pInst;
	static UDT<T>	m_Udt;
};

// static member
template <class T> UDT<T> UDTInst<T>::m_Udt;

/*!
 *  @class UDTSafeArray 
 *  @brief SAFEARRAY of UDT
 *  
 *  
 *  @see 
 */
template <class T>
class UDTSafeArray : public SafeArray<T, VT_RECORD>
{
	typedef SafeArray<T, VT_RECORD> _Base;
public:
	UDTSafeArray() : _Base(UDT<T>()) {}
	UDTSafeArray(SAFEARRAY* pArray) : _Base(pArray) {}
	UDTSafeArray(const VARIANT& varArray) : _Base(varArray) {}
};

VRCOM_END

#pragma warning(pop)

#endif // UDT_H_INCLUDED