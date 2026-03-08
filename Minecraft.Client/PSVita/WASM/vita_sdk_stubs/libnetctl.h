#pragma once

typedef void (*SceNetCtlCallback)(int eventType, void *arg);

typedef union SceNetCtlInfo {
    unsigned int device;
    char ssid[32];
    char name[16];
    unsigned char etherAddr[6];
    unsigned int mtu;
    int linkState;
    char ipAddress[16];
} SceNetCtlInfo;

#define SCE_NETCTL_INFO_GET_IP_ADDRESS    2
#define SCE_NETCTL_INFO_GET_ETHER_ADDR    1
#define SCE_NETCTL_INFO_GET_MTU           3

#define SCE_NET_CTL_STATE_DISCONNECTED    0
#define SCE_NET_CTL_STATE_CONNECTING      1
#define SCE_NET_CTL_STATE_CONNECTED       3

inline int sceNetCtlInit(void) {
    return 0;
}

inline int sceNetCtlGetState(int *state) {
    if (state) *state = SCE_NET_CTL_STATE_DISCONNECTED;
    return 0;
}

inline int sceNetCtlGetInfo(int code, SceNetCtlInfo *info) {
    (void)code; (void)info;
    return 0;
}

inline void sceNetCtlTerm(void) {
}

inline int sceNetCtlRegisterCallback(SceNetCtlCallback func, void *arg, int *cid) {
    (void)func; (void)arg;
    if (cid) *cid = 0;
    return 0;
}

inline int sceNetCtlUnregisterCallback(int cid) {
    (void)cid;
    return 0;
}
