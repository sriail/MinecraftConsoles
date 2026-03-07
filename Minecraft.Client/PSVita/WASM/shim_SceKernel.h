#ifndef SHIM_SCEKERNEL_H
#define SHIM_SCEKERNEL_H

/*
 * shim_SceKernel.h
 * PS Vita SceKernel → Emscripten pthreads shim
 *
 * Covers: thread creation/management, lightweight mutexes, semaphores,
 * condition variables, event flags, and sleep/delay primitives.
 *
 * ABI reference: vitasdk <kernel.h>, <kernel/threadmgr.h>
 */

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- Primitive types (match vitasdk ABI) ---------- */
typedef int32_t  SceUID;
typedef int32_t  SceInt32;
typedef uint32_t SceUInt32;
typedef int32_t  SceKernelUseconds;
typedef int32_t  SceSize;

/* ---------- Return codes ---------- */
#define SCE_OK                         0
#define SCE_KERNEL_ERROR_NO_MEMORY     ((int)0x80020001)
#define SCE_KERNEL_ERROR_INVALID_ARG   ((int)0x80020003)
#define SCE_KERNEL_ERROR_TIMEOUT       ((int)0x80020011)
#define SCE_KERNEL_ERROR_WAIT_TIMEOUT  ((int)0x80020B0D)

/* ---------- Thread attributes / priorities ---------- */
#define SCE_KERNEL_THREAD_ATTR_USER         0x00080000
#define SCE_KERNEL_THREAD_ATTR_VFPU         0x00004000
#define SCE_KERNEL_THREAD_CPU_AFFINITY_MASK_DEFAULT 0

#define SCE_KERNEL_PRIO_FIFO_DEFAULT        160
#define SCE_KERNEL_PRIO_FIFO_HIGHEST         64
#define SCE_KERNEL_PRIO_FIFO_LOWEST         191

/* ---------- Lightweight mutex ---------- */
typedef struct SceKernelLwMutexWork {
    int32_t data[4];
} SceKernelLwMutexWork;

typedef struct SceKernelLwMutexOptParam {
    SceSize size;
} SceKernelLwMutexOptParam;

#define SCE_KERNEL_LW_MUTEX_ATTR_RECURSIVE  0x02

int sceKernelCreateLwMutex(SceKernelLwMutexWork *pWork, const char *pName,
                            uint32_t attr, int32_t initCount,
                            const SceKernelLwMutexOptParam *pOptParam);
int sceKernelDeleteLwMutex(SceKernelLwMutexWork *pWork);
int sceKernelLockLwMutex(SceKernelLwMutexWork *pWork, int32_t lockCount,
                          uint32_t *pTimeout);
int sceKernelUnlockLwMutex(SceKernelLwMutexWork *pWork, int32_t unlockCount);
int sceKernelTryLockLwMutex(SceKernelLwMutexWork *pWork, int32_t lockCount);

/* ---------- Mutex ---------- */
typedef struct SceKernelMutexOptParam {
    SceSize size;
} SceKernelMutexOptParam;

#define SCE_KERNEL_MUTEX_ATTR_RECURSIVE 0x02

SceUID sceKernelCreateMutex(const char *pName, uint32_t attr,
                             int32_t initCount,
                             const SceKernelMutexOptParam *pOptParam);
int sceKernelDeleteMutex(SceUID mutexId);
int sceKernelLockMutex(SceUID mutexId, int32_t lockCount, uint32_t *pTimeout);
int sceKernelUnlockMutex(SceUID mutexId, int32_t unlockCount);

/* ---------- Semaphore ---------- */
typedef struct SceKernelSemaOptParam {
    SceSize size;
} SceKernelSemaOptParam;

SceUID sceKernelCreateSema(const char *pName, uint32_t attr,
                            int32_t initVal, int32_t maxVal,
                            const SceKernelSemaOptParam *pOptParam);
int sceKernelDeleteSema(SceUID semaId);
int sceKernelWaitSema(SceUID semaId, int32_t needCount, uint32_t *pTimeout);
int sceKernelSignalSema(SceUID semaId, int32_t signalCount);

/* ---------- Event flags ---------- */
typedef struct SceKernelEventFlagOptParam {
    SceSize size;
} SceKernelEventFlagOptParam;

#define SCE_KERNEL_EVF_ATTR_MULTI       0x1000
#define SCE_KERNEL_EVF_WAITMODE_AND     0x02
#define SCE_KERNEL_EVF_WAITMODE_OR      0x01
#define SCE_KERNEL_EVF_WAITMODE_CLEAR_ALL  0x20
#define SCE_KERNEL_EVF_WAITMODE_CLEAR_PAT  0x10

SceUID sceKernelCreateEventFlag(const char *pName, uint32_t attr,
                                 uint32_t initPattern,
                                 const SceKernelEventFlagOptParam *pOptParam);
int sceKernelDeleteEventFlag(SceUID evfId);
int sceKernelSetEventFlag(SceUID evfId, uint32_t setBits);
int sceKernelClearEventFlag(SceUID evfId, uint32_t clearBits);
int sceKernelWaitEventFlag(SceUID evfId, uint32_t waitPattern,
                            uint32_t waitMode, uint32_t *pResultPat,
                            uint32_t *pTimeout);

/* ---------- Thread ---------- */
typedef int (*SceKernelThreadEntry)(uint32_t argSize, void *pArgBlock);

typedef struct SceKernelThreadOptParam {
    SceSize size;
    uint32_t attr;
} SceKernelThreadOptParam;

SceUID sceKernelCreateThread(const char *pName, SceKernelThreadEntry entry,
                              int32_t initPriority, SceSize stackSize,
                              uint32_t attr, int32_t cpuAffinityMask,
                              const SceKernelThreadOptParam *pOptParam);
int sceKernelDeleteThread(SceUID thid);
int sceKernelStartThread(SceUID thid, SceSize argSize, void *pArgBlock);
int sceKernelWaitThreadEnd(SceUID thid, int32_t *pExitStatus,
                            uint32_t *pTimeout);
int sceKernelExitThread(int32_t exitStatus);
int sceKernelExitDeleteThread(int32_t exitStatus);
SceUID sceKernelGetThreadId(void);
int sceKernelChangeThreadPriority(SceUID thid, int32_t priority);

/* ---------- Delay / sleep ---------- */
int sceKernelDelayThread(SceKernelUseconds usec);
int sceKernelDelayThreadCB(SceKernelUseconds usec);

/* ---------- Memory allocation (userland heap) ---------- */
#define SCE_KERNEL_MEMBLOCK_TYPE_USER_RW    0x0C20D060
#define SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW  0x40408060

SceUID sceKernelAllocMemBlock(const char *pName, uint32_t type,
                               int32_t size, void *pOpt);
int sceKernelFreeMemBlock(SceUID uid);
int sceKernelGetMemBlockBase(SceUID uid, void **ppBase);

/* ---------- Misc ---------- */
int sceKernelGetRandomNumber(void *pOutput, SceSize size);

#ifdef __cplusplus
}
#endif

#endif /* SHIM_SCEKERNEL_H */
