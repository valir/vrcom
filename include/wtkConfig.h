/*! 
 *********************************************************************
 *  @file     wtkConfig.h
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
#ifndef WTKCONFIG_H_INCLUDED
#define WTKCONFIG_H_INCLUDED

/*! 
 * @namespace WTK 
 * @brief Contains all WTK declarations and implementation
 *
 */
#define WTK_BEGIN() \
	namespace WTK {

#define WTK_END() \
	}; // namespace WTK

#ifdef WTK_EXPORTS
#define WTK_EXP __declspec(dllexport)
#else
#define WTK_EXP __declspec(dllimport)
	#ifdef _UNICODE
		#ifdef _DEBUG
			#pragma comment(lib, "wtkd.lib")
		#else
			#pragma comment(lib, "wtk.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "wtkAD.lib")
		#else
			#pragma comment(lib, "wtkA.lib")
		#endif
	#endif
#endif

#endif // WTKCONFIG_H_INCLUDED
