#include "headers.hpp"

static const char *require_js =
	"function require(name) {\n"
	"var cache = require.cache;\n"
	"if (name in cache) return cache[name];\n"
	"var exports = {};\n"
	"cache[name] = exports;\n"
	"Function('exports', read(name+'.js'))(exports);\n"
	"return exports;\n"
	"}\n"
	"require.cache = Object.create(null);\n";

static const char *stacktrace_js =
	"Error.prototype.toString = function() {\n"
	"if (this.stackTrace) return this.name + ': ' + this.message + this.stackTrace;\n"
	"return this.name + ': ' + this.message;\n"
	"};\n";

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

static void js_read(js_State *J)
{
    string filename = fs::absolute("backend/" + string(js_tostring(J, 1))).string();
    ifstream file(filename);
    string js;

    if(!file)
        js_error(J, "cannot open file '%s': %s", filename.c_str(), std::strerror(errno));

    js = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

	js_pushstring(J, js.c_str());

    file.close();
}

//! construtor da classe
JSBackEnd::JSBackEnd()
{
    // instancia VM do MUJS
    J = js_newstate(NULL, NULL, !JS_STRICT);

    // funcoes hard coded em C visiveis no lado do JS
	js_newcfunction(J, js_print, "print", 0);
    js_setglobal(J, "print");

	js_newcfunction(J, js_read, "read", 1);
	js_setglobal(J, "read");

	js_dostring(J, require_js);
	js_dostring(J, stacktrace_js);

    // carrega funcoes JS do backend
    js_dofile(J, "backend/backend.js");

    sqlite::database db("/tmp/teste.db");
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