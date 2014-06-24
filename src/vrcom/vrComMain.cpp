/*****************************************************************************

	@File : vrComMain.cpp
	
	Description :
	
	You can freely use / redistribute this file as long as this notice is
	left intact :   (c) 1999-2000 Valentin RUSU vrusu@voila.fr
	
	DISCLAIMER : this file is provided 'as is' with no warranty, either
	express nor implied. By no mean could its author, Valentin Rusu, be
	pursued for any damage caused by the use of this file
	
*****************************************************************************/

#include "stdafx.h"
#include "dpException.h"
#include "vrcHelper.h"

using namespace DP;
using namespace VRCOM;

HINSTANCE hInstVRCOMDll = NULL;
VRCOM_EXP char* VRCOM::gszComErrorFileName =NULL;
VRCOM_EXP long VRCOM::glComErrorFileLine =0;

BOOL WINAPI DllMain(HINSTANCE hInstDll, DWORD dwReason, LPVOID)
{
	BOOL result = TRUE;
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		Exception::m_hInstResource = hInstVRCOMDll = hInstDll;
		break;
	default:
		break;
	}
	return result;
};
