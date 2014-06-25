#ifndef QUICKSTART_H_INCLUDED
#define QUICKSTART_H_INCLUDED

#include "QuickStartIDL.h"

class CoQuickStart : public CComponent<CoQuickStart, &__uuidof(QuickStart)>
{
	typedef CComponent<CoQuickStart, &__uuidof(QuickStart)> _Base;
public:
	CoQuickStart(IUnknown* pOuter) : _Base(pOuter) {}

	VRCOM_BEGIN_DUAL_INTERFACE(impQuickStart, IQuickStart, CoQuickStart)
        virtual HRESULT STDMETHODCALLTYPE SquareRoot( 
            /* [in] */ VARIANT number,
            /* [retval][out] */ VARIANT *result);
	VRCOM_END_DUAL_INTERFACE(impQuickStart)
};

#endif // QUICKSTART_H_INCLUDED