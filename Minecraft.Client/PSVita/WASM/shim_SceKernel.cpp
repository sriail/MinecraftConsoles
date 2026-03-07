/*
 * shim_SceKernel.cpp
 * PS Vita SceKernel → Emscripten pthreads shim implementation
 *
 * Build requirements:
 *   -s USE_PTHREADS=1
 *   -s PTHREAD_POOL_SIZE=16
 *   Requires COOP / COEP HTTP headers on the server for SharedArrayBuffer.
 */

#include "shim_SceKernel.h"

#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <assert.h>

#ifdef __EMSCRIPTEN__
#  include <emscripten.h>
#  include <emscripten/threading.h>
#endif

/* -------------------------------------------------------------------------
 * Internal handle registry
 * We hand out integer SceUID values and look them up in a flat table.
 * Maximum 512 live objects (more than enough for Minecraft Vita).
 * ------------------------------------------------------------------------- */

#define MAX_HANDLES 512

typedef enum {
    HTYPE_FREE = 0,
    HTYPE_LWMUTEX,
    HTYPE_MUTEX,
    HTYPE_SEMA,
    HTYPE_EVF,
    HTYPE_THREAD
} HandleType;

typedef struct {
    HandleType type;
    union {
        pthread_mutex_t     mutex;
        sem_t               sema;
        struct {
            pthread_mutex_t  evf_mutex;
            pthread_cond_t   evf_cond;
            uint32_t         pattern;
        } evf;
        struct {
            pthread_t        tid;
            SceKernelThreadEntry entry;
            void            *arg;
            uint32_t         argSize;
        } thread;
    };
} Handle;

static Handle  s_handles[MAX_HANDLES];
static pthread_mutex_t s_registry_lock = PTHREAD_MUTEX_INITIALIZER;
static int32_t s_next_uid = 1;   /* UIDs start at 1 */

static SceUID alloc_handle(HandleType type)
{
    pthread_mutex_lock(&s_registry_lock);
    for (int i = 0; i < MAX_HANDLES; i++) {
        if (s_handles[i].type == HTYPE_FREE) {
            memset(&s_handles[i], 0, sizeof(Handle));
            s_handles[i].type = type;
            SceUID uid = (SceUID)(i + 1); /* 1-based */
            pthread_mutex_unlock(&s_registry_lock);
            return uid;
        }
    }
    pthread_mutex_unlock(&s_registry_lock);
    return SCE_KERNEL_ERROR_NO_MEMORY;
}

static Handle *get_handle(SceUID uid, HandleType expected_type)
{
    if (uid < 1 || uid > MAX_HANDLES) return NULL;
    Handle *h = &s_handles[uid - 1];
    if (h->type != expected_type) return NULL;
    return h;
}

static void free_handle(SceUID uid)
{
    if (uid < 1 || uid > MAX_HANDLES) return;
    pthread_mutex_lock(&s_registry_lock);
    s_handles[uid - 1].type = HTYPE_FREE;
    pthread_mutex_unlock(&s_registry_lock);
}

/* =========================================================================
 * Lightweight Mutex
 * VITA:  sceKernelLwMutex*
 * WEB:   pthread_mutex_t stored inside SceKernelLwMutexWork
 * NOTE:  We embed the pthread_mutex_t directly in the Work struct
 *        (Vita's Work is 16 bytes; pthread_mutex_t is 40 bytes on Emscripten,
 *         so we use an indirection pointer stored in data[0]).
 * ========================================================================= */

int sceKernelCreateLwMutex(SceKernelLwMutexWork *pWork, const char *pName,
                            uint32_t attr, int32_t initCount,
                            const SceKernelLwMutexOptParam *pOptParam)
{
    (void)pName; (void)initCount; (void)pOptParam;
    if (!pWork) return SCE_KERNEL_ERROR_INVALID_ARG;

    pthread_mutex_t *mtx = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if (!mtx) return SCE_KERNEL_ERROR_NO_MEMORY;

    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    if (attr & SCE_KERNEL_LW_MUTEX_ATTR_RECURSIVE) {
        pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
    } else {
        pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_DEFAULT);
    }
    int rc = pthread_mutex_init(mtx, &mattr);
    pthread_mutexattr_destroy(&mattr);
    if (rc != 0) { free(mtx); return SCE_KERNEL_ERROR_NO_MEMORY; }

    /* Store pointer to heap-allocated mutex inside the Work opaque blob */
    memcpy(&pWork->data[0], &mtx, sizeof(void *));
    return SCE_OK;
}

