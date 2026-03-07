#pragma once
/* netcheck_dialog.h — PS Vita network check dialog stubs for Emscripten/WASM builds */

#include <stdint.h>
#include "message_dialog.h"
#include "net.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int      minAge;
    uint32_t countryCode;
} SceNetCheckDialogAgeRestriction;

typedef struct {
    uint32_t size;
    uint32_t mode;
    SceNetCheckDialogAgeRestriction *ageRestriction;
    uint32_t ageRestrictionNum;
} SceNetCheckDialogParam;

typedef struct {
    uint32_t result;
} SceNetCheckDialogResult;

static inline int sceNetCheckDialogInit(const SceNetCheckDialogParam *param)
{ (void)param; return SCE_OK; }

static inline int sceNetCheckDialogTerm(void) { return SCE_OK; }

static inline int sceNetCheckDialogUpdate(void) { return SCE_OK; }

static inline int sceNetCheckDialogGetStatus(void)
{ return SCE_COMMON_DIALOG_STATUS_NONE; }

static inline int sceNetCheckDialogGetResult(SceNetCheckDialogResult *pResult)
{
    if (pResult) pResult->result = SCE_COMMON_DIALOG_RESULT_OK;
    return SCE_OK;
}

static inline int sceNetCheckDialogAbort(void) { return SCE_OK; }
static inline int sceNetCheckDialogClose(void)  { return SCE_OK; }

#ifdef __cplusplus
}
#endif
