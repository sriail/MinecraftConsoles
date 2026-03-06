/*
 * shim_SceSysmodule.cpp
 * PS Vita SceSysmodule → WASM stub implementation
 *
 * All dynamic system module loading/unloading calls are no-ops in the WASM
 * build because the relevant functionality is either statically compiled in
 * (libpthread, Emscripten FS, etc.) or entirely absent (NP, trophies).
 *
 * Every call logs a [VITA_SHIM_STUB] trace to stderr and returns SCE_OK to
 * prevent the game engine from entering fatal error paths.
 */

#include "shim_SceSysmodule.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Tracking table so isLoaded() returns consistent answers */
#define MAX_MODULE_ID 0x00FF

static uint8_t s_loaded[(MAX_MODULE_ID + 1 + 7) / 8]; /* bit-array */

static void set_loaded(uint16_t id, int val)
{
    if (id > MAX_MODULE_ID) return;
    if (val) s_loaded[id / 8] |=  (uint8_t)(1u << (id % 8));
    else     s_loaded[id / 8] &= (uint8_t)~(1u << (id % 8));
}

static int is_loaded(uint16_t id)
{
    if (id > MAX_MODULE_ID) return 0;
    return (s_loaded[id / 8] >> (id % 8)) & 1;
}

/* =========================================================================
 * sceSysmoduleLoadModule
 * VITA:  Dynamically loads a system module into the process
 * WEB:   [VITA_SHIM_STUB] — always succeeds (module is statically present
 *        or irrelevant in the WASM build)
 * ========================================================================= */

int sceSysmoduleLoadModule(uint16_t moduleId)
{
    fprintf(stderr,
        "[VITA_SHIM_STUB] sceSysmoduleLoadModule(0x%04X): module loading "
        "is a no-op in WASM; returning SCE_OK\n", moduleId);
    set_loaded(moduleId, 1);
    return SCE_OK;
}

/* =========================================================================
 * sceSysmoduleUnloadModule
 * VITA:  Unloads a previously loaded system module
 * WEB:   [VITA_SHIM_STUB] — always succeeds
 * ========================================================================= */

int sceSysmoduleUnloadModule(uint16_t moduleId)
{
    fprintf(stderr,
        "[VITA_SHIM_STUB] sceSysmoduleUnloadModule(0x%04X): module unloading "
        "is a no-op in WASM; returning SCE_OK\n", moduleId);
    set_loaded(moduleId, 0);
    return SCE_OK;
}

/* =========================================================================
 * sceSysmoduleIsLoaded
 * VITA:  Returns SCE_OK if the module is loaded, an error code otherwise
 * WEB:   Returns SCE_OK for any ID that was "loaded" via sceSysmoduleLoadModule
 * ========================================================================= */

int sceSysmoduleIsLoaded(uint16_t moduleId)
{
    return is_loaded(moduleId) ? SCE_OK : SCE_SYSMODULE_ERROR_UNLOADED;
}
