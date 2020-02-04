#include "backend.h"
#include <map>

//! funcoes hardcoded acessadas atraves do lado JS precisam ser
//! declaradas como static, antes da declaracao da classe
//! JSBackEnd para ficaram visiveis somente neste escopo
static void js_print(js_State *J)
{
    int i, top = js_gettop(J);
    for (i = 1; i < top; ++i) {
        const char *s = js_tostring(J, i);
        if (i > 1) cout << ' ';
        cout << s;
    }
    cout << endl;

    js_pushundefined(J);
}

//! construtor da classe
JSBackEnd::JSBackEnd()
{
    // instancia VM do MUJS
    J = js_newstate(NULL, NULL, !JS_STRICT);

    // funcoes hard coded em C visiveis no lado do JS
	js_newcfunction(J, js_print, "print", 0);
    js_setglobal(J, "print");

    // carrega funcoes JS do backend
    js_dofile(J, "backend/backend.js");
};

//! destrutor da classe
JSBackEnd::~JSBackEnd()
{
    js_gc(J, 0);
	js_freestate(J);
}

//! executa uma sentença JS na engine instanciada
void JSBackEnd::mujs_execute(string js)
{
    js_dostring(J, js.c_str());
}