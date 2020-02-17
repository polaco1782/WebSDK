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
        auto x = crow::json::load(value);

        if(!!x)
        {
            string m = x["method"].s();
            string c = x["callback"].s();
            string p = x["params"].s();

            cout << "Metodo recebido: " << m << endl;
            cout << "Callback recebido: " << c << endl;
            cout << "Parametros recebido: " << p << endl;
            //cwebview->jsexec_async("window.postMessage('teste','*')"); 

            if(m == "execjs")
            {
                JSBackEnd *js = new JSBackEnd();
                js->mujs_execute("teste_backend(\"teste js!\")");
            }
        }

        free(value);
    }).detach();
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

void WebSDK::httpserver_run()
{
    thread([this]
    {
        crow::SimpleApp app;

        CROW_ROUTE(app, "/<int>")([this](int x)
        {
            cwebview->jsexec_async("alert('bumbou')");
            return "Hello world " + to_string(x);
        });

        app.port(8080).run();
    }).detach();
}

WebSDK::WebSDK()
{
    set_default_size(1024, 600);
    add(*cwebview);
    show_all();

    httpserver_run();
    install_handlers();
    load_index();

    // modo dev
    cwebview->show_inspector();

}