#pragma once
// Minimal PS Vita libnetctl.h stub for Emscripten/WASM builds.

#include <stdint.h>

typedef enum SceNetCtlState {
    SCE_NETCTL_STATE_DISCONNECTED = 0,
    SCE_NETCTL_STATE_NEGOTIATING  = 1,
    SCE_NETCTL_STATE_CONNECTED    = 2
} SceNetCtlState;

typedef union SceNetCtlInfo {
    char ip_config[64];
    struct { char ip[16]; } ip_address;
} SceNetCtlInfo;

typedef void (*SceNetCtlCallback)(int event, void *arg);

#define SCE_NETCTL_INFO_GET_IP_ADDRESS 8

#ifdef __cplusplus
extern "C" {
#endif

int sceNetCtlInit(void);
int sceNetCtlTerm(void);
int sceNetCtlInetGetInfo(int code, SceNetCtlInfo *info);
int sceNetCtlInetGetState(SceNetCtlState *state);
int sceNetCtlInetRegisterCallback(SceNetCtlCallback func, void *arg, int *cid);
int sceNetCtlInetUnregisterCallback(int cid);
int sceNetCtlCheckCallback(void);

#ifdef __cplusplus
}
#endif
