#ifndef FLUTTER_INAPPWEBVIEW_PLUGIN_IN_APP_BROWSER_H_
#define FLUTTER_INAPPWEBVIEW_PLUGIN_IN_APP_BROWSER_H_

#include <optional>
#include <string>
#include <wil/com.h>
#include <Windows.h>

#include "../flutter_inappwebview_windows_plugin.h"
#include "../in_app_webview/in_app_webview.h"
#include "../in_app_webview/in_app_webview_settings.h"
#include "../types/url_request.h"
#include "in_app_browser_channel_delegate.h"
#include "in_app_browser_settings.h"

namespace flutter_inappwebview_plugin
{
  struct InAppBrowserCreationParams
  {
    const std::string id;
    const std::optional<URLRequest> urlRequest;
    const std::shared_ptr<InAppBrowserSettings> initialSettings;
    const std::shared_ptr<InAppWebViewSettings> initialWebViewSettings;
  };

  class InAppBrowser {
  public:
    static inline const std::string METHOD_CHANNEL_NAME_PREFIX = "com.pichillilorenzo/flutter_inappbrowser_";
    static inline const wchar_t* CLASS_NAME = L"InAppBrowser";

    static LRESULT CALLBACK WndProc(HWND window,
      UINT message,
      WPARAM wparam,
      LPARAM lparam) noexcept;

    const FlutterInappwebviewWindowsPlugin* plugin;
    const std::string id;
    const std::optional<URLRequest> initialUrlRequest;
    std::unique_ptr<InAppWebView> webView;
    std::unique_ptr<InAppBrowserChannelDelegate> channelDelegate;
    const std::shared_ptr<InAppBrowserSettings> settings;

    InAppBrowser(const FlutterInappwebviewWindowsPlugin* plugin, const InAppBrowserCreationParams& params);
    ~InAppBrowser();

  private:
    const HINSTANCE m_hInstance;
    HWND m_hWnd;
    bool destroyed_ = false;
    static InAppBrowser* GetThisFromHandle(HWND window) noexcept;
    LRESULT MessageHandler(HWND window,
      UINT message,
      WPARAM wparam,
      LPARAM lparam) noexcept;
  };
}
#endif //FLUTTER_INAPPWEBVIEW_PLUGIN_IN_APP_BROWSER_H_