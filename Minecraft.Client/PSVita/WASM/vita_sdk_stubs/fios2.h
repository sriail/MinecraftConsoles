#pragma once
#include <stdint.h>
#include <stddef.h>
#include <time.h>

#define SCE_FIOS_PATH_MAX 1024

typedef int32_t SceFiosFH;
typedef int32_t SceFiosDH;
typedef int64_t SceFiosOffset;
typedef int64_t SceFiosSize;
typedef int32_t SceFiosOp;
typedef int     SceFiosWhence;
typedef int64_t SceFiosDate;

typedef struct SceFiosBuffer {
    void    *pPtr;
    size_t   length;
} SceFiosBuffer;

typedef struct SceFiosParams {
    uint32_t initialized;
    uint32_t paramsSize;
    void    *pMemcpy;
    int32_t  threadPriority[3];
    int32_t  threadAffinity[3];
    int32_t  threadStackSize;
} SceFiosParams;

typedef struct SceFiosStat {
    SceFiosOffset fileSize;
    SceFiosDate   accessDate;
    SceFiosDate   modificationDate;
    SceFiosDate   creationDate;
    uint32_t      statFlags;
    uint32_t      reserved;
    int64_t       uid;
} SceFiosStat;

typedef struct SceFiosDirEntry {
    SceFiosOffset fileSize;
    uint32_t      statFlags;
    uint16_t      nameLength;
    uint16_t      fullPathLength;
    char          fullPath[SCE_FIOS_PATH_MAX];
    char         *nativePath;
} SceFiosDirEntry;
