// enviar request RPC para o middleware
function SendRPC(method, callback, params)
{
    let data = { 'jsonrpc': '2.0',
                 'method': method,
                 'callback': callback,
                 'params': params,
                 'id': Math.random().toString(36).substr(2, 9) };

    window.webkit.messageHandlers.jsonRPC.postMessage(JSON.stringify(data));
}

function test()
{
    SendRPC('xxx', 'yyy', [1,2,3,4,5]);
}

// retorno da resposta RPC será no EventListener
window.addEventListener("message", function(event)
{
    alert(event.data);
    //alert( "received: " + event.data );
    // can message back using event.source.postMessage(...)
});