#ifndef SHIM_SCENET_H
#define SHIM_SCENET_H

/*
 * shim_SceNet.h
 * PS Vita SceNet BSD socket API → WebSockets / WebRTC shim
 *
 * The Vita uses a POSIX-style socket layer (SceNet) for TCP/UDP networking.
 * Browsers only support WebSockets (TCP) and WebRTC DataChannels (UDP-like).
 * This shim proxies:
 *   TCP → WebSocket (via a proxy server or direct WS endpoint)
 *   UDP → WebRTC DataChannel (unreliable, unordered) when WEBRTC=1 is defined
 *
 * ABI reference: vitasdk <net.h>, <netinet/in.h>
 */

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- Return codes ---------- */
#ifndef SCE_OK
#define SCE_OK 0
#endif
#define SCE_NET_ERROR_EPIPE      ((int)0x80410068)
#define SCE_NET_ERROR_ENOTSUP    ((int)0x80410076)
#define SCE_NET_ERROR_ECONNRESET ((int)0x80410056)
#define SCE_NET_ERROR_EINVAL     ((int)0x80410022)
#define SCE_NET_ERROR_EAGAIN     ((int)0x80410023)
#define SCE_NET_ERROR_ENOMEM     ((int)0x8041000C)
#define SCE_NET_ERROR_ENOENT     ((int)0x80410002)

/* ---------- Address families / socket types (match BSD) ---------- */
#define SCE_NET_AF_INET     2
#define SCE_NET_SOCK_STREAM 1   /* TCP */
#define SCE_NET_SOCK_DGRAM  2   /* UDP */
#define SCE_NET_IPPROTO_TCP 6
#define SCE_NET_IPPROTO_UDP 17

/* ---------- Address structs ---------- */
typedef uint32_t SceNetInAddr_t;

typedef struct SceNetInAddr {
    SceNetInAddr_t s_addr;
} SceNetInAddr;

typedef struct SceNetSockaddrIn {
    uint8_t       sin_len;
    uint8_t       sin_family;
    uint16_t      sin_port;
    SceNetInAddr  sin_addr;
    uint8_t       sin_zero[8];
} SceNetSockaddrIn;

typedef struct SceNetSockaddr {
    uint8_t  sa_len;
    uint8_t  sa_family;
    uint8_t  sa_data[14];
} SceNetSockaddr;

typedef uint32_t SceNetSocklen_t;

/* ---------- Flags ---------- */
#define SCE_NET_MSG_DONTWAIT  0x80

/* ---------- API ---------- */
int sceNetInit(void);
int sceNetTerm(void);

int sceNetSocket(const char *name, int domain, int type, int protocol);
int sceNetClose(int s);
int sceNetConnect(int s, const SceNetSockaddr *addr, SceNetSocklen_t addrlen);
int sceNetBind(int s, const SceNetSockaddr *addr, SceNetSocklen_t addrlen);
int sceNetListen(int s, int backlog);
int sceNetAccept(int s, SceNetSockaddr *addr, SceNetSocklen_t *addrlen);
int sceNetSend(int s, const void *buf, size_t len, int flags);
int sceNetRecv(int s, void *buf, size_t len, int flags);
int sceNetSendto(int s, const void *buf, size_t len, int flags,
                 const SceNetSockaddr *to, SceNetSocklen_t tolen);
int sceNetRecvfrom(int s, void *buf, size_t len, int flags,
                   SceNetSockaddr *from, SceNetSocklen_t *fromlen);

/* Resolver stubs */
int sceNetResolverStartNtoa(const char *hostname, SceNetInAddr *addr,
                             uint32_t timeout, int retry, int flags);

/* Helper: host-to-network byte order */
uint32_t sceNetHtonl(uint32_t h);
uint16_t sceNetHtons(uint16_t h);
uint32_t sceNetNtohl(uint32_t n);
uint16_t sceNetNtohs(uint16_t n);
uint32_t sceNetInetAddr(const char *cp);

#ifdef __cplusplus
}
#endif

#endif /* SHIM_SCENET_H */