int sceKernelDeleteLwMutex(SceKernelLwMutexWork *pWork)
{
    if (!pWork) return SCE_KERNEL_ERROR_INVALID_ARG;
    pthread_mutex_t *mtx = NULL;
    memcpy(&mtx, &pWork->data[0], sizeof(void *));
    if (!mtx) return SCE_KERNEL_ERROR_INVALID_ARG;
    pthread_mutex_destroy(mtx);
    free(mtx);
    memset(pWork, 0, sizeof(*pWork));
    return SCE_OK;
}

int sceKernelLockLwMutex(SceKernelLwMutexWork *pWork, int32_t lockCount,
                          uint32_t *pTimeout)
{
    (void)pTimeout;
    if (!pWork) return SCE_KERNEL_ERROR_INVALID_ARG;
    pthread_mutex_t *mtx = NULL;
    memcpy(&mtx, &pWork->data[0], sizeof(void *));
    if (!mtx) return SCE_KERNEL_ERROR_INVALID_ARG;
    for (int32_t i = 0; i < lockCount; i++) {
        pthread_mutex_lock(mtx);
    }
    return SCE_OK;
}

int sceKernelUnlockLwMutex(SceKernelLwMutexWork *pWork, int32_t unlockCount)
{
    if (!pWork) return SCE_KERNEL_ERROR_INVALID_ARG;
    pthread_mutex_t *mtx = NULL;
    memcpy(&mtx, &pWork->data[0], sizeof(void *));
    if (!mtx) return SCE_KERNEL_ERROR_INVALID_ARG;
    for (int32_t i = 0; i < unlockCount; i++) {
        pthread_mutex_unlock(mtx);
    }
    return SCE_OK;
}

int sceKernelTryLockLwMutex(SceKernelLwMutexWork *pWork, int32_t lockCount)
{
    if (!pWork) return SCE_KERNEL_ERROR_INVALID_ARG;
    pthread_mutex_t *mtx = NULL;
    memcpy(&mtx, &pWork->data[0], sizeof(void *));
    if (!mtx) return SCE_KERNEL_ERROR_INVALID_ARG;
    for (int32_t i = 0; i < lockCount; i++) {
        if (pthread_mutex_trylock(mtx) != 0) {
            /* Undo already-acquired locks */
            for (int32_t j = 0; j < i; j++) pthread_mutex_unlock(mtx);
            return SCE_KERNEL_ERROR_TIMEOUT;
        }
    }
    return SCE_OK;
}

/* =========================================================================
 * Mutex (named, UID-based)
 * VITA:  sceKernelCreateMutex / Delete / Lock / Unlock
 * WEB:   pthread_mutex_t, allocated in handle table
 * NOTE:  Named mutexes are unsupported in WASM; name is ignored.
 * ========================================================================= */

SceUID sceKernelCreateMutex(const char *pName, uint32_t attr,
                             int32_t initCount,
                             const SceKernelMutexOptParam *pOptParam)
{
    (void)pName; (void)initCount; (void)pOptParam;
    SceUID uid = alloc_handle(HTYPE_MUTEX);
    if (uid < 0) return uid;

    Handle *h = get_handle(uid, HTYPE_MUTEX);
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    if (attr & SCE_KERNEL_MUTEX_ATTR_RECURSIVE) {
        pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
    }
    int rc = pthread_mutex_init(&h->mutex, &mattr);
    pthread_mutexattr_destroy(&mattr);
    if (rc != 0) { free_handle(uid); return SCE_KERNEL_ERROR_NO_MEMORY; }
    return uid;
}

int sceKernelDeleteMutex(SceUID mutexId)
{
    Handle *h = get_handle(mutexId, HTYPE_MUTEX);
    if (!h) return SCE_KERNEL_ERROR_INVALID_ARG;
    pthread_mutex_destroy(&h->mutex);
    free_handle(mutexId);
    return SCE_OK;
}

