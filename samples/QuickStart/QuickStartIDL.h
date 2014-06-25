
#pragma warning( disable: 4049 )  /* more than 64k source lines */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */
#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0359 */
/* at Sun Jul 07 16:09:17 2002
 */
/* Compiler settings for C:\DEV\DOC\VCOM\QuickStart\QuickStartIDL.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __QuickStartIDL_h__
#define __QuickStartIDL_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IQuickStart_FWD_DEFINED__
#define __IQuickStart_FWD_DEFINED__
typedef interface IQuickStart IQuickStart;
#endif 	/* __IQuickStart_FWD_DEFINED__ */


#ifndef __QuickStart_FWD_DEFINED__
#define __QuickStart_FWD_DEFINED__

#ifdef __cplusplus
typedef class QuickStart QuickStart;
#else
typedef struct QuickStart QuickStart;
#endif /* __cplusplus */

#endif 	/* __QuickStart_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __QuickStart_LIBRARY_DEFINED__
#define __QuickStart_LIBRARY_DEFINED__

/* library QuickStart */
/* [version][helpstring][uuid] */ 


DEFINE_GUID(LIBID_QuickStart,0x22B26DE5,0x83C8,0x4f6c,0x8B,0xAA,0xDD,0x1A,0x44,0x4B,0xD6,0xA0);

#ifndef __IQuickStart_INTERFACE_DEFINED__
#define __IQuickStart_INTERFACE_DEFINED__

/* interface IQuickStart */
/* [object][oleautomation][dual][version][helpstring][uuid] */ 


DEFINE_GUID(IID_IQuickStart,0x63E310BE,0xBACA,0x4ca0,0xBA,0x5F,0x03,0x8B,0x34,0xCB,0x57,0x18);

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("63E310BE-BACA-4ca0-BA5F-038B34CB5718")
    IQuickStart : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SquareRoot( 
            /* [in] */ VARIANT number,
            /* [retval][out] */ VARIANT *result) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IQuickStartVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IQuickStart * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IQuickStart * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IQuickStart * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IQuickStart * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IQuickStart * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IQuickStart * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IQuickStart * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        HRESULT ( STDMETHODCALLTYPE *SquareRoot )( 
            IQuickStart * This,
            /* [in] */ VARIANT number,
            /* [retval][out] */ VARIANT *result);
        
        END_INTERFACE
    } IQuickStartVtbl;

    interface IQuickStart
    {
        CONST_VTBL struct IQuickStartVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IQuickStart_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IQuickStart_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IQuickStart_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IQuickStart_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IQuickStart_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IQuickStart_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IQuickStart_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IQuickStart_SquareRoot(This,number,result)	\
    (This)->lpVtbl -> SquareRoot(This,number,result)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IQuickStart_SquareRoot_Proxy( 
    IQuickStart * This,
    /* [in] */ VARIANT number,
    /* [retval][out] */ VARIANT *result);


void __RPC_STUB IQuickStart_SquareRoot_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IQuickStart_INTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_QuickStart,0xFBC54865,0x02B9,0x4b83,0x83,0x24,0xDD,0x26,0xA3,0x86,0x8B,0x3B);

#ifdef __cplusplus

class DECLSPEC_UUID("FBC54865-02B9-4b83-8324-DD26A3868B3B")
QuickStart;
#endif
#endif /* __QuickStart_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


