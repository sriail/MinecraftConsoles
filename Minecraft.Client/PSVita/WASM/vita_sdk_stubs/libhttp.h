#pragma once
// Minimal PS Vita libhttp.h stub for Emscripten/WASM builds.
#include <stdint.h>
typedef struct SceSslCert { void *data; } SceSslCert;
#ifdef __cplusplus
extern "C" {
#endif
int sceHttpInit(int memId, int poolSize);
int sceHttpTerm(void);
int sceHttpCreateTemplate(const char *userAgent, int httpVer, int autoProxyConf);
int sceHttpDeleteTemplate(int tmplId);
int sceHttpCreateConnectionWithURL(int tmplId, const char *url, int enableKeepalive);
int sceHttpDeleteConnection(int connId);
int sceHttpCreateRequestWithURL(int connId, int method, const char *url, uint64_t contentLen);
int sceHttpDeleteRequest(int reqId);
int sceHttpSendRequest(int reqId, const void *postData, uint32_t size);
int sceHttpReadData(int reqId, void *data, uint32_t size);
int sceHttpGetStatusCode(int reqId, int *statusCode);
int sceHttpGetResponseContentLength(int reqId, int method, int statusCode, uint64_t *contentLen);
int sceSslInit(int poolSize);
int sceSslTerm(void);
int sceHttpsSetSslCallback(int id, void *callback, void *userdata);
int sceHttpsDisableOption(int id, uint32_t sslFlags);
#ifdef __cplusplus
}
#endif
