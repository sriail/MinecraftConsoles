#pragma once
// Minimal PS Vita pspnet_adhoc.h stub for Emscripten/WASM builds.
#include <stdint.h>
typedef uint8_t  SceNetEtherAddr[6];
typedef struct ScePspnetAdhocPollSd {
    int sd;
    int events;
    int revents;
} ScePspnetAdhocPollSd;
#define SCE_NET_ADHOC_MATCHING_EVENT_HELLO 1
#ifdef __cplusplus
extern "C" {
#endif
int sceNetAdhocInit(void);
int sceNetAdhocTerm(void);
int sceNetAdhocGetEtherAddr(SceNetEtherAddr *addr);
int sceNetAdhocPdpCreate(const SceNetEtherAddr *saddr, uint16_t sport, int bufsize, int flags);
int sceNetAdhocPdpSend(int id, const SceNetEtherAddr *daddr, uint16_t dport, const void *data, int len, int timeout, int flags);
int sceNetAdhocPdpRecv(int id, SceNetEtherAddr *saddr, uint16_t *sport, void *buf, int *buflen, int timeout, int flags);
int sceNetAdhocPdpDelete(int id, int flags);
int sceNetAdhocPollSocket(ScePspnetAdhocPollSd *sds, int nsds, int timeout);
#ifdef __cplusplus
}
#endif
