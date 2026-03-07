#pragma once
#include <stdint.h>

/* NP DRM stubs */
typedef struct SceNpDrmLicense { uint8_t data[512]; } SceNpDrmLicense;

#ifdef __cplusplus
extern "C" {
#endif

int sceNpDrmIsAvailable(const SceNpDrmLicense *license, const char *path);
int sceNpDrmGetContentKey(const char *path, void *key, int keySize);

#ifdef __cplusplus
}
#endif
