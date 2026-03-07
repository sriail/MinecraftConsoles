#pragma once
#include <stdint.h>

typedef int32_t SceVoicePortId;
typedef int SceVoiceBitRate;

typedef struct SceVoiceInitParam {
    int32_t appType;
} SceVoiceInitParam;

typedef struct SceVoiceStartParam {
    int32_t container;
} SceVoiceStartParam;

typedef struct SceVoiceBasePortInfo {
    int32_t portType;
    int32_t state;
    uint32_t *pEdge;
    uint32_t numByte;
    uint32_t numFrame;
    uint32_t totalByte;
} SceVoiceBasePortInfo;

typedef struct SceVoicePortParam {
    int32_t portType;
    uint16_t threshold;
    uint16_t volume;
    SceVoiceBitRate bitRate;
    void *pAttr;
} SceVoicePortParam;

typedef int SceVoicePortState;
typedef struct SceVoiceEventData { int dummy; } SceVoiceEventData;
typedef struct SceUserVoiceEvent { int dummy; } SceUserVoiceEvent;
