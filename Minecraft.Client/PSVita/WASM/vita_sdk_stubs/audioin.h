#pragma once

#define SCE_AUDIO_IN_PORT_TYPE_VOICE 0
#define SCE_AUDIO_IN_PARAM_FORMAT_S16_MONO 0

inline int sceAudioInOpenPort(int portType, int grain, int freq, int param) {
    (void)portType; (void)grain; (void)freq; (void)param;
    return 1;
}

inline int sceAudioInInput(int port, void *buf) {
    (void)port; (void)buf;
    return 0;
}

inline int sceAudioInReleasePort(int port) {
    (void)port;
    return 0;
}
