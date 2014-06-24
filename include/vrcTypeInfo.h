/*! 
 *********************************************************************
 *  @file     vrcTypeInfo.h
 *  @brief    Type Library access classes
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
#ifndef TYPEINFO_H_INCLUDED
#define TYPEINFO_H_INCLUDED

#include "vrcConfig.h"
#include "vrcComSrv.h"


VRCOM_BEGIN

#pragma warning(disable:4251)

/*!
 *  @class CTypeInfoBase
 *  @brief Base class for CTypeInfo template
 *  
 *  This class is a CTypeInfo template class abstraction. It does 
 *  the ModuleBase houskeeping.
 * 
 *  @see CTypeInfo, ModuleBase
 */
class VRCOM_EXP_NOVTABLE CTypeInfoBase
{
public:

    /*!
     * The constructor register this type library instance with ModuleBase.
     *
     * @param none
     *
     */
	CTypeInfoBase()
	{
		ModuleBase::AddTypeInfo(this);
	}


	virtual void Init();

    /*!
     * This defines the GUID accessor signature.
     *
     * @param none
     *
     * @return const GUID*
     */
	virtual const GUID* GetGUID() = 0;

    /*!
     * This defines the GetTypeInfoOfGUID signature.
     *
     * @param pti	pointer to the location where ITypeInfo pointer will be stored.
     * @param clsid object's clsid
     */
	virtual void GetTypeInfoOfGUID(ITypeInfo** pti, REFCLSID clsid) = 0;

private:
	typedef std::map<const GUID*, CTypeInfoBase*> CTypeInfoMap;

	static CTypeInfoMap	m_TypeInfoMap;
};

#pragma warning(default:4251)

/*!
 *  @class CTypeInfo 
 *  @brief 
 *  
 *  Type library management class.
 *  
 *  @see CTypeInfoBase
 */
template <const GUID* LIB_GUID, USHORT VER_MAJ, USHORT VER_MIN, LCID lcid>
class CTypeInfo : public CTypeInfoBase
{
public:

    /*!
     * This default constructor does nothing.
     *
     */
	CTypeInfo() {}
	

    /*!
     * This member will attempt to load the type library by calling LoadRegTypeLib before
	 * calling the base class Init method. For more information, see LoadRegTypeLib documentation
	 * in MS Platform SDK.
     *
     */
	virtual void Init()
	{
		HRESULT hr = ::LoadRegTypeLib(*LIB_GUID, VER_MAJ, VER_MIN, lcid, &m_pTypeLib);
		if (FAILED(hr))
			_com_raise_error(hr);
		CTypeInfoBase::Init();
	}

    /*!
     * This accessor returns this library's GUID. The pointer return need not be deleted.
     *
     * @return const GUID*
     */
	virtual const GUID* GetGUID() { return LIB_GUID; }


    /*!
     * ITypeLib::GetTypeInfoOfGuid wrapper. For more information, refer to MS Platform SDK.
     *
     * @param pti		this will receive the ITypeInfo pointer for the specified clsid
     * @param clsid		object's clsid
     *
     */
	virtual void GetTypeInfoOfGUID(ITypeInfo** pti, REFCLSID clsid)
	{
		HRESULT hr = m_pTypeLib->GetTypeInfoOfGuid(clsid, pti);
		if (FAILED(hr))
			_com_raise_error(hr);
	}

private:
	ITypeLibPtr	m_pTypeLib;
	static CTypeInfo<LIB_GUID, VER_MAJ, VER_MIN, lcid>	m_Instance;
};

VRCOM_END
#endif // TYPEINFO_H_INCLUDED