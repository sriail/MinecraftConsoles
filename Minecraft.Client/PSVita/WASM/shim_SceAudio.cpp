/*
 * shim_SceAudio.cpp
 * PS Vita SceAudio → WebAudio API shim implementation
 *
 * Build requirements:
 *   --js-library shim_SceAudio.js
 *   -s ASYNCIFY=1   (sceAudioOutOutput blocks until the chunk is queued)
 *
 * Threading model:
 *   sceAudioOutOutput is a blocking call on Vita — it returns only when the
 *   previous buffer has been consumed.  In the WASM single-threaded build we
 *   emulate this by queuing the buffer through JS and sleeping for one frame
 *   period with emscripten_sleep.  In the pthreads build the audio thread
 *   calls into WebAudio from a worker.
 */

#include "shim_SceAudio.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#  include <emscripten.h>
#endif

/* =========================================================================
 * Port table
 * ========================================================================= */

#define MAX_AUDIO_PORTS 8

typedef struct {
    int   active;
    int   port_type;
    int   sample_count;  /* samples per buffer (not bytes) */
    int   freq;          /* sample rate Hz */
    int   format;        /* S16_STEREO / S16_MONO */
    int   vol[2];        /* left, right volume (0..32768) */
} AudioPort;

static AudioPort s_ports[MAX_AUDIO_PORTS];

/* =========================================================================
 * sceAudioOutOpenPort
 * VITA:  Allocates a hardware PCM output port
 * WEB:   Registers an AudioWorklet port slot; real Web AudioContext created
 *        in JS via vita_audio_open_port().
 * NOTE:  len = number of samples per output block (typically 256, 512, 1024)
 * ========================================================================= */

int sceAudioOutOpenPort(int portType, int len, int freq, int param)
{
    if (len <= 0 || freq <= 0) return SCE_AUDIO_ERROR_INVALID_SIZE;

    for (int i = 0; i < MAX_AUDIO_PORTS; i++) {
        if (!s_ports[i].active) {
            s_ports[i].active       = 1;
            s_ports[i].port_type    = portType;
            s_ports[i].sample_count = len;
            s_ports[i].freq         = freq;
            s_ports[i].format       = param;
            s_ports[i].vol[0]       = SCE_AUDIO_VOLUME_0DB;
            s_ports[i].vol[1]       = SCE_AUDIO_VOLUME_0DB;

#ifdef __EMSCRIPTEN__
            int channels = (param == SCE_AUDIO_OUT_PARAM_FORMAT_S16_MONO) ? 1 : 2;
            EM_ASM({
                vita_audio_open_port($0, $1, $2, $3);
            }, i, freq, len, channels);
#endif
            return i; /* port handle = array index */
        }
    }
    return SCE_AUDIO_ERROR_PORT_FULL;
}

/* =========================================================================
 * sceAudioOutClosePort
 * VITA:  Releases the hardware port
 * WEB:   Closes the AudioContext/node via vita_audio_close_port()
 * ========================================================================= */

int sceAudioOutClosePort(int port)
{
    if (port < 0 || port >= MAX_AUDIO_PORTS) return SCE_AUDIO_ERROR_INVALID_PORT;
    if (!s_ports[port].active) return SCE_AUDIO_ERROR_INVALID_PORT;

#ifdef __EMSCRIPTEN__
    EM_ASM({ vita_audio_close_port($0); }, port);
#endif

    s_ports[port].active = 0;
    return SCE_OK;
}

/* =========================================================================
 * sceAudioOutOutput
 * VITA:  Writes one PCM buffer and blocks until playback begins
 * WEB:   Queues the buffer to the AudioWorklet and yields via Asyncify
 * NOTE:  buf contains s_ports[port].sample_count × channels × 2 bytes of
 *        16-bit signed PCM.  Requires -s ASYNCIFY=1.
 * ========================================================================= */

int sceAudioOutOutput(int port, const void *buf)
{
    if (port < 0 || port >= MAX_AUDIO_PORTS) return SCE_AUDIO_ERROR_INVALID_PORT;
    if (!s_ports[port].active) return SCE_AUDIO_ERROR_INVALID_PORT;
    if (!buf) return SCE_AUDIO_ERROR_INVALID_PARAM;

    int channels = (s_ports[port].format == SCE_AUDIO_OUT_PARAM_FORMAT_S16_MONO) ? 1 : 2;
    int bytes = s_ports[port].sample_count * channels * sizeof(int16_t);

#ifdef __EMSCRIPTEN__
    EM_ASM({
        vita_audio_queue_buffer($0, $1, $2, $3);
    }, port, buf, bytes, s_ports[port].sample_count);

    /* Yield to let the AudioWorklet pull the buffer.
     * Sleep slightly less than the full period to avoid underruns. */
    double period_ms = (1000.0 * s_ports[port].sample_count) / (double)s_ports[port].freq;
    unsigned int sleep_ms = (unsigned int)(period_ms * 0.9);
    if (sleep_ms == 0) sleep_ms = 1;
    emscripten_sleep(sleep_ms);
#endif

    return SCE_OK;
}

/* =========================================================================
 * sceAudioOutSetVolume
 * VITA:  Sets per-channel volume (0 = mute, 32768 = 0dB)
 * WEB:   Adjusts a GainNode via vita_audio_set_volume()
 * ========================================================================= */

int sceAudioOutSetVolume(int port, int ch, int *vol)
{
    if (port < 0 || port >= MAX_AUDIO_PORTS) return SCE_AUDIO_ERROR_INVALID_PORT;
    if (!s_ports[port].active) return SCE_AUDIO_ERROR_INVALID_PORT;
    if (!vol) return SCE_AUDIO_ERROR_INVALID_PARAM;

    /* ch is a bitmask: bit 0 = left, bit 1 = right */
    if (ch & 1) s_ports[port].vol[0] = vol[0];
    if (ch & 2) s_ports[port].vol[1] = vol[1];

#ifdef __EMSCRIPTEN__
    float left  = (float)s_ports[port].vol[0] / (float)SCE_AUDIO_VOLUME_0DB;
    float right = (float)s_ports[port].vol[1] / (float)SCE_AUDIO_VOLUME_0DB;
    EM_ASM({ vita_audio_set_volume($0, $1, $2); }, port, left, right);
#endif
    return SCE_OK;
}

/* =========================================================================
 * sceAudioOutGetRestSample
 * VITA:  Returns number of samples still queued in hardware ring buffer
 * WEB:   Returns 0 (WebAudio buffering is opaque to us)
 * NOTE:  [VITA_SHIM_STUB]
 * ========================================================================= */

int sceAudioOutGetRestSample(int port)
{
    (void)port;
    /* [VITA_SHIM_STUB] sceAudioOutGetRestSample: always returns 0 */
    fprintf(stderr, "[VITA_SHIM_STUB] sceAudioOutGetRestSample(%d)\n", port);
    return 0;
}
