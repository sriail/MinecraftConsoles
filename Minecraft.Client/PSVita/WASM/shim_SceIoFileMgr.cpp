/*
 * shim_SceIoFileMgr.cpp
 * PS Vita SceIoFileMgr → Emscripten POSIX FS shim implementation
 *
 * Build requirements:
 *   -s FORCE_FILESYSTEM=1
 *   -s EXTRA_EXPORTED_RUNTIME_METHODS='["FS","callMain"]'
 *   Mount IDBFS before game init:
 *     FS.mount(IDBFS, {}, '/savedata');
 *     FS.mount(IDBFS, {}, '/ux0');
 *     FS.syncfs(true, callback);   // load from IDB on startup
 *   After writes: FS.syncfs(false, callback);   // flush to IDB
 *
 * Path translation:
 *   app0:/foo    → /app/foo
 *   ux0:data/foo → /ux0/data/foo
 *   savedata0:/  → /savedata/
 *   cache0:/foo  → /cache/foo
 */

#include "shim_SceIoFileMgr.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

/* =========================================================================
 * Path translation
 * ========================================================================= */

#define TRANSLATED_MAX 512

static void translate_path(const char *vitaPath, char *out, size_t outLen)
{
    if (!vitaPath) { out[0] = '\0'; return; }

    static const struct { const char *prefix; const char *mapped; } map[] = {
        { "app0:",       "/app/"      },
        { "ux0:data/",   "/ux0/data/" },
        { "ux0:",        "/ux0/"      },
        { "savedata0:/", "/savedata/" },
        { "savedata0:",  "/savedata/" },
        { "cache0:",     "/cache/"    },
        { "ur0:",        "/ur0/"      },
        { NULL, NULL }
    };

    for (int i = 0; map[i].prefix; i++) {
        size_t plen = strlen(map[i].prefix);
        if (strncmp(vitaPath, map[i].prefix, plen) == 0) {
            snprintf(out, outLen, "%s%s", map[i].mapped, vitaPath + plen);
            return;
        }
    }
    /* Fallback: use path as-is */
    snprintf(out, outLen, "%s", vitaPath);
}

/* =========================================================================
 * SCE_ERROR from errno
 * ========================================================================= */

static int errno_to_sce(int e)
{
    switch (e) {
        case ENOENT:  return SCE_ERROR_ERRNO_ENOENT;
        case EEXIST:  return SCE_ERROR_ERRNO_EEXIST;
        case EACCES:  return SCE_ERROR_ERRNO_EACCES;
        case ENOSPC:  return SCE_ERROR_ERRNO_ENOSPC;
        case EINVAL:  return SCE_ERROR_ERRNO_EINVAL;
        default:      return SCE_ERROR_ERRNO_EINVAL;
    }
}

/* =========================================================================
 * Open flags translation
 * ========================================================================= */

static int sce_flags_to_posix(int sceFlags)
{
    int f = 0;
    switch (sceFlags & 0x03) {
        case SCE_O_RDONLY: f = O_RDONLY; break;
        case SCE_O_WRONLY: f = O_WRONLY; break;
        case SCE_O_RDWR:   f = O_RDWR;  break;
        default:           f = O_RDONLY; break;
    }
    if (sceFlags & SCE_O_CREAT)  f |= O_CREAT;
    if (sceFlags & SCE_O_TRUNC)  f |= O_TRUNC;
    if (sceFlags & SCE_O_APPEND) f |= O_APPEND;
    return f;
}

/* =========================================================================
 * sceIoOpen / sceIoClose / sceIoRead / sceIoWrite / sceIoLseek
 * VITA:  File open/close/read/write/seek using Vita paths
 * WEB:   Emscripten POSIX FS layer (IDBFS-backed for savedata/ux0)
 * ========================================================================= */

int sceIoOpen(const char *path, int flags, int mode)
{
    char real[TRANSLATED_MAX];
    translate_path(path, real, sizeof(real));
    int posixFlags = sce_flags_to_posix(flags);
    int fd = open(real, posixFlags, (mode_t)mode);
    if (fd < 0) return errno_to_sce(errno);
    return fd;
}

int sceIoClose(int fd)
{
    if (close(fd) != 0) return errno_to_sce(errno);
    return SCE_OK;
}

int sceIoRead(int fd, void *buf, size_t nbytes)
{
    ssize_t n = read(fd, buf, nbytes);
    if (n < 0) return errno_to_sce(errno);
    return (int)n;
}

int sceIoWrite(int fd, const void *buf, size_t nbytes)
{
    ssize_t n = write(fd, buf, nbytes);
    if (n < 0) return errno_to_sce(errno);
    return (int)n;
}

int64_t sceIoLseek(int fd, int64_t offset, int whence)
{
    int posixWhence;
    switch (whence) {
        case SCE_SEEK_SET: posixWhence = SEEK_SET; break;
        case SCE_SEEK_CUR: posixWhence = SEEK_CUR; break;
        case SCE_SEEK_END: posixWhence = SEEK_END; break;
        default: return (int64_t)SCE_ERROR_ERRNO_EINVAL;
    }
    off_t r = lseek(fd, (off_t)offset, posixWhence);
    if (r < 0) return (int64_t)errno_to_sce(errno);
    return (int64_t)r;
}

/* =========================================================================
 * sceIoRemove / sceIoRename / sceIoMkdir / sceIoRmdir
 * VITA:  File/directory management
 * WEB:   POSIX equivalents via Emscripten FS
 * ========================================================================= */

