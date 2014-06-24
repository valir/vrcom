/*! 
 *********************************************************************
 *  @file     vrcRegKey.h
 *  @brief    Registry access class
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
#ifndef REGKEY_H_INCLUDED
#define REGKEY_H_INCLUDED

#include "vrcConfig.h"

#pragma warning(disable:4290)

VRCOM_BEGIN

/*!
 *  @class CRegKey
 *  @brief Registry access class
 *  
 *  
 *  @see 
 */
class CRegKey
{
public:
	class Exception
	{
	public:
		Exception(const Exception& other) : m_dwErrorCode(other.m_dwErrorCode)
		{
		}
		Exception(LONG l) : m_dwErrorCode(l) {}
		
		LONG m_dwErrorCode;
	};
	
	CRegKey() : m_hk(NULL) {}
	CRegKey(CRegKey& other)
	{
		m_hk = other.Detach();
	}
	CRegKey(HKEY hkParent, LPCTSTR lpszKeyName = NULL, REGSAM samDesired = KEY_ALL_ACCESS) throw(Exception) :
		m_hk(NULL)
	{
		Open(hkParent, lpszKeyName, samDesired);
	}

	~CRegKey()
	{
		Close();
	}

	operator HKEY () const { return m_hk; }
	HKEY* operator & () 
	{ 
		_ASSERTE(m_hk == NULL);
		return &m_hk; 
	}
	HKEY Detach()
	{
		HKEY old = m_hk;
		m_hk = NULL;
		return old;
	}

	void Open(HKEY hkParent, LPCTSTR lpszKeyName =NULL, REGSAM samDesired = KEY_ALL_ACCESS) throw (Exception)
	{
		Close();
		Check(::RegOpenKeyEx(hkParent, lpszKeyName, 0, samDesired, &m_hk));
	}
	void Close() throw()
	{
		if (m_hk)
			::RegCloseKey(m_hk), m_hk = NULL;
	}

	CRegKey CreateKey(
		LPCTSTR lpszSubKey, 
		DWORD dwOptions = REG_OPTION_NON_VOLATILE, 
		REGSAM samDesired = KEY_ALL_ACCESS) throw (Exception)
	{
		CRegKey result;

		Check(::RegCreateKeyEx(
			m_hk,
			lpszSubKey,
			0,
			NULL,
			dwOptions,
			samDesired,
			NULL,
			&result,
			NULL));
		return result;
	}

	void SetValue(LPCTSTR lpszValueName, DWORD dwValue)
	{
		Check(::RegSetValueEx(
			m_hk,
			lpszValueName,
			0,
			REG_DWORD,
			(CONST BYTE*)&dwValue,
			sizeof(dwValue)));
	}
	void SetValue(LPCTSTR lpszValueName, LPCTSTR pstrValue)
	{
		if (pstrValue)
			Check(::RegSetValueEx(
				m_hk,
				lpszValueName,
				0,
				REG_SZ,
				(CONST BYTE*)pstrValue,
				_tcslen(pstrValue)*sizeof(TCHAR)));
	}

	void DestroyKey(LPCTSTR lpszSubKey) throw (Exception)
	{
		Check(SHDeleteKey(m_hk, lpszSubKey));
	}

	void DestroyValue(LPCTSTR pszValue, LPCTSTR pszSubkey =NULL) throw (Exception)
	{
		Check(SHDeleteValue(m_hk, pszSubkey, pszValue));
	}

protected:
	void Check(LONG l) throw (Exception)
	{
		if (SUCCEEDED(l))
			return;
		throw Exception(l);
	}
private:
	HKEY	m_hk;
};

VRCOM_END

#endif // REGKEY_H_INCLUDED