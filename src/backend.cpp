#include "backend.h"

JSBackEnd::JSBackEnd()
{
    J = js_newstate(NULL, NULL, !JS_STRICT);

	js_newcfunction(J, js_print, "print", 0);
	js_setglobal(J, "print");

    js_dostring(J, "print('teste do MUJS')");
};

JSBackEnd::~JSBackEnd()
{
    js_gc(J, 0);
	js_freestate(J);
}

void JSBackEnd::js_print(js_State *J)
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
