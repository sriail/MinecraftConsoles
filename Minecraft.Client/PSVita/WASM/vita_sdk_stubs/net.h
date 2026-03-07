#pragma once
#include <stdint.h>

#define SCE_NET_ETHER_ADDR_LEN 6

typedef struct SceNetEtherAddr {
    uint8_t data[SCE_NET_ETHER_ADDR_LEN];
} SceNetEtherAddr;

typedef struct SceNetInAddr {
    uint32_t s_addr;
} SceNetInAddr;

typedef uint32_t SceNetInAddr_t;

typedef struct SceNetSockaddrIn {
    uint8_t sin_len;
    uint8_t sin_family;
    uint16_t sin_port;
    SceNetInAddr sin_addr;
    uint8_t sin_zero[8];
} SceNetSockaddrIn;

typedef struct SceNetSockaddr {
    uint8_t sa_len;
    uint8_t sa_family;
    uint8_t sa_data[14];
} SceNetSockaddr;

typedef uint16_t SceNetInPort_t;
typedef int SceNetId;
typedef uint32_t SceNetSocklen_t;

/* Adhoc types */
typedef struct SceNetAdhocctlAdhocId {
    int type;
    char data[9];
    uint8_t padding[3];
} SceNetAdhocctlAdhocId;

typedef struct SceNetAdhocctlGroupName {
    uint8_t data[8];
} SceNetAdhocctlGroupName;

#define SCE_NET_AF_INET     2
#define SCE_NET_SOCK_STREAM 1
#define SCE_NET_SOCK_DGRAM  2
#define SCE_NET_IPPROTO_TCP 6
#define SCE_NET_IPPROTO_UDP 17
