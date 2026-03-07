#ifndef SHIM_SCEIOFMGR_H
#define SHIM_SCEIOFMGR_H

/*
 * shim_SceIoFileMgr.h
 * PS Vita SceIoFileMgr → Emscripten IDBFS / MEMFS shim
 *
 * Translates Vita file system paths (ux0:, app0:, savedata0:, etc.) into
 * WASM Emscripten virtual filesystem paths backed by IndexedDB (IDBFS).
 *
 * Path mapping:
 *   app0:/      → /app/         (read-only game data)
 *   ux0:data/   → /ux0/data/    (user data, writable)
 *   savedata0:/ → /savedata/    (save game data, IDBFS)
 *   cache0:/    → /cache/       (temporary cache, MEMFS)
 *   ur0:/       → /ur0/         (system resources, read-only stub)
 *
 * ABI reference: vitasdk <iofilemgr.h>
 */

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- Return codes ---------- */
#ifndef SCE_OK
#define SCE_OK 0
#endif
#define SCE_ERROR_ERRNO_ENOENT   ((int)0x80010002)
#define SCE_ERROR_ERRNO_EEXIST   ((int)0x80010011)
#define SCE_ERROR_ERRNO_EACCES   ((int)0x8001000D)
#define SCE_ERROR_ERRNO_EINVAL   ((int)0x80010016)
#define SCE_ERROR_ERRNO_ENOSPC   ((int)0x8001001C)
#define SCE_FIOS_OK              0

/* ---------- Open flags ---------- */
#define SCE_O_RDONLY   0x0001
#define SCE_O_WRONLY   0x0002
#define SCE_O_RDWR     0x0003
#define SCE_O_CREAT    0x0200
#define SCE_O_TRUNC    0x0400
#define SCE_O_APPEND   0x0100

/* ---------- Seek modes ---------- */
#define SCE_SEEK_SET   0
#define SCE_SEEK_CUR   1
#define SCE_SEEK_END   2

/* ---------- Stat struct ---------- */
typedef struct SceDateTime {
    uint16_t year, month, day, hour, minute, second;
    uint32_t microsecond;
} SceDateTime;

typedef struct SceIoStat {
    int      st_mode;
    uint32_t st_attr;
    int64_t  st_size;
    SceDateTime st_ctime, st_atime, st_mtime;
    uint32_t st_private[6];
} SceIoStat;

#define SCE_S_ISDIR(m)  (((m) & 0xF000) == 0x1000)
#define SCE_S_ISREG(m)  (((m) & 0xF000) == 0x2000)
#define SCE_S_IFDIR     0x1000
#define SCE_S_IFREG     0x2000

/* ---------- Directory entry ---------- */
#define SCE_FIOS_PATH_MAX 260

typedef struct SceIoDirent {
    SceIoStat d_stat;
    char      d_name[SCE_FIOS_PATH_MAX];
    void     *d_private;
    int       dummy;
} SceIoDirent;

/* ---------- API ---------- */
int  sceIoOpen(const char *path, int flags, int mode);
int  sceIoClose(int fd);
int  sceIoRead(int fd, void *buf, size_t nbytes);
int  sceIoWrite(int fd, const void *buf, size_t nbytes);
int64_t sceIoLseek(int fd, int64_t offset, int whence);
int  sceIoRemove(const char *path);
int  sceIoRename(const char *oldpath, const char *newpath);
int  sceIoMkdir(const char *path, int mode);
int  sceIoRmdir(const char *path);
int  sceIoGetstat(const char *path, SceIoStat *stat);
int  sceIoDopen(const char *path);
int  sceIoDread(int dd, SceIoDirent *dir);
int  sceIoDclose(int dd);

#ifdef __cplusplus
}
#endif

#endif /* SHIM_SCEIOFMGR_H */
