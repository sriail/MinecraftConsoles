#pragma once
/* voice.h — PS Vita voice chat stubs for Emscripten/WASM builds */

#include <stdint.h>
#include "kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCE_VOICE_SAMPLINGRATE_16000  16000
#define SCE_VOICE_SAMPLINGRATE_48000  48000

#define SCE_VOICE_MEMORY_CONTAINER_SIZE (256*1024)

#define SCE_VOICE_INVALID_PORT_ID       0xFFFFFFFF

/* Port types */
#define SCE_VOICE_PORTTYPE_IN_DEVICE    0
#define SCE_VOICE_PORTTYPE_IN_SOUND     1
#define SCE_VOICE_PORTTYPE_OUT_DEVICE   2
#define SCE_VOICE_PORTTYPE_OUT_SOUND    3
#define SCE_VOICE_PORTTYPE_OUT_VOICE    4

/* Port states */
#define SCE_VOICE_PORTSTATE_NULL        0
#define SCE_VOICE_PORTSTATE_IDLE        1
#define SCE_VOICE_PORTSTATE_READY       2
#define SCE_VOICE_PORTSTATE_BUFFERING   3
#define SCE_VOICE_PORTSTATE_RUNNING     4

/* Bit rate */
typedef int SceVoiceBitRate;
#define SCE_VOICE_BITRATE_3850    3850
#define SCE_VOICE_BITRATE_7300    7300

typedef int SceVoicePortState;

typedef struct {
    uint32_t size;
    uint32_t samplingRate;
    uint32_t frameSize;
    SceVoiceBitRate bitRate;
    uint32_t portType;
} SceVoicePortParam;

typedef struct {
    uint32_t size;
    uint32_t version;
    void    *memBlock;
    uint32_t memBlockSize;
    uint32_t numPorts;
} SceVoiceInitParam;

typedef struct {
    uint32_t size;
    uint32_t samplingRate;
} SceVoiceStartParam;

typedef struct {
    uint32_t      portType;
    uint32_t      numbuf;
    uint32_t      frameSize;
    uint32_t      samplingRate;
    SceVoicePortState state;
    uint32_t      bitRate;
} SceVoiceBasePortInfo;

typedef struct {
    uint32_t size;
    uint32_t data;
} SceVoiceEventData;

static inline int sceVoiceInit(const SceVoiceInitParam *pInitParam, int mode)
{ (void)pInitParam; (void)mode; return SCE_OK; }

static inline int sceVoiceEnd(void) { return SCE_OK; }

static inline int sceVoiceStart(const SceVoiceStartParam *pStartParam)
{ (void)pStartParam; return SCE_OK; }

static inline int sceVoiceStop(void) { return SCE_OK; }

static inline int sceVoiceCreatePort(uint32_t *pPortId,
    const SceVoicePortParam *pParam)
{
    (void)pParam;
    if (pPortId) *pPortId = SCE_VOICE_INVALID_PORT_ID;
    return SCE_OK;
}

static inline int sceVoiceDeletePort(uint32_t portId)
{ (void)portId; return SCE_OK; }

static inline int sceVoiceConnectIPortToOPort(uint32_t iPortId, uint32_t oPortId)
{ (void)iPortId; (void)oPortId; return SCE_OK; }

static inline int sceVoiceDisconnectIPortFromOPort(uint32_t iPortId, uint32_t oPortId)
{ (void)iPortId; (void)oPortId; return SCE_OK; }

static inline int sceVoiceWriteToIPort(uint32_t portId, const void *data,
    uint32_t *pSize, int16_t volume)
{ (void)portId; (void)data; (void)pSize; (void)volume; return SCE_OK; }

static inline int sceVoiceReadFromOPort(uint32_t portId, void *data, uint32_t *pSize)
{ (void)portId; (void)data; if (pSize) *pSize = 0; return SCE_OK; }

static inline int sceVoiceGetPortInfo(uint32_t portId, SceVoiceBasePortInfo *pInfo)
{
    (void)portId;
    if (pInfo) {
        pInfo->state = SCE_VOICE_PORTSTATE_IDLE;
        pInfo->frameSize = 0;
    }
    return SCE_OK;
}

#ifdef __cplusplus
}
#endif
