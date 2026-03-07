#pragma once
/* pspnet_adhoc.h — PS Vita PSP-net Ad-hoc stubs for Emscripten/WASM builds */

#include <stdint.h>
#include "kernel.h"
#include "net.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCE_NET_ADHOCCTL_GROUP_NAME_LEN   8
#define SCE_NET_ADHOCCTL_ADHOC_ID_LEN    16

typedef struct {
    uint8_t data[SCE_NET_ADHOCCTL_GROUP_NAME_LEN];
} SceNetAdhocctlGroupName;

typedef struct {
    uint8_t type;
    uint8_t data[SCE_NET_ADHOCCTL_ADHOC_ID_LEN];
} SceNetAdhocctlAdhocId;

static inline int sceNetAdhocInit(void) { return SCE_OK; }
static inline int sceNetAdhocTerm(void) { return SCE_OK; }

static inline int sceNetAdhocctlInit(const SceNetAdhocctlAdhocId *adhocId)
{ (void)adhocId; return SCE_OK; }

static inline int sceNetAdhocctlTerm(void) { return SCE_OK; }

static inline int sceNetAdhocctlConnect(const SceNetAdhocctlGroupName *groupName)
{ (void)groupName; return SCE_OK; }

static inline int sceNetAdhocctlDisconnect(void) { return SCE_OK; }

static inline int sceNetAdhocctlGetPeerList(int *pBufLen, void *pBuf)
{ (void)pBuf; if (pBufLen) *pBufLen = 0; return SCE_OK; }

static inline int sceNetAdhocctlGetState(int *pState)
{ if (pState) *pState = 0; return SCE_OK; }

#ifdef __cplusplus
}
#endif
