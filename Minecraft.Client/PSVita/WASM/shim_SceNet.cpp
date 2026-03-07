/*
 * shim_SceNet.cpp
 * PS Vita SceNet BSD socket API → WebSockets shim implementation
 *
 * Build requirements:
 *   --js-library shim_SceNet.js
 *   -s ASYNCIFY=1
 *   -s ASYNCIFY_IMPORTS=["vita_net_ws_connect"]
 *
 * Networking model:
 *   All TCP "sockets" are mapped to WebSocket connections.
 *   UDP is unsupported natively in browsers; we return SCE_NET_ERROR_ENOTSUP
 *   for SOCK_DGRAM and log a [VITA_SHIM_STUB] message.
 *   The WebSocket URL is constructed as: ws://<host>:<port>/ where host/port
 *   are extracted from the SceNetSockaddrIn passed to sceNetConnect.
 *
 * Buffer model:
 *   Received data is stored in a per-socket ring buffer (64 KB).
 *   sceNetRecv copies from this ring into the caller's buffer.
 *   The JS side pushes incoming message bytes into the ring.
 */

#include "shim_SceNet.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#ifdef __EMSCRIPTEN__
#  include <emscripten.h>
#endif

/* =========================================================================
 * Socket table
 * ========================================================================= */

#define MAX_SOCKETS   64
#define RECV_RING_SZ  (65536)

typedef struct {
    int      active;
    int      type;          /* SOCK_STREAM or SOCK_DGRAM */
    int      connected;
    uint32_t remote_addr;
    uint16_t remote_port;
    /* Receive ring buffer */
    uint8_t  ring[RECV_RING_SZ];
    int      ring_head;
    int      ring_tail;
} SockSlot;

static SockSlot s_sockets[MAX_SOCKETS];

static int alloc_socket(int type)
{
    for (int i = 0; i < MAX_SOCKETS; i++) {
        if (!s_sockets[i].active) {
            memset(&s_sockets[i], 0, sizeof(SockSlot));
            s_sockets[i].active = 1;
            s_sockets[i].type   = type;
            return i;
        }
    }
    return SCE_NET_ERROR_ENOMEM;
}

/* =========================================================================
 * Ring buffer helpers
 * ========================================================================= */

static int ring_available(SockSlot *s)
{
    return (s->ring_tail - s->ring_head + RECV_RING_SZ) % RECV_RING_SZ;
}

static void ring_push(SockSlot *s, const uint8_t *data, int len)
{
    for (int i = 0; i < len; i++) {
        int next = (s->ring_tail + 1) % RECV_RING_SZ;
        if (next == s->ring_head) break; /* overflow: drop */
        s->ring[s->ring_tail] = data[i];
        s->ring_tail = next;
    }
}

static int ring_pop(SockSlot *s, uint8_t *dst, int maxLen)
{
    int n = 0;
    while (n < maxLen && s->ring_head != s->ring_tail) {
        dst[n++] = s->ring[s->ring_head];
        s->ring_head = (s->ring_head + 1) % RECV_RING_SZ;
    }
    return n;
}

/* Called from JS (shim_SceNet.js) when a WebSocket message arrives */
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void vita_net_ws_push_recv(int sockId, const uint8_t *data, int len)
{
    if (sockId < 0 || sockId >= MAX_SOCKETS) return;
    if (!s_sockets[sockId].active) return;
    ring_push(&s_sockets[sockId], data, len);
}

/* =========================================================================
 * sceNetInit / sceNetTerm
 * VITA:  Initialises / terminates the networking module
 * WEB:   No-op; WebSockets are always available in the browser
 * ========================================================================= */

int sceNetInit(void)
{
    memset(s_sockets, 0, sizeof(s_sockets));
    return SCE_OK;
}

int sceNetTerm(void)
{
    for (int i = 0; i < MAX_SOCKETS; i++) {
        if (s_sockets[i].active) sceNetClose(i);
    }
    return SCE_OK;
}

/* =========================================================================
 * sceNetSocket
 * VITA:  Creates a BSD socket
 * WEB:   Allocates a slot; WebSocket object created in sceNetConnect
 * NOTE:  UDP (SOCK_DGRAM) is stubbed with a [VITA_SHIM_STUB] warning.
 * ========================================================================= */

