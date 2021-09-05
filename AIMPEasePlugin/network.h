#pragma once

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Web.Http.h>
#include <winrt/Windows.Data.Json.h>

#include <winerror.h>

#include <functional>
#include <format>

namespace winrt {
    using namespace Windows::Foundation;
    using namespace Windows::Web::Http;
    using namespace Windows::Web::Http::Headers;
    using namespace Windows::Data::Json;
    using Windows::Storage::Streams::IBuffer;
}

namespace AIMPEase {

    class NetworkHelper {
    public:
        // use common client to minimize socket overhead
        static winrt::HttpClient client;

        static winrt::hresult to_hresult(winrt::HttpStatusCode code) {
            return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_HTTP, code);
        }

        static winrt::IAsyncOperation<winrt::JsonObject> RestGetAsync(winrt::Uri rest_uri) {
            auto resp { co_await client.GetAsync(rest_uri) };
            if (!resp.IsSuccessStatusCode())
                winrt::throw_hresult(to_hresult(resp.StatusCode()));
            
            auto rest_content { co_await resp.Content().ReadAsStringAsync() };

            auto res_obj = winrt::JsonObject::Parse(rest_content);
            co_return res_obj;
        }

        static winrt::IAsyncOperation<winrt::JsonObject> RestDoAsync(winrt::HttpMethod method, winrt::Uri rest_uri) {
            winrt::HttpRequestMessage req_msg{};
            req_msg.Method(method);
            req_msg.RequestUri(rest_uri);
            
            auto resp { co_await client.SendRequestAsync(req_msg) };
            if (!resp.IsSuccessStatusCode())
                winrt::throw_hresult(to_hresult(resp.StatusCode()));

            auto rest_content { co_await resp.Content().ReadAsStringAsync() };
            co_return winrt::JsonObject::Parse(rest_content);
        }

        using download_header_handler_t = std::function<winrt::IAsyncAction(winrt::HttpContentHeaderCollection)>;

    private:
        static constexpr auto default_download_header_handler
            = [](winrt::HttpContentHeaderCollection ) -> winrt::IAsyncAction { co_return; };

    public:
        static winrt::IAsyncOperation<winrt::IBuffer>
        DownloadToMemoryAsync(
            winrt::Uri file_uri,
            download_header_handler_t handler_callback = default_download_header_handler) {
            
            winrt::HttpRequestMessage req_msg{};
            req_msg.Method(winrt::HttpMethod::Get());
            req_msg.RequestUri(file_uri);

            auto resp { co_await client.SendRequestAsync(req_msg, winrt::HttpCompletionOption::ResponseHeadersRead) };

            if (!resp.IsSuccessStatusCode())
                winrt::throw_hresult(to_hresult(resp.StatusCode()));

            co_await handler_callback(resp.Content().Headers());

            co_await winrt::resume_background();

            co_return co_await resp.Content().ReadAsBufferAsync();
        }
    
    };
}

// definition (inline global static)
inline winrt::HttpClient AIMPEase::NetworkHelper::client;
