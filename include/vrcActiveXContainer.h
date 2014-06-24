/*! 
 *********************************************************************
 *  @file     vrcActiveXContainer.h
 *  @brief    Ole container logic.
 *
 * The classes in this file are still under developpement.
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
#ifndef ACTIVEXCONTAINER_H_INCLUDED
#define ACTIVEXCONTAINER_H_INCLUDED


#include "vrcConfig.h"
#include "vrcImpInterface.h"

//#include <wtHelper.h>

VRCOM_BEGIN

#pragma warning(disable:4355)


/*!
 *  @class CImpOleContainer 
 *  @brief IOleContainer implementation
 *  
 *  
 *  @see 
 */
template <class COMP>
class CImpOleContainer : public CImpCompInterface<IOleContainer, COMP>
{
	typedef CImpCompInterface<IOleContainer, COMP> _Base;
public:
	CImpOleContainer(COMP* pHolder) : _Base(pHolder)
	{
	}

// IParseDisplayName
    STDMETHOD(ParseDisplayName)( 
        /* [unique][in] */ IBindCtx __RPC_FAR *pbc,
        /* [in] */ LPOLESTR pszDisplayName,
        /* [out] */ ULONG __RPC_FAR *pchEaten,
        /* [out] */ IMoniker __RPC_FAR *__RPC_FAR *ppmkOut)
	{
		// this method is not mandatory
		return E_NOTIMPL;
	}


// IOleContainer
    STDMETHOD(EnumObjects)( 
        /* [in] */ DWORD grfFlags,
        /* [out] */ IEnumUnknown __RPC_FAR *__RPC_FAR *ppenum)
	{
		return E_NOTIMPL;
	}
    
    STDMETHOD(LockContainer)( 
        /* [in] */ BOOL fLock)
	{
		CComSrv::GetInstance().LockServer(fLock);
		return S_OK;
	}
};

/*!
 *  @class CImpOleClientSite 
 *  @brief IOleClientSite implementation
 *  
 *  
 *  @see 
 */
template <class COMP>
class CImpOleClientSite : public CImpCompInterface<IOleClientSite, COMP>
{
	typedef CImpCompInterface<IOleClientSite, COMP> _Base;
public:
	CImpOleClientSite(COMP* pHolder) : _Base(pHolder)
	{
	}

// IOleClientSite
    STDMETHOD(SaveObject)( void)
	{
		// persistence not (yet) supported
		return E_NOTIMPL;
	}
    
    STDMETHOD(GetMoniker)( 
        /* [in] */ DWORD dwAssign,
        /* [in] */ DWORD dwWhichMoniker,
        /* [out] */ IMoniker __RPC_FAR *__RPC_FAR *ppmk)
	{
		return E_NOTIMPL;
	}
    
    STDMETHOD(GetContainer)( 
        /* [out] */ IOleContainer __RPC_FAR *__RPC_FAR *ppContainer)
	{
		return GetComponent()->QueryInterface(__uuidof(*ppContainer), reinterpret_cast<void**>(ppContainer));
	}
    
    STDMETHOD(ShowObject)( void)
	{
		// TODO:
		return S_OK;
	}
    
    STDMETHOD(OnShowWindow)( 
        /* [in] */ BOOL fShow)
	{
		// TODO:
		return S_OK;
	}
    
    STDMETHOD(RequestNewObjectLayout)( void)
	{
		// TODO:
		return S_OK;
	}
};

/*!
 *  @class CImpOleControlSite 
 *  @brief IOleControlSite implementation
 *  
 *  
 *  @see 
 */
template <class COMP>
class CImpOleControlSite : public CImpCompInterface<IOleControlSite, COMP>
{
	typedef CImpCompInterface<IOleControlSite, COMP> _Base;
public:
	CImpOleControlSite(COMP* pHolder) : _Base(pHolder)
	{
	}

// IOleControlSite
    STDMETHOD(OnControlInfoChanged)( void)
	{
		// TODO:
		return S_OK;
	}
    
    STDMETHOD(LockInPlaceActive)( 
        /* [in] */ BOOL fLock)
	{
		// optional
		return E_NOTIMPL;
	}
    
    STDMETHOD(GetExtendedControl)( 
        /* [out] */ IDispatch __RPC_FAR *__RPC_FAR *ppDisp)
	{
		// not required
		return E_NOTIMPL;
	}
    