int sceNetSocket(const char *name, int domain, int type, int protocol)
{
    (void)name; (void)domain; (void)protocol;
    if (type == SCE_NET_SOCK_DGRAM) {
        fprintf(stderr,
            "[VITA_SHIM_STUB] sceNetSocket: UDP (SOCK_DGRAM) unsupported "
            "in WASM; returning error\n");
        return SCE_NET_ERROR_ENOTSUP;
    }
    return alloc_socket(type);
}

/* =========================================================================
 * sceNetClose
 * VITA:  Closes a socket
 * WEB:   Closes the underlying WebSocket via vita_net_ws_close()
 * ========================================================================= */

int sceNetClose(int s)
{
    if (s < 0 || s >= MAX_SOCKETS) return SCE_NET_ERROR_EINVAL;
    if (!s_sockets[s].active) return SCE_NET_ERROR_EINVAL;
#ifdef __EMSCRIPTEN__
    EM_ASM({ vita_net_ws_close($0); }, s);
#endif
    s_sockets[s].active = 0;
    return SCE_OK;
}

/* =========================================================================
 * sceNetConnect
 * VITA:  Establishes a TCP connection (blocking)
 * WEB:   Opens a WebSocket to ws://<addr>:<port>/ and waits for open event
 *        via Asyncify suspension (vita_net_ws_connect is an async import).
 * ========================================================================= */

int sceNetConnect(int s, const SceNetSockaddr *addr, SceNetSocklen_t addrlen)
{
    (void)addrlen;
    if (s < 0 || s >= MAX_SOCKETS) return SCE_NET_ERROR_EINVAL;
    if (!s_sockets[s].active) return SCE_NET_ERROR_EINVAL;
    if (!addr) return SCE_NET_ERROR_EINVAL;

    const SceNetSockaddrIn *in = (const SceNetSockaddrIn *)addr;
    uint32_t a = sceNetNtohl(in->sin_addr.s_addr);
    uint16_t p = sceNetNtohs(in->sin_port);

    s_sockets[s].remote_addr = a;
    s_sockets[s].remote_port = p;

    /* Format dotted-decimal IP */
    char ip[32];
    snprintf(ip, sizeof(ip), "%u.%u.%u.%u",
             (a >> 24) & 0xFF, (a >> 16) & 0xFF,
             (a >>  8) & 0xFF, (a      ) & 0xFF);

#ifdef __EMSCRIPTEN__
    /* vita_net_ws_connect is declared __async:true in the JS lib.
     * Asyncify will automatically suspend here until the Promise resolves.
     * The return value from the resolved Promise is the integer result. */
    int result = EM_ASM_INT({
        return vita_net_ws_connect($0, UTF8ToString($1), $2);
    }, s, ip, (int)p);
    if (result != 0) return SCE_NET_ERROR_EPIPE;
    s_sockets[s].connected = 1;
#endif
    return SCE_OK;
}

/* =========================================================================
 * sceNetBind / sceNetListen / sceNetAccept
 * VITA:  Server-side socket calls
 * WEB:   Browsers cannot listen for incoming connections
 *        [VITA_SHIM_STUB] — always return error
 * ========================================================================= */

int sceNetBind(int s, const SceNetSockaddr *addr, SceNetSocklen_t addrlen)
{
    (void)s; (void)addr; (void)addrlen;
    fprintf(stderr, "[VITA_SHIM_STUB] sceNetBind: server sockets unsupported\n");
    return SCE_NET_ERROR_ENOTSUP;
}

int sceNetListen(int s, int backlog)
{
    (void)s; (void)backlog;
    fprintf(stderr, "[VITA_SHIM_STUB] sceNetListen: server sockets unsupported\n");
    return SCE_NET_ERROR_ENOTSUP;
}

int sceNetAccept(int s, SceNetSockaddr *addr, SceNetSocklen_t *addrlen)
{
    (void)s; (void)addr; (void)addrlen;
    fprintf(stderr, "[VITA_SHIM_STUB] sceNetAccept: server sockets unsupported\n");
    return SCE_NET_ERROR_ENOTSUP;
}

