#pragma once
// Force-included by the Emscripten/WASM build (via CMake -include flag) to
// override PSVita-specific macros before any other headers are processed.

#include <stdio.h>

// Provide a no-op SCE_BREAK for WASM — no native debugger trap available.
#ifndef SCE_BREAK
#define SCE_BREAK() ((void)0)
#endif

// PSVita stub notification — silent on WASM since there is no SCE debugger.
#ifndef PSVITA_STUBBED
#define PSVITA_STUBBED \
    { static bool _bSet = false; if (!_bSet) { \
        printf("PSVITA_STUBBED: %s\n", __FUNCTION__); \
        _bSet = true; } }
#endif

// SCE_OK success code used throughout the codebase.
#ifndef SCE_OK
#define SCE_OK 0
#endif
