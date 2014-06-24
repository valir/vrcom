/*! 
 *********************************************************************
 *  @file     vrcLock.h
 *  @brief    Property lock support classes
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
#ifndef LOCK_H_INCLUDED
#define LOCK_H_INCLUDED

#include "vrcConfig.h"
#include "vrcHelper.h"

#include "wtkHandleWrapper.h"	// vrusu design pattern
#include "dpException.h"

using namespace WTK;
using namespace DP;		

VRCOM_BEGIN

/*!
 *  @class CGenericLock
 *  @brief COM component lock suport
 *  
 *  This base class does no lock. It is used in the case of
 *  apartment threading model.
 *  
 *  @see 
 */
class CGenericLock
{
public:
	CGenericLock() {}
	virtual ~CGenericLock() {}

	virtual void Lock() {}
	virtual void Unlock() {}
	virtual LONG Increment(LPLONG pul) { return ++(*pul); }
	virtual LONG Decrement(LPLONG pul) { return --(*pul); }
};


/*!
 *  @class CMTALock 
 *  @brief Multithreaded Appartment threading model lock
 *  
 *  Uses an internal critical section to do thread synchronization.
 *  
 *  @see 
 */
class CMTALock : public CGenericLock
{
	typedef Exception _ExceptionType;
public:
	CMTALock()
	{
		::InitializeCriticalSection(&m_cs);
	}
	~CMTALock()
	{
		::DeleteCriticalSection(&m_cs);
	}

	virtual void Lock()
	{
		::EnterCriticalSection(&m_cs);
	}
	virtual void Unlock()
	{
		::LeaveCriticalSection(&m_cs);
	}
	virtual LONG Increment(LPLONG pul) { return ::InterlockedIncrement(pul); }
	virtual LONG Decrement(LPLONG pul) { return ::InterlockedDecrement(pul); }

private:
	CRITICAL_SECTION	m_cs;
};

VRCOM_END
#endif // LOCK_H_INCLUDED