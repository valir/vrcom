
#include "stdafx.h"
#include "QuickStart.h"

#include <math.h>

STDMETHODIMP CoQuickStart::impQuickStart::SquareRoot( 
            /* [in] */ VARIANT number,
            /* [retval][out] */ VARIANT *result)
{
	HRESULT hr = S_OK;

	if (NULL == result)
		hr = E_POINTER;
	else
		try
		{
			_variant_t vnum(number);
			_variant_t vres;

			vres = sqrt(vnum);

			*result = vres.Detach();
		}
		catch (_com_error& e)
		{
			hr = e.Error();
		}

	return hr;
}

