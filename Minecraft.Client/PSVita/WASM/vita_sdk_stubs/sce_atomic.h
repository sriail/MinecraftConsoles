#pragma once
/* sce_atomic.h — PS Vita atomic operation stubs for Emscripten/WASM builds */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 32-bit compare-and-swap */
static inline int32_t sceAtomicCompareAndSwap32(volatile int32_t *dest,
    int32_t expected, int32_t replacement)
{
    /* GCC/Clang built-in */
    return __sync_val_compare_and_swap(dest, expected, replacement);
}

/* 64-bit compare-and-swap */
static inline int64_t sceAtomicCompareAndSwap64(volatile int64_t *dest,
    int64_t expected, int64_t replacement)
{
    return __sync_val_compare_and_swap(dest, expected, replacement);
}

/* Add and fetch */
static inline int32_t sceAtomicAdd32(volatile int32_t *dest, int32_t val)
{ return __sync_add_and_fetch(dest, val); }

static inline int64_t sceAtomicAdd64(volatile int64_t *dest, int64_t val)
{ return __sync_add_and_fetch(dest, val); }

/* OR, AND */
static inline int32_t sceAtomicOr32(volatile int32_t *dest, int32_t val)
{ return __sync_or_and_fetch(dest, val); }

static inline int32_t sceAtomicAnd32(volatile int32_t *dest, int32_t val)
{ return __sync_and_and_fetch(dest, val); }

/* Exchange */
static inline int32_t sceAtomicExchange32(volatile int32_t *dest, int32_t val)
{ return __sync_lock_test_and_set(dest, val); }

#ifdef __cplusplus
}
#endif
