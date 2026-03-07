#pragma once
/* wasm_platform_override.h
 * Force-included before ALL source files when building for Emscripten/WASM.
 * Defines every PS Vita / PSP2 SDK type and macro that the game codebase
 * references, preventing the real (proprietary) Sony headers from being needed.
 *
 * Injection: add -include Minecraft.Client/PSVita/WASM/wasm_platform_override.h
 * to the compiler flags in the Emscripten CMake build.
 */

/* ── Guard: only active in Emscripten/WASM builds ─────────────── */
#if defined(__EMSCRIPTEN__) || defined(__wasm__)

/* ── Ensure __PSVITA__ and __psp2__ are defined ─────────────── */
#ifndef __PSVITA__
#define __PSVITA__ 1
#endif
#ifndef __psp2__
#define __psp2__   1
#endif
#ifndef _PSVita
#define _PSVita    1
#endif

/* ── Standard C headers first ───────────────────────────────── */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <sys/stat.h>

/* ── Silence non-portable intrinsics ────────────────────────── */
/* ARM/x86 SIMD intrinsics are not available; let source files see
 * empty stubs rather than fail hard.  Real SIMD paths are guarded
 * by the platform defines above so they won't be compiled. */
#define _INCLUDED_IMM
#define _MM_HINT_T0 1
#define __arm_neon__
#define __mmintrin_h_included

/* ── Display stubs ───────────────────────────────────────────── */
#define sceDisplaySetFrameBuf(fb, sync) (0)
#define SCE_DISPLAY_SETBUF_NEXTFRAME    1
#define SCE_DISPLAY_SETBUF_IMMEDIATE    0

/* ── Debug macros ────────────────────────────────────────────── */
#ifndef SCE_DBG_LOG
#define SCE_DBG_LOG(...)         do {} while(0)
#define SCE_DBG_LOG_ERROR(...)   do {} while(0)
#define SCE_DBG_LOG_WARNING(...) do {} while(0)
#define SCE_DBG_LOG_INFO(...)    do {} while(0)
#endif

#ifndef SCE_BREAK
/* Note: _SCE_BREAK/_SCE_MACRO_BEGIN/_SCE_MACRO_END use reserved-style names to
 * match the existing PSVitaStubs.h convention; they are guarded by #ifndef so
 * PSVitaStubs.h definitions take precedence if that header is included first. */
#define _SCE_BREAK() __builtin_trap()
#define SCE_BREAK()  _SCE_BREAK()
#endif

#ifndef SCE_DBG_ASSERT
#define _SCE_MACRO_BEGIN  do {
#define _SCE_MACRO_END    } while(0)
#define SCE_DBG_ASSERT(test) _SCE_MACRO_BEGIN { (void)sizeof((test)); } _SCE_MACRO_END
#endif

/* ── PSVITA_STUBBED — logs missing stubs to stderr ──────────── */
#ifdef _CONTENT_PACKAGE
#define PSVITA_STUBBED { }
#else
#define PSVITA_STUBBED \
    { static int _bSet = 0; if(!_bSet){ _bSet=1; fprintf(stderr, \
      "[WASM stub] %s not implemented\n", __FUNCTION__); } }
#endif

/* ── Windows-style type definitions ───────────────────────────── */
/* Include PSVitaTypes.h for DWORD, ULONGLONG, HANDLE, FILETIME, etc.
 * PSVitaTypes.h uses #pragma once so it won't conflict when stdafx.h
 * includes it again later.                                           */
#include "../PSVitaExtras/PSVitaTypes.h"

/* ── Prevent inclusion of real Sony SDK headers ──────────────── */
/* The vita_sdk_stubs directory is prepended to the include path so
 * <kernel.h>, <gxm.h>, etc. resolve to the stub versions.         */

#endif /* __EMSCRIPTEN__ || __wasm__ */