/* =========================================================================
 * sceNetSend / sceNetRecv
 * VITA:  BSD send/recv (blocking)
 * WEB:   sceNetSend → WebSocket.send (async in JS, called synchronously)
 *        sceNetRecv → polls ring buffer; if empty, sleeps via Asyncify
 * ========================================================================= */

int sceNetSend(int s, const void *buf, size_t len, int flags)
{
    (void)flags;
    if (s < 0 || s >= MAX_SOCKETS) return SCE_NET_ERROR_EINVAL;
    if (!s_sockets[s].active || !s_sockets[s].connected)
        return SCE_NET_ERROR_EPIPE;
    if (!buf || len == 0) return 0;

#ifdef __EMSCRIPTEN__
    int sent = EM_ASM_INT({
        return vita_net_ws_send($0, $1, $2);
    }, s, buf, (int)len);
    return sent < 0 ? SCE_NET_ERROR_EPIPE : sent;
#else
    return (int)len;
#endif
}

int sceNetRecv(int s, void *buf, size_t len, int flags)
{
    if (s < 0 || s >= MAX_SOCKETS) return SCE_NET_ERROR_EINVAL;
    if (!s_sockets[s].active) return SCE_NET_ERROR_EINVAL;
    if (!buf || len == 0) return 0;

    int nonblock = (flags & SCE_NET_MSG_DONTWAIT);

#ifdef __EMSCRIPTEN__
    /* Poll for data; yield if nothing available and not non-blocking */
    while (ring_available(&s_sockets[s]) == 0) {
        if (nonblock) return SCE_NET_ERROR_EAGAIN;
        emscripten_sleep(1); /* yield 1 ms */
    }
#endif
    return ring_pop(&s_sockets[s], (uint8_t *)buf, (int)len);
}

int sceNetSendto(int s, const void *buf, size_t len, int flags,
                 const SceNetSockaddr *to, SceNetSocklen_t tolen)
{
    (void)to; (void)tolen;
    return sceNetSend(s, buf, len, flags);
}

int sceNetRecvfrom(int s, void *buf, size_t len, int flags,
                   SceNetSockaddr *from, SceNetSocklen_t *fromlen)
{
    (void)from; (void)fromlen;
    return sceNetRecv(s, buf, len, flags);
}

/* =========================================================================
 * DNS resolver stub
 * VITA:  sceNetResolverStartNtoa
 * WEB:   Browsers restrict DNS lookups; we stub this as SCE_OK with zeroed addr
 * NOTE:  [VITA_SHIM_STUB]
 * ========================================================================= */

int sceNetResolverStartNtoa(const char *hostname, SceNetInAddr *addr,
                             uint32_t timeout, int retry, int flags)
{
    (void)timeout; (void)retry; (void)flags;
    fprintf(stderr,
        "[VITA_SHIM_STUB] sceNetResolverStartNtoa(\"%s\"): DNS resolution "
        "unavailable in WASM; addr zeroed\n", hostname ? hostname : "(null)");
    if (addr) addr->s_addr = 0;
    return SCE_OK;
}

/* =========================================================================
 * Byte-order helpers
 * ========================================================================= */

uint32_t sceNetHtonl(uint32_t h)
{
    return ((h & 0xFF000000u) >> 24) |
           ((h & 0x00FF0000u) >>  8) |
           ((h & 0x0000FF00u) <<  8) |
           ((h & 0x000000FFu) << 24);
}

uint16_t sceNetHtons(uint16_t h)
{
    return (uint16_t)(((h & 0xFF00u) >> 8) | ((h & 0x00FFu) << 8));
}

uint32_t sceNetNtohl(uint32_t n) { return sceNetHtonl(n); }
uint16_t sceNetNtohs(uint16_t n) { return sceNetHtons(n); }

uint32_t sceNetInetAddr(const char *cp)
{
    if (!cp) return 0;
    unsigned int a, b, c, d;
    if (sscanf(cp, "%u.%u.%u.%u", &a, &b, &c, &d) != 4) return 0;
    return sceNetHtonl((a << 24) | (b << 16) | (c << 8) | d);
}
