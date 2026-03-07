#pragma once
/*
 * Minimal PS Vita kernel SDK stubs for Emscripten/WASM builds.
 * Provides kernel types used by PSVitaStubs.h (CRITICAL_SECTION, etc.)
 */

#include <stdint.h>

typedef int8_t   SceInt8;
typedef int16_t  SceInt16;
typedef int32_t  SceInt32;
typedef int64_t  SceInt64;
typedef uint8_t  SceUInt8;
typedef uint16_t SceUInt16;
typedef uint32_t SceUInt32;
typedef uint64_t SceUInt64;
typedef float    SceFloat;
typedef double   SceDouble;
typedef char     SceChar8;
typedef int      SceBool;

typedef int      SceUID;

/* Lightweight mutex work area used by PSVitaCriticalSection */
typedef struct SceKernelLwMutexWork {
    uint8_t opaque[32];
} SceKernelLwMutexWork;

/* Debug break — becomes a trap on Emscripten/WASM */
#ifndef _SCE_BREAK
#  define _SCE_BREAK() __builtin_trap()
#endif
