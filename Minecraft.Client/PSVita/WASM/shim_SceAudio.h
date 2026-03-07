#ifndef SHIM_SCEAUDIO_H
#define SHIM_SCEAUDIO_H

/*
 * shim_SceAudio.h
 * PS Vita SceAudio → WebAudio API shim
 *
 * Covers: output port creation/deletion, blocking PCM write, volume control.
 * Vita audio: 48 kHz, 16-bit signed stereo PCM.
 *
 * ABI reference: vitasdk <audioout.h>
 */

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- Return codes ---------- */
#ifndef SCE_OK
#define SCE_OK 0
#endif
#define SCE_AUDIO_ERROR_INVALID_PORT    ((int)0x80260001)
#define SCE_AUDIO_ERROR_INVALID_SIZE    ((int)0x80260002)
#define SCE_AUDIO_ERROR_INVALID_PARAM   ((int)0x80260003)
#define SCE_AUDIO_ERROR_PORT_FULL       ((int)0x80260004)

/* ---------- Port types ---------- */
#define SCE_AUDIO_OUT_PORT_TYPE_MAIN    0   /* Normal stereo output */
#define SCE_AUDIO_OUT_PORT_TYPE_BGM     1   /* Background music */
#define SCE_AUDIO_OUT_PORT_TYPE_VOICE   2   /* Voice chat */

/* ---------- Format ---------- */
#define SCE_AUDIO_OUT_PARAM_FORMAT_S16_STEREO   0   /* 16-bit signed stereo */
#define SCE_AUDIO_OUT_PARAM_FORMAT_S16_MONO     1

/* ---------- Volume ---------- */
#define SCE_AUDIO_VOLUME_0DB            32768

/* ---------- API ---------- */
int sceAudioOutOpenPort(int portType, int len, int freq, int param);
int sceAudioOutClosePort(int port);
int sceAudioOutOutput(int port, const void *buf);
int sceAudioOutSetVolume(int port, int ch, int *vol);
int sceAudioOutGetRestSample(int port);

#ifdef __cplusplus
}
#endif

#endif /* SHIM_SCEAUDIO_H */
