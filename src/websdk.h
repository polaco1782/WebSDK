#ifndef WEBSDK_H
#define WEBSDK_H

#include "json.hpp"
#include <filesystem>
#include <fstream>
#include <gtkmm.h>
#include <gtkmm/window.h>
#include <iostream>
#include <sstream>
#include <webkit2/webkit2.h>

namespace fs = std::filesystem;
using namespace std;
using namespace Gtk;
using namespace Glib;
using namespace nlohmann;

struct jsdata
{
    WebKitWebView *w;
    char *c;
};

class CWebView : public Widget
{
  public:
    CWebView() : Widget(webkit_web_view_new()) { setup_settings(); }
    virtual ~CWebView(){};

    operator WebKitWebView *() { return WEBKIT_WEB_VIEW(gobj()); }

    // metodos simples declarados diretamente no header
    void load_uri(string &uri) { webkit_web_view_load_uri(*this, uri.c_str()); }
    void load_html(string &html, string &uri) { webkit_web_view_load_html(*this, html.c_str(), uri.c_str()); }
    void connect(const string &sig, GCallback callback) { g_signal_connect(*this, sig.c_str(), G_CALLBACK(callback), this); }

    // funcao de callback C, libera a variavel alocada pela thread js async
    static void jscallback(GObject *source_object, GAsyncResult *res, gpointer data)
    {
        jsdata *args = reinterpret_cast<jsdata *>(data);
        free(args->c);
    }

    // execuçao de JS assincrono da thread principal
    void jsexec_async(string js)
    {
        jsdata *args = new jsdata;

        args->w = *this;
        args->c = strdup(js.c_str());   // free sera executado no jscallback

        g_idle_add([](void *data)
        {
            jsdata *args = reinterpret_cast<jsdata *>(data);

            webkit_web_view_run_javascript(args->w, args->c, nullptr, CWebView::jscallback, data);
            return FALSE;
        }, args);
    }

    WebKitJavascriptResult *jsexec_sync() {}

    // registra o handler de evento no webkit
    void register_message(const string &handler, GCallback callback)
    {
        webkit_user_content_manager_register_script_message_handler(ContentManager, handler.c_str());
        g_signal_connect(ContentManager, string("script-message-received::" + handler).c_str(), G_CALLBACK(callback), this);
    }

    void setup_settings()
    {
        ContentManager = webkit_web_view_get_user_content_manager(*this);
        Settings = webkit_web_view_get_settings(*this);

        // permitir acesso a arquivos usando file:///
        webkit_settings_set_allow_file_access_from_file_urls(Settings, TRUE);
        webkit_settings_set_enable_developer_extras(Settings, TRUE);
        webkit_web_view_set_settings(*this, Settings);
    }

    void show_inspector()
    {
        inspector = webkit_web_view_get_inspector(*this);
        webkit_web_inspector_show(inspector);
    }

  protected:
    WebKitSettings *Settings;
    WebKitUserContentManager *ContentManager;
    WebKitWebInspector *inspector;
};

// WebSDK herda a classe Window do GTK
class WebSDK : public Window
{
  protected:
    CWebView *cwebview = new CWebView();

  public:
    WebSDK();
    virtual ~WebSDK(){};

    void load_index();
    void install_handlers();
    void httpserver_run();

    static void on_resource_load(WebKitWebView *web_view, WebKitWebResource *resource, WebKitURIRequest *request, void *thisclass);
};

#endif