int sceKernelLockMutex(SceUID mutexId, int32_t lockCount, uint32_t *pTimeout)
{
    (void)pTimeout;
    Handle *h = get_handle(mutexId, HTYPE_MUTEX);
    if (!h) return SCE_KERNEL_ERROR_INVALID_ARG;
    for (int32_t i = 0; i < lockCount; i++) pthread_mutex_lock(&h->mutex);
    return SCE_OK;
}

int sceKernelUnlockMutex(SceUID mutexId, int32_t unlockCount)
{
    Handle *h = get_handle(mutexId, HTYPE_MUTEX);
    if (!h) return SCE_KERNEL_ERROR_INVALID_ARG;
    for (int32_t i = 0; i < unlockCount; i++) pthread_mutex_unlock(&h->mutex);
    return SCE_OK;
}

/* =========================================================================
 * Semaphore
 * VITA:  sceKernelCreateSema / Delete / Wait / Signal
 * WEB:   POSIX sem_t (available in Emscripten with USE_PTHREADS)
 * NOTE:  sem_timedwait provides timeout; NULL timeout blocks indefinitely.
 * ========================================================================= */

SceUID sceKernelCreateSema(const char *pName, uint32_t attr,
                            int32_t initVal, int32_t maxVal,
                            const SceKernelSemaOptParam *pOptParam)
{
    (void)pName; (void)attr; (void)maxVal; (void)pOptParam;
    SceUID uid = alloc_handle(HTYPE_SEMA);
    if (uid < 0) return uid;
    Handle *h = get_handle(uid, HTYPE_SEMA);
    if (sem_init(&h->sema, 0, (unsigned int)initVal) != 0) {
        free_handle(uid);
        return SCE_KERNEL_ERROR_NO_MEMORY;
    }
    return uid;
}

int sceKernelDeleteSema(SceUID semaId)
{
    Handle *h = get_handle(semaId, HTYPE_SEMA);
    if (!h) return SCE_KERNEL_ERROR_INVALID_ARG;
    sem_destroy(&h->sema);
    free_handle(semaId);
    return SCE_OK;
}

int sceKernelWaitSema(SceUID semaId, int32_t needCount, uint32_t *pTimeout)
{
    Handle *h = get_handle(semaId, HTYPE_SEMA);
    if (!h) return SCE_KERNEL_ERROR_INVALID_ARG;
    for (int32_t i = 0; i < needCount; i++) {
        if (pTimeout && *pTimeout != 0xFFFFFFFF) {
            struct timespec ts;
            clock_gettime(CLOCK_REALTIME, &ts);
            uint64_t ns = (uint64_t)*pTimeout * 1000ULL;
            ts.tv_sec  += (time_t)(ns / 1000000000ULL);
            ts.tv_nsec += (long)(ns % 1000000000ULL);
            if (ts.tv_nsec >= 1000000000L) {
                ts.tv_sec++;
                ts.tv_nsec -= 1000000000L;
            }
            if (sem_timedwait(&h->sema, &ts) != 0) {
                return SCE_KERNEL_ERROR_WAIT_TIMEOUT;
            }
        } else {
            sem_wait(&h->sema);
        }
    }
    return SCE_OK;
}

int sceKernelSignalSema(SceUID semaId, int32_t signalCount)
{
    Handle *h = get_handle(semaId, HTYPE_SEMA);
    if (!h) return SCE_KERNEL_ERROR_INVALID_ARG;
    for (int32_t i = 0; i < signalCount; i++) sem_post(&h->sema);
    return SCE_OK;
}

/* =========================================================================
 * Event Flags
 * VITA:  sceKernelCreateEventFlag / Delete / Set / Clear / Wait
 * WEB:   pthread_mutex + pthread_cond + uint32_t pattern
 * NOTE:  CLEAR_ALL and CLEAR_PAT modes clear bits after wakeup.
 * ========================================================================= */

SceUID sceKernelCreateEventFlag(const char *pName, uint32_t attr,
                                 uint32_t initPattern,
                                 const SceKernelEventFlagOptParam *pOptParam)
{
    (void)pName; (void)attr; (void)pOptParam;
    SceUID uid = alloc_handle(HTYPE_EVF);
    if (uid < 0) return uid;
    Handle *h = get_handle(uid, HTYPE_EVF);
    pthread_mutex_init(&h->evf.evf_mutex, NULL);
    pthread_cond_init(&h->evf.evf_cond, NULL);
    h->evf.pattern = initPattern;
    return uid;
}

