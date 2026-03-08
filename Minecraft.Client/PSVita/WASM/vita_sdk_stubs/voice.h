#pragma once

typedef int SceVoicePortId;
typedef int SceVoicePort;

typedef struct SceVoiceInitParam {
    int appType;
    int maxPlayers;
} SceVoiceInitParam;

typedef struct SceVoicePortParam {
    int portType;
    int threshold;
} SceVoicePortParam;

#define SCE_VOICE_PORT_TYPE_IN_DEVICE  0
#define SCE_VOICE_PORT_TYPE_OUT_DEVICE 1
#define SCE_VOICE_PORT_TYPE_IN_PCMDATA 2
#define SCE_VOICE_PORT_TYPE_OUT_PCMDATA 3

inline int sceVoiceInit(SceVoiceInitParam *param) {
    (void)param;
    return 0;
}

inline int sceVoiceEnd(void) {
    return 0;
}

inline int sceVoiceStart(void) {
    return 0;
}

inline int sceVoiceStop(void) {
    return 0;
}

inline int sceVoiceCreatePort(SceVoicePortId *portId, const SceVoicePortParam *param) {
    (void)portId; (void)param;
    return 0;
}

inline int sceVoiceDeletePort(SceVoicePortId portId) {
    (void)portId;
    return 0;
}

inline int sceVoiceConnectIPortToOPort(SceVoicePortId inPortId, SceVoicePortId outPortId) {
    (void)inPortId; (void)outPortId;
    return 0;
}

inline int sceVoiceWriteToIPort(SceVoicePortId portId, const void *data, unsigned int *size) {
    (void)portId; (void)data; (void)size;
    return 0;
}

inline int sceVoiceReadFromOPort(SceVoicePortId portId, void *data, unsigned int *size) {
    (void)portId; (void)data; (void)size;
    return 0;
}
