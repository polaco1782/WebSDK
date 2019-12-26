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

window.addEventListener("message", function(event)
{
    console.log(event.data);
    //alert( "received: " + event.data );
    // can message back using event.source.postMessage(...)
});