#ifndef HEADERS_H
#define HEADERS_H

#include "crow_all.h"
#include "mujs/mujs.h"
#include "sqlite.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <sstream>
#include <thread>
#include <functional>
#include <map>

#include <gtkmm.h>
#include <gtkmm/window.h>
#include <webkit2/webkit2.h>

namespace fs = std::filesystem;
using namespace std;
using namespace Gtk;
using namespace Glib;

struct jsdata
{
    WebKitWebView *w;
    char *c;
};

// metodos estaticos executados pela API em C do MUJS
class JSBackEnd
{
    private:
    js_State *J;

    public:
    JSBackEnd();
    ~JSBackEnd();

    void mujs_execute(std::string js);
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

    void jsexec_async(string js);
    WebKitJavascriptResult *jsexec_sync() { return NULL; }
    void register_message(const string &handler, GCallback callback);
    void setup_settings();
    void show_inspector();
    string process_request(string json_request);

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

    void process_request(string json_request);

    static void on_resource_load(WebKitWebView *web_view, WebKitWebResource *resource, WebKitURIRequest *request, void *thisclass);
};

#endif