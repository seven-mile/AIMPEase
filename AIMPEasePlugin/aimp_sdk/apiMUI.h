/************************************************/
/*                                              */
/*          AIMP Programming Interface          */
/*               v4.50 build 2000               */
/*                                              */
/*                Artem Izmaylov                */
/*                (C) 2006-2017                 */
/*                 www.aimp.ru                  */
/*                                              */
/*            Mail: support@aimp.ru             */
/*                                              */
/************************************************/

#ifndef apiMUIH
#define apiMUIH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"

static const GUID IID_IAIMPServiceMUI = {0x41494D50, 0x5372, 0x764D, 0x55, 0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* IAIMPServiceMUI */
class __declspec(uuid("41494D50-5372-764D-5549-000000000000"))
IAIMPServiceMUI: public IUnknown
{
	public:
 		virtual HRESULT WINAPI GetName(IAIMPString **Value) = 0;
		virtual HRESULT WINAPI GetValue(IAIMPString *KeyPath, IAIMPString **Value) = 0;
		virtual HRESULT WINAPI GetValuePart(IAIMPString *KeyPath, int Part, IAIMPString **Value) = 0;
};

#endif // !apiMUIH