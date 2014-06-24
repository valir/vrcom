/*! 
 *********************************************************************
 *  @file     dpSingleton.h
 *  @brief    Singleton design pattern implementation
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
#ifndef SINGLETON_H_INCLUDED
#define SINGLETON_H_INCLUDED


#include "dpConfig.h"
#include <assert.h>

DP_BEGIN()

/*!
 *  @class SingletonClass
 *  @brief Implement the 'singleton' design pattern
 *  @param T is the child class type
 *  
 *  This class ensures that only and only one instance exists. The template
 *  parameter is used to implement the static GetSingleton accessor. This
 *  way, no downcast will be needed into the user code.
 *  
 */
template <class T>
class SingletonClass
{
protected:

    /*!
     * Only child classes are allowed to instantiate this class. Therefore,
	 * this constructor is protected. The static instance member is checked
	 * and setup if found null (e.g. first time instantiation). All further
	 * instantiation attempts will generate an assertion in debug mode.
     *
     * @param pT : pointer to child instance
	 * @see m_pSingleton
     *
     */
	explicit SingletonClass(T* pT)
	{
		assert(m_pSingleton == NULL);
		m_pSingleton = pT;
		assert(m_pSingleton != NULL);
	}
public:

    /*!
     * Clear instance tracking member.
     *
     */
	virtual ~SingletonClass()
	{
		m_pSingleton = NULL;
	}


    /*!
     * Use this method to get a pointer to the unique instance.
     *
     * @return T*  : pointer to instance or NULL if no instance
     */
	static T* GetSingleton() { return m_pSingleton; }

private:
	/*! 
	 * This is the static instance member used to track singleton instance.
	 */
	static T*	m_pSingleton;
};

template <class T>
T* SingletonClass<T>::m_pSingleton = NULL;


DP_END()
#endif // SINGLETON_H_INCLUDED
