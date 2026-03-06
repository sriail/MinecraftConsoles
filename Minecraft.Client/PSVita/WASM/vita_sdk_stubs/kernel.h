#pragma once
#include <stdint.h>

typedef int32_t SceUID;
typedef int32_t SceInt32;
typedef int16_t SceInt16;
typedef uint32_t SceUInt32;
typedef uint16_t SceUInt16;
typedef uint8_t SceUInt8;
typedef unsigned int SceSize;

typedef struct SceKernelLwMutexWork { int32_t data[4]; } SceKernelLwMutexWork;
typedef struct SceKernelLwMutexOptParam { SceSize size; } SceKernelLwMutexOptParam;

#define SCE_KERNEL_LW_MUTEX_ATTR_RECURSIVE 0x02

/* _SCE_BREAK / SCE_BREAK - on the real Vita SDK these halt the debugger.
   In the WASM build they are no-ops (no hardware debugger). */
#ifndef _SCE_BREAK
#define _SCE_BREAK() ((void)0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

int sceKernelCreateLwMutex(SceKernelLwMutexWork *pWork, const char *pName, uint32_t attr, int32_t initCount, const SceKernelLwMutexOptParam *pOptParam);
int sceKernelDeleteLwMutex(SceKernelLwMutexWork *pWork);
int sceKernelLockLwMutex(SceKernelLwMutexWork *pWork, int32_t lockCount, uint32_t *pTimeout);
int sceKernelUnlockLwMutex(SceKernelLwMutexWork *pWork, int32_t unlockCount);
int sceKernelCreateRWLock(const char *pName, uint32_t attr, const void *pOptParam);
int sceKernelDeleteRWLock(SceUID rwLockId);
int sceKernelLockReadRWLock(SceUID rwLockId, uint32_t *pTimeout);
int sceKernelUnlockReadRWLock(SceUID rwLockId);
int sceKernelLockWriteRWLock(SceUID rwLockId, uint32_t *pTimeout);
int sceKernelUnlockWriteRWLock(SceUID rwLockId);

#ifdef __cplusplus
}
#endif
