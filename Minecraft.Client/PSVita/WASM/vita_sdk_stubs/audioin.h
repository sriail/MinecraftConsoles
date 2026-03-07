#pragma once
/* audioin.h — PS Vita audio input stubs for Emscripten/WASM builds */

#include <stdint.h>
#include "kernel.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCE_AUDIO_IN_TYPE_GENERAL  0
#define SCE_AUDIO_IN_TYPE_VOICE    1

#define SCE_AUDIO_IN_PARAM_FORMAT_S16_MONO  0
#define SCE_AUDIO_IN_FREQ_16000  16000
#define SCE_AUDIO_IN_FREQ_48000  48000

static inline int sceAudioInOpenPort(int portType, int grain, int freq, int format)
{ (void)portType; (void)grain; (void)freq; (void)format; return 1; }

static inline int sceAudioInClosePort(int portId)
{ (void)portId; return SCE_OK; }

static inline int sceAudioInInput(int portId, void *destPtr)
{ (void)portId; (void)destPtr; return SCE_OK; }

static inline int sceAudioInGetAdditionalInformation(int portId, int *pInfo)
{ (void)portId; (void)pInfo; return SCE_OK; }

#ifdef __cplusplus
}
#endif
