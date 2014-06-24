/*! 
 *********************************************************************
 *  @file     vrcHelper.h
 *  @brief    This file contains several helper macros
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
#ifndef VRCOMHELPER_H_INCLUDED
#define VRCOMHELPER_H_INCLUDED

#include "vrcConfig.h"

#pragma once
#pragma warning(disable:4290) // C++ Exception Specification ignored

/*****************************************************************************
**
*****************************************************************************/
VRCOM_BEGIN

extern VRCOM_EXP char* gszComErrorFileName;
extern VRCOM_EXP long glComErrorFileLine;

#define CLOSE_HANDLE(h) \
	if (h != NULL) \
	{ \
		::CloseHandle(h); \
		h = NULL; \
	}

#define DELETE_POINTER(p) \
	if (p != NULL) \
	{ \
		delete p; \
		p = NULL; \
	}

#define _THROW_HR(expr) \
	{ HRESULT __hr = expr; \
	if (FAILED(__hr)) {\
		_RPTF1(_CRT_WARN, "throwing _com_error for failed code 0x%X\n", __hr); \
		gszComErrorFileName = __FILE__; \
		glComErrorFileLine = __LINE__; \
		_com_issue_error(__hr); } \
	}

#define _COUNT_OF(sz) \
	sizeof(sz)/sizeof(sz[0])

#define CHECK_POINTER(p) \
	if (NULL == p) return E_POINTER;

#define GETPROP(pOut, prop) \
	return GetProperty(pOut, GetComponent()-> ## prop ());

#define PUTPROP(prop, in) \
	return GetComponent() -> prop (in);


template <typename TCOM, typename TPROG>
__inline HRESULT GetProperty(TCOM* pOut, const TPROG prop)
{
	*pOut = prop;
	return S_OK;
}

template <>
__inline HRESULT GetProperty<BSTR, _bstr_t>(BSTR* pOut, const _bstr_t prop)
{
	CHECK_POINTER(pOut);
	*pOut = prop.copy();
	return S_OK;
}

template <>
__inline HRESULT GetProperty<BSTR, LPCSTR>(BSTR* pOut, const LPCSTR prop)
{
	return GetProperty(pOut, _bstr_t(prop));
}


__inline PutProp(_bstr_t& prop, BSTR pIn) throw(_com_error)
{
	try
	{
		prop = pIn;
	}
	catch (_com_error e)
	{
		return e.Error();
	}
	return S_OK;
}

VRCOM_END
#endif // VRCOMHELPER_H_INCLUDED