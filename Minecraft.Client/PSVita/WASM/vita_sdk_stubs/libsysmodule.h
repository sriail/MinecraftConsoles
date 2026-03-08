#pragma once

#define SCE_SYSMODULE_NET    0x0001
#define SCE_SYSMODULE_HTTP   0x0002
#define SCE_SYSMODULE_VOICE  0x0003

inline int sceSysmoduleLoadModule(int moduleId) {
    (void)moduleId;
    return 0;
}

inline int sceSysmoduleUnloadModule(int moduleId) {
    (void)moduleId;
    return 0;
}
