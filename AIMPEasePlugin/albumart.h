#pragma once

#include "aimp_sdk/apiAlbumArt.h"

namespace AIMPEase::Plugin {

	class AlbumArtProvider : winrt::implements<AlbumArtProvider, IAIMPExtensionAlbumArtProvider2>
	{
		winrt::com_ptr<IAIMPCore> m_core;
		winrt::hstring m_apihost;

		winrt::hresult get_api_host(winrt::hstring &apihost);

	public:
		AlbumArtProvider(winrt::com_ptr<IAIMPCore> core);

		HRESULT WINAPI Get(IAIMPString *FileURI, IAIMPString *Artist, IAIMPString *Album, IAIMPPropertyList *Options, IAIMPImageContainer **Image) override;
		DWORD WINAPI GetCategory() override;

		HRESULT WINAPI Get2(IAIMPFileInfo *FileInfo, IAIMPPropertyList *Options, IAIMPImageContainer **Image) override;

	};

}

