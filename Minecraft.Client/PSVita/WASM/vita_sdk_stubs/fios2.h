#pragma once
// Minimal PS Vita fios2.h stub for Emscripten/WASM builds.
// Provides the types, macros and function signatures used by this codebase.
// Actual FIOS2 I/O is replaced by POSIX file operations at link time.

#include <stdint.h>
#include <stddef.h>

typedef int32_t  SceFiosFH;     // file handle
typedef int64_t  SceFiosDH;     // directory handle
typedef int64_t  SceFiosSize;   // byte count / return value
typedef int64_t  SceFiosOffset; // file offset

typedef enum SceFiosWhence {
    SCE_FIOS_SEEK_SET = 0,
    SCE_FIOS_SEEK_CUR = 1,
    SCE_FIOS_SEEK_END = 2
} SceFiosWhence;

typedef struct SceFiosStat {
    SceFiosOffset fileSize;
    int64_t       accessDate;
    int64_t       modifiedDate;
    int64_t       createdDate;
    uint32_t      statFlags;
    uint32_t      _reserved;
} SceFiosStat;

typedef struct SceFiosOpStorage    { void *pPtr; size_t length; } SceFiosOpStorage;
typedef struct SceFiosChunkStorage { void *pPtr; size_t length; } SceFiosChunkStorage;
typedef struct SceFiosFHStorage    { void *pPtr; size_t length; } SceFiosFHStorage;
typedef struct SceFiosDHStorage    { void *pPtr; size_t length; } SceFiosDHStorage;

typedef struct SceFiosParams {
    uint32_t          version;
    uint32_t          _reserved;
    SceFiosOpStorage    opStorage;
    SceFiosChunkStorage chunkStorage;
    SceFiosFHStorage    fhStorage;
    SceFiosDHStorage    dhStorage;
    uint32_t            pathMax;
    void *(*pMemcpy)(void *, const void *, size_t);
} SceFiosParams;

// Storage size helper macros (return a safe overestimate for WASM).
#define SCE_FIOS_OP_STORAGE_SIZE(nOps, pathMax)     \
    (sizeof(void*) * 16 * (nOps) + (pathMax) * (nOps))
#define SCE_FIOS_CHUNK_STORAGE_SIZE(nChunks)         \
    (sizeof(void*) * 8 * (nChunks))
#define SCE_FIOS_FH_STORAGE_SIZE(nFH, pathMax)      \
    (sizeof(void*) * 8 * (nFH) + (pathMax) * (nFH))
#define SCE_FIOS_DH_STORAGE_SIZE(nDH, pathMax)      \
    (sizeof(void*) * 8 * (nDH) + (pathMax) * (nDH))

#define SCE_FIOS_PARAMS_INITIALIZER { 1, 0, {0,0}, {0,0}, {0,0}, {0,0}, 0, 0 }

#define SCE_FIOS_OK    0
#define SCE_FIOS_ERROR_INVALID_ARGUMENT (-1)

typedef struct SceFiosOpenParams {
    uint32_t openFlags;
    uint32_t _reserved;
} SceFiosOpenParams;

#ifdef __cplusplus
extern "C" {
#endif

int         sceFiosInitialize(const SceFiosParams *pParams);
void        sceFiosTerminate(void);
int         sceFiosFHOpenSync(void *pAttr, SceFiosFH *pOutFH,
                              const char *pPath,
                              const SceFiosOpenParams *pOpenParams);
void        sceFiosFHCloseSync(void *pAttr, SceFiosFH fh);
SceFiosSize sceFiosFHReadSync(void *pAttr, SceFiosFH fh,
                              void *pBuf, SceFiosSize length);
SceFiosSize sceFiosFileWriteSync(void *pAttr, const char *pPath,
                                  const void *pBuf, SceFiosSize length,
                                  uint32_t writeFlags);
int         sceFiosFHStatSync(void *pAttr, SceFiosFH fh, SceFiosStat *pStat);
int         sceFiosStatSync(void *pAttr, const char *pPath, SceFiosStat *pStat);
SceFiosOffset sceFiosFHSeek(SceFiosFH fh, int64_t offset, SceFiosWhence whence);
int         sceFiosMkdirSync(void *pAttr, const char *pPath);

#ifdef __cplusplus
}
#endif
