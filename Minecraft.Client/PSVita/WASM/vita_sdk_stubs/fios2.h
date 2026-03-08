#pragma once

#include <stddef.h>
#include <stdint.h>

typedef int SceFiosFH;
typedef int SceFiosDH;
typedef int SceFiosOp;

typedef struct SceFiosBuffer {
    void *ptr;
    size_t length;
} SceFiosBuffer;

typedef struct SceFiosParams {
    uint32_t initialized;
    uint32_t paramsSize;
    void *opStorage;
    void *chunkStorage;
    void *fhStorage;
    void *dhStorage;
} SceFiosParams;

#define SCE_FIOS_OK 0

#define SCE_FIOS_OP_STORAGE_SIZE(n) ((n) * 128)
#define SCE_FIOS_CHUNK_STORAGE_SIZE(n, s) ((n) * (s))
#define SCE_FIOS_FH_STORAGE_SIZE(n, p) ((n) * 256)
#define SCE_FIOS_DH_STORAGE_SIZE(n, p) ((n) * 256)
#define SCE_FIOS_ALIGN_UP(x, a) (((x) + (a) - 1) & ~((a) - 1))

inline int sceFiosInitialize(const SceFiosParams *params) {
    (void)params;
    return 0;
}

inline int sceFiosFHOpenSync(void *attr, SceFiosFH *fh, const char *path, void *params) {
    (void)attr; (void)fh; (void)path; (void)params;
    return 0;
}

inline int sceFiosFHReadSync(void *attr, SceFiosFH fh, void *buf, size_t nbytes) {
    (void)attr; (void)fh; (void)buf; (void)nbytes;
    return 0;
}

inline int sceFiosFHWriteSync(void *attr, SceFiosFH fh, const void *buf, size_t nbytes) {
    (void)attr; (void)fh; (void)buf; (void)nbytes;
    return 0;
}

inline int sceFiosFHCloseSync(void *attr, SceFiosFH fh) {
    (void)attr; (void)fh;
    return 0;
}

inline long long sceFiosFHSeek(SceFiosFH fh, long long offset, int whence) {
    (void)fh; (void)offset; (void)whence;
    return 0;
}

inline long long sceFiosFHGetSize(SceFiosFH fh) {
    (void)fh;
    return 0;
}
