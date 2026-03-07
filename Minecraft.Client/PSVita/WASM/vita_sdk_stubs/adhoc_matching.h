#pragma once
/* adhoc_matching.h — PS Vita Ad-hoc Matching API stubs for Emscripten/WASM builds */

#include <stdint.h>
#include "kernel.h"
#include "net.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCE_NET_ADHOC_MATCHING_POOLSIZE_DEFAULT  (128*1024)

#define SCE_NET_ADHOC_MATCHING_EVENT_HELLO           1
#define SCE_NET_ADHOC_MATCHING_EVENT_REQUEST         2
#define SCE_NET_ADHOC_MATCHING_EVENT_LEAVE           3
#define SCE_NET_ADHOC_MATCHING_EVENT_DENY            4
#define SCE_NET_ADHOC_MATCHING_EVENT_CANCEL          5
#define SCE_NET_ADHOC_MATCHING_EVENT_ACCEPT          6
#define SCE_NET_ADHOC_MATCHING_EVENT_ESTABLISHED     7
#define SCE_NET_ADHOC_MATCHING_EVENT_TIMEOUT         8
#define SCE_NET_ADHOC_MATCHING_EVENT_ERROR           9
#define SCE_NET_ADHOC_MATCHING_EVENT_BYE             10
#define SCE_NET_ADHOC_MATCHING_EVENT_DATA            11
#define SCE_NET_ADHOC_MATCHING_EVENT_DATA_ACK        12
#define SCE_NET_ADHOC_MATCHING_EVENT_DATA_TIMEOUT    13

#define SCE_NET_ADHOC_MATCHING_MODE_HOST    1
#define SCE_NET_ADHOC_MATCHING_MODE_CLIENT  2
#define SCE_NET_ADHOC_MATCHING_MODE_P2P     3

typedef int SceNetAdhocMatchingContext;

typedef void (*SceNetAdhocMatchingHandler)(int id, int event,
    SceNetEtherAddr *peer, int optlen, void *opt);

static inline int sceNetAdhocMatchingInit(int poolSize, void *pool)
{ (void)poolSize; (void)pool; return SCE_OK; }

static inline int sceNetAdhocMatchingTerm(void) { return SCE_OK; }

static inline int sceNetAdhocMatchingCreate(int mode, int maxPeers,
    uint16_t port, int rxBufLen, unsigned int helloInterval,
    unsigned int keepaliveInterval, int retryCount,
    unsigned int rexmtInterval, SceNetAdhocMatchingHandler handler)
{
    (void)mode; (void)maxPeers; (void)port; (void)rxBufLen;
    (void)helloInterval; (void)keepaliveInterval; (void)retryCount;
    (void)rexmtInterval; (void)handler;
    return 1; /* returns context ID */
}

static inline int sceNetAdhocMatchingStart(int id, int threadPriority,
    int threadStackSize, int cpuAffinityMask, int helloOptLen, void *helloOpt)
{
    (void)id; (void)threadPriority; (void)threadStackSize;
    (void)cpuAffinityMask; (void)helloOptLen; (void)helloOpt;
    return SCE_OK;
}

static inline int sceNetAdhocMatchingStop(int id)
{ (void)id; return SCE_OK; }

static inline int sceNetAdhocMatchingDelete(int id)
{ (void)id; return SCE_OK; }

static inline int sceNetAdhocMatchingSelectTarget(int id,
    SceNetEtherAddr *target, int optLen, void *opt)
{ (void)id; (void)target; (void)optLen; (void)opt; return SCE_OK; }

static inline int sceNetAdhocMatchingCancelTarget(int id, SceNetEtherAddr *target)
{ (void)id; (void)target; return SCE_OK; }

static inline int sceNetAdhocMatchingSendData(int id, SceNetEtherAddr *target,
    int dataLen, void *data)
{ (void)id; (void)target; (void)dataLen; (void)data; return SCE_OK; }

#ifdef __cplusplus
}
#endif
