/*! 
 *********************************************************************
 *  @file     vrcTracer.h
 *  @brief    Contains tracing support classes
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
#ifndef VRCTRACER_H_INCLUDED
#define VRCTRACER_H_INCLUDED

#include "vrcConfig.h"

#include <assert.h>

VRCOM_BEGIN

#pragma warning(disable:4290)	// C++ Exception Specification ignored
#pragma warning(disable:4251)	//

/*!
 * @def TRACE_DEFAULT_FLAGS
 * @brief Controls tracing detail level
 *
 * Define TRACE_DEFAULT_FLAGS into the client code to control
 * the detail level of tracing. It's a combination of TraceFlags.
 *
 *  @see TracerBase::TraceFlags
 */
#ifndef TRACE_DEFAULT_FLAGS
	#define TRACE_DEFAULT_FLAGS 0xFFFFFFFF
#endif

// utiliser ce macro dans le cas où le nom de la classe devient
// beaucoup trop long pour être lisible ;-)
/*!
 * @def DEFINE_TRACING_ALIAS
 *
 * Use this macro inside CTracerBase descendant class to quickly
 * override it's GetTracingAlias member.
 *
 * @see CTracerBase::GetTracingAlias
 */
#define DEFINE_TRACING_ALIAS(name) \
	virtual LPCTSTR GetTracingAlias() const { return _T(name); }

#ifdef _DEBUG
/*!
 *  @class CTracerBase
 *  @brief 
 *  
 *  Every tracing-enabled class must inherit this class. This
 *  way the tracing mode can be easily changed or adjusted.
 *  
 */
class VRCOM_EXP CTracerBase
{
    /*!
     * This private copy constructor prevent accidental copying 
	 * of one instance into another.
     *
     * @param that 
     *
     */
	CTracerBase(const CTracerBase& that);
public:

    /*!
     * Default constructor
     */
	CTracerBase();

	virtual ~CTracerBase();

	/*!
	 * @enum TraceFlags
	 * @brief Trace level flags
	 *
	 * 
	 */
	enum
	{
		TRACE_REFCOUNT = 1,
		TRACE_QI = 2,
		TRACE_SRVLOCK = 4,
		TRACE_ERRORS = 8,
		TRACE_GENERIC = 16,
		TRACE_FACTORY = 32,
		TRACE_OBJECT = 64,
		TRACE_MODULE = 128,
		TRACE_ACTIVESCRIPT = 256
	} TraceFlags;

	virtual LPCTSTR GetTracingAlias() const { return NULL; }

	// virtual member to make this class polymorphic
	// in order to make type_id(*this) return actual typeid
	virtual void Trace(DWORD dwFlags, LPCTSTR pstrFormat, ...) const;

	LPOLESTR FindInterfaceName(REFIID riid) throw (_com_error)
	{
		static HKEY hKey = NULL;
		LONG lr;
		LPOLESTR pszKey = NULL;
		if (FAILED(::StringFromCLSID(riid, &pszKey)))
			_com_raise_error(E_OUTOFMEMORY);
		
		if (NULL == hKey)
		{
			lr = ::RegOpenKeyEx(
				HKEY_CLASSES_ROOT,
				_T("Interface"),
				0,
				KEY_READ,
				&hKey);
			if (ERROR_SUCCESS != lr)
				_com_raise_error(lr);
		}
		
		HKEY hKeyInt = NULL;
		lr = ::RegOpenKeyExW(
			hKey,
			pszKey,
			0,
			KEY_READ,
			&hKeyInt);
		if (ERROR_SUCCESS == lr)
		{
			DWORD dwData = 0;
			lr = ::RegQueryValueEx(
				hKeyInt,
				_T(""),
				NULL,
				NULL,
				NULL,
				&dwData);
			if (ERROR_SUCCESS != lr)
				_com_raise_error(lr);

			::CoTaskMemFree(pszKey);
			pszKey = (LPOLESTR)::CoTaskMemAlloc(dwData +1);
			if (NULL == pszKey)
				_com_raise_error(E_OUTOFMEMORY);

			lr = ::RegQueryValueEx(
				hKeyInt,
				_T(""),
				NULL,
				NULL,
				(LPBYTE)pszKey,
				&dwData);

			::RegCloseKey(hKeyInt);
		}

		return pszKey;
	}

	inline void SetTraceActive(bool bActive = true) { m_bTraceActive = bActive; }
	inline bool GetTraceActive() const { return m_bTraceActive; }

private:
	bool	m_bTraceActive;
	mutable CRITICAL_SECTION	m_cs;
	mutable _bstr_t	m_strAlias;
public:
	static DWORD m_dwTraceFlags;
};


#else
/*****************************************************************************
 *	in release builds, remove this class
 *****************************************************************************/

class VRCOM_EXP CTracerBase
{
public:
	CTracerBase() {}
	~CTracerBase() {}

	enum
	{
		TRACE_REFCOUNT = 1,
		TRACE_QI = 2,
		TRACE_SRVLOCK = 4,
		TRACE_ERRORS = 8,
		TRACE_GENERIC = 16,
		TRACE_FACTORY = 32,
		TRACE_OBJECT = 64,
		TRACE_MODULE = 128
	} TraceFlags;

	virtual LPCTSTR GetTracingAlias() const { return NULL; }
	void Trace(DWORD dwFlags, LPCTSTR pstrFormat, ...) const {}
	LPOLESTR FindInterfaceName(REFIID) { return NULL; }
	inline void SetTraceActive(bool bActive = true) { }
	inline bool GetTraceActive() const { return false; }

	static DWORD m_dwTraceFlags;
};
#endif // _DEBUG

#pragma warning(default:4290)	// C++ Exception Specification ignored

VRCOM_END

#endif // VRCTRACER_H_INCLUDED