// enviar request RPC para o backend/webserver
function SendRPC(engine = 'webkit', method = '', params = '', callback = '')
{
    let data = { 'engine': engine,
                 'method': method,
                 'callback': callback,
                 'params': params,
                 'id': Math.random().toString(36).substr(2, 9) };

    window.webkit.messageHandlers.jsonRPC.postMessage(JSON.stringify(data));
}