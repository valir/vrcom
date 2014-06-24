/*! 
 *********************************************************************
 *  @file     vrcOleInit.h
 *  @brief    OLE initialization class
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
#ifndef OLEINIT_H_INCLUDED
#define OLEINIT_H_INCLUDED

#include <dpSingleton.h>

using namespace DP;

#pragma warning(disable:4355) // 'this' : used in base member initializer list

#define VRCOM vrcom
#define VRCOM_BEGIN namespace VRCOM {
#define VRCOM_END };

VRCOM_BEGIN

template <COINIT model>
/*!
 *  @class OleInit
 *  @brief Automagically calls CoInitializeEx and CoUnitialize
 *  
 *  
 *  @see 
 */
class OleInit
{
public:
	OleInit()
	{
		if (FAILED(::CoInitializeEx(NULL, model)))
		{
			_RPTF0(_CRT_WARN, "WARNING: CoInitializeEx failed\n");
		}
	}

	~OleInit()
	{
		::CoUninitialize();
	}
};

VRCOM_END
#endif // OLEINIT_H_INCLUDED
