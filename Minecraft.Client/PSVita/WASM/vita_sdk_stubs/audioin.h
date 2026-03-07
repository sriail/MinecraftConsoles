#pragma once
// Minimal PS Vita audioin.h stub for Emscripten/WASM builds.
#include <stdint.h>
#define SCE_AUDIOIN_TYPE_VOICE 0
#define SCE_AUDIOIN_TYPE_RAW   1
#ifdef __cplusplus
extern "C" {
#endif
int sceAudioInOpenPort(int portType, int grain, int freq, int format);
int sceAudioInInput(int port, void *destPtr);
int sceAudioInReleasePort(int port);
int sceAudioInGetAdopt(int portType);
#ifdef __cplusplus
}
#endif
