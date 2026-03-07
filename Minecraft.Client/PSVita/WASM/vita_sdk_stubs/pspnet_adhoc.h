#pragma once
#include <stdint.h>

/* pspnet_adhoc.h - PS Vita adhoc networking stubs */

typedef struct SceNetAdhocctlPeerInfo {
    struct SceNetAdhocctlPeerInfo *next;
    uint8_t macAddr[6];
    uint8_t nickname[128];
    uint32_t padding;
    uint64_t lastRecv;
} SceNetAdhocctlPeerInfo;

#ifdef __cplusplus
extern "C" {
#endif

int sceNetAdhocctlInit(const void *adhocId);
int sceNetAdhocctlTerm(void);
int sceNetAdhocctlConnect(const void *groupName);
int sceNetAdhocctlDisconnect(void);
int sceNetAdhocctlGetPeerList(int *buflen, void *buf);
int sceNetAdhocctlGetAdhocId(void *adhocId);
int sceNetAdhocPdpCreate(const void *addr, uint16_t port, uint32_t bufSize, int flag);
int sceNetAdhocPdpSend(int id, const void *destAddr, uint16_t port, const void *data, int len, uint32_t timeout, int flag);
int sceNetAdhocPdpRecv(int id, void *srcAddr, uint16_t *port, void *buf, int *len, uint32_t timeout, int flag);
int sceNetAdhocPdpDelete(int id, int flag);

#ifdef __cplusplus
}
#endif
