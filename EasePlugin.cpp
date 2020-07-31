#include "pre.h"
#include "EasePlugin.h"

#include "EaseLyricsProvider.h"

using namespace rapidjson;

// provide a symbol
EasePlugin* EasePlugin::m_ins = nullptr;
IAIMPCore* g_core;

HRESULT __declspec(dllexport) WINAPI AIMPPluginGetHeader(IAIMPPlugin** Header) {
	*Header = EasePlugin::instance();
	(**Header).AddRef();
	return S_OK;
}

HRESULT EasePlugin::Initialize(IAIMPCore* pCore)
{
	BEGIN_EH();

	if (!pCore) return E_INVALIDARG;
	g_core = pCore;

	auto ext_lyrics = new EaseLyricsProvider;
	ext_lyrics->AddRef();
	_hr = g_core->RegisterExtension(IID_IAIMPServiceLyrics, ext_lyrics);

	return S_OK;
}

HRESULT EasePlugin::Finalize()
{
	if (m_finalized) return S_OK;

	return S_OK;
}

IAIMPPlaylistItem* EasePlugin::GetCurrentTrack()
{
	return nullptr;
}
