#pragma once
// Minimal PS Vita rudp.h stub for Emscripten/WASM builds.
#include <stdint.h>
typedef int SceRudpContext;
typedef void (*SceRudpEventHandler)(void *ctx, int event, void *param);
#define SCE_RUDP_OK 0
#ifdef __cplusplus
extern "C" {
#endif
int sceRudpInit(int memId);
int sceRudpEnd(void);
int sceRudpCreateContext(void **ppCtx, const void *params);
int sceRudpDestroyContext(void *pCtx);
int sceRudpBind(void *pCtx, const void *addr);
int sceRudpInitiate(void *pCtx, const void *peer, const void *params);
int sceRudpTerminate(void *pCtx, int reasonCode);
int sceRudpSetEventHandler(void *pCtx, SceRudpEventHandler handler, void *userdata);
int sceRudpEnableInternalIOThread(void);
int sceRudpSend(void *pCtx, const void *buf, int len, int flags);
int sceRudpRecv(void *pCtx, void *buf, int len, int flags);
int sceRudpFlush(void *pCtx);
int sceRudpProcess(void);
#ifdef __cplusplus
}
#endif