    STDMETHOD(TransformCoords)( 
        /* [out][in] */ POINTL __RPC_FAR *pPtlHimetric,
        /* [out][in] */ POINTF __RPC_FAR *pPtfContainer,
        /* [in] */ DWORD dwFlags)
	{
		// TODO:
		return S_OK;
	}
    
    STDMETHOD(TranslateAccelerator)( 
        /* [in] */ MSG __RPC_FAR *pMsg,
        /* [in] */ DWORD grfModifiers)
	{
		// not required
		return S_FALSE;
	}
    
    STDMETHOD(OnFocus)( 
        /* [in] */ BOOL fGotFocus)
	{
		// TODO:
		return S_OK;
	}
    
    STDMETHOD(ShowPropertyFrame)( void)
	{
		// not required
		return E_NOTIMPL;
	}
};

/*!
 *  @class CImpOleWindow 
 *  @brief IOleWindow implementation
 *  
 *  
 *  @see ActiveSite
 */
template <class COMP, class WND, class BASE = IOleWindow>
class CImpOleWindow : public CImpCompInterface<BASE, COMP>
{
	typedef CImpCompInterface<BASE, COMP> _Base;
public:
	CImpOleWindow(COMP* pHolder, WND* pWnd) : 
		_Base(pHolder), m_pWnd(pWnd) 
	{ 
		_ASSERTE(m_pWnd != NULL);
	}

	virtual bool HasIID(REFIID riid) const 
	{
		if (riid == __uuidof(IOleWindow))
			return true;
		return _Base::HasIID(riid);	// this will check the BASE riid
	}

// IOleWindow
    STDMETHOD(GetWindow)( 
        /* [out] */ HWND __RPC_FAR *phwnd)
	{
		CHECK_POINTER(phwnd);
		*phwnd = *m_pWnd;
		return S_OK;
	}
    
    STDMETHOD(ContextSensitiveHelp)( 
        /* [in] */ BOOL fEnterMode)
	{
		// optional
		return E_NOTIMPL;
	}
protected:
	WND*	m_pWnd;
};

/*!
 *  @class CImpOleInPlaceUIWindow 
 *  @brief IOleInPlaceUIWindow implementation
 *  
 *  
 *  @see CImpOleWindow
 */
template <class COMP, class WND, class BASE = IOleInPlaceUIWindow>
class CImpOleInPlaceUIWindow : public CImpOleWindow<COMP, WND, BASE>
{
	typedef CImpOleWindow<COMP, WND, BASE> _Base;
public:
	CImpOleInPlaceUIWindow(COMP* pHolder, WND* pWnd) : _Base(pHolder, pWnd) {}

	virtual bool HasIID(REFIID riid) const 
	{
		if (riid == __uuidof(IOleInPlaceUIWindow))
			return true;
		return _Base::HasIID(riid);
	}

// IOleInPlaceUIWindow
    STDMETHOD(GetBorder)( 
        /* [out] */ LPRECT lprectBorder)
	{
		// not required
		return E_NOTIMPL;
	}
    
    STDMETHOD(RequestBorderSpace)( 
        /* [unique][in] */ LPCBORDERWIDTHS pborderwidths)
	{
		// not required
		return E_NOTIMPL;
	}
    
    STDMETHOD(SetBorderSpace)( 
        /* [unique][in] */ LPCBORDERWIDTHS pborderwidths)
	{
		// not required
		return E_NOTIMPL;
	}
    
    STDMETHOD(SetActiveObject)( 
        /* [unique][in] */ IOleInPlaceActiveObject __RPC_FAR *pActiveObject,
        /* [unique][string][in] */ LPCOLESTR pszObjName)
	{
		m_spActiveObject = pActiveObject;
		m_pWnd->OnSetActiveObject(pActiveObject, pszObjName);
		return S_OK;
	}

protected:
	IOleInPlaceActiveObjectPtr	m_spActiveObject;
};

/*!
 *  @class CImpOleInPlaceFrame 
 *  @brief IOleInPlaceFrame implementation
 *  
 *  
 *  @see ActiveSite
 */
