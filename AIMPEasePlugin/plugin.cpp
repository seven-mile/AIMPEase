#include "pch.h"

#include "plugin.h"

#include "lyrics.h"

extern "C" HRESULT __declspec(dllexport) WINAPI AIMPPluginGetHeader(IAIMPPlugin** Header) {
	auto plug = winrt::make<AIMPEase::Plugin::Plugin>();

    *Header = plug.get();
    plug->AddRef();

	return S_OK;
}

PWCHAR __stdcall AIMPEase::Plugin::Plugin::InfoGet(int Index)
{
    switch (Index) {
    case AIMP_PLUGIN_INFO_NAME:
        return const_cast<PWCHAR>(L"AIMPEasePlugin");
    case AIMP_PLUGIN_INFO_AUTHOR:
        return const_cast<PWCHAR>(L"7mile");
    case AIMP_PLUGIN_INFO_SHORT_DESCRIPTION:
    case AIMP_PLUGIN_INFO_FULL_DESCRIPTION:
        return const_cast<PWCHAR>(L"eval(AIMP ** NetEase)");
    default:
        break;
    }
    return nullptr;
}

DWORD __stdcall AIMPEase::Plugin::Plugin::InfoGetCategories()
{
    return AIMP_PLUGIN_CATEGORY_ADDONS;
}

HRESULT __stdcall AIMPEase::Plugin::Plugin::Initialize(IAIMPCore* Core)
{
    if (!Core) return E_INVALIDARG;
	m_core.attach(Core);


    // config private fields
    const std::vector<std::pair<const wchar_t*, const wchar_t*>> kvs {
        { L"AIMPEase\\ApiHost", L"http://localhost:8777" }

    };

    if (auto cfg = m_core.as<IAIMPServiceConfig>()) {
        winrt::com_ptr<IAIMPString> key_path, prop_value;
        RETURN_IF_FAILED(m_core->CreateObject(IID_IAIMPString, key_path.put_void()));
        RETURN_IF_FAILED(m_core->CreateObject(IID_IAIMPString, prop_value.put_void()));

        for (auto &[key, value] : kvs) {
            if (!key || !value) return E_INVALIDARG;

            RETURN_IF_FAILED(key_path->SetData(const_cast<wchar_t*>(key), -1));
            RETURN_IF_FAILED(prop_value->SetData(const_cast<wchar_t*>(value), -1));
            RETURN_IF_FAILED(cfg->SetValueAsString(key_path.get(), prop_value.get()));
        }
 
    } else {
        return E_NOINTERFACE;
    }

    

    // registering extension list
    const std::vector<std::pair<const IID&, winrt::com_ptr<IUnknown>>> exts {
        { IID_IAIMPServiceLyrics, winrt::make<LyricsProvider>(m_core) },

    };


    for (auto &ext : exts) {
        ext.second->AddRef();
        RETURN_IF_FAILED(m_core->RegisterExtension(ext.first, ext.second.get()));
        ext.second->Release();
    }

	return S_OK;
}

HRESULT __stdcall AIMPEase::Plugin::Plugin::Finalize()
{
    return S_OK;
}

void __stdcall AIMPEase::Plugin::Plugin::SystemNotification(int NotifyID, IUnknown* Data)
{
    // may: check dependency...
}
