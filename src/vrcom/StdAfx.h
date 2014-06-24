// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__CFE48AF0_F50D_11D2_8458_0060B0541558__INCLUDED_)
#define AFX_STDAFX_H__CFE48AF0_F50D_11D2_8458_0060B0541558__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define WINVER 0x0500

#pragma warning(disable:4786)

// Windows
#include <basetsd.h>
#include <windows.h>

// OLE
#include <ole2.h>
#include <initguid.h>
#include <comdef.h>

#include <tchar.h>
#include <typeinfo.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include <list>
#include <map>
#include <vector>

#include <crtdbg.h>

#include <shlwapi.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CFE48AF0_F50D_11D2_8458_0060B0541558__INCLUDED_)
