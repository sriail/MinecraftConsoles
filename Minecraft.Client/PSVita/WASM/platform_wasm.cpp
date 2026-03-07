/*
 * platform_wasm.cpp
 *
 * Minimal runtime implementations for PS Vita platform functions
 * when building for Emscripten / WebAssembly.
 *
 * All functions that the game calls at runtime (via PsVitaStubs.cpp,
 * PSVita_App.cpp, etc.) and that cannot be satisfied purely by the
 * inline stubs in the vita_sdk_stubs headers go here.
 *
 * This file is added to the Emscripten build via CMakeLists.txt.
 */

#if defined(__EMSCRIPTEN__) || defined(__wasm__)

#include "wasm_platform_override.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#ifdef __EMSCRIPTEN__
#  include <emscripten.h>
#  include <emscripten/html5.h>
#endif

/* ── Tick counter ────────────────────────────────────────────── */
extern "C" unsigned int GetTickCount(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned int)(tv.tv_sec * 1000u + tv.tv_usec / 1000u);
}

/* ── Performance counter ─────────────────────────────────────── */
extern "C" int QueryPerformanceCounter(long long *lpPerformanceCount)
{
    if (lpPerformanceCount) {
#ifdef __EMSCRIPTEN__
        *lpPerformanceCount = (long long)(emscripten_get_now() * 1000.0);
#else
        struct timeval tv;
        gettimeofday(&tv, NULL);
        *lpPerformanceCount = (long long)tv.tv_sec * 1000000LL + tv.tv_usec;
#endif
    }
    return 1;
}

extern "C" int QueryPerformanceFrequency(long long *lpFrequency)
{
    if (lpFrequency) *lpFrequency = 1000000LL; /* microseconds */
    return 1;
}

/* ── Output debug string ─────────────────────────────────────── */
extern "C" void OutputDebugStringA(const char *str)
{
    if (str) fputs(str, stderr);
}

extern "C" void OutputDebugString(const char *str)
{
    OutputDebugStringA(str);
}

extern "C" void OutputDebugStringW(const wchar_t *str)
{
    if (!str) return;
    /* Simple ASCII fallback */
    while (*str) {
        if (*str < 128) fputc((int)*str, stderr);
        else            fputc('?', stderr);
        ++str;
    }
}

/* ── Sleep ───────────────────────────────────────────────────── */
extern "C" void Sleep(unsigned int ms)
{
    /* emscripten_sleep requires ASYNCIFY; use a busy-wait
     * approximation when not available. */
    (void)ms;
}

/* ── GlobalMemoryStatus ──────────────────────────────────────── */
extern "C" {
    typedef struct {
        unsigned int dwLength;
        unsigned int dwMemoryLoad;
        size_t       dwTotalPhys;
        size_t       dwAvailPhys;
        size_t       dwTotalPageFile;
        size_t       dwAvailPageFile;
        size_t       dwTotalVirtual;
        size_t       dwAvailVirtual;
    } MEMORYSTATUS, *LPMEMORYSTATUS;

    void GlobalMemoryStatus(LPMEMORYSTATUS lpBuffer)
    {
        if (!lpBuffer) return;
        lpBuffer->dwLength        = sizeof(MEMORYSTATUS);
        lpBuffer->dwMemoryLoad    = 20;
        lpBuffer->dwTotalPhys     = 256ull * 1024 * 1024;
        lpBuffer->dwAvailPhys     = 200ull * 1024 * 1024;
        lpBuffer->dwTotalPageFile = 0;
        lpBuffer->dwAvailPageFile = 0;
        lpBuffer->dwTotalVirtual  = 2048ull * 1024 * 1024;
        lpBuffer->dwAvailVirtual  = 1024ull * 1024 * 1024;
    }
}

/* ── GetLastError ────────────────────────────────────────────── */
static unsigned int g_lastError = 0;

extern "C" unsigned int GetLastError(void) { return g_lastError; }

