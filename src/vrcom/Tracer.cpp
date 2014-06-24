/*! 
 *********************************************************************
 *  @file     Tracer.cpp
 *  @brief    Logging utility
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

#include "stdafx.h"


#include "vrcTracer.h"

using namespace vrcom;

DWORD CTracerBase::m_dwTraceFlags = TRACE_DEFAULT_FLAGS;

#ifdef _DEBUG

CTracerBase::CTracerBase()
{
	m_bTraceActive = true;
	::InitializeCriticalSection(&m_cs);
}

CTracerBase::~CTracerBase()
{
	::DeleteCriticalSection(&m_cs);
}

void CTracerBase::Trace(DWORD dwFlags, LPCTSTR pstrFormat, ...) const
{
	if (m_bTraceActive && (m_dwTraceFlags & dwFlags))
	{
		::EnterCriticalSection(&m_cs);
		if (m_strAlias.length() == 0)
		{
			m_strAlias = GetTracingAlias();
			if (m_strAlias.length() == 0)
			{
				const type_info& ti = typeid(*this);
				m_strAlias = ti.name();
			}
		}

		static TCHAR pszTrace[2048];
		int n = _sntprintf(pszTrace, 2047, _T("[0x%lX, 0x%lX] %s | "), 
				GetCurrentProcessId(), GetCurrentThreadId(), (LPCTSTR)m_strAlias);
		
		va_list va;
		va_start(va, pstrFormat);
		_vsntprintf(pszTrace + n, 2047 -n, pstrFormat, va);
		va_end(va);

		::OutputDebugString(pszTrace);

		::LeaveCriticalSection(&m_cs);
	}
}

#endif // _DEBUG