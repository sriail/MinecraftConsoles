#pragma once
#include <stdint.h>

#define SCE_NETCTL_INFO_GET_IP_ADDRESS 2

typedef struct SceNetCtlInfo {
    char ip_address[16];
} SceNetCtlInfo;

#ifdef __cplusplus
extern "C" {
#endif

int sceNetCtlInit(void);
void sceNetCtlTerm(void);
int sceNetCtlGetInfo(int code, SceNetCtlInfo *info);
int sceNetCtlGetState(int *state);

#ifdef __cplusplus
}
#endif
