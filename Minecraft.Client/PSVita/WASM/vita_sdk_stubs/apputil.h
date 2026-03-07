#pragma once
#include <stdint.h>

#define SCE_APPUTIL_SAVEDATA_SLOT_TITLE_MAXSIZE    64
#define SCE_APPUTIL_SAVEDATA_SLOT_SUBTITLE_MAXSIZE 128

typedef struct SceAppUtilInitParam { uint32_t workBufSize; } SceAppUtilInitParam;
typedef struct SceAppUtilBootParam { uint32_t attr; } SceAppUtilBootParam;
typedef struct SceAppUtilAppEventParam { uint32_t type; uint8_t dat[1024]; } SceAppUtilAppEventParam;
typedef struct SceAppUtilBgdlStatus { int type; } SceAppUtilBgdlStatus;

typedef struct SceAppUtilSaveDataSlotParam {
    uint32_t status;
    char     title[SCE_APPUTIL_SAVEDATA_SLOT_TITLE_MAXSIZE];
    char     subTitle[SCE_APPUTIL_SAVEDATA_SLOT_SUBTITLE_MAXSIZE];
    char     detail[512];
    char     iconPath[1024];
    uint32_t sizeKB;
} SceAppUtilSaveDataSlotParam;

typedef struct SceAppUtilSaveDataSlot { uint32_t id; int32_t status; } SceAppUtilSaveDataSlot;
typedef struct SceAppUtilSaveDataDataSlot { uint32_t id; } SceAppUtilSaveDataDataSlot;

typedef struct SceAppUtilNpBasicJoinablePresenceParam {
    uint32_t size;
} SceAppUtilNpBasicJoinablePresenceParam;

typedef struct SceAppMgrAppState { int systemEvent; int isSystemUiOverlaid; } SceAppMgrAppState;
typedef struct SceAppMgrSystemEvent { int systemEvent; } SceAppMgrSystemEvent;
