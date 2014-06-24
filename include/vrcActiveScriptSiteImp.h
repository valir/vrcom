/*! 
 *********************************************************************
 *  @file     vrcActiveScriptSiteImp.h
 *  @brief	  Custom script hosting
 *
 * This file contains implementation for interfaces needed when hosting
 * scripts (VBScript, JScript or other).
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
#ifndef ACTIVESCRIPTSITEIMP_H_INCLUDED
#define ACTIVESCRIPTSITEIMP_H_INCLUDED

#include <activscp.h>
#include "vrcImpInterface.h"
#include "MSScript.h"

#pragma warning(disable:4355)
#pragma warning(disable:4290)	// C++ Exception Specification ignored

VRCOM_BEGIN

_COM_SMARTPTR_TYPEDEF(IActiveScript, __uuidof(IActiveScript));
_COM_SMARTPTR_TYPEDEF(IActiveScriptParse, __uuidof(IActiveScriptParse));
_COM_SMARTPTR_TYPEDEF(IActiveScriptSite, __uuidof(IActiveScriptSite));

struct bstricmp
{
	bool operator () (const _bstr_t& left, const _bstr_t& right) const { return wcsicmp(left, right) <0; }
};

typedef std::map<_bstr_t, IUnknownPtr, bstricmp>	_ActiveScriptItems;

template <typename HOLDER>
class IActiveScriptSiteImp : public CImpCompInterface<IActiveScriptSite, HOLDER>
{
	typedef CImpCompInterface<IActiveScriptSite, HOLDER> _Base;
public:

	IActiveScriptSiteImp(HOLDER* pHolder, _ActiveScriptItems& items) : _Base(pHolder), m_items(items) {}

	// IActiveScript
    virtual HRESULT STDMETHODCALLTYPE GetLCID( 
        /* [out] */ LCID __RPC_FAR *plcid) { return E_NOTIMPL; }
    
    virtual HRESULT STDMETHODCALLTYPE GetItemInfo( 
        /* [in] */ LPCOLESTR pstrName,
        /* [in] */ DWORD dwReturnMask,
        /* [out] */ IUnknown __RPC_FAR *__RPC_FAR *ppiunkItem,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppti)
	{
		HRESULT hr = E_INVALIDARG;
		if (NULL != pstrName)
		{
			hr = TYPE_E_ELEMENTNOTFOUND;
			_ActiveScriptItems::iterator pos = m_items.find(pstrName);
			if (pos != m_items.end())
			{
				try
				{
					if (dwReturnMask & SCRIPTINFO_IUNKNOWN)
						_THROW_HR((*pos).second->QueryInterface(IID_IUnknown, (void**)ppiunkItem));

					if (dwReturnMask & SCRIPTINFO_ITYPEINFO)
					{
						IProvideClassInfoPtr spClassInfo;
						spClassInfo = (*pos).second;
						_THROW_HR(spClassInfo->GetClassInfo(ppti));
					}
					hr = S_OK;
				}
				catch (_com_error& e)
				{
					hr = e.Error();
				}
			}
		}
		return hr;
	}
    
    virtual HRESULT STDMETHODCALLTYPE GetDocVersionString( 
        /* [out] */ BSTR __RPC_FAR *pbstrVersion)
	{
		return E_NOTIMPL; 
	}
    
    virtual HRESULT STDMETHODCALLTYPE OnScriptTerminate( 
        /* [in] */ const VARIANT __RPC_FAR *pvarResult,
        /* [in] */ const EXCEPINFO __RPC_FAR *pexcepinfo)
	{
		Trace(TRACE_ACTIVESCRIPT, _T("OnScriptTerminate\n"));
		return S_OK;
	}
    
    virtual HRESULT STDMETHODCALLTYPE OnStateChange( 
        /* [in] */ SCRIPTSTATE ssScriptState)
	{
		Trace(TRACE_ACTIVESCRIPT, _T("OnStateChange %d\n"), ssScriptState);
		return S_OK;
	}
    
    virtual HRESULT STDMETHODCALLTYPE OnScriptError( 
        /* [in] */ IActiveScriptError __RPC_FAR *pscripterror)
	{
		Trace(TRACE_ACTIVESCRIPT, _T("OnScriptError\n"));
		try	{
			GetComponent()->OnScriptError(pscripterror); }
		catch (_com_error& e){
			return e.Error(); }
		return S_OK;
	}
    
    virtual HRESULT STDMETHODCALLTYPE OnEnterScript( void)
	{
		Trace(TRACE_ACTIVESCRIPT, _T("OnEnterScript\n"));
		return S_OK;
	}
    
    virtual HRESULT STDMETHODCALLTYPE OnLeaveScript( void)
	{
		Trace(TRACE_ACTIVESCRIPT, _T("OnLeaveScript\n"));
		return S_OK;
	}

private:
	_ActiveScriptItems&		m_items;
};

template <typename BASE>
class ActiveScriptSite : public BASE
{
	typedef BASE _Base;
public:
	ActiveScriptSite(IUnknown* pOuter) : 
	  _Base(pOuter),
	  m_site(this, m_items)
	{
	}

	virtual void Init(ModuleBase* pModule)
	{
		_Base::Init(pModule);
		_THROW_HR(m_parser.CreateInstance(CLSID_VBScript));
		m_script = m_parser;
		m_parser->InitNew();
		IActiveScriptSitePtr spSite;
		spSite = dynamic_cast<IUnknown*>(this);
		m_script->SetScriptSite(spSite);
	}

	void AddNamedItem(LPCTSTR pszName, IUnknownPtr spItem, DWORD dwFlags =SCRIPTITEM_ISVISIBLE|SCRIPTITEM_ISSOURCE)
	{
		m_items[pszName] = spItem;
		m_script->AddNamedItem(pszName, dwFlags);
	}

	void AddTypeLib(ITypeLibPtr spTypeLib)
	{
		TLIBATTR* pattr =NULL;
		_THROW_HR(spTypeLib->GetLibAttr(&pattr));
		HRESULT hr = m_script->AddTypeLib(pattr->guid, pattr->wMajorVerNum, pattr->wMinorVerNum, 0);
		spTypeLib->ReleaseTLibAttr(pattr);
		_THROW_HR(hr);
	}

	void AddTypeLib(REFGUID guid, DWORD verMaj, DWORD verMin =0, DWORD dwFlags =0)
	{
		_THROW_HR(m_script->AddTypeLib(guid, verMaj, verMin, dwFlags));
	}

	void ParseScript(
		BSTR pstrScript, 
		DWORD dwFlags =SCRIPTTEXT_ISVISIBLE, 
		VARIANT* pvarResult =NULL, 
		ULONG startLine =0)
	{
		EXCEPINFO ei = {0};
		_THROW_HR(m_parser->ParseScriptText(
			pstrScript, 
			NULL, NULL, NULL, 0, 
			startLine, dwFlags, pvarResult, &ei));
		_THROW_HR(m_script->SetScriptState(SCRIPTSTATE_CONNECTED));
	}

	/*! IActiveScriptSite::OnScriptError call this */
    virtual void OnScriptError(IActiveScriptError __RPC_FAR *pscripterror) =0;

private:
	IActiveScriptSiteImp<ActiveScriptSite>	m_site;
	IActiveScriptParsePtr					m_parser;
	IActiveScriptPtr						m_script;
	_ActiveScriptItems						m_items;
};

VRCOM_END
#endif // ACTIVESCRIPTSITEIMP_H_INCLUDED