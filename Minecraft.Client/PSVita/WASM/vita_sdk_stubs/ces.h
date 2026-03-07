#pragma once
/* ces.h — PS Vita CES (character encoding) stubs for Emscripten/WASM builds */

#include <stdint.h>
#include <string.h>
#include "kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct { uint8_t data[128]; } SceCesUcsContext;

#define SCE_CES_UCS_ENCODING_UTF8   0x0001
#define SCE_CES_UCS_ENCODING_UTF16  0x0004

static inline int sceCesUcsContextInit(SceCesUcsContext *ctx,
    unsigned int srcEncoding, unsigned int dstEncoding)
{
    (void)ctx; (void)srcEncoding; (void)dstEncoding;
    return SCE_OK;
}

static inline int sceCesUcsContextTerm(SceCesUcsContext *ctx)
{ (void)ctx; return SCE_OK; }

/* Return byte length of the UTF-8 string that would result from converting
 * a UTF-16 string of 'len' characters.  Conservative estimate: 3 bytes/char. */
static inline int sceCesUtf16StrGetUtf8Len(const uint16_t *src, size_t srcLen)
{ (void)src; return (int)(srcLen * 3 + 1); }

/* Simple UTF-16 to UTF-8 conversion — ASCII-only approximation for stubs */
static inline int sceCesUtf16StrToUtf8Str(SceCesUcsContext *ctx,
    const uint16_t *src, size_t *srcLen,
    uint8_t *dst, size_t *dstLen)
{
    (void)ctx;
    if (!src || !dst || !srcLen || !dstLen) return -1;
    size_t i;
    size_t out = 0;
    for (i = 0; i < *srcLen && src[i] != 0; i++) {
        uint16_t c = src[i];
        if (c < 0x80) {
            if (out + 1 >= *dstLen) break;
            dst[out++] = (uint8_t)c;
        } else if (c < 0x800) {
            if (out + 2 >= *dstLen) break;
            dst[out++] = (uint8_t)(0xC0 | (c >> 6));
            dst[out++] = (uint8_t)(0x80 | (c & 0x3F));
        } else {
            if (out + 3 >= *dstLen) break;
            dst[out++] = (uint8_t)(0xE0 | (c >> 12));
            dst[out++] = (uint8_t)(0x80 | ((c >> 6) & 0x3F));
            dst[out++] = (uint8_t)(0x80 | (c & 0x3F));
        }
    }
    if (out < *dstLen) dst[out] = '\0';
    *srcLen = i;
    *dstLen = out;
    return SCE_OK;
}

#ifdef __cplusplus
}
#endif
