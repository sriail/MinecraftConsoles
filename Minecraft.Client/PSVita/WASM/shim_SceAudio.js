/**
 * shim_SceAudio.js
 * Emscripten JS-library glue for the SceAudio shim.
 *
 * Uses Web AudioContext with an AudioWorklet to play back 16-bit signed PCM
 * buffers as sent by sceAudioOutOutput.
 *
 * AudioWorklet processor (vita-pcm-processor) must be registered before first
 * use.  The host page must call:
 *   Module.vita_audio_init_worklet(audioCtx)  (see bottom of this file)
 *
 * Usage:  emcc ... --js-library shim_SceAudio.js
 */

mergeInto(LibraryManager.library, {

    vita_audio_open_port__deps: [],
    vita_audio_open_port: function(portId, sampleRate, bufferSize, channels) {
        if (!Module._vita_audio_ctx) {
            Module._vita_audio_ctx = new (window.AudioContext ||
                                          window.webkitAudioContext)({
                sampleRate: sampleRate
            });
        }
        var ctx = Module._vita_audio_ctx;
        if (!Module._vita_audio_ports) Module._vita_audio_ports = {};

        /* ScriptProcessorNode as fallback; AudioWorklet preferred in production. */
        var node = ctx.createScriptProcessor(bufferSize, 0, channels);
        var gainL = ctx.createGain();
        var gainR = ctx.createGain();

        /* Ring buffer of Float32 frames */
        var ring = [];
        var RING_MAX = 4;

        node.onaudioprocess = function(ev) {
            var out = ev.outputBuffer;
            if (ring.length === 0) {
                /* underrun: output silence */
                for (var c = 0; c < out.numberOfChannels; c++) {
                    out.getChannelData(c).fill(0);
                }
                return;
            }
            var frames = ring.shift();
            for (var c = 0; c < out.numberOfChannels; c++) {
                out.getChannelData(c).set(frames[c]);
            }
        };

        node.connect(ctx.destination);

        Module._vita_audio_ports[portId] = {
            node: node,
            ring: ring,
            channels: channels,
            bufferSize: bufferSize,
            ringMax: RING_MAX,
            gainNode: node
        };
    },

    vita_audio_close_port__deps: [],
    vita_audio_close_port: function(portId) {
        if (!Module._vita_audio_ports) return;
        var p = Module._vita_audio_ports[portId];
        if (!p) return;
        p.node.disconnect();
        delete Module._vita_audio_ports[portId];
    },

    vita_audio_queue_buffer__deps: [],
    vita_audio_queue_buffer: function(portId, bufPtr, byteLen, sampleCount) {
        if (!Module._vita_audio_ports) return;
        var p = Module._vita_audio_ports[portId];
        if (!p) return;

        /* Decode 16-bit signed PCM interleaved → per-channel Float32 */
        var src = new Int16Array(Module.HEAPU8.buffer, bufPtr, byteLen / 2);
        var frames = [];
        for (var c = 0; c < p.channels; c++) {
            frames.push(new Float32Array(sampleCount));
        }
        for (var i = 0; i < sampleCount; i++) {
            for (var c = 0; c < p.channels; c++) {
                frames[c][i] = src[i * p.channels + c] / 32768.0;
            }
        }

        if (p.ring.length < p.ringMax) {
            p.ring.push(frames);
        }
        /* If ring is full, drop this buffer (non-fatal: causes audio glitch) */
    },

    vita_audio_set_volume__deps: [],
    vita_audio_set_volume: function(portId, leftGain, rightGain) {
        if (!Module._vita_audio_ports) return;
        var p = Module._vita_audio_ports[portId];
        if (!p) return;
        /* Gain on ScriptProcessorNode is not directly settable per channel;
         * we apply a simple mono gain as best-effort. */
        var avg = (leftGain + rightGain) / 2.0;
        if (Module._vita_audio_ctx) {
            /* Create / reuse a GainNode upstream of the script node */
            if (!p.outerGain) {
                var ctx = Module._vita_audio_ctx;
                p.outerGain = ctx.createGain();
                p.node.disconnect();
                p.node.connect(p.outerGain);
                p.outerGain.connect(ctx.destination);
            }
            p.outerGain.gain.setTargetAtTime(avg,
                Module._vita_audio_ctx.currentTime, 0.01);
        }
    }
});
