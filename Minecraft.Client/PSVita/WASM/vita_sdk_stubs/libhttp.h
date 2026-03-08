#pragma once

#include <stddef.h>

typedef int SceHttpStatusCode;

inline int sceHttpInit(int poolSize) {
    (void)poolSize;
    return 0;
}

inline int sceHttpTerm(void) {
    return 0;
}

inline int sceHttpCreateTemplate(const char *userAgent, int httpVer, int autoProxyConf) {
    (void)userAgent; (void)httpVer; (void)autoProxyConf;
    return 1;
}

inline int sceHttpCreateConnectionWithURL(int tmplId, const char *url, int enableKeepalive) {
    (void)tmplId; (void)url; (void)enableKeepalive;
    return 1;
}

inline int sceHttpCreateRequestWithURL(int connId, int method, const char *url, unsigned long long contentLength) {
    (void)connId; (void)method; (void)url; (void)contentLength;
    return 1;
}

inline int sceHttpSendRequest(int reqId, const void *postData, size_t size) {
    (void)reqId; (void)postData; (void)size;
    return 0;
}

inline int sceHttpReadData(int reqId, void *data, size_t size) {
    (void)reqId; (void)data; (void)size;
    return 0;
}

inline int sceHttpGetStatusCode(int reqId, int *statusCode) {
    (void)reqId;
    if (statusCode) *statusCode = 200;
    return 0;
}

inline int sceHttpDeleteRequest(int reqId) {
    (void)reqId;
    return 0;
}

inline int sceHttpDeleteConnection(int connId) {
    (void)connId;
    return 0;
}

inline int sceHttpDeleteTemplate(int tmplId) {
    (void)tmplId;
    return 0;
}

inline int sceHttpAddRequestHeader(int reqId, const char *name, const char *value, int mode) {
    (void)reqId; (void)name; (void)value; (void)mode;
    return 0;
}

inline int sceHttpGetResponseContentLength(int reqId, unsigned long long *contentLength) {
    (void)reqId;
    if (contentLength) *contentLength = 0;
    return 0;
}

#define SCE_HTTP_METHOD_GET  0
#define SCE_HTTP_METHOD_POST 1
#define SCE_HTTP_METHOD_HEAD 2

#define SCE_HTTP_HEADER_OVERWRITE 0
#define SCE_HTTP_HEADER_ADD       1
