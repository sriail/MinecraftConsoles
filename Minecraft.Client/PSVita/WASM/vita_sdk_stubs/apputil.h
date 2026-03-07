#pragma once
// Minimal PS Vita apputil.h stub for Emscripten/WASM builds.

#include <stdint.h>
#include "np.h"

typedef struct SceAppUtilInitParam {
    uint32_t workBufSize;
    uint8_t  reserved[60];
} SceAppUtilInitParam;

typedef struct SceAppUtilBootParam {
    uint32_t attr;
    uint32_t appVersion;
    uint8_t  reserved[32];
} SceAppUtilBootParam;

// App event types
#define SCE_APPUTIL_APPEVENT_TYPE_NP_APP_DATA_MESSAGE        0x04
#define SCE_APPUTIL_APPEVENT_TYPE_NP_BASIC_JOINABLE_PRESENCE 0x08

typedef struct SceAppUtilAppEventParam {
    uint32_t type;
    uint8_t  param[512];
} SceAppUtilAppEventParam;

// NP joinable presence parameters (for session joining via friends list)
typedef struct SceAppUtilNpBasicJoinablePresenceParam {
    SceNpId  npId;
    uint8_t  data[512];
    uint32_t dataSize;
} SceAppUtilNpBasicJoinablePresenceParam;

#ifdef __cplusplus
extern "C" {
#endif

int sceAppUtilInit(SceAppUtilInitParam *initParam, SceAppUtilBootParam *bootParam);
int sceAppUtilShutdown(void);
int sceAppUtilReceiveAppEvent(SceAppUtilAppEventParam *param);

#ifdef __cplusplus
}
#endif
