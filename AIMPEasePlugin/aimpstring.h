#pragma once

#include <winrt/base.h>
#include "aimp_sdk/apiCore.h"
#include "aimp_sdk/apiObjects.h"

namespace AIMPEase::Plugin {

    inline winrt::hstring AstrToHstr(winrt::com_ptr<IAIMPString> str) {
        return str->GetData();
    }

    inline winrt::com_ptr<IAIMPString> HstrToAstr(winrt::com_ptr<IAIMPCore> core, winrt::hstring str = L"") {
        winrt::com_ptr<IAIMPString> res;
        winrt::check_hresult(core->CreateObject(IID_IAIMPString, res.put_void()));
        winrt::check_hresult(res->SetData(const_cast<wchar_t*>(str.data()), -1));
        return res;
    }

}
