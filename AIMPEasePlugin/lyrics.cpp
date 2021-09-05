#include "pch.h"

#include "lyrics.h"

#include "aimpstring.h"
#include "network.h"

winrt::hresult AIMPEase::Plugin::LyricsProvider::get_api_host(winrt::hstring& apihost)
{
    apihost = L"___FAILED__";

    auto cfg = m_core.as<IAIMPServiceConfig>();
    if (cfg == nullptr) return E_INVALIDARG;
    winrt::com_ptr<IAIMPString> raw_apihost;
    RETURN_IF_FAILED(cfg->GetValueAsString(HstrToAstr(m_core, L"AIMPEase\\ApiHost").get(), raw_apihost.put()));

    apihost = raw_apihost->GetData();

    return S_OK;
}

AIMPEase::Plugin::LyricsProvider::LyricsProvider(winrt::com_ptr<IAIMPCore> core) : m_core(core) {
    LOG_IF_FAILED(get_api_host(m_apihost));
}

HRESULT WINAPI AIMPEase::Plugin::LyricsProvider::Get(IAIMPTaskOwner* Owner, IAIMPFileInfo* FileInfo, DWORD Flags, IAIMPLyrics* Lyrics)
{
try {
    if (m_apihost == L"__FAILED__")
        return HRESULT_FROM_WIN32(ERROR_API_UNAVAILABLE);

    // construct a restful api call
    winrt::com_ptr<IAIMPString> raw_title;
    RETURN_IF_FAILED(FileInfo->GetValueAsObject(AIMP_FILEINFO_PROPID_TITLE, IID_IAIMPString, raw_title.put_void()));
    winrt::hstring title { raw_title->GetData() };
    
    winrt::Uri rest_uri { m_apihost + L"/lyrics?title=" + title };
    
    auto get_op = NetworkHelper::RestGetAsync(rest_uri);
    while (get_op.Status() == winrt::AsyncStatus::Started)
        if (Owner->IsCanceled())
            get_op.Cancel();

    if (get_op.Status() != winrt::AsyncStatus::Completed)
        return get_op.Status() == winrt::AsyncStatus::Canceled ? E_ABORT : get_op.ErrorCode().value;

        /* the expected structure of doc
        {
            format: number; // {0,1,2}
            lyrics: string; // lrc file content
        }
        */
        winrt::JsonObject doc = get_op.GetResults();

        int lyrics_format = static_cast<int>(std::round(doc.GetNamedNumber(L"format")));
    
        winrt::com_ptr<IAIMPString> strLyrics;
        RETURN_IF_FAILED(m_core->CreateObject(IID_IAIMPString, strLyrics.put_void()));
        RETURN_IF_FAILED(strLyrics->SetData(const_cast<wchar_t*>(doc.GetNamedString(L"lyrics").data()), -1));

        RETURN_IF_FAILED(Lyrics->LoadFromString(strLyrics.get(), lyrics_format));
} catch (const winrt::hresult_error &e) {
    return e.code();
}

    return S_OK;
}

DWORD WINAPI AIMPEase::Plugin::LyricsProvider::GetCategory()
{
    return AIMP_LYRICS_PROVIDER_CATEGORY_INTERNET;
}
