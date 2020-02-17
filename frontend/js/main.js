// enviar request RPC para o backend/webserver
function SendRPC(method, callback = '', params = '')
{
    let data = { 'method': method,
                 'callback': callback,
                 'params': params,
                 'id': Math.random().toString(36).substr(2, 9) };

    window.webkit.messageHandlers.jsonRPC.postMessage(JSON.stringify(data));
}

// recebe um request do backend/webserver
function RecvRPC(method, callback, params)
{
    
}