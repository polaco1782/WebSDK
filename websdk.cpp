#include "websdk.h"
#include "backend.h"
#include <thread>
#include <functional>

static void on_script_rpc(WebKitUserContentManager *manager, WebKitJavascriptResult *message, void *thisclass)
{
    auto *cwebview = reinterpret_cast<CWebView *>(thisclass);
    auto *msg = webkit_javascript_result_get_js_value(message);
    char *value = strdup(jsc_value_to_string(msg));

    thread([cwebview, value]
    {
        auto j = json::parse(value);

        if(!j.empty())
        {
            string cmd = j["method"].get<string>();

            cout << cmd << endl;
            cwebview->jsexec_async("window.postMessage('teste','*')"); 
        }

        free(value);
    }).detach();

    // for(int i=0; i<10; i++)
    //     cwebview->jsexec_async("window.postMessage('teste','*')");

    // if(!j.empty())
    // {
    //     stringstream s;
    //     s << j["callback"].get<string>() << "('" << "tabaco" << "')";

    //     cout << "Callback recebido: " << s.str() << endl;

    //     for(int i=0; i<10; i++)
    //     {
    //         cout << "Running....." << endl;

    //         cwebview->jsexec_async(s.str());
    //         sleep(1);
    //     }
    // }
}

void WebSDK::on_resource_load(WebKitWebView *web_view, WebKitWebResource *resource, WebKitURIRequest *request, void *thisclass)
{
    cout << webkit_uri_request_get_uri(request) << endl;
}

void WebSDK::install_handlers()
{
    // evento de load de cada arquivo
    cwebview->connect("resource-load-started", G_CALLBACK(on_resource_load));

    // registra no webkit o metodo RPC
    cwebview->register_message("jsonRPC", G_CALLBACK(on_script_rpc));
}

//! metodo responsavel por carregar o arquivo principal do frontend
void WebSDK::load_index()
{
    ifstream file("frontend/index.html");
    string html = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    // define o caminho absoluto a partir do relativo
    string absname = "file:///" + fs::absolute("frontend/index.html").string();

    cwebview->load_html(html, absname);
}

WebSDK::WebSDK()
{
    set_default_size(1024, 600);
    add(*cwebview);
    show_all();

    install_handlers();
    load_index();

    cwebview->show_inspector();

    JSBackEnd *j = new JSBackEnd();
}