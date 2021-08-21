#include "pch.h"
#include "albumart.h"

#include "aimpstring.h"

#include <winrt/Windows.Web.Http.h>
#include <winrt/Windows.Data.Json.h>

namespace winrt {
	using namespace Windows::Web::Http;
	using namespace Windows::Data::Json;
}

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
	if (m_apihost == L"___FAILED__")
		return HRESULT_FROM_WIN32(ERROR_API_UNAVAILABLE);

	int max_file_size;
	RETURN_IF_FAILED(Options->GetValueAsInt32(AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_INTERNET_MAX_FILE_SIZE, &max_file_size));

	// construct a restful api call
	winrt::com_ptr<IAIMPString> raw_title;
	RETURN_IF_FAILED(FileInfo->GetValueAsObject(AIMP_FILEINFO_PROPID_TITLE, IID_IAIMPString, raw_title.put_void()));
	winrt::hstring title { raw_title->GetData() };
	
	winrt::Uri rest_uri { m_apihost + L"/albumart?query=" + title };
	
	winrt::HttpClient client {};

	auto http_resp = client.GetAsync(rest_uri).get();
	// if http response is not successful
	if (!http_resp.IsSuccessStatusCode()) {
		return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_HTTP, http_resp.StatusCode());
	}

	// try reading the content of restful response
	auto rest_content = http_resp.Content().ReadAsStringAsync().get();
	winrt::JsonObject doc;
	if (!winrt::JsonObject::TryParse(rest_content, doc))
		return E_INVALID_PROTOCOL_FORMAT;

	/* the expected structure of doc
	{
		albumart: string; // image file Uri
	}
	*/

	winrt::Uri albumart { doc.GetNamedString(L"albumart") };

	winrt::HttpRequestMessage req_msg{};

	req_msg.Method(winrt::HttpMethod::Head());
	req_msg.RequestUri(albumart);

	client.SendRequestAsync(req_msg);
	
	auto buffer = client.GetBufferAsync(albumart).get();

	winrt::com_ptr<IAIMPImageContainer> res_cont;
	RETURN_IF_FAILED(m_core->CreateObject(IID_IAIMPImageContainer, res_cont.put_void()));
	RETURN_IF_FAILED(res_cont->SetDataSize(buffer.Length()));

	std::copy(buffer.data(), buffer.data() + buffer.Length(), res_cont->GetData());
	{ // check data
		static const wchar_t* fmt_conv[5] { L"unknown", L"bmp", L"gif", L"jpg", L"png" };

		SIZE sz; int fmt;
		RETURN_IF_FAILED(res_cont->GetInfo(&sz, &fmt));
		OutputDebugString(std::format(L"image size {}x{}, format: {}", sz.cx, sz.cy, fmt_conv[fmt]).c_str());
	}
	
	*Image = res_cont.get();
	res_cont->AddRef();

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