/* ── __debugbreak / DebugBreak ───────────────────────────────── */
extern "C" void __debugbreak(void) { __builtin_trap(); }
extern "C" void DebugBreak(void)   { __builtin_trap(); }

/* ── _itoa_s / _i64toa_s ─────────────────────────────────────── */
extern "C" int _itoa_s(int value, char *buf, size_t size, int radix)
{
    if (!buf || size == 0) return -1;
    snprintf(buf, size, radix == 16 ? "%x" : (radix == 8 ? "%o" : "%d"), value);
    return 0;
}

extern "C" int _i64toa_s(long long value, char *buf, size_t size, int radix)
{
    if (!buf || size == 0) return -1;
    snprintf(buf, size, radix == 16 ? "%llx" : (radix == 8 ? "%llo" : "%lld"), value);
    return 0;
}

/* ── GetModuleHandle ─────────────────────────────────────────── */
extern "C" void *GetModuleHandle(const char *name)
{ (void)name; return (void *)(intptr_t)1; }

/* ── GetSystemTime / GetLocalTime ────────────────────────────── */
extern "C" {
    typedef struct {
        unsigned short wYear, wMonth, wDayOfWeek, wDay;
        unsigned short wHour, wMinute, wSecond, wMilliseconds;
    } SYSTEMTIME, *LPSYSTEMTIME;

    void GetSystemTime(LPSYSTEMTIME st)
    {
        if (!st) return;
        time_t t = time(NULL);
        struct tm *tm = gmtime(&t);
        st->wYear         = (unsigned short)(tm->tm_year + 1900);
        st->wMonth        = (unsigned short)(tm->tm_mon  + 1);
        st->wDayOfWeek    = (unsigned short)(tm->tm_wday);
        st->wDay          = (unsigned short)(tm->tm_mday);
        st->wHour         = (unsigned short)(tm->tm_hour);
        st->wMinute       = (unsigned short)(tm->tm_min);
        st->wSecond       = (unsigned short)(tm->tm_sec);
        st->wMilliseconds = 0;
    }

    void GetLocalTime(LPSYSTEMTIME st) { GetSystemTime(st); }
}

/* ── XGetLanguage / XGetLocale ───────────────────────────────── */
extern "C" unsigned int XGetLanguage(void) { return 1; /* English */ }
extern "C" unsigned int XGetLocale(void)   { return 1; }
extern "C" unsigned int XEnableGuestSignin(int enable) { (void)enable; return 0; }

/* ── GetCurrentThreadId ──────────────────────────────────────── */
extern "C" unsigned int GetCurrentThreadId(void) { return 1; }

/* ── Thread stubs ────────────────────────────────────────────── */
extern "C" {
    void *CreateThread(void *, unsigned int, void *, void *, unsigned int,
        unsigned int *)
    { return (void *)(intptr_t)1; }

    unsigned int ResumeThread(void *) { return 0; }
    unsigned int GetExitCodeThread(void *, unsigned int *code)
    { if (code) *code = 0; return 1; }

    int SetThreadPriority(void *, int) { return 1; }

    unsigned int WaitForSingleObject(void *, unsigned int) { return 0; /* WAIT_OBJECT_0 */ }
    unsigned int WaitForMultipleObjects(unsigned int, void * const *, int, unsigned int)
    { return 0; }
    unsigned int WaitForMultipleObjectsEx(unsigned int, void * const *, int,
        unsigned int, int)
    { return 0; }

    int CloseHandle(void *) { return 1; }
    void *CreateEvent(void *, int, int, const char *) { return (void *)(intptr_t)1; }
    int  SetEvent(void *) { return 1; }
}

/* ── TLS ─────────────────────────────────────────────────────── */
/* Single-slot TLS using a static array (adequate for WASM single-thread) */
#define WASM_TLS_MAX_SLOTS 256

static void *g_tlsSlots[WASM_TLS_MAX_SLOTS];
static unsigned int g_tlsNextSlot = 0;

