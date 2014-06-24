/*! 
 *********************************************************************
 *  @file     dpRefCounted.h
 *  @brief    This file implements one of mine's desing patterns, RefCounted.
 *
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
#ifndef DPREFCOUNTED_H_INCLUDED
#define DPREFCOUNTED_H_INCLUDED


#include "dpConfig.h"

DP_BEGIN()

class PATTERNS_EXP RefCounted
{
public:
	RefCounted() : m_counter(0) {}
	virtual ~RefCounted() {}

	void Ref() { ::InterlockedIncrement(&m_counter); }
	bool Unref() {
		if (0 == ::InterlockedDecrement(&m_counter)) 
		{
			delete this;
			return true;
		}
		return false;
	}

private:
	LONG	m_counter;
};

template <class T>
class PtrRef
{
	public:
		PtrRef() : m_pT(NULL) {}
		PtrRef(T* pT) : m_pT(pT) { if (pT) pT->Ref(); }
		PtrRef(const PtrRef& that) : m_pT(that.m_pT) { if (m_pT) m_pT->Ref(); }
		~PtrRef() { if (m_pT) m_pT->Unref(); }

		T* operator -> () const { return m_pT; }
		T& operator * () const { return *m_pT; }
		operator T* () const { return m_pT; }

		PtrRef& operator = (T* pT) { 
			if (m_pT)
				m_pT->Unref();
			m_pT = pT; 
			if (m_pT) 
				m_pT->Ref(); 
			return *this; 
		}


    /*!
     * this method is required when PtrRef objects are to be hold into
	 * std::vector and this vector is to be exported from a DLL. For
	 * more info, see Q168958 into MS Knowledge Base.
     *
     * @param that 
     *
     * @return bool operator 
     */
		bool operator < (const PtrRef& that) const { return m_pT < that.m_pT; }
		bool operator == (const PtrRef& that) const { return m_pT == that.m_pT; }
		bool operator == (T* pThat) const { return m_pT == pThat; }

	private:
		T*	m_pT;
};

DP_END()

#endif // DPREFCOUNTED_H_INCLUDED
