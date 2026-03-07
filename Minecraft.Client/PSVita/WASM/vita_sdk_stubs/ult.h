#pragma once
/* ult.h — PS Vita Ultra Low-latency Threading stubs for Emscripten/WASM builds */

#include <stdint.h>
#include "kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct { uint8_t data[256]; } SceUltUlthreadRuntime;
typedef struct { uint8_t data[256]; } SceUltUlthread;
typedef struct { uint8_t data[64];  } SceUltMutex;
typedef struct { uint8_t data[64];  } SceUltConditionVariable;
typedef struct { uint8_t data[32];  } SceUltReaderWriterLock;
typedef struct { uint8_t data[32];  } SceUltSemaphore;

typedef struct {
    uint32_t oneShotThreadStackSize;
    uint32_t workerThreadPriority;
    uint32_t workerThreadCpuAffinityMask;
    uint32_t workerThreadAttr;
    const char *workerThreadOptionalName;
} SceUltUlthreadRuntimeOptParam;

#define SCE_ULT_ULTHREAD_RUNTIME_OPT_PARAM_INITIALIZER { 4096, 0, 0, 0, NULL }

static inline int sceUltUlthreadRuntimeCreate(SceUltUlthreadRuntime *pRuntime,
    const char *pName, uint32_t maxNumUlthread, uint32_t numWorkerThread,
    void *pWorkerThreadStack, size_t workerThreadStackSize,
    const SceUltUlthreadRuntimeOptParam *pOptParam)
{
    (void)pRuntime; (void)pName; (void)maxNumUlthread; (void)numWorkerThread;
    (void)pWorkerThreadStack; (void)workerThreadStackSize; (void)pOptParam;
    return SCE_OK;
}

static inline int sceUltUlthreadRuntimeDestroy(SceUltUlthreadRuntime *pRuntime)
{ (void)pRuntime; return SCE_OK; }

static inline int sceUltUlthreadCreate(SceUltUlthread *pUlthread,
    const char *pName, void *(*entry)(void *arg), void *pArg,
    void *pStack, size_t stackSize,
    SceUltUlthreadRuntime *pRuntime, void *pOptParam)
{
    (void)pUlthread; (void)pName; (void)entry; (void)pArg;
    (void)pStack; (void)stackSize; (void)pRuntime; (void)pOptParam;
    return SCE_OK;
}

static inline int sceUltUlthreadJoin(SceUltUlthread *pUlthread, int *pExitStatus)
{ (void)pUlthread; if (pExitStatus) *pExitStatus = 0; return SCE_OK; }

static inline int sceUltUlthreadYield(void) { return SCE_OK; }

static inline int sceUltMutexCreate(SceUltMutex *pMutex,
    const char *pName, uint32_t attr, void *pOptParam)
{ (void)pMutex; (void)pName; (void)attr; (void)pOptParam; return SCE_OK; }

static inline int sceUltMutexDestroy(SceUltMutex *pMutex)
{ (void)pMutex; return SCE_OK; }

static inline int sceUltMutexLock(SceUltMutex *pMutex)
{ (void)pMutex; return SCE_OK; }

static inline int sceUltMutexUnlock(SceUltMutex *pMutex)
{ (void)pMutex; return SCE_OK; }

#ifdef __cplusplus
}
#endif
