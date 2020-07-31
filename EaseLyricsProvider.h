#pragma once

#include "IUnknownImpl.h"

class EaseLyricsProvider : public IUnknownImpl<IAIMPExtensionLyricsProvider>
{
public:
	virtual ~EaseLyricsProvider() {  }
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppvObj) override {
		if (!ppvObj) return E_POINTER;
		if (riid == IID_IAIMPExtensionLyricsProvider)
			return *ppvObj = (IAIMPExtensionLyricsProvider*)this, AddRef(), S_OK;
		return E_NOINTERFACE;
	}
	virtual HRESULT WINAPI Get(IAIMPTaskOwner*, IAIMPFileInfo*, DWORD, IAIMPLyrics*) override;
	virtual DWORD WINAPI GetCategory() override {
		return AIMP_LYRICS_PROVIDER_CATEGORY_INTERNET;
	}
};

