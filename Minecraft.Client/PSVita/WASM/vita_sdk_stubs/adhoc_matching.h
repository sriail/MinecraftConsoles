#pragma once
// Minimal PS Vita adhoc_matching.h stub for Emscripten/WASM builds.
#include <stdint.h>
#include "pspnet_adhoc.h"
typedef void (*SceNetAdhocMatchingCallback)(int id, int event, const SceNetEtherAddr *peer, int optlen, const void *opt);
typedef struct SceNetAdhocMatchingMember {
    SceNetEtherAddr addr;
    struct SceNetAdhocMatchingMember *next;
} SceNetAdhocMatchingMember;
#define SCE_NET_ADHOC_MATCHING_EVENT_HELLO   1
#define SCE_NET_ADHOC_MATCHING_EVENT_REQUEST 2
#define SCE_NET_ADHOC_MATCHING_EVENT_LEAVE   3
#define SCE_NET_ADHOC_MATCHING_EVENT_DENY    4
#define SCE_NET_ADHOC_MATCHING_EVENT_CANCEL  5
#define SCE_NET_ADHOC_MATCHING_EVENT_ACCEPT  6
#define SCE_NET_ADHOC_MATCHING_EVENT_COMPLETE 7
#define SCE_NET_ADHOC_MATCHING_EVENT_TIMEOUT 8
#define SCE_NET_ADHOC_MATCHING_EVENT_DATA    9
#define SCE_NET_ADHOC_MATCHING_MODE_HOST   1
#define SCE_NET_ADHOC_MATCHING_MODE_CLIENT 2
#define SCE_NET_ADHOC_MATCHING_MODE_P2P    3
#ifdef __cplusplus
extern "C" {
#endif
int sceNetAdhocMatchingInit(int memId);
int sceNetAdhocMatchingTerm(void);
int sceNetAdhocMatchingCreate(int mode, int maxPeers, uint16_t port, int rxBufLen, int helloPeriod, int idlePeriod, int responseDelay, int keepAlivePeriod, int retryCount, SceNetAdhocMatchingCallback callback);
int sceNetAdhocMatchingStart(int id, int threadPri, int cpuAffinityMask, int stackSize, int optLen, const void *opt);
int sceNetAdhocMatchingStop(int id);
int sceNetAdhocMatchingDelete(int id);
int sceNetAdhocMatchingSelectTarget(int id, const SceNetEtherAddr *target, int optLen, const void *opt);
int sceNetAdhocMatchingCancelTarget(int id, const SceNetEtherAddr *target);
int sceNetAdhocMatchingSendData(int id, const SceNetEtherAddr *target, int optLen, const void *opt);
int sceNetAdhocMatchingGetMembers(int id, int *membersNum, SceNetAdhocMatchingMember *members);
#ifdef __cplusplus
}
#endif
