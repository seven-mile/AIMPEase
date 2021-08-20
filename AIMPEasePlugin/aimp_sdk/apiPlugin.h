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

#ifndef apiPluginH
#define apiPluginH

#include <windows.h>
#include <unknwn.h>
#include "apiCore.h"
 
// IAIMPPlugin.InfoGetCategories
const int  AIMP_PLUGIN_CATEGORY_ADDONS   = 0x1;
const int  AIMP_PLUGIN_CATEGORY_DECODERS = 0x2;
const int  AIMP_PLUGIN_CATEGORY_VISUALS  = 0x4;
const int  AIMP_PLUGIN_CATEGORY_DSP	     = 0x8;

// IAIMPPlugin.InfoGet
const int  AIMP_PLUGIN_INFO_NAME              = 0x0;
const int  AIMP_PLUGIN_INFO_AUTHOR            = 0x1;
const int  AIMP_PLUGIN_INFO_SHORT_DESCRIPTION = 0x2;
const int  AIMP_PLUGIN_INFO_FULL_DESCRIPTION  = 0x3;
  
// IAIMPPlugin.SystemNotification
const int  AIMP_SYSTEM_NOTIFICATION_SERVICE_ADDED   = 0x1;
const int  AIMP_SYSTEM_NOTIFICATION_SERVICE_REMOVED = 0x2;
const int  AIMP_SYSTEM_NOTIFICATION_EXTENSION_REMOVED = 0x3;


static const GUID IID_IAIMPExternalSettingsDialog = {0x41494D50, 0x4578, 0x7472, 0x6E, 0x4F, 0x70, 0x74, 0x44, 0x6C, 0x67, 0x00};
static const GUID IID_IAIMPPlugin = {0x2a8a8633, 0x4b6a, 0x41c5, 0xb4, 0x8b, 0x64, 0xf4, 0x4c, 0x2a, 0x8e, 0x7a};


/* IAIMPExternalSettingsDialog */
class __declspec(uuid("41494D50-4578-7472-6E4F-7074446C6700"))
IAIMPExternalSettingsDialog: public IUnknown
{
	public:
		virtual void WINAPI Show(HWND ParentWindow) = 0;
};
  
/* IAIMPPlugin */
class __declspec(uuid("2A8A8633-4B6A-41C5-B48B-64F44C2A8E7A"))
IAIMPPlugin: public IUnknown
{
	public:
		// Information about the plugin
		virtual PWCHAR WINAPI InfoGet(int Index) = 0;
		virtual DWORD WINAPI InfoGetCategories() = 0;
		// Initialization / Finalization
		virtual HRESULT WINAPI Initialize(IAIMPCore* Core) = 0;
		virtual HRESULT WINAPI Finalize() = 0;
		// System Notifications
		virtual void WINAPI SystemNotification(int NotifyID, IUnknown* Data) = 0;
};

// Export function name: AIMPPluginGetHeader
typedef HRESULT (WINAPI *TAIMPPluginGetHeaderProc)(IAIMPPlugin **Header);
#endif // !apiPluginH