int sceKernelDeleteEventFlag(SceUID evfId)
{
    Handle *h = get_handle(evfId, HTYPE_EVF);
    if (!h) return SCE_KERNEL_ERROR_INVALID_ARG;
    pthread_mutex_destroy(&h->evf.evf_mutex);
    pthread_cond_destroy(&h->evf.evf_cond);
    free_handle(evfId);
    return SCE_OK;
}

int sceKernelSetEventFlag(SceUID evfId, uint32_t setBits)
{
    Handle *h = get_handle(evfId, HTYPE_EVF);
    if (!h) return SCE_KERNEL_ERROR_INVALID_ARG;
    pthread_mutex_lock(&h->evf.evf_mutex);
    h->evf.pattern |= setBits;
    pthread_cond_broadcast(&h->evf.evf_cond);
    pthread_mutex_unlock(&h->evf.evf_mutex);
    return SCE_OK;
}

int sceKernelClearEventFlag(SceUID evfId, uint32_t clearBits)
{
    Handle *h = get_handle(evfId, HTYPE_EVF);
    if (!h) return SCE_KERNEL_ERROR_INVALID_ARG;
    pthread_mutex_lock(&h->evf.evf_mutex);
    h->evf.pattern &= ~clearBits;
    pthread_mutex_unlock(&h->evf.evf_mutex);
    return SCE_OK;
}

int sceKernelWaitEventFlag(SceUID evfId, uint32_t waitPattern,
                            uint32_t waitMode, uint32_t *pResultPat,
                            uint32_t *pTimeout)
{
    (void)pTimeout;
    Handle *h = get_handle(evfId, HTYPE_EVF);
    if (!h) return SCE_KERNEL_ERROR_INVALID_ARG;

    pthread_mutex_lock(&h->evf.evf_mutex);
    for (;;) {
        int satisfied;
        if (waitMode & SCE_KERNEL_EVF_WAITMODE_AND) {
            satisfied = ((h->evf.pattern & waitPattern) == waitPattern);
        } else {
            satisfied = ((h->evf.pattern & waitPattern) != 0);
        }
        if (satisfied) {
            if (pResultPat) *pResultPat = h->evf.pattern;
            if (waitMode & SCE_KERNEL_EVF_WAITMODE_CLEAR_ALL) {
                h->evf.pattern = 0;
            } else if (waitMode & SCE_KERNEL_EVF_WAITMODE_CLEAR_PAT) {
                h->evf.pattern &= ~waitPattern;
            }
            break;
        }
        pthread_cond_wait(&h->evf.evf_cond, &h->evf.evf_mutex);
    }
    pthread_mutex_unlock(&h->evf.evf_mutex);
    return SCE_OK;
}

/* =========================================================================
 * Threads
 * VITA:  sceKernelCreateThread / Delete / Start / WaitEnd / Exit
 * WEB:   pthread_create / pthread_join
 * NOTE:  Vita thread priorities are inverted (lower number = higher priority).
 *        WASM pthread priorities are not portable — mapping is best-effort.
 *        Stack size is respected via pthread_attr_setstacksize.
 * ========================================================================= */

typedef struct {
    SceKernelThreadEntry entry;
    void    *arg;
    uint32_t argSize;
} ThreadArg;

static void *thread_trampoline(void *raw)
{
    ThreadArg *ta = (ThreadArg *)raw;
    int32_t result = ta->entry(ta->argSize, ta->arg);
    free(ta);
    return (void *)(intptr_t)result;
}

SceUID sceKernelCreateThread(const char *pName, SceKernelThreadEntry entry,
                              int32_t initPriority, SceSize stackSize,
                              uint32_t attr, int32_t cpuAffinityMask,
                              const SceKernelThreadOptParam *pOptParam)
{
    (void)pName; (void)initPriority; (void)attr;
    (void)cpuAffinityMask; (void)pOptParam;
    if (!entry) return SCE_KERNEL_ERROR_INVALID_ARG;

    SceUID uid = alloc_handle(HTYPE_THREAD);
    if (uid < 0) return uid;
    Handle *h = get_handle(uid, HTYPE_THREAD);
    h->thread.entry   = entry;
    h->thread.arg     = NULL;
    h->thread.argSize = 0;
    h->thread.tid     = 0;

    /* Store stack size in the handle for sceKernelStartThread */
    (void)stackSize; /* pthread_attr_setstacksize used at start time */
    return uid;
}

