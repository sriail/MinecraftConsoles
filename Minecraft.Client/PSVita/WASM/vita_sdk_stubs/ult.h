#pragma once
// Minimal PS Vita ult.h stub for Emscripten/WASM builds.
// ULT = User-Level Threads
#include <stdint.h>
typedef struct SceUltWaitingQueueOptParam { uint32_t attr; } SceUltWaitingQueueOptParam;
typedef struct SceUltQueueOptParam { uint32_t attr; } SceUltQueueOptParam;
#ifdef __cplusplus
extern "C" {
#endif
int sceUltUlthreadRuntimeCreate(void *pRuntime, const char *name, uint32_t numMaxUlthread, uint32_t numWorkerThread, void *pWorkArea, size_t workAreaSize, const void *pOptParam);
int sceUltUlthreadRuntimeDestroy(void *pRuntime);
int sceUltUlthreadCreate(void *pUlthread, const char *name, void *entry, uint64_t arg, void *pContext, size_t contextSize, int priority, const void *pOptParam);
int sceUltUlthreadJoin(void *pUlthread, int *exitStatus);
int sceUltUlthreadTryJoin(void *pUlthread, int *exitStatus);
int sceUltUlthreadExit(int exitStatus);
int sceUltMutexCreate(void *pMutex, const char *name, const SceUltWaitingQueueOptParam *pQueueOptParam);
int sceUltMutexDestroy(void *pMutex);
int sceUltMutexLock(void *pMutex);
int sceUltMutexUnlock(void *pMutex);
uint32_t sceUltGetRuntimeWorkAreaSize(uint32_t numMaxUlthread, uint32_t numWorkerThread);
#ifdef __cplusplus
}
#endif
