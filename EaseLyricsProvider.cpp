#include "pre.h"

#include "EaseLyricsProvider.h"

inline auto GetLrcPureText(std::wstring str) {
	for (auto it = str.begin(); it != str.end(); it++)
		if (*it == L'[') {
			while (it != str.end()  && *it != L']')
				str.erase(it);
			str.erase(it); // for L']'
		}
	
	PString ps(str);
	return ps.GetStr();
}

HRESULT WINAPI EaseLyricsProvider::Get(IAIMPTaskOwner* owner,
					IAIMPFileInfo* file, DWORD flags, IAIMPLyrics* lrc)
{
	// 1. (todo) use 163 key to find it

	// 2. use "Artist - Title" to find it
	ComPtr<IAIMPString> strArtist, strTitle;
	file->GetValueAsObject(AIMP_FILEINFO_PROPID_ARTIST, IID_IAIMPString, (void**)&strArtist);
	file->GetValueAsObject(AIMP_FILEINFO_PROPID_TITLE,  IID_IAIMPString, (void**)&strTitle );

	auto strSearch = std::wstring(strArtist->GetData()) + L"-" + strTitle->GetData();
	strSearch = PString::UriEncodeW(strSearch);
	auto d = g_http.Get(L"http://h.7li.moe:3000/search?type=1&limit=5&keywords=" + strSearch);
	
	if (d.HasMember("result") && d["result"].HasMember("songs"))
	if (d["result"]["songs"].IsArray()) {
		auto& lst = d["result"]["songs"];
		for (auto it = lst.Begin(); it != lst.End(); it++) {
			auto& ele = *it;
			auto lrcd = g_http.Get(L"http://h.7li.moe:3000/lyric?id=" + PString::Num2Str(ele["id"].GetInt()));
			if (lrcd.HasMember("lrc") && lrcd["lrc"].HasMember("lyric"))
			if (lrcd["lrc"]["lyric"].IsString())
			{
				lrc->SetValueAsObject(
					AIMP_LYRICS_PROPID_TEXT,
					GetLrcPureText(PString::U2W(lrcd["lrc"]["lyric"].GetString()))
				);
				lrc->SetValueAsObject(
					AIMP_LYRICS_PROPID_APP,
					PString(L"AIMPEase Plug-in").GetStr()
				);
				lrc->SetValueAsInt32(
					AIMP_LYRICS_PROPID_TYPE,
					AIMP_LYRICS_TYPE_SYNCED
				);
				return S_OK;
			}
			
		}
	}

	return E_FAIL;
}
