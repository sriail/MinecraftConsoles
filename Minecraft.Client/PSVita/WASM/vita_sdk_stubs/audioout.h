#pragma once
// Minimal PS Vita audioout.h stub for Emscripten/WASM builds.

#include <stdint.h>

#define SCE_AUDIO_OUT_PORT_TYPE_MAIN    0
#define SCE_AUDIO_OUT_PORT_TYPE_BGM     1
#define SCE_AUDIO_OUT_PORT_TYPE_VOICE   2

#define SCE_AUDIO_OUT_PARAM_FORMAT_S16_MONO   0
#define SCE_AUDIO_OUT_PARAM_FORMAT_S16_STEREO 1

#define SCE_AUDIO_OUT_MODE_MONO   0
#define SCE_AUDIO_OUT_MODE_STEREO 1

#ifdef __cplusplus
extern "C" {
#endif

int sceAudioOutOpenPort(int portType, int len, int freq, int param);
int sceAudioOutOutput(int port, const void *ptr);
int sceAudioOutSetVolume(int port, int flag, int *vol);
int sceAudioOutReleasePort(int port);
int sceAudioOutGetConfig(int port, int *pConfig);

#ifdef __cplusplus
}
#endif
