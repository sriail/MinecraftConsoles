#pragma once
// Minimal PS Vita netcheck_dialog.h stub for Emscripten/WASM builds.
#include <stdint.h>
typedef enum SceNetcheckDialogMode {
    SCE_NETCHECK_DIALOG_MODE_ERRORCODE = 1,
    SCE_NETCHECK_DIALOG_MODE_NETWORK_CHECK = 2
} SceNetcheckDialogMode;
typedef struct SceNetcheckDialogParam {
    uint32_t size;
    SceNetcheckDialogMode mode;
    int errorCode;
    uint8_t reserved[28];
} SceNetcheckDialogParam;
#ifdef __cplusplus
extern "C" {
#endif
void sceNetcheckDialogParamInit(SceNetcheckDialogParam *param);
int sceNetcheckDialogOpen(const SceNetcheckDialogParam *param);
int sceNetcheckDialogClose(void);
int sceNetcheckDialogGetStatus(void);
int sceNetcheckDialogGetResult(void *result);
int sceNetcheckDialogTerm(void);
#ifdef __cplusplus
}
#endif
