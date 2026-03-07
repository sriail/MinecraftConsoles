#pragma once
/* audioout.h — PS Vita audio output stubs for Emscripten/WASM builds */

#include <stdint.h>
#include "kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCE_AUDIO_OUT_PORT_TYPE_MAIN    0
#define SCE_AUDIO_OUT_PORT_TYPE_BGM     1
#define SCE_AUDIO_OUT_PORT_TYPE_VOICE   2

#define SCE_AUDIO_OUT_PARAM_FORMAT_S16_MONO    0
#define SCE_AUDIO_OUT_PARAM_FORMAT_S16_STEREO  1
#define SCE_AUDIO_OUT_PARAM_FORMAT_FLOAT_MONO  2
#define SCE_AUDIO_OUT_PARAM_FORMAT_FLOAT_STEREO 3

#define SCE_AUDIO_OUT_FREQ_48000   48000
#define SCE_AUDIO_OUT_FREQ_44100   44100

#define SCE_AUDIO_VOLUME_0DB   32768

static inline int sceAudioOutOpenPort(int portType, int len, int freq, int format)
{ (void)portType; (void)len; (void)freq; (void)format; return 1; /* port handle */ }

static inline int sceAudioOutClosePort(int portId)
{ (void)portId; return SCE_OK; }

static inline int sceAudioOutOutput(int portId, const void *ptr)
{ (void)portId; (void)ptr; return SCE_OK; }

static inline int sceAudioOutSetVolume(int portId, int flag, int *vol)
{ (void)portId; (void)flag; (void)vol; return SCE_OK; }

static inline int sceAudioOutGetRestSample(int portId)
{ (void)portId; return 0; }

#ifdef __cplusplus
}
#endif
