#pragma once
// Minimal PS Vita ces.h stub for Emscripten/WASM builds.
// CES = Character Encoding System (text conversion).
#include <stdint.h>
typedef int SceCesConversionAttributes;
typedef struct SceCes {
    void *data;
} SceCes;
#ifdef __cplusplus
extern "C" {
#endif
int sceCesConvertCharset(int srcCharset, int dstCharset,
                          const void *src, uint32_t srcLen,
                          void *dst, uint32_t *dstLen, uint32_t *srcConv);
#ifdef __cplusplus
}
#endif
