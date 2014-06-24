/*! 
 *********************************************************************
 *  @file     vrcPersistStream.h
 *  @brief    This file contains implementation for IPersistSteram 
 *            interface.
 *
 *	          A SmartStream class is provided for Property serialization.
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

#ifndef PERSISTSTREAM_H_INCLUDED
#define PERSISTSTREAM_H_INCLUDED


#include "vrcConfig.h"

#pragma warning(disable:4290)

VRCOM_BEGIN

/*!
 *  @class CImpPersistStream
 *  @brief IPersistStream implementation for CComponent derived classes
 *  
 *  This class can be used to implement IPersistStream interface of some
 *  COM component. This implementation delegates to CComponent derived class
 *  
 *  @see CComponent
 */
template <class COMP>
class CImpPersistStream : public CImpCompInterface<IPersistStream, COMP>
{
	typedef CImpCompInterface<IPersistStream, COMP> _Base;
public:
	CImpPersistStream(COMP* pComp) : _Base(pComp) {}

    virtual HRESULT STDMETHODCALLTYPE GetClassID( 
        /* [out] */ CLSID __RPC_FAR *pClassID)
	{
		CHECK_POINTER(pClassID);
		*pClassID = GetComponent()->GetCLSID();
		return S_OK;
	}

    virtual HRESULT STDMETHODCALLTYPE IsDirty( void)
	{
		return GetComponent()->IsDirty();
	}
    
    virtual HRESULT STDMETHODCALLTYPE Load( 
        /* [unique][in] */ IStream __RPC_FAR *pStm)
	{
		return GetComponent()->Load(pStm);
	}
    
    virtual HRESULT STDMETHODCALLTYPE Save( 
        /* [unique][in] */ IStream __RPC_FAR *pStm,
        /* [in] */ BOOL fClearDirty)
	{
		return GetComponent()->Save(pStm, fClearDirty);
	}
    
    virtual HRESULT STDMETHODCALLTYPE GetSizeMax( 
        /* [out] */ ULARGE_INTEGER __RPC_FAR *pcbSize)
	{
		return GetComponent()->GetSizeMax(pcbSize);
	}
};

/*!
 *  @class SmartStream
 *  @brief Generalization for a Property stream
 *  
 *  This stream implementation is specially designed for Property
 *  serialization.
 *  
 *  @see 
 */
template <class STM>
class SmartStream
{
	typedef SmartStream<STM> _MyType;
public:
	SmartStream(STM* pSTM) : m_pstm(pSTM)
	{
		_ASSERTE(pSTM != NULL);
		pSTM->AddRef();
	}
	~SmartStream()
	{
		m_pstm->Release();
	}


	operator STM* () const { return m_pstm; }
	STM* operator -> () const { return m_pstm; }

	template <typename T>
	const _MyType& operator << (const Property<T>& val) const throw (_com_error)
	{
		_Write((const T&)val);
		return *this;
	}

	const _MyType& operator << (const byte& val)  const throw (_com_error){_Write(val); return *this;}
	const _MyType& operator << (const bool& val)  const throw (_com_error){_Write(val); return *this;}
	const _MyType& operator << (const short& val) const throw (_com_error){_Write(val); return *this;}
	const _MyType& operator << (const long& val)  const throw (_com_error){_Write(val); return *this;}
	const _MyType& operator << (const unsigned int& val)  const throw (_com_error){_Write(val); return *this;}
private:
	template <typename T>
	void _Write(const T& val) const throw(_com_error)
	{
		_THROW_HR(m_pstm->Write(&val, sizeof(T), NULL));
	}

	template <>
	void _Write(const _bstr_t& str) const throw(_com_error)
	{
		unsigned int len = str.length();
		_Write(len);
		if (len >0)
			_THROW_HR(m_pstm->Write((wchar_t*)str, str.length()*sizeof(wchar_t), NULL));
	}

public:
	template <typename T>
	const _MyType& operator >> (Property<T>& dest) const throw (_com_error)
	{
		T input;
		_Read(input);
		dest = input;
		return *this;
	}

	const _MyType& operator >> (byte& val)  const throw (_com_error){_Read(val); return *this;}
	const _MyType& operator >> (bool& val)  const throw (_com_error){_Read(val); return *this;}
	const _MyType& operator >> (short& val)  const throw (_com_error){_Read(val); return *this;}
	const _MyType& operator >> (long& val)  const throw (_com_error){_Read(val); return *this;}
	const _MyType& operator >> (unsigned int& val)  const throw (_com_error){_Read(val); return *this;}

	void RawRead(void* pv, ULONG toRead) const
	{
		while (toRead)
		{
			ULONG cbRead = 0;
			_THROW_HR(m_pstm->Read(pv, toRead, &cbRead));
			reinterpret_cast<byte*&>(pv) += cbRead;
			toRead -= cbRead;
		}
	}

private:
	template <typename T>
	void _Read(T& dest) const
	{
		RawRead(&dest, sizeof(T));
	}

	template <>
	void _Read(_bstr_t& dest) const
	{
		unsigned int len =0;
		_Read(len);
		if (len >0)
		{
			BSTR psz = ::SysAllocStringLen(NULL, len);
			if (NULL == psz)
				_THROW_HR(E_OUTOFMEMORY);
			RawRead(psz, len*sizeof(TCHAR));
			dest = _bstr_t(psz, false);
		}
	}

private:
	STM*	m_pstm;
};

VRCOM_END
#endif // PERSISTSTREAM_H_INCLUDED