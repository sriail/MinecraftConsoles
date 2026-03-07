#pragma once
// Minimal PS Vita libdbg.h stub for Emscripten/WASM builds.
// SCE_BREAK and SCE_DBG_ASSERT are defined in wasm_platform_override.h.
#include <stdio.h>
#ifndef SCE_BREAK
#define SCE_BREAK() ((void)0)
#endif
#define SCE_DBG_LOG_LEVEL_TRACE 0
#define SCE_DBG_LOG_LEVEL_DEBUG 1
#define SCE_DBG_LOG_LEVEL_INFO  2
#define SCE_DBG_LOG_LEVEL_WARNING 3
#define SCE_DBG_LOG_LEVEL_ERROR 4
#define SCE_DBG_LOG_LEVEL_ALWAYS 5
#define SCE_DBG_LOG(level, ...) ((void)0)
#define SCE_DBG_LOG_TRACE(...) ((void)0)
#define SCE_DBG_LOG_DEBUG(...) ((void)0)
#define SCE_DBG_LOG_INFO(...)  ((void)0)
#define SCE_DBG_LOG_WARNING(...) ((void)0)
#define SCE_DBG_LOG_ERROR(...) ((void)0)
