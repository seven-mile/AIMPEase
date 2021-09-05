#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

// WIL & cpp/winrt
#include <wil/cppwinrt.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>

namespace winrt {
    using namespace Windows::Foundation;
}

// AIMP SDK
#include "aimp_sdk/apiCore.h"