extern "C" unsigned int TlsAlloc(void)
{
    if (g_tlsNextSlot >= WASM_TLS_MAX_SLOTS) return (unsigned int)-1;
    unsigned int slot = g_tlsNextSlot++;
    g_tlsSlots[slot] = NULL;
    return slot;
}

extern "C" void *TlsGetValue(unsigned int idx)
{
    if (idx >= WASM_TLS_MAX_SLOTS) return NULL;
    return g_tlsSlots[idx];
}

extern "C" int TlsSetValue(unsigned int idx, void *val)
{
    if (idx >= WASM_TLS_MAX_SLOTS) return 0;
    g_tlsSlots[idx] = val;
    return 1;
}

/* ── Critical sections (single-threaded WASM — all no-ops) ───── */
extern "C" {
    void InitializeCriticalSection(void *)                          {}
    void InitializeCriticalSectionAndSpinCount(void *, unsigned int) {}
    void DeleteCriticalSection(void *)                              {}
    void EnterCriticalSection(void *)                               {}
    void LeaveCriticalSection(void *)                               {}
    unsigned int TryEnterCriticalSection(void *)                    { return 1; }

    void InitializeCriticalRWSection(void *)            {}
    void DeleteCriticalRWSection(void *)                {}
    void EnterCriticalRWSection(void *, int)            {}
    void LeaveCriticalRWSection(void *, int)            {}
}

/* ── Interlocked operations ──────────────────────────────────── */
extern "C" long InterlockedCompareExchangeRelease(
    long volatile *dest, long exch, long comp)
{
    long old = *dest;
    if (old == comp) *dest = exch;
    return old;
}

extern "C" long long InterlockedCompareExchangeRelease64(
    long long volatile *dest, long long exch, long long comp)
{
    long long old = *dest;
    if (old == comp) *dest = exch;
    return old;
}

/* ── Virtual memory (used by VirtualAlloc in PSVitaStubs) ──────
 * On WASM real virtual memory management is unavailable; use malloc/free. */
extern "C" {
    void *VirtualAlloc(void *addr, size_t size, unsigned int, unsigned int)
    { (void)addr; return calloc(1, size); }

    int VirtualFree(void *addr, size_t, unsigned int)
    { free(addr); return 1; }

    void VirtualCopyTo(void *dst, void *src, size_t n)   { memcpy(dst, src, n); }
    void VirtualCopyFrom(void *dst, void *src, size_t n) { memcpy(dst, src, n); }
    void VirtualMove(void *dst, void *src, size_t n)     { memmove(dst, src, n); }
    void VirtualMemset(void *dst, int val, size_t n)     { memset(dst, val, n); }

    void VirtualCompress(void *, unsigned int *, void *, size_t)   {}
    void VirtualDecompress(void *, size_t)                         {}

    int VirtualWriteFile(const char *name, const void *buf,
        unsigned int nBytes, unsigned int *written, void *)
    {
        FILE *f = fopen(name, "wb");
        if (!f) { if (written) *written = 0; return 0; }
        size_t w = fwrite(buf, 1, nBytes, f);
        fclose(f);
        if (written) *written = (unsigned int)w;
        return w == nBytes ? 1 : 0;
    }
}

/* ── File helpers (used by PSVitaStubs CreateFileA, etc.) ──────
 * These are implemented in PsVitaStubs.cpp for the real Vita;
 * on WASM we provide POSIX-based equivalents.                    */

/* wcsnlen / _wcsicmp */
extern "C" size_t wcsnlen(const wchar_t *s, size_t maxlen)
{
    size_t i;
    for (i = 0; i < maxlen && s[i]; ++i) {}
    return i;
}

/* PSVitaInit — FIOS2 init not needed on WASM */
extern "C" void PSVitaInit(void) {}

/* getUsrDirPath — return empty path; the game prepends asset paths */
static char g_usrDirPath[4] = "";
extern "C" char *getUsrDirPath(void) { return g_usrDirPath; }

#endif /* __EMSCRIPTEN__ || __wasm__ */
