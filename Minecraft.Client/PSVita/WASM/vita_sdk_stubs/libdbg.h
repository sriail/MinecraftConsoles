#pragma once

/* libdbg.h stubs - debug utilities are no-ops in WASM builds */

#ifndef SCE_DBG_ASSERT
#define SCE_DBG_ASSERT(x) ((void)(x))
#endif

#ifndef SCE_DBG_LOG_ERROR
#define SCE_DBG_LOG_ERROR(...) ((void)0)
#endif

#ifndef SCE_DBG_LOG_WARNING
#define SCE_DBG_LOG_WARNING(...) ((void)0)
#endif

#ifndef SCE_DBG_LOG_INFO
#define SCE_DBG_LOG_INFO(...) ((void)0)
#endif