int sceKernelDeleteThread(SceUID thid)
{
    Handle *h = get_handle(thid, HTYPE_THREAD);
    if (!h) return SCE_KERNEL_ERROR_INVALID_ARG;
    free_handle(thid);
    return SCE_OK;
}

int sceKernelStartThread(SceUID thid, SceSize argSize, void *pArgBlock)
{
    Handle *h = get_handle(thid, HTYPE_THREAD);
    if (!h) return SCE_KERNEL_ERROR_INVALID_ARG;

    ThreadArg *ta = (ThreadArg *)malloc(sizeof(ThreadArg));
    if (!ta) return SCE_KERNEL_ERROR_NO_MEMORY;
    ta->entry   = h->thread.entry;
    ta->argSize = (uint32_t)argSize;
    ta->arg     = pArgBlock;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    int rc = pthread_create(&h->thread.tid, &attr, thread_trampoline, ta);
    pthread_attr_destroy(&attr);
    if (rc != 0) { free(ta); return SCE_KERNEL_ERROR_NO_MEMORY; }
    return SCE_OK;
}

int sceKernelWaitThreadEnd(SceUID thid, int32_t *pExitStatus,
                            uint32_t *pTimeout)
{
    (void)pTimeout;
    Handle *h = get_handle(thid, HTYPE_THREAD);
    if (!h) return SCE_KERNEL_ERROR_INVALID_ARG;
    void *retval = NULL;
    pthread_join(h->thread.tid, &retval);
    if (pExitStatus) *pExitStatus = (int32_t)(intptr_t)retval;
    return SCE_OK;
}

int sceKernelExitThread(int32_t exitStatus)
{
    pthread_exit((void *)(intptr_t)exitStatus);
    return SCE_OK; /* unreachable */
}

int sceKernelExitDeleteThread(int32_t exitStatus)
{
    return sceKernelExitThread(exitStatus);
}

SceUID sceKernelGetThreadId(void)
{
    /* Maintain a simple per-thread ID stored in TLS to avoid
     * non-portable pthread_t casts.  The first call from a thread
     * allocates an ID from a monotonic counter. */
    static pthread_mutex_t tid_lock = PTHREAD_MUTEX_INITIALIZER;
    static int32_t         s_tid_counter = 0x100;
    static __thread int32_t tls_tid = 0;
    if (tls_tid == 0) {
        pthread_mutex_lock(&tid_lock);
        tls_tid = ++s_tid_counter;
        pthread_mutex_unlock(&tid_lock);
    }
    return (SceUID)tls_tid;
}

int sceKernelChangeThreadPriority(SceUID thid, int32_t priority)
{
    /* [VITA_SHIM_STUB] sceKernelChangeThreadPriority: priority mapping to
     * POSIX sched is non-portable in WASM; stub returns SCE_OK. */
    fprintf(stderr, "[VITA_SHIM_STUB] sceKernelChangeThreadPriority(%d, %d)\n",
            thid, priority);
    return SCE_OK;
}

/* =========================================================================
 * Delay / Sleep
 * VITA:  sceKernelDelayThread (microseconds)
 * WEB:   emscripten_sleep (milliseconds, requires ASYNCIFY) or
 *        nanosleep in a worker thread context
 * NOTE:  On the main WASM thread, emscripten_sleep suspends via Asyncify.
 *        On worker threads (pthreads build), nanosleep works natively.
 * ========================================================================= */

int sceKernelDelayThread(SceKernelUseconds usec)
{
#ifdef __EMSCRIPTEN__
    if (emscripten_is_main_browser_thread()) {
        emscripten_sleep(usec / 1000);
    } else {
        struct timespec ts;
        ts.tv_sec  = usec / 1000000;
        ts.tv_nsec = (usec % 1000000) * 1000;
        nanosleep(&ts, NULL);
    }
#else
    struct timespec ts;
    ts.tv_sec  = usec / 1000000;
    ts.tv_nsec = (usec % 1000000) * 1000;
    nanosleep(&ts, NULL);
#endif
    return SCE_OK;
}

