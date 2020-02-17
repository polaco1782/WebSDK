#ifndef WEBSDK_H
#define WEBSDK_H

#include "crow_all.h"
#include "cwebview.hpp"
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