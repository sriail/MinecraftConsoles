#pragma once
/* net.h — PS Vita network API stubs for Emscripten/WASM builds */

#include <stdint.h>
#include <string.h>
#include "kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Address families / socket types */
#define SCE_NET_AF_INET          2
#define SCE_NET_SOCK_STREAM      1
#define SCE_NET_SOCK_DGRAM       2
#define SCE_NET_SOCK_DGRAM_P2P   11
#define SCE_NET_SOL_SOCKET       0xFFFF

/* Socket options */
#define SCE_NET_SO_REUSEADDR     0x0004
#define SCE_NET_SO_NBIO          0x1200
#define SCE_NET_SO_USECRYPTO     0x1300
#define SCE_NET_SO_USESIGNATURE  0x1301

/* Network info types */
#define SCE_NETCTL_INFO_GET_IP_ADDRESS   8
#define SCE_NETCTL_INFO_GET_ETHER_ADDR   1

#define SCE_NET_ETHER_ADDR_LEN           6

/* Types */
typedef uint32_t SceNetInAddr_t;
typedef uint32_t SceNetSocklen_t;

typedef struct { uint8_t data[SCE_NET_ETHER_ADDR_LEN]; } SceNetEtherAddr;

typedef struct {
    uint32_t s_addr;
} SceNetInAddr;

typedef struct {
    uint8_t sa_len;
    uint8_t sa_family;
    uint8_t sa_data[14];
} SceNetSockaddr;

typedef struct {
    uint8_t          sin_len;
    uint8_t          sin_family;
    uint16_t         sin_port;
    SceNetInAddr     sin_addr;
    uint16_t         sin_vport;
    uint8_t          sin_zero[6];
} SceNetSockaddrIn;

typedef union {
    struct { uint32_t s_addr; } ip;
    char ipString[16];
} SceNetCtlInfo;

/* Byte-order helpers */
static inline uint16_t sceNetHtons(uint16_t v) { return (uint16_t)((v << 8) | (v >> 8)); }
static inline uint16_t sceNetNtohs(uint16_t v) { return sceNetHtons(v); }
static inline uint32_t sceNetHtonl(uint32_t v)
{ return ((v & 0xFF) << 24) | (((v >> 8) & 0xFF) << 16) | (((v >> 16) & 0xFF) << 8) | (v >> 24); }
static inline uint32_t sceNetNtohl(uint32_t v) { return sceNetHtonl(v); }

static inline int sceNetInetPton(int af, const char *src, void *dst)
{ (void)af; (void)src; (void)dst; return 0; }
static inline const char *sceNetInetNtop(int af, const void *src,
    char *dst, SceNetSocklen_t size)
{ (void)af; (void)src; if (dst && size) dst[0] = '\0'; return dst; }

/* Socket API stubs */
static inline int sceNetSocket(const char *name, int domain, int type, int protocol)
{ (void)name; (void)domain; (void)type; (void)protocol; return -1; }

static inline int sceNetClose(int s)
{ (void)s; return SCE_OK; }

static inline int sceNetBind(int s, const SceNetSockaddr *addr, SceNetSocklen_t addrLen)
{ (void)s; (void)addr; (void)addrLen; return SCE_OK; }

static inline int sceNetConnect(int s, const SceNetSockaddr *addr, SceNetSocklen_t addrLen)
{ (void)s; (void)addr; (void)addrLen; return SCE_OK; }

static inline int sceNetSend(int s, const void *buf, unsigned int len, int flags)
{ (void)s; (void)buf; (void)len; (void)flags; return 0; }

static inline int sceNetRecv(int s, void *buf, unsigned int len, int flags)
{ (void)s; (void)buf; (void)len; (void)flags; return 0; }

static inline int sceNetSendto(int s, const void *buf, unsigned int len, int flags,
    const SceNetSockaddr *addr, SceNetSocklen_t addrLen)
{ (void)s; (void)buf; (void)len; (void)flags; (void)addr; (void)addrLen; return 0; }

static inline int sceNetRecvfrom(int s, void *buf, unsigned int len, int flags,
    SceNetSockaddr *addr, SceNetSocklen_t *addrLen)
{ (void)s; (void)buf; (void)len; (void)flags; (void)addr; (void)addrLen; return 0; }

static inline int sceNetSetsockopt(int s, int level, int optname,
    const void *optval, SceNetSocklen_t optlen)
{ (void)s; (void)level; (void)optname; (void)optval; (void)optlen; return SCE_OK; }

static inline int sceNetGetsockopt(int s, int level, int optname,
    void *optval, SceNetSocklen_t *optlen)
{ (void)s; (void)level; (void)optname; (void)optval; (void)optlen; return SCE_OK; }

static inline int sceNetGetMacAddress(SceNetEtherAddr *addr, int flags)
{
    (void)flags;
    if (addr) memset(addr->data, 0, SCE_NET_ETHER_ADDR_LEN);
    return SCE_OK;
}

/* Network control */
#define SCE_NETCTL_STATE_DISCONNECTED   0
#define SCE_NETCTL_STATE_CONNECTED      3

static inline int sceNetCtlInit(void) { return SCE_OK; }
static inline int sceNetCtlTerm(void) { return SCE_OK; }

static inline int sceNetCtlInetGetState(int *pState)
{
    if (pState) *pState = SCE_NETCTL_STATE_CONNECTED;
    return SCE_OK;
}

static inline int sceNetCtlInetGetInfo(int code, SceNetCtlInfo *pInfo)
{
    (void)code;
    if (pInfo) memset(pInfo, 0, sizeof(*pInfo));
    return SCE_OK;
}

static inline int sceNetCtlInetRegisterCallback(void *cb, void *arg, int *hid)
{ (void)cb; (void)arg; if (hid) *hid = 0; return SCE_OK; }

static inline int sceNetCtlInetUnregisterCallback(int hid)
{ (void)hid; return SCE_OK; }

static inline int sceNetCtlCheckCallback(void) { return SCE_OK; }

#ifdef __cplusplus
}
#endif