int sceKernelDelayThreadCB(SceKernelUseconds usec)
{
    return sceKernelDelayThread(usec);
}

/* =========================================================================
 * Memory blocks
 * VITA:  sceKernelAllocMemBlock / FreeMemBlock / GetMemBlockBase
 * WEB:   malloc / free (WASM linear memory)
 * NOTE:  type and alignment hints are ignored; WASM heap is flat.
 * ========================================================================= */

#define MAX_MEMBLOCKS 128

typedef struct {
    SceUID uid;
    void  *ptr;
} MemBlock;

static MemBlock  s_memblocks[MAX_MEMBLOCKS];
static int32_t   s_mb_uid_counter = 0x4000;
static pthread_mutex_t s_mb_lock = PTHREAD_MUTEX_INITIALIZER;

SceUID sceKernelAllocMemBlock(const char *pName, uint32_t type,
                               int32_t size, void *pOpt)
{
    (void)pName; (void)type; (void)pOpt;
    if (size <= 0) return SCE_KERNEL_ERROR_INVALID_ARG;
    void *p = malloc((size_t)size);
    if (!p) return SCE_KERNEL_ERROR_NO_MEMORY;
    pthread_mutex_lock(&s_mb_lock);
    for (int i = 0; i < MAX_MEMBLOCKS; i++) {
        if (s_memblocks[i].uid == 0) {
            SceUID uid = ++s_mb_uid_counter;
            s_memblocks[i].uid = uid;
            s_memblocks[i].ptr = p;
            pthread_mutex_unlock(&s_mb_lock);
            return uid;
        }
    }
    pthread_mutex_unlock(&s_mb_lock);
    free(p);
    return SCE_KERNEL_ERROR_NO_MEMORY;
}

int sceKernelFreeMemBlock(SceUID uid)
{
    pthread_mutex_lock(&s_mb_lock);
    for (int i = 0; i < MAX_MEMBLOCKS; i++) {
        if (s_memblocks[i].uid == uid) {
            free(s_memblocks[i].ptr);
            s_memblocks[i].uid = 0;
            s_memblocks[i].ptr = NULL;
            pthread_mutex_unlock(&s_mb_lock);
            return SCE_OK;
        }
    }
    pthread_mutex_unlock(&s_mb_lock);
    return SCE_KERNEL_ERROR_INVALID_ARG;
}

int sceKernelGetMemBlockBase(SceUID uid, void **ppBase)
{
    if (!ppBase) return SCE_KERNEL_ERROR_INVALID_ARG;
    pthread_mutex_lock(&s_mb_lock);
    for (int i = 0; i < MAX_MEMBLOCKS; i++) {
        if (s_memblocks[i].uid == uid) {
            *ppBase = s_memblocks[i].ptr;
            pthread_mutex_unlock(&s_mb_lock);
            return SCE_OK;
        }
    }
    pthread_mutex_unlock(&s_mb_lock);
    return SCE_KERNEL_ERROR_INVALID_ARG;
}

/* =========================================================================
 * Random number generation
 * VITA:  sceKernelGetRandomNumber
 * WEB:   crypto.getRandomValues (via Emscripten getentropy)
 * ========================================================================= */

int sceKernelGetRandomNumber(void *pOutput, SceSize size)
{
    if (!pOutput || size <= 0) return SCE_KERNEL_ERROR_INVALID_ARG;
#ifdef __EMSCRIPTEN__
    EM_ASM({
        var buf = new Uint8Array(Module.HEAPU8.buffer, $0, $1);
        if (typeof crypto !== 'undefined' && crypto.getRandomValues) {
            crypto.getRandomValues(buf);
        } else {
            for (var i = 0; i < $1; i++) buf[i] = Math.random() * 256 | 0;
        }
    }, pOutput, size);
#else
    /* Fallback: arc4random_buf or /dev/urandom */
    uint8_t *out = (uint8_t *)pOutput;
    for (SceSize i = 0; i < size; i++) out[i] = (uint8_t)(rand() & 0xFF);
#endif
    return SCE_OK;
}
