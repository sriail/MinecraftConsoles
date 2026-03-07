#pragma once
/* libsysmodule.h — PS Vita system module loading stubs for Emscripten/WASM builds */

#include "kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

/* System module IDs */
#define SCE_SYSMODULE_NET              0x0001
#define SCE_SYSMODULE_HTTP             0x0002
#define SCE_SYSMODULE_SSL              0x0005
#define SCE_SYSMODULE_HTTPS            0x0006
#define SCE_SYSMODULE_PERF             0x000B
#define SCE_SYSMODULE_FIBER            0x000C
#define SCE_SYSMODULE_ULT              0x001A
#define SCE_SYSMODULE_SAS              0x0001
#define SCE_SYSMODULE_APPUTIL          0x001B
#define SCE_SYSMODULE_FACE_TRACKER     0x001E
#define SCE_SYSMODULE_RUDP             0x0022
#define SCE_SYSMODULE_NETCHECK_DIALOG  0x0027
#define SCE_SYSMODULE_NP_TOOLKIT       0x0028
#define SCE_SYSMODULE_NP               0x0029
#define SCE_SYSMODULE_VOICE            0x002E
#define SCE_SYSMODULE_NP_BASIC         0x0030
#define SCE_SYSMODULE_NP_TROPHY        0x003F
#define SCE_SYSMODULE_NP_MESSAGE       0x0043
#define SCE_SYSMODULE_NP_SCORE         0x0044
#define SCE_SYSMODULE_NP_TUS           0x0045
#define SCE_SYSMODULE_AVPLAYER         0x0058
#define SCE_SYSMODULE_GAME_CUSTOM_DATA_DIALOG 0x004D
#define SCE_SYSMODULE_SAVE_DATA_DIALOG 0x0032

static inline int sceSysmoduleLoadModule(unsigned int id)
{ (void)id; return SCE_OK; }

static inline int sceSysmoduleUnloadModule(unsigned int id)
{ (void)id; return SCE_OK; }

static inline int sceSysmoduleIsLoaded(unsigned int id)
{ (void)id; return SCE_OK; }

#ifdef __cplusplus
}
#endif
