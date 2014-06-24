/*! 
 *********************************************************************
 *  @file     vrcConnectionPointBase.h
 *  @brief    Connection point base class
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
#ifndef CONNECTIONPOINTBASE_H_INCLUDED
#define CONNECTIONPOINTBASE_H_INCLUDED

#include "vrcConfig.h"

VRCOM_BEGIN

/*!
 *  @class CImpConnectionPointBase
 *  @brief IConnectionPoint interface implementation base class
 *  
 *  
 *  @see CImpConnectionPoint
 */
class VRCOM_EXP CImpConnectionPointBase
{
public:
	CImpConnectionPointBase() {}
	virtual ~CImpConnectionPointBase() {}

	virtual REFIID GetConnectionIID() const =0;
};



VRCOM_END
#endif // CONNECTIONPOINTBASE_H_INCLUDED