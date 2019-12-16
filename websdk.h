#ifndef WEBSDK_H
#define WEBSDK_H

#include <webkit2/webkit2.h>
#include <gtkmm.h>
#include <gtkmm/window.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;
using namespace Gtk;
using namespace Glib;

class CWebView : public Widget
{
    public:
        friend class WebSDK;

        CWebView() : Widget(webkit_web_view_new()) { setup_settings(); }
        virtual ~CWebView() {};

        operator WebKitWebView *() { return WEBKIT_WEB_VIEW(gobj()); }

        // metodos simples declarados diretamente no header
        void load_uri(string& uri) { webkit_web_view_load_uri(*this, uri.c_str()); }
        void load_html(string& html, string& uri) { webkit_web_view_load_html(*this, html.c_str(), uri.c_str()); }
        void jsexec_async(const string& js) { webkit_web_view_run_javascript(*this, js.c_str(), NULL, NULL, NULL); }
        void connect(const string& sig, GCallback callback) { g_signal_connect(*this, sig.c_str(), G_CALLBACK(callback), this); }

        WebKitJavascriptResult *jsexec_sync()
        {

        }

        // registra o handler de evento no webkit
        void register_message(const string& handler, GCallback callback)
        {
            webkit_user_content_manager_register_script_message_handler(ContentManager, handler.c_str());
            g_signal_connect(ContentManager, string("script-message-received::"+handler).c_str(), G_CALLBACK(callback), this);
        }

        void setup_settings()
        {
            ContentManager = webkit_web_view_get_user_content_manager(*this);
            Settings = webkit_web_view_get_settings(*this);

            // permitir acesso a arquivos usando file:///
            webkit_settings_set_allow_file_access_from_file_urls(Settings, TRUE);
            webkit_web_view_set_settings(*this, Settings);
        }

    protected:
        WebKitSettings *Settings;
        WebKitUserContentManager *ContentManager;
};

// WebSDK herda a classe Window do GTK
class WebSDK: public Window
{
    protected:
        CWebView *cwebview = new CWebView();

    public:
        WebSDK();
        virtual ~WebSDK() { };

    void load_index();
    void install_handlers();
};

#endif