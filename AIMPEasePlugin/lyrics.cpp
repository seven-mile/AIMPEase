#include "pch.h"

#include "lyrics.h"

#include "aimpstring.h"

#include <winrt/Windows.Web.Http.h>
#include <winrt/Windows.Data.Json.h>

namespace winrt {
	using namespace Windows::Web::Http;
	using namespace Windows::Data::Json;
}

winrt::hresult AIMPEase::Plugin::LyricsProvider::get_api_host(winrt::hstring& apihost)
{
	auto cfg = m_core.as<IAIMPServiceConfig>();
	winrt::com_ptr<IAIMPString> raw_apihost;
	RETURN_IF_FAILED(cfg->GetValueAsString(HstrToAstr(m_core, L"AIMPEase\\ApiHost").get(), raw_apihost.put()));

	apihost = raw_apihost->GetData();

	return S_OK;
}

AIMPEase::Plugin::LyricsProvider::LyricsProvider(winrt::com_ptr<IAIMPCore> core) : m_core(core) {
	if (FAILED(get_api_host(m_apihost)))
		m_apihost = L"__FAILED__";
}

HRESULT __stdcall AIMPEase::Plugin::LyricsProvider::Get(IAIMPTaskOwner* Owner, IAIMPFileInfo* FileInfo, DWORD Flags, IAIMPLyrics* Lyrics)
{
	if (m_apihost == L"__FAILED__")
		return HRESULT_FROM_WIN32(ERROR_API_UNAVAILABLE);

	// construct a restful api call
	winrt::com_ptr<IAIMPString> raw_title;
	RETURN_IF_FAILED(FileInfo->GetValueAsObject(AIMP_FILEINFO_PROPID_TITLE, IID_IAIMPString, raw_title.put_void()));
	winrt::hstring title { raw_title->GetData() };
	
	winrt::Uri rest_uri { m_apihost + L"/lyrics?query=" + title };
	
	winrt::HttpClient client {};
	auto get_op = client.GetAsync(rest_uri);

	// process cancellation from owner
	while (get_op.Status() == winrt::AsyncStatus::Started) {
		if (Owner->IsCanceled())
			get_op.Cancel();
	}

	// if operation status is not "Completed"
	if (get_op.Status() == winrt::AsyncStatus::Canceled) {
		return E_ABORT;
	} else if (get_op.Status() == winrt::AsyncStatus::Error) {
		return get_op.ErrorCode();
	}

	auto http_resp = get_op.GetResults();
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
		format: number; // {0,1,2}
		lyrics: string; // lrc file content
	}
	*/
	int lyrics_format = static_cast<int>(std::round(doc.GetNamedNumber(L"format")));
	
	winrt::com_ptr<IAIMPString> strLyrics;
	RETURN_IF_FAILED(m_core->CreateObject(IID_IAIMPString, strLyrics.put_void()));
	RETURN_IF_FAILED(strLyrics->SetData(const_cast<wchar_t*>(doc.GetNamedString(L"lyrics").data()), -1));

	RETURN_IF_FAILED(Lyrics->LoadFromString(strLyrics.get(), lyrics_format));

	return S_OK;
}

DWORD __stdcall AIMPEase::Plugin::LyricsProvider::GetCategory()
{
	return AIMP_LYRICS_PROVIDER_CATEGORY_INTERNET;
}
