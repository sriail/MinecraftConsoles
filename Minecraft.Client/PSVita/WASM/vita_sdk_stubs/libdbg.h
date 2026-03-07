#pragma once
/* libdbg.h — PS Vita debug library stubs for Emscripten/WASM builds */

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCE_DBG_LOG(...)         do {} while(0)
#define SCE_DBG_LOG_ERROR(...)   do {} while(0)
#define SCE_DBG_LOG_WARNING(...) do {} while(0)
#define SCE_DBG_LOG_INFO(...)    do {} while(0)

#ifndef SCE_BREAK
#define _SCE_BREAK() __builtin_trap()
#define SCE_BREAK()  _SCE_BREAK()
#endif

#define SCE_DBG_ASSERT(test) do { (void)sizeof((test)); } while(0)

#ifdef __cplusplus
}
#endif
