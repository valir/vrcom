/*! 
 *********************************************************************
 *  @file     TypeInfo.cpp
 *  @brief    CTypeInfoBase class implementation
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


#include "vrcTypeInfo.h"

using namespace VRCOM;

CTypeInfoBase::CTypeInfoMap CTypeInfoBase::m_TypeInfoMap;


/*!
 * This member inserts our GUID into m_TypeInfoMap map.
 *
 */
void CTypeInfoBase::Init()
{
	m_TypeInfoMap.insert(CTypeInfoMap::value_type(GetGUID(), this)); 
}