int sceIoRemove(const char *path)
{
    char real[TRANSLATED_MAX];
    translate_path(path, real, sizeof(real));
    if (unlink(real) != 0) return errno_to_sce(errno);
    return SCE_OK;
}

int sceIoRename(const char *oldpath, const char *newpath)
{
    char ro[TRANSLATED_MAX], rn[TRANSLATED_MAX];
    translate_path(oldpath, ro, sizeof(ro));
    translate_path(newpath, rn, sizeof(rn));
    if (rename(ro, rn) != 0) return errno_to_sce(errno);
    return SCE_OK;
}

int sceIoMkdir(const char *path, int mode)
{
    char real[TRANSLATED_MAX];
    translate_path(path, real, sizeof(real));
    if (mkdir(real, (mode_t)mode) != 0 && errno != EEXIST)
        return errno_to_sce(errno);
    return SCE_OK;
}

int sceIoRmdir(const char *path)
{
    char real[TRANSLATED_MAX];
    translate_path(path, real, sizeof(real));
    if (rmdir(real) != 0) return errno_to_sce(errno);
    return SCE_OK;
}

/* =========================================================================
 * sceIoGetstat
 * VITA:  Retrieves file/directory metadata
 * WEB:   POSIX stat()
 * ========================================================================= */

static void fill_sce_datetime(SceDateTime *dt, time_t t)
{
    struct tm *tm = gmtime(&t);
    if (!tm) { memset(dt, 0, sizeof(*dt)); return; }
    dt->year   = (uint16_t)(tm->tm_year + 1900);
    dt->month  = (uint16_t)(tm->tm_mon + 1);
    dt->day    = (uint16_t)tm->tm_mday;
    dt->hour   = (uint16_t)tm->tm_hour;
    dt->minute = (uint16_t)tm->tm_min;
    dt->second = (uint16_t)tm->tm_sec;
    dt->microsecond = 0;
}

int sceIoGetstat(const char *path, SceIoStat *stat_out)
{
    if (!path || !stat_out) return SCE_ERROR_ERRNO_EINVAL;
    char real[TRANSLATED_MAX];
    translate_path(path, real, sizeof(real));

    struct stat s;
    if (stat(real, &s) != 0) return errno_to_sce(errno);

    memset(stat_out, 0, sizeof(*stat_out));
    stat_out->st_size = (int64_t)s.st_size;
    if (S_ISDIR(s.st_mode)) {
        stat_out->st_mode = SCE_S_IFDIR;
        stat_out->st_attr = 0x10; /* directory attribute */
    } else {
        stat_out->st_mode = SCE_S_IFREG;
        stat_out->st_attr = 0x20;
    }
    fill_sce_datetime(&stat_out->st_ctime, s.st_ctime);
    fill_sce_datetime(&stat_out->st_atime, s.st_atime);
    fill_sce_datetime(&stat_out->st_mtime, s.st_mtime);
    return SCE_OK;
}

/* =========================================================================
 * Directory iteration
 * VITA:  sceIoDopen / sceIoDread / sceIoDclose
 * WEB:   POSIX opendir / readdir / closedir
 * NOTE:  We use a handle table (not pointer casts) for portability.
 * ========================================================================= */

#define MAX_DHANDLES 32

static DIR *s_dhandles[MAX_DHANDLES];

static int alloc_dhandle(DIR *d)
{
    for (int i = 0; i < MAX_DHANDLES; i++) {
        if (!s_dhandles[i]) {
            s_dhandles[i] = d;
            return i + 1; /* 1-based handle */
        }
    }
    return -1;
}

static DIR *get_dhandle(int dh)
{
    if (dh < 1 || dh > MAX_DHANDLES) return NULL;
    return s_dhandles[dh - 1];
}

static void free_dhandle(int dh)
{
    if (dh >= 1 && dh <= MAX_DHANDLES)
        s_dhandles[dh - 1] = NULL;
}

int sceIoDopen(const char *path)
{
    if (!path) return SCE_ERROR_ERRNO_EINVAL;
    char real[TRANSLATED_MAX];
    translate_path(path, real, sizeof(real));
    DIR *d = opendir(real);
    if (!d) return errno_to_sce(errno);
    int dh = alloc_dhandle(d);
    if (dh < 0) { closedir(d); return SCE_ERROR_ERRNO_EINVAL; }
    return dh;
}

int sceIoDread(int dd, SceIoDirent *dir)
{
    if (!dir) return SCE_ERROR_ERRNO_EINVAL;
    DIR *d = get_dhandle(dd);
    if (!d) return SCE_ERROR_ERRNO_EINVAL;
    errno = 0;
    struct dirent *de = readdir(d);
    if (!de) return (errno == 0) ? 0 : errno_to_sce(errno);

    memset(dir, 0, sizeof(*dir));
    strncpy(dir->d_name, de->d_name, SCE_FIOS_PATH_MAX - 1);
    dir->d_stat.st_mode = (de->d_type == DT_DIR) ? SCE_S_IFDIR : SCE_S_IFREG;
    return 1; /* 1 = entry returned */
}

int sceIoDclose(int dd)
{
    DIR *d = get_dhandle(dd);
    if (!d) return SCE_ERROR_ERRNO_EINVAL;
    closedir(d);
    free_dhandle(dd);
    return SCE_OK;
}
