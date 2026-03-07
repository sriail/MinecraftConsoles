#pragma once
// Minimal PS Vita libsysmodule.h stub for Emscripten/WASM builds.
#include <stdint.h>
typedef uint16_t SceSysmoduleModuleId;
#define SCE_SYSMODULE_NET              0x0001
#define SCE_SYSMODULE_HTTP             0x0002
#define SCE_SYSMODULE_SSL              0x0003
#define SCE_SYSMODULE_HTTPS            0x0004
#define SCE_SYSMODULE_NP_MANAGER       0x0011
#define SCE_SYSMODULE_NP_BASIC         0x0012
#define SCE_SYSMODULE_NP_SCORE         0x0013
#define SCE_SYSMODULE_NP_TUS           0x0014
#define SCE_SYSMODULE_NP_MATCHING2     0x0024
#define SCE_SYSMODULE_RUDP             0x0017
#define SCE_SYSMODULE_VOICE            0x0018
#define SCE_SYSMODULE_APP_UTIL         0x001B
#define SCE_SYSMODULE_FACE             0x001C
#define SCE_SYSMODULE_MESSAGE_DIALOG   0x0019
#define SCE_SYSMODULE_IME_DIALOG       0x001A
#define SCE_SYSMODULE_NETCHECK_DIALOG  0x003C
#ifdef __cplusplus
extern "C" {
#endif
int sceSysmoduleLoadModule(SceSysmoduleModuleId moduleId);
int sceSysmoduleUnloadModule(SceSysmoduleModuleId moduleId);
int sceSysmoduleIsLoaded(SceSysmoduleModuleId moduleId);
#ifdef __cplusplus
}
#endif
