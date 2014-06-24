/*! 
 *********************************************************************
 *  @file     wtkHandleWrapper.h
 *  @brief    Contains WTK configuration macros
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
#ifndef HANDLEWRAPPER_H_INCLUDED
#define HANDLEWRAPPER_H_INCLUDED

#include "wtkConfig.h"

#pragma warning(disable:4290)	// warning C4290: C++ Exception Specification ignored


WTK_BEGIN()

/*****************************************************************************
 *	Handle manipulation
 *****************************************************************************/
template <typename H, class T, int INVALID, class EXCEPTION>
class HandleWrapper
{
	typedef HandleWrapper<H, T, INVALID, EXCEPTION> _MyType;
	H	m_handle;

public:
	typedef EXCEPTION _ExceptionType;

	static const int _Invalid_Value;
	HandleWrapper() : m_handle(reinterpret_cast<H>(INVALID))
	{
	}

	HandleWrapper(H handle) : m_handle(INVALID)
	{
		*this = handle;
	}
	
	virtual ~HandleWrapper()
	{
		Close();
	}

	operator H () const { return m_handle; }

	_MyType& operator = (H handle)
	{
		Close();
		m_handle = handle;
		return *this;
	}
	void SetHandle(H handle)
	{
		*this = handle;
	}

	bool operator == (int null) const
	{
		_ASSERT(null == INVALID);
		return m_handle == INVALID;
	}

	bool operator != (int null) const
	{
		_ASSERT(null == INVALID);
		return m_handle != INVALID;
	}

	H Detach()
	{
		H ret = m_handle;
		m_handle = INVALID;
		return ret;
	}

	// this may or may not work (for GDI handles, it doesn't work)
	HANDLE Duplicate(bool bInherit = false, HANDLE hTargetProcess = GetCurrentProcess()) const throw (EXCEPTION)
	{
		HANDLE hResult = INVALID;
		if (m_handle != INVALID)
		{
			if (!::DuplicateHandle(
				GetCurrentProcess(),
				m_handle,
				hTargetProcess,
				&hResult,
				0,
				bInherit ? TRUE : FALSE,
				DUPLICATE_SAME_ACCESS))
			{
				_ExceptionType::ThrowSystemError(__FILE__, __LINE__);
			}
		}
		return hResult;
	}

	void Close()
	{
		if (m_handle != reinterpret_cast<H>(INVALID))
		{
			T* pThis = static_cast<T*>(this);
			// please note that DestroyHandle must not be virtual !
			pThis->CloseHandle(m_handle);
			m_handle = reinterpret_cast<H>(INVALID);
		}
	}
};

template <typename H, class T, int INVALID, class EXCEPTION>
const int HandleWrapper<H, T, INVALID, EXCEPTION>::_Invalid_Value = INVALID;

WTK_END()
#endif // HANDLEWRAPPER_H_INCLUDED
