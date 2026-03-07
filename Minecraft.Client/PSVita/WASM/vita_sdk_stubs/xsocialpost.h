#pragma once
// Minimal PS Vita xsocialpost.h stub for Emscripten/WASM builds.
#include <stdint.h>
typedef struct SceXSocialPostInitParam { uint32_t size; } SceXSocialPostInitParam;
#ifdef __cplusplus
extern "C" {
#endif
int sceXSocialPostInitialize(const SceXSocialPostInitParam *params);
int sceXSocialPostFinalize(void);
int sceXSocialPostOpenAddDialog(const void *params);
int sceXSocialPostGetStatus(void);
#ifdef __cplusplus
}
#endif
