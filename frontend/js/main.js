$(function() {
    $('button').on('click', function() {
        let data = [];

        data.push({'id': 1234});
        data.push({'method': 'test'});

        // envia o RPC pelo handler do webkit
        window.webkit.messageHandlers.jsonRPC.postMessage(JSON.stringify(data));
    });
});

function test_callback(params) {
    $('#retorno').html(params);
}