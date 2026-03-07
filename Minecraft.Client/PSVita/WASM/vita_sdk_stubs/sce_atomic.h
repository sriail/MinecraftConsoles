#pragma once
// Minimal PS Vita sce_atomic.h stub for Emscripten/WASM builds.

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline int32_t sceAtomicCompareAndSwap32(int32_t *ptr,
                                                  int32_t cmpVal,
                                                  int32_t exchVal)
{
    return __sync_val_compare_and_swap(ptr, cmpVal, exchVal);
}

static inline int64_t sceAtomicCompareAndSwap64(int64_t *ptr,
                                                  int64_t cmpVal,
                                                  int64_t exchVal)
{
    return __sync_val_compare_and_swap(ptr, cmpVal, exchVal);
}

static inline int32_t sceAtomicAdd32(int32_t *ptr, int32_t val)
{
    return __sync_fetch_and_add(ptr, val);
}

static inline int64_t sceAtomicAdd64(int64_t *ptr, int64_t val)
{
    return __sync_fetch_and_add(ptr, val);
}

#ifdef __cplusplus
}
#endif
