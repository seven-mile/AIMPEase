#pragma once

#include "aimp_sdk/apiLyrics.h"

namespace AIMPEase::Plugin {

	class LyricsProvider : public winrt::implements<LyricsProvider, IAIMPExtensionLyricsProvider> {
		winrt::com_ptr<IAIMPCore> m_core;
		winrt::hstring m_apihost;

		winrt::hresult get_api_host(winrt::hstring &apihost);

	public:
		LyricsProvider(winrt::com_ptr<IAIMPCore> core);

		HRESULT WINAPI Get(IAIMPTaskOwner* Owner, IAIMPFileInfo* FileInfo, DWORD Flags, IAIMPLyrics* Lyrics) override;
		DWORD WINAPI GetCategory() override;
	};

}
