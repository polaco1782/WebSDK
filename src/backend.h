#ifndef BACKEND_H
#define BACKEND_H

#include "websdk.h"
#include "mujs/mujs.h"

// metodos estaticos executados pela API em C do MUJS
class JSBackEnd
{
    private:
    js_State *J;

    public:
    JSBackEnd();
    ~JSBackEnd();

    void mujs_execute(string js);
};


#endif