template <class COMP, class WND>
class CImpOleInPlaceFrame : public CImpOleInPlaceUIWindow<COMP, WND, IOleInPlaceFrame>
{
	typedef CImpOleInPlaceUIWindow<COMP, WND, IOleInPlaceFrame> _Base;
public:
	CImpOleInPlaceFrame(COMP* pHolder, WND* pWnd) : _Base(pHolder, pWnd)
	{
	}

// IOleInPlaceFrame
    STDMETHOD(InsertMenus)( 
        /* [in] */ HMENU hmenuShared,
        /* [out][in] */ LPOLEMENUGROUPWIDTHS lpMenuWidths)
	{
		// not required
		return E_NOTIMPL;
	}
    
    STDMETHOD(SetMenu)( 
        /* [in] */ HMENU hmenuShared,
        /* [in] */ HOLEMENU holemenu,
        /* [in] */ HWND hwndActiveObject)
	{
		// not required
		return E_NOTIMPL;
	}
    
    STDMETHOD(RemoveMenus)( 
        /* [in] */ HMENU hmenuShared)
	{
		// not required
		return E_NOTIMPL;
	}
    
    STDMETHOD(SetStatusText)( 
        /* [unique][in] */ LPCOLESTR pszStatusText)
	{
		m_pWnd->SetStatusText(pszStatusText);
		return S_OK;
	}
    
    STDMETHOD(EnableModeless)( 
        /* [in] */ BOOL fEnable)
	{
		// optional
		return E_NOTIMPL;
	}
    
    STDMETHOD(TranslateAccelerator)( 
        /* [in] */ LPMSG lpmsg,
        /* [in] */ WORD wID)
	{
		// optional
		return E_NOTIMPL;
	}
};

/*!
 *  @class CImpOleInPlaceSite 
 *  @brief IOleInPlaceSite implementation
 *  
 *  
 *  @see 
 */
template <class COMP, class WND>
class CImpOleInPlaceSite : public CImpOleWindow<COMP, WND, IOleInPlaceSite>
{
	typedef CImpOleWindow<COMP, WND, IOleInPlaceSite> _Base;
public:
	CImpOleInPlaceSite(COMP* pHolder, WND* pWnd) : 
	  _Base(pHolder, pWnd),
	  m_bInPlaceActive(false),
	  m_bUIActive(false) {}


// IOleInPlaceSite
    STDMETHOD(CanInPlaceActivate)( void)
	{
		// TODO:
		return S_OK;
	}
    
    STDMETHOD(OnInPlaceActivate)( void)
	{
		m_bInPlaceActive = true;
		return S_OK;
	}
    
    STDMETHOD(OnUIActivate)( void)
	{
		m_bUIActive = true;
		return S_OK;
	}
    
    STDMETHOD(GetWindowContext)( 
        /* [out] */ IOleInPlaceFrame __RPC_FAR *__RPC_FAR *ppFrame,
        /* [out] */ IOleInPlaceUIWindow __RPC_FAR *__RPC_FAR *ppDoc,
        /* [out] */ LPRECT lprcPosRect,
        /* [out] */ LPRECT lprcClipRect,
        /* [out][in] */ LPOLEINPLACEFRAMEINFO lpFrameInfo)
	{
		CHECK_POINTER(ppFrame);
		CHECK_POINTER(ppDoc);
		CHECK_POINTER(lprcPosRect);
		CHECK_POINTER(lprcClipRect);
		CHECK_POINTER(lpFrameInfo);

		IOleInPlaceFramePtr spFrame;
		_THROW_HR(GetComponent()->QueryInterface(IID_IOleInPlaceFrame, reinterpret_cast<void**>(&spFrame)));
		_THROW_HR(spFrame->GetWindow(&lpFrameInfo->hwndFrame));

		lpFrameInfo->fMDIApp = FALSE;
		lpFrameInfo->haccel = NULL;

		*lprcPosRect = GetComponent()->m_rcPos;
		*lprcClipRect = GetComponent()->m_rcClip;

		GetComponent()->QueryInterface(__uuidof(*ppFrame), reinterpret_cast<void**>(ppFrame));
		GetComponent()->QueryInterface(__uuidof(*ppDoc), reinterpret_cast<void**>(ppDoc));

		return S_OK;
	}
    
    STDMETHOD(Scroll)( 
        /* [in] */ SIZE scrollExtant)
	{
		// not required
		return E_NOTIMPL;
	}
    
