#include "headers.hpp"

// execuçao de JS assincrono da thread principal
void CWebView::jsexec_async(string js)
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

// registra o handler de evento no webkit
void CWebView::register_message(const string &handler, GCallback callback)
{
    webkit_user_content_manager_register_script_message_handler(ContentManager, handler.c_str());
    g_signal_connect(ContentManager, string("script-message-received::" + handler).c_str(), G_CALLBACK(callback), this);
}

void CWebView::setup_settings()
{
    ContentManager = webkit_web_view_get_user_content_manager(*this);
    Settings = webkit_web_view_get_settings(*this);

    // permitir acesso a arquivos usando file:///
    webkit_settings_set_allow_file_access_from_file_urls(Settings, TRUE);
    webkit_settings_set_enable_developer_extras(Settings, TRUE);
    webkit_web_view_set_settings(*this, Settings);
}

void CWebView::show_inspector()
{
    inspector = webkit_web_view_get_inspector(*this);
    webkit_web_inspector_show(inspector);
}

string CWebView::process_request(string json_request)
{
    auto x = crow::json::load(json_request);

    if(!!x)
    {
        string e = x["engine"].s();
        string m = x["method"].s();
        string c = x["callback"].s();
        string p = x["params"].s();

        cout << "Metodo recebido: " << m << endl;
        cout << "Callback recebido: " << c << endl;
        cout << "Parametros recebido: " << p << endl;

        if(e == "mujs")
        {
            JSBackEnd *js = new JSBackEnd();
            js->mujs_execute(m+"('"+p+"','"+c+"')");
        }
        else
        if(e == "webkit")
        {
            jsexec_async(m+"('"+p+"','"+c+"')");
        }
        else
        if(e == "websocket")
        {

        }
    }
}
