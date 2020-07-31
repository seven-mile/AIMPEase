#pragma once

class EaseHttp
{
	ComPtr<IAIMPServiceHTTPClient> m_http;
	static EaseHttp* m_ins;
public:
	static EaseHttp* instance() {
		return m_ins = (m_ins ? m_ins : new EaseHttp);
	}

	EaseHttp() {
		if (g_core) g_core->QueryInterface(IID_IAIMPServiceHTTPClient, (void**)&m_http);
	}

	rapidjson::Document Get(std::wstring str);
};