    STDMETHOD(OnUIDeactivate)( 
        /* [in] */ BOOL fUndoable)
	{
		m_bUIActive = false;
		return S_OK;
	}
    
    STDMETHOD(OnInPlaceDeactivate)( void)
	{
		m_bInPlaceActive = false;
		return S_OK;
	}
    
    STDMETHOD(DiscardUndoState)( void)
	{
		// not required
		return E_NOTIMPL;
	}
    
    STDMETHOD(DeactivateAndUndo)( void)
	{
		// not required
		return E_NOTIMPL;
	}
    
    STDMETHOD(OnPosRectChange)( 
        /* [in] */ LPCRECT lprcPosRect)
	{
		// TODO:
		return S_OK;
	}

protected:
	bool	m_bInPlaceActive;
	bool	m_bUIActive;
};

/*!
 *  @class CImpOleDocumentSite 
 *  @brief IOleDocumentSite implementation
 *  
 *  
 *  @see ActiveSite
 */
template <class COMP>
class CImpOleDocumentSite : public CImpCompInterface<IOleDocumentSite, COMP>
{
	typedef CImpCompInterface<IOleDocumentSite, COMP> _Base;
public:
	CImpOleDocumentSite(COMP* pComp) : _Base(pComp) {}

	// IOleDocumentSite
	STDMETHOD(ActivateMe)(IOleDocumentView* pViewToActivate)
	{
		if (pViewToActivate)
		{
			IOleInPlaceSitePtr spSite;
			_THROW_HR(GetComponent()->QueryInterface(IID_IOleInPlaceSite, reinterpret_cast<void**>(&spSite)));
			pViewToActivate->SetInPlaceSite(spSite);
		}
		return S_OK;
	}
};

/*!
 *  @class CImpAdviseSink 
 *  @brief IAdviseSink implementation
 *  
 *  
 *  @see 
 */
template <class COMP>
class CImpAdviseSink : public CImpCompInterface<IAdviseSink, COMP>
{
	typedef CImpCompInterface<IAdviseSink, COMP> _Base;
public:
	CImpAdviseSink(COMP* pComp) : _Base(pComp) {}

    STDMETHOD_(void, OnDataChange)( 
        /* [unique][in] */ FORMATETC __RPC_FAR *pFormatetc,
        /* [unique][in] */ STGMEDIUM __RPC_FAR *pStgmed)
	{
	}
    
    STDMETHOD_(void, OnViewChange)( 
        /* [in] */ DWORD dwAspect,
        /* [in] */ LONG lindex)
	{
	}
    
    STDMETHOD_(void, OnRename)( 
        /* [in] */ IMoniker __RPC_FAR *pmk)
	{
	}
    
    STDMETHOD_(void, OnSave)( void)
	{
	}
    
    STDMETHOD_(void, OnClose)( void)
	{
	}
};

/*!
 *  @class ActiveSite 
 *  @brief ActiveX site
 *  
 *  
 *  @see CImpOleWindow, CImpOleInPlaceFrame, DefaultAmbient 
 */
template <class WND, class FRM, class AMBIENT =DefaultAmbient>
class ActiveSite : public CImpInnerUnknown<IDispatch>
{
	typedef ActiveSite<WND, FRM, AMBIENT> _MyType;
	typedef CImpInnerUnknown<IDispatch> _Base;
public:
	ActiveSite(WND* pWnd, FRM* pFrm) : 
		m_pWnd(pWnd),
		m_AdviseSink(this),
		m_dwAdviseConnection(0),
		m_OleClientSite(this),
		m_OleControlSite(this),
		m_OleDocumentSite(this),
		m_OleInPlaceSite(this, pWnd),
		m_OleInPlaceFrame(this, pFrm),
		m_OleInPlaceUIWindow(this, pWnd),
		m_OleContainer(this),
		_Base(NULL)
	{
	//	_ASSERTE(pWnd && ::IsWindow(*pWnd));
	}

	void CreateControl(LPCTSTR pszProgId)
	{
		CLSID	clsid;
		_THROW_HR(::CLSIDFromProgID(_bstr_t(pszProgId), &clsid));
		CreateControl(clsid);
	}

