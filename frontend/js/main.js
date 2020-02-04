// enviar request RPC para o middleware
function SendRPC(method, callback = '', params = '')
{
    let data = { 'jsonrpc': '2.0',
                 'method': method,
                 'callback': callback,
                 'params': params,
                 'id': Math.random().toString(36).substr(2, 9) };

    window.webkit.messageHandlers.jsonRPC.postMessage(JSON.stringify(data));
}