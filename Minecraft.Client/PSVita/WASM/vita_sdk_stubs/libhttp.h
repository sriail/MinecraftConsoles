#pragma once
/* libhttp.h — PS Vita HTTP client stubs for Emscripten/WASM builds */

#include <stdint.h>
#include "kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCE_HTTP_DEFAULT_RESPONSE_HEADER_MAX  (64*1024)
#define SCE_HTTP_MAX_URI_LENGTH               512

#define SCE_HTTP_REQUEST_METHOD_GET     0
#define SCE_HTTP_REQUEST_METHOD_POST    1
#define SCE_HTTP_REQUEST_METHOD_HEAD    2
#define SCE_HTTP_REQUEST_METHOD_PUT     3
#define SCE_HTTP_REQUEST_METHOD_DELETE  4

typedef struct { int id; } SceHttpTemplate;
typedef struct { int id; } SceHttpConnection;
typedef struct { int id; } SceHttpRequest;

static inline int sceHttpInit(int poolSize) { (void)poolSize; return SCE_OK; }
static inline int sceHttpTerm(void) { return SCE_OK; }

static inline int sceHttpCreateTemplate(const char *userAgent, int httpVer,
    int autoProxyConf)
{ (void)userAgent; (void)httpVer; (void)autoProxyConf; return 1; }

static inline int sceHttpDeleteTemplate(int tmplId)
{ (void)tmplId; return SCE_OK; }

static inline int sceHttpCreateConnectionWithURL(int tmplId, const char *url,
    int keepAlive)
{ (void)tmplId; (void)url; (void)keepAlive; return 1; }

static inline int sceHttpDeleteConnection(int connId)
{ (void)connId; return SCE_OK; }

static inline int sceHttpCreateRequestWithURL(int connId, int method,
    const char *url, uint64_t contentLength)
{ (void)connId; (void)method; (void)url; (void)contentLength; return 1; }

static inline int sceHttpDeleteRequest(int reqId)
{ (void)reqId; return SCE_OK; }

static inline int sceHttpSendRequest(int reqId, const void *postData,
    unsigned int size)
{ (void)reqId; (void)postData; (void)size; return SCE_OK; }

static inline int sceHttpReadData(int reqId, void *data, unsigned int size)
{ (void)reqId; (void)data; (void)size; return 0; }

static inline int sceHttpGetStatusCode(int reqId, int *statusCode)
{ (void)reqId; if (statusCode) *statusCode = 200; return SCE_OK; }

static inline int sceHttpAddRequestHeader(int id, const char *name,
    const char *value, unsigned int mode)
{ (void)id; (void)name; (void)value; (void)mode; return SCE_OK; }

static inline int sceHttpSetRequestContentLength(int reqId, uint64_t contentLength)
{ (void)reqId; (void)contentLength; return SCE_OK; }

#ifdef __cplusplus
}
#endif
