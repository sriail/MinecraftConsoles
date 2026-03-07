#pragma once
// Minimal PS Vita net.h stub for Emscripten/WASM builds.

#include <stdint.h>
#include <stddef.h>

typedef uint32_t SceNetId;
typedef uint32_t SceNetSocklen_t;
typedef uint32_t SceNetInAddr_t;

typedef struct SceNetInAddr {
    SceNetInAddr_t s_addr;
} SceNetInAddr;

typedef struct SceNetSockaddrIn {
    uint8_t      sin_len;
    uint8_t      sin_family;
    uint16_t     sin_port;
    SceNetInAddr sin_addr;
    uint16_t     sin_vport;
    char         sin_zero[6];
} SceNetSockaddrIn;

typedef struct SceNetSockaddr {
    uint8_t sa_len;
    uint8_t sa_family;
    char    sa_data[14];
} SceNetSockaddr;

typedef struct SceNetInitParam {
    void    *memory;
    int      size;
    int      flags;
} SceNetInitParam;

#define SCE_NET_AF_INET   2
#define SCE_NET_SOCK_STREAM 1
#define SCE_NET_SOCK_DGRAM  2
#define SCE_NET_IPPROTO_TCP 6
#define SCE_NET_IPPROTO_UDP 17

#define SCE_NET_SO_REUSEADDR 0x0004
#define SCE_NET_SO_BROADCAST 0x0020
#define SCE_NET_SOL_SOCKET   0xffff

#define SCE_NET_ERROR_EAGAIN      (-2120022267)
#define SCE_NET_ERROR_EWOULDBLOCK SCE_NET_ERROR_EAGAIN

#ifdef __cplusplus
extern "C" {
#endif

int     sceNetInit(SceNetInitParam *param);
int     sceNetTerm(void);
int     sceNetPoolCreate(const char *name, int size, int flags);
int     sceNetPoolDestroy(int memid);
SceNetId sceNetSocket(const char *name, int domain, int type, int protocol);
int     sceNetSocketClose(SceNetId s);
int     sceNetBind(SceNetId s, const SceNetSockaddr *addr, SceNetSocklen_t addrlen);
int     sceNetSetsockopt(SceNetId s, int level, int optname,
                          const void *optval, SceNetSocklen_t optlen);
uint16_t sceNetHtons(uint16_t hostshort);
uint32_t sceNetHtonl(uint32_t hostlong);
uint16_t sceNetNtohs(uint16_t netshort);
uint32_t sceNetNtohl(uint32_t netlong);
int     sceNetConnect(SceNetId s, const SceNetSockaddr *addr, SceNetSocklen_t addrlen);
int     sceNetSend(SceNetId s, const void *msg, size_t len, int flags);
int     sceNetRecv(SceNetId s, void *buf, size_t len, int flags);
int     sceNetSendto(SceNetId s, const void *msg, size_t len, int flags,
                      const SceNetSockaddr *to, SceNetSocklen_t tolen);
int     sceNetRecvfrom(SceNetId s, void *buf, size_t len, int flags,
                        SceNetSockaddr *from, SceNetSocklen_t *fromlen);
int     sceNetAccept(SceNetId s, SceNetSockaddr *addr, SceNetSocklen_t *addrlen);
int     sceNetListen(SceNetId s, int backlog);
int     sceNetInetPton(int af, const char *src, void *dst);

#ifdef __cplusplus
}
#endif
