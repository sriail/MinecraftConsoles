#pragma once
/* fios2.h — PS Vita FIOS2 file I/O API stubs for Emscripten/WASM builds */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── Types ───────────────────────────────────────────────────── */
typedef int64_t  SceFiosOffset;
typedef int64_t  SceFiosSize;
typedef int64_t  SceFiosFH;
typedef int64_t  SceFiosDH;

typedef enum {
    SCE_FIOS_SEEK_SET = 0,
    SCE_FIOS_SEEK_CUR = 1,
    SCE_FIOS_SEEK_END = 2
} SceFiosWhence;

#define SCE_FIOS_OK                  0
#define SCE_FIOS_STATUS_DIRECTORY    0x0001
#define SCE_FIOS_STATUS_FILE         0x0002

typedef struct {
    int64_t  fileSize;
    int64_t  accessDate;
    int64_t  modificationDate;
    int64_t  creationDate;
    uint32_t statFlags;
    uint32_t padding;
} SceFiosStat;

typedef struct {
    void        *pPtr;
    size_t       length;
} SceFiosBuffer;

typedef struct {
    SceFiosBuffer opStorage;
    SceFiosBuffer chunkStorage;
    SceFiosBuffer fhStorage;
    SceFiosBuffer dhStorage;
    size_t        pathMax;
    void         *(*pMemcpy)(void *, const void *, size_t);
} SceFiosParams;

#define SCE_FIOS_PARAMS_INITIALIZER { {NULL,0},{NULL,0},{NULL,0},{NULL,0}, 1024, NULL }

/* Storage size macros — return conservative sizes for static buffer allocation */
#define SCE_FIOS_OP_STORAGE_SIZE(numOps, pathMax) \
    ((size_t)(numOps) * (256 + (pathMax)))
#define SCE_FIOS_CHUNK_STORAGE_SIZE(numChunks) \
    ((size_t)(numChunks) * 64)
#define SCE_FIOS_FH_STORAGE_SIZE(numFH, pathMax) \
    ((size_t)(numFH) * (128 + (pathMax)))
#define SCE_FIOS_DH_STORAGE_SIZE(numDH, pathMax) \
    ((size_t)(numDH) * (128 + (pathMax)))

/* ── Init / term ─────────────────────────────────────────────── */
static inline int sceFiosInitialize(const SceFiosParams *pParams)
{ (void)pParams; return SCE_FIOS_OK; }

static inline int sceFiosTerminate(void) { return SCE_FIOS_OK; }

/* ── File operations ─────────────────────────────────────────── */
static inline int sceFiosFHOpenSync(void *pAttr, SceFiosFH *pFH,
    const char *path, void *pOpenParams)
{
    (void)pAttr; (void)pOpenParams;
    FILE *f = fopen(path, "rb");
    if (!f) { *pFH = -1; return -1; }
    *pFH = (SceFiosFH)(intptr_t)f;
    return SCE_FIOS_OK;
}

static inline SceFiosSize sceFiosFHReadSync(void *pAttr, SceFiosFH fh,
    void *pBuf, SceFiosSize len)
{
    (void)pAttr;
    FILE *f = (FILE *)(intptr_t)fh;
    if (!f) return -1;
    return (SceFiosSize)fread(pBuf, 1, (size_t)len, f);
}

static inline SceFiosOffset sceFiosFHSeek(SceFiosFH fh,
    SceFiosOffset offset, SceFiosWhence whence)
{
    FILE *f = (FILE *)(intptr_t)fh;
    if (!f) return -1;
    fseek(f, (long)offset, (int)whence);
    return (SceFiosOffset)ftell(f);
}

static inline int sceFiosFHCloseSync(void *pAttr, SceFiosFH fh)
{
    (void)pAttr;
    FILE *f = (FILE *)(intptr_t)fh;
    if (f) fclose(f);
    return SCE_FIOS_OK;
}

static inline SceFiosSize sceFiosFileWriteSync(void *pAttr, const char *path,
    const void *pBuf, SceFiosSize len, int64_t offset)
{
    (void)pAttr; (void)offset;
    FILE *f = fopen(path, "wb");
    if (!f) return -1;
    SceFiosSize written = (SceFiosSize)fwrite(pBuf, 1, (size_t)len, f);
    fclose(f);
    return written;
}

static inline int sceFiosStatSync(void *pAttr, const char *path, SceFiosStat *pStat)
{
    (void)pAttr;
    struct stat st;
    if (stat(path, &st) != 0) return -1;
    if (pStat) {
        pStat->fileSize = (int64_t)st.st_size;
        pStat->statFlags = S_ISDIR(st.st_mode) ? SCE_FIOS_STATUS_DIRECTORY
                                                : SCE_FIOS_STATUS_FILE;
    }
    return SCE_FIOS_OK;
}

static inline int sceFiosDHOpenSync(void *pAttr, SceFiosDH *pDH,
    const char *path, void *pOpenParams)
{
    (void)pAttr; (void)path; (void)pOpenParams;
    *pDH = 1; return SCE_FIOS_OK;
}

static inline int sceFiosDHReadSync(void *pAttr, SceFiosDH dh,
    SceFiosStat *pStat)
{
    (void)pAttr; (void)dh; (void)pStat;
    return -1; /* no entries */
}

static inline int sceFiosDHCloseSync(void *pAttr, SceFiosDH dh)
{ (void)pAttr; (void)dh; return SCE_FIOS_OK; }

/* sceIoMkdir — thin POSIX wrapper */
static inline int sceIoMkdir(const char *path, int mode)
{
    (void)mode;
#if defined(_WIN32)
    return mkdir(path) == 0 ? 0 : -1;
#else
    return mkdir(path, 0755) == 0 ? 0 : -1;
#endif
}

#ifdef __cplusplus
}
#endif
