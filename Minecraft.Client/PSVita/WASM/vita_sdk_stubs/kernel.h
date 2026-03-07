#pragma once
// Minimal PS Vita kernel.h stub for Emscripten/WASM builds.
// Provides only the types and constants actually used by this codebase.

#include <stdint.h>
#include <stddef.h>
#include <pthread.h>

typedef int32_t  SceInt8;   // note: real SDK uses int8_t but int8_t causes issues
typedef int8_t   SceInt8_;  // for completeness
typedef int16_t  SceInt16;
typedef int32_t  SceInt32;
typedef int64_t  SceInt64;
typedef uint8_t  SceUInt8;
typedef uint16_t SceUInt16;
typedef uint32_t SceUInt32;
typedef uint64_t SceUInt64;
typedef size_t   SceSize;
typedef ssize_t  SceSSize;
typedef int32_t  SceUID;
typedef uint32_t SceMode;
typedef int32_t  SceBool;
typedef void*    SceVoid;
typedef float    SceFloat;
typedef double   SceDouble;
typedef char     SceChar8;
typedef uint16_t SceWChar16;
typedef uint32_t SceWChar32;

// Lightweight mutex work area (opaque 64-byte region on real hardware;
// we back it with a pthreads mutex for WASM).
typedef struct SceKernelLwMutexWork {
    pthread_mutex_t _mutex;
    int             _init;
    char            _pad[24];
} SceKernelLwMutexWork;

// Read-write lock (backed by a pthreads rwlock).
typedef struct SceKernelRWLockWork {
    pthread_rwlock_t _rwlock;
    int              _init;
} SceKernelRWLockWork;

// Attribute flags
#define SCE_KERNEL_LW_MUTEX_ATTR_TH_PRIO    0x00000002
#define SCE_KERNEL_LW_MUTEX_ATTR_RECURSIVE  0x00000004
#define SCE_KERNEL_RW_LOCK_ATTR_TH_PRIO     0x00000002
#define SCE_KERNEL_RW_LOCK_ATTR_RECURSIVE   0x00000004

#ifndef SCE_OK
#define SCE_OK 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Lightweight mutex operations (WASM stubs use pthreads).
static inline int sceKernelCreateLwMutex(SceKernelLwMutexWork *pWork,
                                          const char *name,
                                          unsigned int attr,
                                          int initCount,
                                          const void *pOptParam)
{
    (void)name; (void)attr; (void)initCount; (void)pOptParam;
    pthread_mutexattr_t a;
    pthread_mutexattr_init(&a);
    pthread_mutexattr_settype(&a, PTHREAD_MUTEX_RECURSIVE);
    int r = pthread_mutex_init(&pWork->_mutex, &a);
    pthread_mutexattr_destroy(&a);
    pWork->_init = 1;
    return r == 0 ? SCE_OK : -1;
}

static inline int sceKernelDeleteLwMutex(SceKernelLwMutexWork *pWork)
{
    pWork->_init = 0;
    return pthread_mutex_destroy(&pWork->_mutex) == 0 ? SCE_OK : -1;
}

static inline int sceKernelLockLwMutex(SceKernelLwMutexWork *pWork,
                                        int lockCount,
                                        unsigned int *pTimeout)
{
    (void)lockCount; (void)pTimeout;
    return pthread_mutex_lock(&pWork->_mutex) == 0 ? SCE_OK : -1;
}

static inline int sceKernelUnlockLwMutex(SceKernelLwMutexWork *pWork,
                                          int unlockCount)
{
    (void)unlockCount;
    return pthread_mutex_unlock(&pWork->_mutex) == 0 ? SCE_OK : -1;
}

static inline int sceKernelTryLockLwMutex(SceKernelLwMutexWork *pWork,
                                            int lockCount)
{
    (void)lockCount;
    return pthread_mutex_trylock(&pWork->_mutex) == 0 ? SCE_OK : -1;
}

// Read-write lock operations.
static inline SceUID sceKernelCreateRWLock(const char *name,
                                            unsigned int attr,
                                            const void *pOptParam)
{
    (void)name; (void)attr; (void)pOptParam;
    SceKernelRWLockWork *pWork =
        (SceKernelRWLockWork *)malloc(sizeof(SceKernelRWLockWork));
    if (!pWork) return -1;
    pthread_rwlock_init(&pWork->_rwlock, NULL);
    pWork->_init = 1;
    return (SceUID)(intptr_t)pWork;
}

static inline int sceKernelDeleteRWLock(SceUID uid)
{
    SceKernelRWLockWork *pWork = (SceKernelRWLockWork *)(intptr_t)uid;
    if (!pWork) return -1;
    pthread_rwlock_destroy(&pWork->_rwlock);
    free(pWork);
    return SCE_OK;
}

static inline int sceKernelLockReadRWLock(SceUID uid, int timeout)
{
    (void)timeout;
    SceKernelRWLockWork *pWork = (SceKernelRWLockWork *)(intptr_t)uid;
    return pWork ? (pthread_rwlock_rdlock(&pWork->_rwlock) == 0 ? SCE_OK : -1) : -1;
}

static inline int sceKernelUnlockReadRWLock(SceUID uid)
{
    SceKernelRWLockWork *pWork = (SceKernelRWLockWork *)(intptr_t)uid;
    return pWork ? (pthread_rwlock_unlock(&pWork->_rwlock) == 0 ? SCE_OK : -1) : -1;
}

static inline int sceKernelLockWriteRWLock(SceUID uid, int timeout)
{
    (void)timeout;
    SceKernelRWLockWork *pWork = (SceKernelRWLockWork *)(intptr_t)uid;
    return pWork ? (pthread_rwlock_wrlock(&pWork->_rwlock) == 0 ? SCE_OK : -1) : -1;
}

static inline int sceKernelUnlockWriteRWLock(SceUID uid)
{
    SceKernelRWLockWork *pWork = (SceKernelRWLockWork *)(intptr_t)uid;
    return pWork ? (pthread_rwlock_unlock(&pWork->_rwlock) == 0 ? SCE_OK : -1) : -1;
}

// Process time (microseconds since process start).
static inline SceUInt64 sceKernelGetProcessTimeWide(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (SceUInt64)ts.tv_sec * 1000000ULL + (SceUInt64)ts.tv_nsec / 1000ULL;
}

// Thread ID.
static inline SceUID sceKernelGetThreadId(void)
{
    return (SceUID)(intptr_t)pthread_self();
}

#ifdef __cplusplus
}
#endif
