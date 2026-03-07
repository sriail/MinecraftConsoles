#pragma once
/* rudp.h — PS Vita Reliable UDP stubs for Emscripten/WASM builds */

#include <stdint.h>
#include "kernel.h"
#include "net.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCE_RUDP_MUXMODE_P2P         0
#define SCE_RUDP_MUXMODE_SERVER      1
#define SCE_RUDP_ERROR_WOULDBLOCK    -2144600023
#define SCE_KERNEL_DEFAULT_PRIORITY  160

typedef void *SceRudpContext;

typedef void (*SceRudpContextCallback)(SceRudpContext ctx, int event, void *arg);
typedef void (*SceRudpEventCallback)(int event, int soc, const uint8_t *data,
    size_t datalen, const SceNetSockaddr *addr,
    SceNetSocklen_t addrlen, void *arg);

static inline int sceRudpInit(void *pool, size_t poolSize)
{ (void)pool; (void)poolSize; return SCE_OK; }

static inline int sceRudpEnd(void) { return SCE_OK; }

static inline int sceRudpEnableInternalIOThread(size_t stackSize, int priority)
{ (void)stackSize; (void)priority; return SCE_OK; }

static inline int sceRudpCreateContext(SceRudpContextCallback cb,
    void *arg, SceRudpContext *pCtx)
{
    (void)cb; (void)arg;
    if (pCtx) *pCtx = (SceRudpContext)(intptr_t)1;
    return SCE_OK;
}

static inline int sceRudpTerminate(SceRudpContext ctx)
{ (void)ctx; return SCE_OK; }

static inline int sceRudpBind(SceRudpContext ctx, int soc, int param, int muxmode)
{ (void)ctx; (void)soc; (void)param; (void)muxmode; return SCE_OK; }

static inline int sceRudpInitiate(SceRudpContext ctx, const SceNetSockaddr *addr,
    SceNetSocklen_t addrLen, int flags)
{ (void)ctx; (void)addr; (void)addrLen; (void)flags; return SCE_OK; }

static inline int sceRudpSend(SceRudpContext ctx, const void *data, size_t len,
    int flags)
{ (void)ctx; (void)data; (void)len; (void)flags; return 0; }

static inline unsigned int sceRudpGetSizeReadable(SceRudpContext ctx)
{ (void)ctx; return 0; }

static inline unsigned int sceRudpRead(SceRudpContext ctx, void *buf,
    unsigned int size, int flags, SceNetSockaddr *addr)
{ (void)ctx; (void)buf; (void)size; (void)flags; (void)addr; return 0; }

static inline int sceRudpRegisterEventCallback(int soc,
    SceRudpEventCallback cb, void *arg)
{ (void)soc; (void)cb; (void)arg; return SCE_OK; }

static inline int sceRudpUnregisterEventCallback(int soc)
{ (void)soc; return SCE_OK; }

static inline int sceRudpFlush(SceRudpContext ctx)
{ (void)ctx; return SCE_OK; }

#ifdef __cplusplus
}
#endif
