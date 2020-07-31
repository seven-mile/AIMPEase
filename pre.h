#pragma once

#include <windows.h>
#include <atlbase.h>
#include <atldef.h>
#include <atlcom.h>
#include <comdef.h>

#include <iostream>
#include <string>
#include <sstream>
#include <locale>
#include <codecvt>

#include "aimpsdk/apiCore.h"
#include "aimpsdk/apiPlugin.h"
#include "aimpsdk/apiMenu.h"
#include "aimpsdk/apiPlayer.h"
#include "aimpsdk/apiLyrics.h"
#include "aimpsdk/apiInternet.h"

#undef max
#undef min

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
using rapidjson::UTF8;
using rapidjson::UTF16;

template<class Interface>
using ComPtr = _com_ptr_t<_com_IIID<Interface, nullptr>>;

#define BEGIN_EH() HRESULT _hr;
#define IFF(x) if (FAILED(_hr = (x))) return _hr

#include "global.h"
#include "PString.h"
#include "EaseHttp.h"
