#include "pch.h"
#include "albumart.h"

#include "aimpstring.h"
#include "network.h"

#include <winrt/Windows.Web.Http.Headers.h>
#include <winrt/Windows.Storage.Streams.h>

#include <format>

winrt::hresult AIMPEase::Plugin::AlbumArtProvider::get_api_host(winrt::hstring& apihost)
{
    apihost = L"___FAILED__";

    auto cfg = m_core.as<IAIMPServiceConfig>();
    if (cfg == nullptr) return E_INVALIDARG;
    winrt::com_ptr<IAIMPString> raw_apihost;
    RETURN_IF_FAILED(cfg->GetValueAsString(HstrToAstr(m_core, L"AIMPEase\\ApiHost").get(), raw_apihost.put()));

    apihost = raw_apihost->GetData();

    return S_OK;
}

AIMPEase::Plugin::AlbumArtProvider::AlbumArtProvider(winrt::com_ptr<IAIMPCore> core) : m_core(core)
{
    LOG_IF_FAILED(get_api_host(m_apihost));
}


HRESULT WINAPI AIMPEase::Plugin::AlbumArtProvider::Get2(IAIMPFileInfo* FileInfo, IAIMPPropertyList* Options, IAIMPImageContainer** Image)
{
try {
    if (m_apihost == L"___FAILED__")
        return HRESULT_FROM_WIN32(ERROR_API_UNAVAILABLE);

    int max_file_size;
    RETURN_IF_FAILED(Options->GetValueAsInt32(AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_INTERNET_MAX_FILE_SIZE, &max_file_size));

    // construct a restful api call
    winrt::com_ptr<IAIMPString> raw_title;
    RETURN_IF_FAILED(FileInfo->GetValueAsObject(AIMP_FILEINFO_PROPID_TITLE, IID_IAIMPString, raw_title.put_void()));
    winrt::hstring title { raw_title->GetData() };
    
    winrt::Uri rest_uri { m_apihost + L"/albumart?title=" + title };
    
    auto get_op = NetworkHelper::RestGetAsync(rest_uri);
    // wait and ensure get is successful
    while (get_op.Status() == winrt::AsyncStatus::Started)
        ; // just wait
    
    if (get_op.Status() != winrt::AsyncStatus::Completed)
        return get_op.Status() == winrt::AsyncStatus::Canceled ? E_ABORT : get_op.ErrorCode().value;

    /* the expected structure of doc
    {
        albumart: string; // image file Uri
    }
    */
    winrt::JsonObject doc = get_op.GetResults();

    winrt::Uri albumart { doc.GetNamedString(L"albumart") };
    
    // download the image to buffer
    winrt::IAsyncOperation<winrt::IBuffer> dl_op = NetworkHelper::DownloadToMemoryAsync(
        albumart,
        [&](winrt::HttpContentHeaderCollection header_coll) -> winrt::IAsyncAction {
            if (header_coll.ContentLength().Value() > max_file_size) {
                OutputDebugString(std::format(L"invalid content size: {} bytes exceeded limit {} bytes\n", header_coll.ContentLength().Value(), max_file_size).c_str());
                dl_op.Cancel();
                OutputDebugString(L"give up the download operation\n");
            } else {
                OutputDebugString(std::format(L"content size: {} bytes.", header_coll.ContentLength().Value()).c_str());
            }
            co_return;
        }
    );

    // wait and ensure download is successful
    while (dl_op.Status() == winrt::AsyncStatus::Started)
        ; // just wait
    
    if (dl_op.Status() != winrt::AsyncStatus::Completed)
        return dl_op.Status() == winrt::AsyncStatus::Canceled ? E_ABORT : dl_op.ErrorCode().value;
        winrt::IBuffer buffer = dl_op.GetResults();

        winrt::com_ptr<IAIMPImageContainer> res_cont;
        RETURN_IF_FAILED(m_core->CreateObject(IID_IAIMPImageContainer, res_cont.put_void()));
        RETURN_IF_FAILED(res_cont->SetDataSize(buffer.Length()));

        std::copy(buffer.data(), buffer.data() + buffer.Length(), res_cont->GetData());
        { // check data
            static const wchar_t* fmt_conv[5] { L"unknown", L"bmp", L"gif", L"jpg", L"png" };

            SIZE sz; int fmt;
            RETURN_IF_FAILED(res_cont->GetInfo(&sz, &fmt));
            OutputDebugString(std::format(L"image size {}x{}, format: {}\n", sz.cx, sz.cy, fmt_conv[fmt]).c_str());
        }
    
        *Image = res_cont.get();
        res_cont->AddRef();

} catch (const winrt::hresult_error &e) {
    return e.code();
}

    return S_OK;
}

DWORD WINAPI AIMPEase::Plugin::AlbumArtProvider::GetCategory()
{
    return AIMP_ALBUMART_PROVIDER_CATEGORY_INTERNET;
}

HRESULT WINAPI AIMPEase::Plugin::AlbumArtProvider::Get(IAIMPString* FileURI, IAIMPString* Artist, IAIMPString* Album, IAIMPPropertyList* Options, IAIMPImageContainer** Image)
{
    return E_NOTIMPL;
}
