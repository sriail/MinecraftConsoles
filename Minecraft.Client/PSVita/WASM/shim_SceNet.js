/**
 * shim_SceNet.js
 * Emscripten JS-library glue for the SceNet shim.
 *
 * Maps Vita BSD socket operations to browser WebSocket API.
 * vita_net_ws_connect is listed as an ASYNCIFY_IMPORT so that Asyncify can
 * suspend the C call stack while waiting for the WebSocket handshake.
 *
 * Usage:  emcc ... --js-library shim_SceNet.js
 *              -s ASYNCIFY_IMPORTS='["vita_net_ws_connect"]'
 */

mergeInto(LibraryManager.library, {

    vita_net_ws_connect__deps: [],
    vita_net_ws_connect__async: true,
    vita_net_ws_connect: function(sockId, ipPtr, port) {
        return new Promise(function(resolve) {
            var ip  = UTF8ToString(ipPtr);
            var url = 'ws://' + ip + ':' + port + '/';
            var ws  = new WebSocket(url);
            ws.binaryType = 'arraybuffer';

            if (!Module._vita_net_sockets) Module._vita_net_sockets = {};
            Module._vita_net_sockets[sockId] = { ws: ws, closed: false };

            ws.onopen = function() { resolve(0); };
            ws.onerror = function() { resolve(-1); };

            ws.onmessage = function(ev) {
                var data = new Uint8Array(ev.data);
                /* Push bytes into the C-side ring buffer */
                var ptr = Module._malloc(data.length);
                Module.HEAPU8.set(data, ptr);
                Module._vita_net_ws_push_recv(sockId, ptr, data.length);
                Module._free(ptr);
            };

            ws.onclose = function() {
                if (Module._vita_net_sockets[sockId]) {
                    Module._vita_net_sockets[sockId].closed = true;
                }
            };
        });
    },

    vita_net_ws_close__deps: [],
    vita_net_ws_close: function(sockId) {
        if (!Module._vita_net_sockets) return;
        var entry = Module._vita_net_sockets[sockId];
        if (!entry) return;
        entry.ws.close();
        delete Module._vita_net_sockets[sockId];
    },

    vita_net_ws_send__deps: [],
    vita_net_ws_send: function(sockId, bufPtr, len) {
        if (!Module._vita_net_sockets) return -1;
        var entry = Module._vita_net_sockets[sockId];
        if (!entry || entry.closed) return -1;
        var data = Module.HEAPU8.slice(bufPtr, bufPtr + len);
        entry.ws.send(data.buffer);
        return len;
    }
});
