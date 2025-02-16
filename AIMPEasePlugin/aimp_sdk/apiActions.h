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

#ifndef apiActionsH
#define apiActionsH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"

static const GUID IID_IAIMPAction = {0x41494D50, 0x4163, 0x7469, 0x6F, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPActionEvent = {0x41494D50, 0x4163, 0x7469, 0x6F, 0x6E, 0x45, 0x76, 0x65, 0x6E, 0x74, 0x00};
static const GUID IID_IAIMPServiceActionManager = {0x41494D50, 0x5372, 0x7641, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x4D, 0x61, 0x6E};

// IAIMPAction Properties
const int AIMP_ACTION_PROPID_CUSTOM				  = 0;
const int AIMP_ACTION_PROPID_ID                   = 1;
const int AIMP_ACTION_PROPID_NAME                 = 2;
const int AIMP_ACTION_PROPID_GROUPNAME            = 3;
const int AIMP_ACTION_PROPID_ENABLED              = 4;
const int AIMP_ACTION_PROPID_DEFAULTLOCALHOTKEY   = 5;
const int AIMP_ACTION_PROPID_DEFAULTGLOBALHOTKEY  = 6;
const int AIMP_ACTION_PROPID_DEFAULTGLOBALHOTKEY2 = 7;
const int AIMP_ACTION_PROPID_EVENT                = 8;

// Flags for IAIMPServiceActionManager.MakeHotkey function
const int AIMP_ACTION_HOTKEY_MODIFIER_CTRL  = 1;
const int AIMP_ACTION_HOTKEY_MODIFIER_ALT   = 2;
const int AIMP_ACTION_HOTKEY_MODIFIER_SHIFT = 4;
const int AIMP_ACTION_HOTKEY_MODIFIER_WIN   = 8;

/* IAIMPAction */
class __declspec(uuid("41494D50-4163-7469-6F6E-000000000000"))
IAIMPAction: public IAIMPPropertyList
{
	// nothing
};

/* IAIMPActionEvent */
class __declspec(uuid("41494D50-4163-7469-6F6E-4576656E7400"))
IAIMPActionEvent: public IUnknown
{
	public:
		virtual void WINAPI OnExecute(IUnknown *Data) = 0;
};

/* IAIMPServiceActionManager */
class __declspec(uuid("41494D50-5372-7641-6374-696F6E4D616E"))
IAIMPServiceActionManager: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetByID(IAIMPString *ID, IAIMPAction **Action) = 0;
		virtual int WINAPI MakeHotkey(WORD Modifiers, WORD Key) = 0;
};

#endif // !apiActionsH