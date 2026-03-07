#pragma once
/* kernel.h — PS Vita kernel API stubs for Emscripten/WASM builds */

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── Primitive SCE types ─────────────────────────────────────── */
typedef int32_t   SceInt32;
typedef uint32_t  SceUInt32;
typedef int16_t   SceInt16;
typedef uint16_t  SceUInt16;
typedef int8_t    SceInt8;
typedef uint8_t   SceUInt8;
typedef int64_t   SceInt64;
typedef uint64_t  SceUInt64;
typedef float     SceFloat;
typedef char      SceChar8;
typedef void *    SceUID;
typedef uint32_t  SceSize;
typedef intptr_t  SceSSize;

/* ── Return codes ───────────────────────────────────────────── */
#define SCE_OK       0
#define SCE_ERROR_ERRNO_ENOENT (-2147418092)

/* ── LW Mutex ──────────────────────────────────────────────── */
typedef struct { uint64_t data[4]; } SceKernelLwMutexWork;
typedef struct { int      dummy; } SceKernelLwMutexOptParam;

#define SCE_KERNEL_LW_MUTEX_ATTR_TH_PRIO   0x00000001
#define SCE_KERNEL_LW_MUTEX_ATTR_RECURSIVE  0x00000002

static inline int sceKernelCreateLwMutex(SceKernelLwMutexWork *pWork,
    const char *pName, unsigned int attr, int initCount,
    const SceKernelLwMutexOptParam *pOptParam)
{ (void)pWork; (void)pName; (void)attr; (void)initCount; (void)pOptParam; return SCE_OK; }

static inline int sceKernelDeleteLwMutex(SceKernelLwMutexWork *pWork)
{ (void)pWork; return SCE_OK; }

static inline int sceKernelLockLwMutex(SceKernelLwMutexWork *pWork,
    int lockCount, unsigned int *pTimeout)
{ (void)pWork; (void)lockCount; (void)pTimeout; return SCE_OK; }

static inline int sceKernelUnlockLwMutex(SceKernelLwMutexWork *pWork, int unlockCount)
{ (void)pWork; (void)unlockCount; return SCE_OK; }

static inline int sceKernelTryLockLwMutex(SceKernelLwMutexWork *pWork, int lockCount)
{ (void)pWork; (void)lockCount; return SCE_OK; }

/* ── RW Lock ───────────────────────────────────────────────── */
#define SCE_KERNEL_RWLOCK_ATTR_TH_PRIO 0x00000001

static inline int sceKernelCreateRWLock(SceUID *pUid, const char *pName,
    unsigned int attr, void *pOptParam)
{ (void)pName; (void)attr; (void)pOptParam; *pUid = (SceUID)(intptr_t)1; return SCE_OK; }

static inline int sceKernelDeleteRWLock(SceUID uid)
{ (void)uid; return SCE_OK; }

static inline int sceKernelLockReadRWLock(SceUID uid, unsigned int *pTimeout)
{ (void)uid; (void)pTimeout; return SCE_OK; }

static inline int sceKernelUnlockReadRWLock(SceUID uid)
{ (void)uid; return SCE_OK; }

static inline int sceKernelLockWriteRWLock(SceUID uid, unsigned int *pTimeout)
{ (void)uid; (void)pTimeout; return SCE_OK; }

static inline int sceKernelUnlockWriteRWLock(SceUID uid)
{ (void)uid; return SCE_OK; }

/* ── Thread ID / timing ────────────────────────────────────── */
static inline int sceKernelGetThreadId(void) { return 1; }
static inline int sceKernelDelayThread(unsigned int usec) { (void)usec; return SCE_OK; }
static inline uint64_t sceKernelGetProcessTimeWide(void) { return 0; }

/* ── Memory blocks ─────────────────────────────────────────── */
#define SCE_KERNEL_MEMBLOCK_TYPE_USER_RW           0x0C20D060
#define SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW     0x09408060
#define SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_PHYCONT_RW 0x0C80D060

static inline SceUID sceKernelAllocMemBlock(const char *name, unsigned int type,
    int size, void *pOptParam)
{ (void)name; (void)type; (void)pOptParam; return (SceUID)malloc((size_t)size); }

static inline int sceKernelFreeMemBlock(SceUID uid)
{ free(uid); return SCE_OK; }

static inline int sceKernelGetMemBlockBase(SceUID uid, void **ppBase)
{ *ppBase = uid; return SCE_OK; }

/* ── Free memory info ──────────────────────────────────────── */
typedef struct {
    unsigned int size;
    unsigned int freeSize;
    unsigned int maxFreeBlockSize;
} SceKernelFreeMemorySizeInfo;

static inline int sceKernelGetFreeMemorySize(SceKernelFreeMemorySizeInfo *pInfo)
{
    if (pInfo) {
        pInfo->size             = sizeof(SceKernelFreeMemorySizeInfo);
        pInfo->freeSize         = 256 * 1024 * 1024;
        pInfo->maxFreeBlockSize = 128 * 1024 * 1024;
    }
    return SCE_OK;
}

/* ── Event queue (equeue) stubs ───────────────────────────── */
typedef int SceKernelEqueue;
typedef struct { int filter; uintptr_t udata; } SceKernelEvent;

static inline int sceKernelCreateEqueue(SceKernelEqueue *eq, const char *name)
{ (void)name; *eq = 0; return SCE_OK; }

static inline int sceKernelDeleteEqueue(SceKernelEqueue eq)
{ (void)eq; return SCE_OK; }

static inline int sceKernelWaitEqueue(SceKernelEqueue eq,
    SceKernelEvent *evList, int numEv, int *outNum, unsigned int *pTimeout)
{
    (void)eq; (void)evList; (void)numEv; (void)pTimeout;
    if (outNum) *outNum = 0;
    return SCE_OK;
}

#ifdef __cplusplus
}
#endif
