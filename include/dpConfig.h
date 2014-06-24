/*! 
 *********************************************************************
 *  @file     dpConfig.h
 *  @brief    Contains various configuration macros
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
#ifndef DPCONFIG_H_INCLUDED
#define DPCONFIG_H_INCLUDED

#pragma once

#ifndef _SET_
#include <set>
#endif

#ifndef _MAP_
#include <map>
#endif

/*! 
 * @namespace DP
 * @brief Contains all Patterns declarations
 *
 * Patterns implementation is encapsulated into the \c DP namespace to
 * avoid name collisions. Therefore, you need to include the following into your code :
 * @code 
 *         using namespace DP; 
 * @endcode 
 * 
 * Alternatively, you can prefix all declaration with \c DP:: as in :
 * @code
 *         DP::Singleton<Type> singleton;
 * @endcode
 *
 */
/*! 
 * @def DP 
 * Define the DP namespace
 */
/*!
 * @def DP_BEGIN
 * This guard macro marks the begin of Patterns declarations or implementation.
 */
/*! 
 * @def DP_END
 * This guard macro marks the end of Patterns declarations or implementation.
 */

#define DP dp
#define DP_BEGIN() namespace DP {
#define DP_END() }

/*! 
 * @def PATTERNS_EXP
 * Macro used to control class import/export from the DLL. When compiling Patterns 
 * project, it expands to a export directive. When including Patterns files from
 * other projects, it expands to a import directive. The PATTERNS_EXPORTS macro
 * controls this behavoir.
 * 
 * @see PATTERNS_EXPORTS
 */

/*! 
 * @def PATTERNS_EXP_TEMPLATE
 * Macro used to control template instances export. This is MS specific. When 
 * compiling Patterns project (e.g. PATTERNS_EXPORTS is defined), this macro expands 
 * to nothing. When including Patterns in other projects, this macro expands to C++ 
 * \c extern keyword.
 */

/*! 
 * @def PATTERNS_EXPORTS
 * This macro is defined when compiling Patterns project. When defined, the classes
 * are exported from the DLL.
 * 
 * @see PATTERNS_EXP
 * @see PATTERNS_EXP_TEMPLATE
 */

#ifdef PATTERNS_EXPORTS
	#define PATTERNS_EXP __declspec(dllexport)
	#define PATTERNS_EXP_TEMPLATE
#else
	#define PATTERNS_EXP __declspec(dllimport)
	#define PATTERNS_EXP_TEMPLATE extern
	// automatic link
	#ifdef _UNICODE
		#ifdef _DEBUG
			#pragma comment(lib, "PatternsD.lib")
		#else
			#pragma comment(lib, "Patterns.lib")
		#endif // _DEBUG
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "PatternsAD.lib")
		#else
			#pragma comment(lib, "PatternsA.lib")
		#endif // _DEBUG
	#endif // _UNICODE
#endif

/*!
 * @mainpage
 *
 * @section welcome Welcome to the Patterns documentation
 * 
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
 *
 * @section abstract What is the Patterns library
 *
 * This is an implementation of the 'gang of four' most current design patterns. 
 * I originally built this library for my own purpose. Since then, as
 * it continued to grow and enhance, I decided to make it avaiable
 * to the public domain. 
 * 
 * Please note that this library may be used to 
 * build 'commercial' software.
 * 
 * In this version, you'll find the implementation of the following patterns:
 * Singleton
 *
 */

#endif // DPCONFIG_H_INCLUDED