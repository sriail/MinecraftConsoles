#pragma once
// Minimal PS Vita voice.h stub for Emscripten/WASM builds.
#include <stdint.h>
typedef struct SceVoicePort { int id; } SceVoicePort;
typedef struct SceVoicePCMFormat {
    uint32_t bitRate;
    uint32_t sampleRate;
    uint16_t numChannels;
    uint16_t audioType;
} SceVoicePCMFormat;
#ifdef __cplusplus
extern "C" {
#endif
int sceVoiceInit(void *params, int mode);
int sceVoiceEnd(void);
int sceVoiceStart(void);
int sceVoiceStop(void);
int sceVoiceCreatePort(int *portId, const void *portInfo);
int sceVoiceDeletePort(int portId);
int sceVoiceConnectIPortToOPort(int oportId, int iportId);
int sceVoiceDisconnectIPortFromOPort(int oportId, int iportId);
int sceVoiceGetPortInfo(int portId, void *portInfo);
int sceVoiceWriteToIPort(int oportId, const void *data, uint32_t *size, int16_t volume);
int sceVoiceReadFromOPort(int oportId, void *data, uint32_t *size);
int sceVoiceGetMicVolume(int portId, uint16_t *volume);
int sceVoiceSetVolume(int portId, uint16_t volume);
int sceVoiceGetBitRate(int portId, uint32_t *bitrate);
int sceVoiceSetBitRate(int portId, uint32_t bitrate);
#ifdef __cplusplus
}
#endif
