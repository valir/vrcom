/*! 
 *********************************************************************
 *  @file     vrCOM.h
 *  @brief    General include file for VRCOM library
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
#ifndef VRCOM_H_INCLUDED
#define VRCOM_H_INCLUDED

#include "vrcConfig.h"
#include "vrcHelper.h"
#include "vrcOleInit.h"

/*! @def NO_ACTIVEX_CONTAINER
 *  @brief Disable ActiveX container classes
 * 
 *  Define this before including vrcom.h to reduce the compile time
 *  if you're building a server-only application or a client component
 *  that don't need to host ActiveX controls.
 *
 */
#if 0 /* this is for doxygen */
#define NO_ACTIVEX_CONTAINER
#endif

#ifndef VRCOM_CLIENTONLY
	#include "vrcComSrv.h"
	#include "vrcTypeInfo.h"
	#include "vrcImpInterface.h"
	#include "vrcCollection.h"
	#include "vrcObject.h"
	#include "vrcConnectable.h"
	#include "vrcSafeArray.h"
	#include "vrcUDT.h"
	#include "vrcPersistStream.h"
	#ifndef NO_ACTIVEX_CONTAINER
		#include "vrcActiveXContainer.h"
	#endif

	/*****************************************************************************
	 *	dual interfaces implementation
	 *****************************************************************************/
	#define VRCOM_BEGIN_DUAL_INTERFACE(impName, intName, compName) \
		private: \
		class impName : public VRCOM::CImpDualInterface<intName, compName> \
		{ \
			typedef CImpDualInterface<intName, compName> _Base; \
		public: \
			impName() : \
				_Base(  (compName*)((char*)this - offsetof(compName, m_ ## impName))  ) {} \
			~impName() {} 

	#define VRCOM_END_DUAL_INTERFACE(impName) \
		}; \
		friend class impName; \
		impName m_ ## impName;

	#define VRCOM_BEGIN_INTERFACE(impName, intName, compName) \
		private: \
		class impName : public VRCOM::CImpCompInterface<intName, compName> { \
			typedef CImpCompInterface<intName, compName> _Base; \
		public: \
			impName() : _Base( (compName*)((char*)this - offsetof(compName, m_ ## impName)) ) {} \
			~impName() {}

	#define VRCOM_END_INTERFACE(impName) \
		}; \
		friend class impName; \
		impName m_ ## impName;

	/*****************************************************************************
	 *	Error exceptions handling
	 *****************************************************************************/
	#define CATCH_COM_ERROR() \
		catch (_com_error& e) { Unlock(); return VerifyError(e.Error()); }

	#define CATCH_UNKNOWN_ERROR() \
		catch (...) { Unlock(); return VerifyError(E_FAIL); }

	#define CATCH_HRESULT() \
		catch (HRESULT hr) { Unlock(); return VerfiyError(hr); }
#endif




#endif // VRCOM_H_INCLUDED