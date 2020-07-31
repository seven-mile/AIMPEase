#include "pre.h"

#include "EaseHttp.h"

EaseHttp* EaseHttp::m_ins = nullptr;

rapidjson::Document EaseHttp::Get(std::wstring str) {
	PString strReq(str);
	static ComPtr<IAIMPStream> pStream;
	HTASK tsk; // what ever...

	if (FAILED(g_core->CreateObject(IID_IAIMPMemoryStream, (void**)&pStream)))
		return nullptr;

	HRESULT hrGet = m_http->Get(strReq.GetStr(),
		AIMP_SERVICE_HTTPCLIENT_FLAGS_WAITFOR
		| AIMP_SERVICE_HTTPCLIENT_FLAGS_UTF8
		,
		pStream,
		nullptr, // null is valid
		nullptr,
		(void**)&tsk
	);

	pStream->Seek(0, AIMP_STREAM_SEEKMODE_FROM_BEGINNING);

	auto sz = pStream->GetSize();
	auto buffer = new BYTE[sz + 10];
	int res = pStream->Read(buffer, sz + 10);
	if (res == -1) return nullptr;
	// Set it null-terminated.
	*(WCHAR*)(buffer + sz) = L'\0';
	rapidjson::Document doc;
	doc.Parse(reinterpret_cast<const char*>(buffer));

	return doc;
}
