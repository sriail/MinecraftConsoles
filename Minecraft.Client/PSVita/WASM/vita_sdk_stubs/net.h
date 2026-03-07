#pragma once
/*
 * Minimal PS Vita networking SDK stubs for Emscripten/WASM builds.
 */

#include <stdint.h>

/* -------------------------------------------------------------------------
 * Ethernet / addressing constants
 * ---------------------------------------------------------------------- */
#define SCE_NET_ETHER_ADDR_LEN  6

/* -------------------------------------------------------------------------
 * Network address types
 * ---------------------------------------------------------------------- */
typedef uint32_t SceNetInAddr_t;

typedef struct SceNetInAddr {
    SceNetInAddr_t s_addr;
} SceNetInAddr;

typedef struct SceNetSockaddr {
    uint8_t sa_len;
    uint8_t sa_family;
    char    sa_data[14];
} SceNetSockaddr;

typedef uint32_t SceNetSocklen_t;
