#pragma once
#include <stdint.h>

#ifndef SCE_OK
#define SCE_OK 0
#endif

#define SCE_AUDIO_OUT_PORT_TYPE_MAIN  0
#define SCE_AUDIO_OUT_PORT_TYPE_BGM   1
#define SCE_AUDIO_OUT_PORT_TYPE_VOICE 2

#define SCE_AUDIO_OUT_MODE_MONO   0
#define SCE_AUDIO_OUT_MODE_STEREO 1

#ifdef __cplusplus
extern "C" {
#endif

int sceAudioOutOpenPort(int type, int len, int freq, int mode);
int sceAudioOutOutput(int port, const void *buf);
int sceAudioOutReleasePort(int port);
int sceAudioOutSetVolume(int port, int flag, int *vol);
int sceAudioOutGetRestSample(int port);

#ifdef __cplusplus
}
#endif
