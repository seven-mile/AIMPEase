#pragma once

#include "aimp_sdk/apiPlugin.h"

namespace AIMPEase::Plugin {

	class Plugin : public winrt::implements<Plugin, IAIMPPlugin> {
		winrt::com_ptr<IAIMPCore> m_core;

	public:
        ~Plugin() {
            Finalize();
        }

		PWCHAR WINAPI InfoGet(int Index) override;

        DWORD WINAPI InfoGetCategories() override;

        HRESULT WINAPI Initialize(IAIMPCore* Core) override;
        HRESULT WINAPI Finalize() override;

        void WINAPI SystemNotification(int NotifyID, IUnknown* Data) override;
	};

}
