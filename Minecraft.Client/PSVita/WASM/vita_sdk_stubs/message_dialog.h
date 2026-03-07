#pragma once
/* message_dialog.h — PS Vita message dialog stubs for Emscripten/WASM builds */

#include <stdint.h>
#include "kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Common dialog status / result */
#define SCE_COMMON_DIALOG_STATUS_NONE       0
#define SCE_COMMON_DIALOG_STATUS_RUNNING    1
#define SCE_COMMON_DIALOG_STATUS_FINISHED   2

#define SCE_COMMON_DIALOG_RESULT_OK              0
#define SCE_COMMON_DIALOG_RESULT_USER_CANCELED   1
#define SCE_COMMON_DIALOG_RESULT_ABORTED         2

#define SCE_NP_MESSAGE_DIALOG_MAX_INDEX_ICON_SIZE  (64*1024)

/* ── Save data dialog ──────────────────────────────────────── */
typedef struct {
    uint32_t size;
    uint32_t mode;
    uint32_t slotId;
    uint8_t  pad[4];
} SceSaveDataDialogParam;

typedef struct {
    uint32_t mode;
    uint32_t msgType;
} SceSaveDataDialogSystemMessageParam;

typedef struct {
    uint32_t size;
    uint8_t  pad[4];
} SceSaveDataDialogFinishParam;

typedef struct {
    uint32_t  result;
    int       slotId;
    uint8_t   pad[8];
} SceSaveDataDialogResult;

typedef struct {
    uint32_t  slotId;
    uint32_t  slotCount;
} SceSaveDataDialogListParam;

static inline int sceSaveDataDialogInit(const SceSaveDataDialogParam *param)
{ (void)param; return SCE_OK; }

static inline int sceSaveDataDialogTerm(void) { return SCE_OK; }

static inline int sceSaveDataDialogOpen(const SceSaveDataDialogParam *param)
{ (void)param; return SCE_OK; }

static inline int sceSaveDataDialogUpdate(void) { return SCE_OK; }

static inline int sceSaveDataDialogGetStatus(void)
{ return SCE_COMMON_DIALOG_STATUS_NONE; }

static inline int sceSaveDataDialogGetSubStatus(void)
{ return SCE_COMMON_DIALOG_STATUS_NONE; }

static inline int sceSaveDataDialogGetResult(SceSaveDataDialogResult *pResult)
{
    if (pResult) {
        pResult->result = SCE_COMMON_DIALOG_RESULT_USER_CANCELED;
        pResult->slotId = 0;
    }
    return SCE_OK;
}

static inline int sceSaveDataDialogClose(const SceSaveDataDialogFinishParam *param)
{ (void)param; return SCE_OK; }

/* ── Game custom data dialog ────────────────────────────────── */
typedef struct { uint32_t reserved[8]; } SceGameCustomDataDialogParam;
typedef struct { uint32_t result; } SceGameCustomDataDialogResult;

static inline int sceGameCustomDataDialogInit(const void *param)
{ (void)param; return SCE_OK; }

static inline int sceGameCustomDataDialogTerm(void) { return SCE_OK; }

static inline int sceGameCustomDataDialogOpen(const SceGameCustomDataDialogParam *param)
{ (void)param; return SCE_OK; }

static inline int sceGameCustomDataDialogUpdate(void) { return SCE_OK; }

static inline int sceGameCustomDataDialogGetStatus(void)
{ return SCE_COMMON_DIALOG_STATUS_NONE; }

static inline int sceGameCustomDataDialogGetResult(SceGameCustomDataDialogResult *pResult)
{
    if (pResult) pResult->result = SCE_COMMON_DIALOG_RESULT_USER_CANCELED;
    return SCE_OK;
}

static inline int sceGameCustomDataDialogClose(void) { return SCE_OK; }

#ifdef __cplusplus
}
#endif