	void CreateControl(REFCLSID clsid)
	{
		_THROW_HR(CoCreateInstance(
			clsid, 
			NULL, 
			CLSCTX_ALL, 
			IID_IOleObject, 
			reinterpret_cast<void**>(&m_spOleObject)));
/*
		IStoragePtr	spStorage;

		_THROW_HR(::StgCreateStorageEx(
			NULL,
			STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_DELETEONRELEASE,
			STGFMT_STORAGE,
			0,
			NULL,
			0,
			IID_IStorage,
			reinterpret_cast<void**>(&spStorage)));

		_THROW_HR(::OleCreate(
			clsid,
			IID_IOleObject,
			OLERENDER_DRAW,
			NULL, NULL,
			spStorage,
			reinterpret_cast<void**>(&m_spOleObject)));
*/
		DWORD dwStatus;
		_THROW_HR(m_spOleObject->GetMiscStatus(DVASPECT_CONTENT, &dwStatus));

		if (OLEMISC_SETCLIENTSITEFIRST & dwStatus)
			_THROW_HR(m_spOleObject->SetClientSite(&m_OleClientSite));
		/*
		IPersistStoragePtr spStorage;
		if (SUCCEEDED(m_spOleObject.QueryInterface(IID_IPersistStorage, &spStorage)))
		{
			spStorage->InitNew(m_spStorage);
		}
		else
		{
		}
		*/
		if (OLEMISC_ALWAYSRUN & dwStatus)
			_THROW_HR(OleRun(m_spOleObject));
	
		if (!(OLEMISC_SETCLIENTSITEFIRST & dwStatus))
			_THROW_HR(m_spOleObject->SetClientSite(&m_OleClientSite));
		_THROW_HR(m_spOleObject->Advise(&m_AdviseSink, &m_dwAdviseConnection));
		_THROW_HR(OleSetContainedObject(m_spOleObject, TRUE));

		_THROW_HR(m_spOleObject->SetHostNames(_T("Container"), _T("Container doc")));
	}

	void DoVerb(LONG iVerb, LPMSG lpMsg =NULL)
	{
		RECT rc;
		SetRect(&rc, 0, 0, 100, 100);
		_THROW_HR(m_spOleObject->DoVerb(
			iVerb, 
			lpMsg,
			&m_OleClientSite,
			0,
			*m_pWnd,
			&rc));
	}

	IOleObjectPtr GetOleObject() { return m_spOleObject; }

	DEFINE_TRACING_ALIAS("ActiveSite");

	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject)
	{
		if (::InlineIsEqualGUID(riid, IID_IDispatch))
		{
			*ppvObject = this;
			return S_OK;
		}
		return _Base::QueryInterface(riid, ppvObject);
	}

