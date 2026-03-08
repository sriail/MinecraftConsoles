#pragma once

#include <stddef.h>

typedef int SceUID;
typedef unsigned int SceSize;
typedef int SceInt32;
typedef unsigned int SceUInt32;
typedef long long SceInt64;
typedef unsigned long long SceUInt64;
typedef short SceInt16;

typedef struct SceKernelLwMutexWork {
    unsigned char data[32];
} SceKernelLwMutexWork;

#define SCE_KERNEL_RW_LOCK_ATTR_TH_PRIO 0x00000000
#define SCE_KERNEL_RW_LOCK_ATTR_RECURSIVE 0x00000002

inline int sceKernelCreateLwMutex(SceKernelLwMutexWork *work, const char *name, unsigned int attr, int initCount, void *optParam) {
    (void)work; (void)name; (void)attr; (void)initCount; (void)optParam;
    return 0;
}

inline int sceKernelLockLwMutex(SceKernelLwMutexWork *work, int lockCount, void *timeout) {
    (void)work; (void)lockCount; (void)timeout;
    return 0;
}

inline int sceKernelUnlockLwMutex(SceKernelLwMutexWork *work, int unlockCount) {
    (void)work; (void)unlockCount;
    return 0;
}

inline int sceKernelTryLockLwMutex(SceKernelLwMutexWork *work, int lockCount) {
    (void)work; (void)lockCount;
    return 0;
}

inline int sceKernelDeleteLwMutex(SceKernelLwMutexWork *work) {
    (void)work;
    return 0;
}

inline SceUID sceKernelCreateRWLock(const char *name, unsigned int attr, void *optParam) {
    (void)name; (void)attr; (void)optParam;
    return 0;
}

inline int sceKernelLockReadRWLock(SceUID rwlockId, unsigned int timeout) {
    (void)rwlockId; (void)timeout;
    return 0;
}

inline int sceKernelLockWriteRWLock(SceUID rwlockId, unsigned int timeout) {
    (void)rwlockId; (void)timeout;
    return 0;
}

inline int sceKernelUnlockReadRWLock(SceUID rwlockId) {
    (void)rwlockId;
    return 0;
}

inline int sceKernelUnlockWriteRWLock(SceUID rwlockId) {
    (void)rwlockId;
    return 0;
}

inline int sceKernelDeleteRWLock(SceUID rwlockId) {
    (void)rwlockId;
    return 0;
}

inline SceUInt64 sceKernelGetProcessTimeWide(void) {
    return 0;
}

inline SceUID sceKernelGetThreadId(void) {
    return 0;
}
