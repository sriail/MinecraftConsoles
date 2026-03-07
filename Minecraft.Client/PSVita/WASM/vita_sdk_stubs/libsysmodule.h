#pragma once
#include <stdint.h>

#define SCE_SYSMODULE_NET          0x0001
#define SCE_SYSMODULE_HTTP         0x0002
#define SCE_SYSMODULE_SSL          0x0003
#define SCE_SYSMODULE_NP           0x0004
#define SCE_SYSMODULE_NP_COMMERCE2 0x0005
#define SCE_SYSMODULE_NP_TOOLKIT   0x0009
#define SCE_SYSMODULE_RUDP         0x000E

#ifdef __cplusplus
extern "C" {
#endif

int sceSysmoduleLoadModule(uint16_t id);
int sceSysmoduleUnloadModule(uint16_t id);
int sceSysmoduleIsLoaded(uint16_t id);

#ifdef __cplusplus
}
#endif