// IDispatch
    STDMETHOD(GetTypeInfoCount)( 
        /* [out] */ UINT __RPC_FAR *pctinfo)
	{
		CHECK_POINTER(pctinfo);
		*pctinfo = 0;
		return S_OK;
	}
    
    STDMETHOD(GetTypeInfo)( 
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo)
	{
		return E_NOTIMPL;
	}
    
    STDMETHOD(GetIDsOfNames)( 
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId)
	{
		return E_NOTIMPL;
	}
    
    STDMETHOD(Invoke)( 
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr)
	{
		HRESULT hr = S_OK;
		CHECK_POINTER(pDispParams);
		CHECK_POINTER(pVarResult);
		CHECK_POINTER(pExcepInfo);
		CHECK_POINTER(puArgErr);

		_variant_t varResult;

		switch (dispIdMember)
		{
		case DISPID_AMBIENT_BACKCOLOR        :
			varResult = (long)m_Ambient.GetBackColor(); break;
		case DISPID_AMBIENT_DISPLAYNAME      :
			varResult = m_Ambient.GetDisplayName(); break;
		case DISPID_AMBIENT_FONT             :
			varResult = m_Ambient.GetFont(); break;
		case DISPID_AMBIENT_FORECOLOR        :
			varResult = (long)m_Ambient.GetForeColor(); break;
		case DISPID_AMBIENT_LOCALEID         :
			varResult = (long)m_Ambient.GetLocaleId(); break;
		case DISPID_AMBIENT_MESSAGEREFLECT   :
			varResult = m_Ambient.GetMessageReflect(); break;
		case DISPID_AMBIENT_SCALEUNITS       :
			varResult = m_Ambient.GetScaleUnits(); break;
		case DISPID_AMBIENT_TEXTALIGN        :
			varResult = m_Ambient.GetTextAlign(); break;
		case DISPID_AMBIENT_USERMODE         :
			varResult = m_Ambient.GetUserMode(); break;
		case DISPID_AMBIENT_UIDEAD           :
			varResult = m_Ambient.GetUIDead(); break;
		case DISPID_AMBIENT_SHOWGRABHANDLES  :
			varResult = m_Ambient.GetShowGrabHandles(); break;
		case DISPID_AMBIENT_SHOWHATCHING     :
			varResult = m_Ambient.GetShowHatching(); break;
		case DISPID_AMBIENT_DISPLAYASDEFAULT :
			varResult = m_Ambient.GetDisplayAsDefault(); break;
		case DISPID_AMBIENT_SUPPORTSMNEMONICS :
			varResult = m_Ambient.GetSupportsMnemonics(); break;
		case DISPID_AMBIENT_AUTOCLIP         :
			varResult = m_Ambient.GetAutoClip(); break;
		case DISPID_AMBIENT_APPEARANCE       :
			varResult = m_Ambient.GetAppearance(); break;
//		case DISPID_AMBIENT_CODEPAGE         :
//			varResult = m_Ambient.GetCodePage(); break;
//		case DISPID_AMBIENT_PALETTE          :
//			varResult = m_Ambient.GetPalette(); break;
//		case DISPID_AMBIENT_CHARSET          :
//			varResult = m_Ambient.GetCharset(); break;
//		case DISPID_AMBIENT_TRANSFERPRIORITY :
//			varResult = m_Ambient.GetTransferPriority(); break;
//		case DISPID_AMBIENT_RIGHTTOLEFT      :
//			varResult = m_Ambient.GetRightToLeft()			 ; break;
//		case DISPID_AMBIENT_TOPTOBOTTOM      :
//			varResult = m_Ambient.GetTopToBottom(); break;
		default:
			return DISP_E_MEMBERNOTFOUND;
		}

		*pVarResult = varResult.Detach();

		return hr;
	}

	WND* GetWnd() const { return m_pWnd; }

private:
	WND*									m_pWnd;		// la fenêtre qui nous contient
	AMBIENT									m_Ambient;

	CImpOleClientSite<_MyType>				m_OleClientSite;
	CImpOleControlSite<_MyType>				m_OleControlSite;
	CImpOleDocumentSite<_MyType>			m_OleDocumentSite;
	CImpOleInPlaceSite<_MyType, WND>		m_OleInPlaceSite;
	CImpOleInPlaceFrame<_MyType, FRM>		m_OleInPlaceFrame;
	CImpOleInPlaceUIWindow<_MyType, WND>	m_OleInPlaceUIWindow;
	CImpOleContainer<_MyType>				m_OleContainer;
	CImpAdviseSink<_MyType>					m_AdviseSink;
	DWORD									m_dwAdviseConnection;

	IOleObjectPtr							m_spOleObject;
public:
	WT::Rect	m_rcPos;
	WT::Rect	m_rcClip;
};

/*!
 *  @class DefaultAmbient
 *  @brief Default implementation for an AMBIENT
 *  
 *  
 *  @see  ActiveSite
 */
class DefaultAmbient
{
public:
	DefaultAmbient() {}

	OLE_COLOR GetBackColor() { return RGB(255, 255, 255); }
	_bstr_t GetDisplayName() { return _T(""); }
	IFontPtr GetFont() { return NULL; }
	OLE_COLOR GetForeColor() { return RGB(0, 0, 0); }
	LCID GetLocaleId() { return LOCALE_USER_DEFAULT; }
	bool GetMessageReflect() { return false; }
	_bstr_t GetScaleUnits() { return ""; }
	short GetTextAlign() { return 0; }
	bool GetUserMode() { return true; }
	bool GetUIDead() { return false; }
	bool GetShowGrabHandles() { return false; }
	bool GetShowHatching() { return false; }
	bool GetDisplayAsDefault() { return false; }
	bool GetSupportsMnemonics() { return false; }
	bool GetAutoClip() { return true; }
	short GetAppearance() { return 1; }	
};

VRCOM_END

#endif // ACTIVEXCONTAINER_H_INCLUDED
