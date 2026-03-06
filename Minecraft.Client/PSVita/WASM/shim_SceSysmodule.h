#ifndef SHIM_SCESYSMODULE_H
#define SHIM_SCESYSMODULE_H

/*
 * shim_SceSysmodule.h
 * PS Vita SceSysmodule → WASM stub
 *
 * The Vita dynamically loads optional system modules (e.g. NP, RUDP, HTTP).
 * In the WASM build these modules are statically linked (or absent), so
 * load/unload calls are stubbed to return SCE_OK.
 *
 * ABI reference: vitasdk <sysmodule.h>
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- Return codes ---------- */
#ifndef SCE_OK
#define SCE_OK 0
#endif
#define SCE_SYSMODULE_ERROR_INVALID_VALUE  ((int)0x805A1000)
#define SCE_SYSMODULE_ERROR_UNLOADED       ((int)0x805A1001)
#define SCE_SYSMODULE_ERROR_FATAL          ((int)0x805A10FF)

/* ---------- Module IDs (vitasdk subset used by Minecraft Vita) ---------- */
#define SCE_SYSMODULE_NET             0x0001
#define SCE_SYSMODULE_HTTP            0x0002
#define SCE_SYSMODULE_SSL             0x0005
#define SCE_SYSMODULE_HTTPS           0x0006
#define SCE_SYSMODULE_NP              0x0018
#define SCE_SYSMODULE_NP_MANAGER      0x0019
#define SCE_SYSMODULE_NP_BASIC        0x001A
#define SCE_SYSMODULE_NP_COMMERCE2    0x001B
#define SCE_SYSMODULE_NP_UTILITY      0x001D
#define SCE_SYSMODULE_RUDP            0x0026
#define SCE_SYSMODULE_VOICE           0x002C
#define SCE_SYSMODULE_AUDIOOUT        0x0030
#define SCE_SYSMODULE_APPUTIL         0x0036
#define SCE_SYSMODULE_FIOS2           0x0038
#define SCE_SYSMODULE_IME             0x0050
#define SCE_SYSMODULE_AVPLAYER        0x005C
#define SCE_SYSMODULE_JSON            0x0090

/* ---------- API ---------- */
int sceSysmoduleLoadModule(uint16_t moduleId);
int sceSysmoduleUnloadModule(uint16_t moduleId);
int sceSysmoduleIsLoaded(uint16_t moduleId);

#ifdef __cplusplus
}
#endif

#endif /* SHIM_SCESYSMODULE_H */
