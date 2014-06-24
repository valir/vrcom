/*! 
 *********************************************************************
 *  @file     vrcConfig.h
 *  @brief    VRCOM Library configuration macros
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
#ifndef VRCCONFIG_H_INCLUDED
#define VRCCONFIG_H_INCLUDED

#pragma once

#pragma warning(disable:4786)
#pragma warning(disable:4290)

#ifndef _CPPRTTI
#error VRCOM must be compiled with Run-Time Type Information (RTTI) activated !
#endif

/*! @namespace VRCOM
 *  @brief implementation namespace
 *
 *  this namespace encapsulates all the library.
 *
 */
/*!
 * @def VRCOM_BEGIN
 * 
 * Macro used to start VRCOM library declarations and implementation code.
 *
 * @see VRCOM, VRCOM_END
 */
/*!
 * @def VRCOM_END
 *
 * Macro used to end VRCOM library declarations and implementation code.
 *
 * @see VRCOM, VRCOM_START
 */
#define VRCOM vrcom
#define VRCOM_BEGIN namespace VRCOM {
#define VRCOM_END };


#if !(defined(VRCOM_DLL) || defined(VRCOM_EXE))
#endif

/*!
 * @def VRCOM_EXPORTS
 * 
 * This macro is defined when compiling VRCOM libraries. It controls class export
 * from the DLL. 
 */
/*! @def VRCOM_EXP
 *
 *  This macro controls the export or import of the VRCOM base classes.
 */
/*! @def VRCOM_EXP_NOVTABLE
 *
 * Doxygen seems to be troubled when more thant two items are present between keyword
 * class and that class's name. This macro combines __declspec(novtable) and VRCOM_EXP macro.
 */
/*!
 * @def VRCOM_EXP_TEMPLATE
 *
 * This macro controls C++ templates instances from the DLL.
 */
/*!
 * @def VRCOM_CLIENTLONY
 *
 * Define this in your client application to exclude COM server stuff. This
 * will yeld better compilation time for COM client-only applications.
 */
/*!
 * @def VRCOM_DLL
 *
 * Define this when your client application is a DLL.
 */
/*!
 * def VRCOM_EXE
 *
 * Define this whe your client application is an EXE.
 */
/*!
 * @def VRCOM_THREADING_MODEL
 *
 * Define this in your client application to control VRCOM threading model.
 * Possible values are:
 * @li \c COINIT_APARTMENTTHREADED to use apartment threading model
 * @li \c COINIT_MULTITHREADED to use free-threaded model
 *
 * @see CThreaded::CreateLockObject
 * @see Property
 */
#ifndef VRCOM_EXPORTS
	#ifndef VRCOM_CLIENTONLY
		#ifdef VRCOM_THREADING_MODEL
			#if (VRCOM_THREADING_MODEL == COINIT_APARTMENTTHREADED) || (VRCOM_THREADING_MODEL == COINIT_MULTITHREADED)
				VRCOM_BEGIN
					template <COINIT MODEL, bool bDLL> class Module;
				#if defined(VRCOM_DLL)
					typedef Module<VRCOM_THREADING_MODEL, true> _Module;
				#elif defined(VRCOM_EXE)
					typedef Module<VRCOM_THREADING_MODEL, false> _Module;
				#else
					#error Please define one of VRCOM_DLL or VRCOM_EXE
				#endif
				VRCOM_END
			#else
				#error Please, define the VRCOM_THREADING_MODEL to one the COINIT values !
			#endif
		#else
		#error Please, define the VRCOM_THREADING_MODEL to one of the COINIT values !
		#endif
	#endif
#endif

#ifndef _OLE2_H_
#include <ole2.h>
#endif

#include <docobj.h>

#include <initguid.h>

#ifndef _INC_COMDEF
#include <comdef.h>
#endif

#ifndef _INC_TCHAR
#include <tchar.h>
#endif

/*
#ifndef _INC_TYPEINFO
#include <vrcTypeInfo.h>
#endif
*/

#ifndef _INC_STDLIB
#include <stdlib.h>
#endif

#ifndef _INC_MALLOC
#include <malloc.h>
#endif 

#ifndef _INC_STDIO
#include <stdio.h>
#endif

#ifndef _INC_STDARG
#include <stdarg.h>
#endif

#include <assert.h>

#ifndef _LIST_
#include <list>
#endif

#ifndef _MAP_
#include <map>
#endif

#ifndef _VECTOR_
#include <vector>
#endif

#ifndef _INC_CRTDBG
#include <crtdbg.h>
#endif

#ifdef VRCOM_EXPORTS
	#define VRCOM_EXP __declspec(dllexport)
	#define VRCOM_EXP_TEMPLATE
#else
	#define VRCOM_EXP __declspec(dllimport)
	#define VRCOM_EXP_TEMPLATE extern

	#ifdef _UNICODE
		#ifdef _DEBUG
			#pragma comment(lib, "vrcomD.lib")
		#else
			#pragma comment(lib, "vrcom.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "vrcomAD.lib")
		#else
			#pragma comment(lib, "vrcomA.lib")
		#endif
	#endif

#endif 

#define VRCOM_EXP_NOVTABLE __declspec(novtable) VRCOM_EXP

#endif // VRCCONFIG_H_INCLUDED