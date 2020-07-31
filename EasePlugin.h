#pragma once

#include "IUnknownImpl.h"

class EasePlugin : public IUnknownImpl<IAIMPPlugin>
{
	static EasePlugin* m_ins;
	BOOL m_finalized = FALSE;

public:
	static EasePlugin* instance() {
		return m_ins = (m_ins ? m_ins : new EasePlugin);
	}

    ~EasePlugin() {
        Finalize();
        m_ins = nullptr;
    }

    PWCHAR WINAPI InfoGet(int Index) {
        switch (Index) {
        case AIMP_PLUGIN_INFO_NAME:
            return const_cast<PWCHAR>(L"AIMPEase");
        case AIMP_PLUGIN_INFO_AUTHOR:
            return const_cast<PWCHAR>(L"7mile");
        case AIMP_PLUGIN_INFO_SHORT_DESCRIPTION:
            return const_cast<PWCHAR>(L"Get lyrics from netease cloud music, and more...");
        }
        return nullptr;
    }

    DWORD WINAPI InfoGetCategories() {
        return AIMP_PLUGIN_CATEGORY_ADDONS;
    }

    HRESULT WINAPI Initialize(IAIMPCore* Core);
    HRESULT WINAPI Finalize();

    void WINAPI SystemNotification(int NotifyID, IUnknown* Data) {

    }

    IAIMPPlaylistItem* GetCurrentTrack();

};

