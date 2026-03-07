#pragma once
/* xsocialpost.h — PS Vita social post stubs for Emscripten/WASM builds */

#include <stdint.h>
#include "kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

#define XSOCIAL_CAPABILITY_POSTIMAGE  0x0001
#define XSOCIAL_CAPABILITY_POSTLINK   0x0002

typedef struct {
    uint32_t dwFlags;
    char     caption[64];
    char     description[128];
    uint8_t *pImageBuffer;
    uint32_t imageSize;
} XSOCIAL_POST_IMAGE_REQUEST;

static inline int XSocialPostImage(
    uint32_t dwUserIndex,
    const XSOCIAL_POST_IMAGE_REQUEST *pRequest,
    void *pOverlapped)
{
    (void)dwUserIndex; (void)pRequest; (void)pOverlapped;
    return -1; /* not available on WASM */
}

static inline uint32_t XSocialGetCapability(uint32_t dwUserIndex)
{ (void)dwUserIndex; return 0; }

#ifdef __cplusplus
}
#endif
