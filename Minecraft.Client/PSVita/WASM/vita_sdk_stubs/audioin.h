#pragma once
#include <stdint.h>

#ifndef SCE_OK
#define SCE_OK 0
#endif

#define SCE_AUDIO_IN_PORT_TYPE_RAW 0

#ifdef __cplusplus
extern "C" {
#endif

int sceAudioInOpenPort(int type, int grain, int freq, int param);
int sceAudioInInput(int port, void *destBuf);
int sceAudioInReleasePort(int port);

#ifdef __cplusplus
}
#endif
