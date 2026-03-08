#pragma once

typedef int SceAudioOutPortType;

#define SCE_AUDIO_OUT_PORT_TYPE_MAIN  0
#define SCE_AUDIO_OUT_PORT_TYPE_BGM   1
#define SCE_AUDIO_OUT_PORT_TYPE_VOICE 2

#define SCE_AUDIO_OUT_MODE_MONO   0
#define SCE_AUDIO_OUT_MODE_STEREO 1

#define SCE_AUDIO_OUT_PARAM_FORMAT_S16_MONO   0
#define SCE_AUDIO_OUT_PARAM_FORMAT_S16_STEREO 1

inline int sceAudioOutOpenPort(int portType, int grain, int freq, int mode) {
    (void)portType; (void)grain; (void)freq; (void)mode;
    return 1;
}

inline int sceAudioOutOutput(int port, const void *buf) {
    (void)port; (void)buf;
    return 0;
}

inline int sceAudioOutSetVolume(int port, int flag, int *vol) {
    (void)port; (void)flag; (void)vol;
    return 0;
}

inline int sceAudioOutReleasePort(int port) {
    (void)port;
    return 0;
}

inline int sceAudioOutGetRestSample(int port) {
    (void)port;
    return 0;
}

inline int sceAudioOutSetConfig(int port, int grain, int freq, int mode) {
    (void)port; (void)grain; (void)freq; (void)mode;
    return 0;
}
