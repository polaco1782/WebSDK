#include "websdk.h"

static void on_resource_load (WebKitWebView *web_view, WebKitWebResource *resource, WebKitURIRequest *request, void *thisclass)
{
    cout <<  webkit_uri_request_get_uri(request) << endl;
}

static void on_script_rpc(WebKitUserContentManager *manager, WebKitJavascriptResult *message, void *thisclass)
{
    auto *cwebview = reinterpret_cast<CWebView *>(thisclass);

    auto *msg = webkit_javascript_result_get_js_value(message);
    char *value = jsc_value_to_string(msg);

    cwebview->jsexec_async("test_callback('Retorno do WebKit OK: "+string(value)+"')");
}

void WebSDK::install_handlers()
{
    cwebview->jsexec_async("alert('teste de evento gerado no webkit')");

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
}