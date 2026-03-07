#pragma once
/* apputil.h — PS Vita App Utility API stubs for Emscripten/WASM builds */

#include <stdint.h>
#include <string.h>
#include "kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ── System params ──────────────────────────────────────────── */
#define SCE_SYSTEM_PARAM_ID_LANG            1
#define SCE_SYSTEM_PARAM_ID_ENTER_BUTTON    2
#define SCE_SYSTEM_PARAM_ID_USERNAME        3

#define SCE_SYSTEM_PARAM_LANG_JAPANESE      0
#define SCE_SYSTEM_PARAM_LANG_ENGLISH_US    1
#define SCE_SYSTEM_PARAM_LANG_FRENCH        2
#define SCE_SYSTEM_PARAM_LANG_SPANISH       3
#define SCE_SYSTEM_PARAM_LANG_GERMAN        4
#define SCE_SYSTEM_PARAM_LANG_ITALIAN       5
#define SCE_SYSTEM_PARAM_LANG_DUTCH         6
#define SCE_SYSTEM_PARAM_LANG_PORTUGUESE_PT 7
#define SCE_SYSTEM_PARAM_LANG_RUSSIAN       8
#define SCE_SYSTEM_PARAM_LANG_KOREAN        9
#define SCE_SYSTEM_PARAM_LANG_CHINESE_T     10
#define SCE_SYSTEM_PARAM_LANG_CHINESE_S     11
#define SCE_SYSTEM_PARAM_LANG_FINNISH       12
#define SCE_SYSTEM_PARAM_LANG_SWEDISH       13
#define SCE_SYSTEM_PARAM_LANG_DANISH        14
#define SCE_SYSTEM_PARAM_LANG_NORWEGIAN     15
#define SCE_SYSTEM_PARAM_LANG_POLISH        16
#define SCE_SYSTEM_PARAM_LANG_PORTUGUESE_BR 17
#define SCE_SYSTEM_PARAM_LANG_ENGLISH_GB    18
#define SCE_SYSTEM_PARAM_LANG_TURKISH       19

/* ── Init/Boot params ───────────────────────────────────────── */
typedef struct { uint32_t reserved[8]; } SceAppUtilInitParam;
typedef struct { uint32_t reserved[8]; } SceAppUtilBootParam;

static inline int sceAppUtilInit(SceAppUtilInitParam *pInit, SceAppUtilBootParam *pBoot)
{ (void)pInit; (void)pBoot; return SCE_OK; }

static inline int sceAppUtilShutdown(void) { return SCE_OK; }

static inline int sceAppUtilSystemParamGetInt(int paramId, SceInt32 *pValue)
{
    (void)paramId;
    if (pValue) *pValue = SCE_SYSTEM_PARAM_LANG_ENGLISH_US;
    return SCE_OK;
}

/* ── App event ──────────────────────────────────────────────── */
#define SCE_APP_EVENT_TYPE_INVALID              0
#define SCE_APP_EVENT_TYPE_RESUME_TITLE         1
#define SCE_APP_EVENT_TYPE_NP_BASIC_JOINABLE    5

typedef struct {
    int  type;
    char dat[1024];
} SceAppUtilAppEventParam;

static inline int sceAppUtilReceiveAppEvent(SceAppUtilAppEventParam *pParam)
{
    if (pParam) { memset(pParam, 0, sizeof(*pParam)); pParam->type = SCE_APP_EVENT_TYPE_INVALID; }
    return SCE_OK;
}

/* ── NP joinable presence ───────────────────────────────────── */
typedef struct {
    char  data[1024];
    int   dataSize;
    int   npSessionIdLen;
    char  npSessionId[64];
} SceAppUtilNpBasicJoinablePresenceParam;

static inline int sceAppUtilAppEventParseNpBasicJoinablePresence(
    const SceAppUtilAppEventParam *pEvent,
    SceAppUtilNpBasicJoinablePresenceParam *pParam)
{
    (void)pEvent;
    if (pParam) memset(pParam, 0, sizeof(*pParam));
    return -1; /* not available on WASM */
}

/* ── Save data slot ─────────────────────────────────────────── */
#define SCE_APPUTIL_SAVEDATA_SLOT_MAX_SLOTS 32

typedef struct {
    int  id;
    int  status;
} SceAppUtilSaveDataSlot;

typedef struct {
    char title[128];
    char subTitle[128];
    char detail[512];
    char iconPath[64];
    int  modifiedTime;
} SceAppUtilSaveDataSlotParam;

static inline int sceAppUtilSaveDataSlotGetParam(unsigned int slotId,
    SceAppUtilSaveDataSlotParam *pParam, void *pOpt)
{
    (void)slotId; (void)pOpt;
    if (pParam) memset(pParam, 0, sizeof(*pParam));
    return -1;
}

/* ── App manager ────────────────────────────────────────────── */
typedef struct {
    int  isSystemUiOverlaid;
    int  isScreensaverOn;
} SceAppMgrAppState;

#define SCE_APPMGR_SYSTEM_EVENT_ON_RESUME   0
#define SCE_APPMGR_SYSTEM_EVENT_ON_SUSPEND  1
#define SCE_APPMGR_SYSTEM_EVENT_ON_QUIT     2

typedef struct {
    int  systemEvent;
} SceAppMgrSystemEvent;

static inline int sceAppMgrGetAppState(SceAppMgrAppState *pState)
{
    if (pState) { pState->isSystemUiOverlaid = 0; pState->isScreensaverOn = 0; }
    return SCE_OK;
}

static inline int sceAppMgrReceiveSystemEvent(SceAppMgrSystemEvent *pEvent)
{
    (void)pEvent;
    return -1; /* no pending events */
}

/* ── Background download ─────────────────────────────────────── */
typedef struct { int status; } SceAppUtilBgdlStatus;

static inline int sceAppUtilBgdlGetStatus(SceAppUtilBgdlStatus *pStatus)
{
    if (pStatus) pStatus->status = 0;
    return SCE_OK;
}

#ifdef __cplusplus
}
#endif
