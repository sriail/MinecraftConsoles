#include "stdafx.h"

#ifdef __EMSCRIPTEN__

#include "WebAudio.h"
#include <cstring>

// ============================================================================
// WebAudioEngine Implementation
// ============================================================================

WebAudioEngine::WebAudioEngine()
    : m_initialized(false)
    , m_masterVolume(1.0f)
    , m_soundEffectVolume(1.0f)
    , m_musicVolume(1.0f)
    , m_musicPlaying(false)
{
    memset(m_soundLoaded, 0, sizeof(m_soundLoaded));
}

WebAudioEngine::~WebAudioEngine()
{
    Shutdown();
}

WebAudioEngine& WebAudioEngine::GetInstance()
{
    static WebAudioEngine instance;
    return instance;
}

bool WebAudioEngine::Init()
{
    if (m_initialized)
        return true;

    EM_ASM({
        // Create the AudioContext
        window.audioContext = new (window.AudioContext || window.webkitAudioContext)();

        // Sound buffer storage (keyed by sound ID)
        window.audioBuffers = {};

        // Active sound source tracking
        window.activeSources = [];

        // Master gain node
        window.masterGain = audioContext.createGain();
        window.masterGain.connect(audioContext.destination);

        // Sound effects gain node
        window.sfxGain = audioContext.createGain();
        window.sfxGain.connect(window.masterGain);

        // Music gain node
        window.musicGain = audioContext.createGain();
        window.musicGain.connect(window.masterGain);

        // Current music element (for streaming)
        window.currentMusic = null;
        window.currentMusicSource = null;

        console.log("WebAudio: Initialized audio context, sample rate = " +
                     audioContext.sampleRate + " Hz");
    });

    m_initialized = true;
    return true;
}

void WebAudioEngine::Shutdown()
{
    if (!m_initialized)
        return;

    EM_ASM({
        // Stop all active sources
        if (window.activeSources) {
            for (var i = 0; i < window.activeSources.length; i++) {
                try { window.activeSources[i].stop(); } catch(e) {}
            }
            window.activeSources = [];
        }

        // Stop music
        if (window.currentMusic) {
            window.currentMusic.pause();
            window.currentMusic = null;
        }
        if (window.currentMusicSource) {
            try { window.currentMusicSource.stop(); } catch(e) {}
            window.currentMusicSource = null;
        }

        // Close context
        if (window.audioContext) {
            window.audioContext.close();
            window.audioContext = null;
        }

        window.audioBuffers = {};
    });

    m_initialized = false;
}

void WebAudioEngine::LoadSound(const char* path, int soundId)
{
    if (!m_initialized || soundId < 0 || soundId >= WEBAUDIO_MAX_SOUNDS)
        return;

    EM_ASM_({
        var path = UTF8ToString($0);
        var soundId = $1;

        fetch(path)
            .then(function(response) { return response.arrayBuffer(); })
            .then(function(buffer) { return audioContext.decodeAudioData(buffer); })
            .then(function(decoded) {
                audioBuffers[soundId] = decoded;
                console.log("WebAudio: Loaded sound " + soundId + " from " + path);
            })
            .catch(function(err) {
                console.error("WebAudio: Failed to load sound " + soundId + ": " + err);
            });
    }, path, soundId);

    // Note: m_soundLoaded tracks that loading was initiated;
    // actual buffer availability is asynchronous and checked in JS via audioBuffers[id]
    m_soundLoaded[soundId] = true;
}

void WebAudioEngine::UnloadSound(int soundId)
{
    if (soundId < 0 || soundId >= WEBAUDIO_MAX_SOUNDS)
        return;

    EM_ASM_({
        delete audioBuffers[$0];
    }, soundId);

    m_soundLoaded[soundId] = false;
}

bool WebAudioEngine::IsSoundLoaded(int soundId) const
{
    if (soundId < 0 || soundId >= WEBAUDIO_MAX_SOUNDS)
        return false;
    return m_soundLoaded[soundId];
}

void WebAudioEngine::PlaySound(int soundId, float volume, float pitch)
{
    if (!m_initialized)
        return;

    float effectiveVolume = volume * m_soundEffectVolume * m_masterVolume;

    EM_ASM_({
        if (!audioContext || !audioBuffers[$0]) return;

        var source = audioContext.createBufferSource();
        var gainNode = audioContext.createGain();

        source.buffer = audioBuffers[$0];
        source.playbackRate.value = $2; // pitch
        gainNode.gain.value = $1;       // volume

        source.connect(gainNode);
        gainNode.connect(window.sfxGain);
        source.start();

        // Track source for cleanup
        window.activeSources.push(source);
        source.onended = function() {
            var idx = window.activeSources.indexOf(source);
            if (idx !== -1) window.activeSources.splice(idx, 1);
        };
    }, soundId, effectiveVolume, pitch);
}

void WebAudioEngine::PlaySound3D(int soundId, float x, float y, float z, float volume, float pitch)
{
    if (!m_initialized)
        return;

    float effectiveVolume = volume * m_soundEffectVolume * m_masterVolume;

    EM_ASM_({
        if (!audioContext || !audioBuffers[$0]) return;

        var source = audioContext.createBufferSource();
        var gainNode = audioContext.createGain();
        var panner = audioContext.createPanner();

        source.buffer = audioBuffers[$0];
        source.playbackRate.value = $5; // pitch
        gainNode.gain.value = $4;       // volume

        // Set 3D position
        panner.positionX.value = $1;
        panner.positionY.value = $2;
        panner.positionZ.value = $3;
        panner.distanceModel = 'linear';
        panner.maxDistance = 16.0;
        panner.refDistance = 1.0;
        panner.rolloffFactor = 1.0;

        source.connect(gainNode);
        gainNode.connect(panner);
        panner.connect(window.sfxGain);
        source.start();

        window.activeSources.push(source);
        source.onended = function() {
            var idx = window.activeSources.indexOf(source);
            if (idx !== -1) window.activeSources.splice(idx, 1);
        };
    }, soundId, x, y, z, effectiveVolume, pitch);
}

void WebAudioEngine::StopAllSounds()
{
    if (!m_initialized)
        return;

    EM_ASM({
        if (window.activeSources) {
            for (var i = 0; i < window.activeSources.length; i++) {
                try { window.activeSources[i].stop(); } catch(e) {}
            }
            window.activeSources = [];
        }
    });
}

void WebAudioEngine::PlayMusic(const char* path, float volume, bool loop)
{
    if (!m_initialized)
        return;

    float effectiveVolume = volume * m_musicVolume * m_masterVolume;

    // Stop any currently playing music first
    StopMusic();

    EM_ASM_({
        var path = UTF8ToString($0);
        var volume = $1;
        var doLoop = $2;

        // Use HTML5 Audio element for streaming music (more efficient for large files)
        var audio = new Audio(path);
        audio.loop = doLoop ? true : false;
        // Volume controlled exclusively through gain node chain; set element to 1.0
        audio.volume = 1.0;

        // Connect to Web Audio API for gain control
        var source = audioContext.createMediaElementSource(audio);
        source.connect(window.musicGain);
        window.musicGain.gain.value = volume;

        window.currentMusic = audio;
        window.currentMusicSource = source;

        audio.play().catch(function(err) {
            console.error("WebAudio: Failed to play music: " + err);
        });
    }, path, effectiveVolume, loop ? 1 : 0);

    m_musicPlaying = true;
}

void WebAudioEngine::StopMusic()
{
    if (!m_initialized)
        return;

    EM_ASM({
        if (window.currentMusic) {
            window.currentMusic.pause();
            window.currentMusic.currentTime = 0;
            window.currentMusic = null;
        }
        if (window.currentMusicSource) {
            try { window.currentMusicSource.disconnect(); } catch(e) {}
            window.currentMusicSource = null;
        }
    });

    m_musicPlaying = false;
}

void WebAudioEngine::SetMusicVolume(float volume)
{
    m_musicVolume = volume;
    if (!m_initialized)
        return;

    float effectiveVolume = volume * m_masterVolume;
    EM_ASM_({
        if (window.musicGain) {
            window.musicGain.gain.value = $0;
        }
    }, effectiveVolume);
}

bool WebAudioEngine::IsMusicPlaying() const
{
    return m_musicPlaying;
}

void WebAudioEngine::SetMasterVolume(float volume)
{
    m_masterVolume = volume;
    if (!m_initialized)
        return;

    EM_ASM_({
        if (window.masterGain) {
            window.masterGain.gain.value = $0;
        }
    }, volume);
}

void WebAudioEngine::SetSoundEffectVolume(float volume)
{
    m_soundEffectVolume = volume;
    if (!m_initialized)
        return;

    EM_ASM_({
        if (window.sfxGain) {
            window.sfxGain.gain.value = $0;
        }
    }, volume * m_masterVolume);
}

void WebAudioEngine::SetMusicMasterVolume(float volume)
{
    SetMusicVolume(volume);
}

void WebAudioEngine::SetListenerPosition(float x, float y, float z)
{
    if (!m_initialized)
        return;

    EM_ASM_({
        if (audioContext && audioContext.listener) {
            if (audioContext.listener.positionX) {
                audioContext.listener.positionX.value = $0;
                audioContext.listener.positionY.value = $1;
                audioContext.listener.positionZ.value = $2;
            } else {
                audioContext.listener.setPosition($0, $1, $2);
            }
        }
    }, x, y, z);
}

void WebAudioEngine::SetListenerOrientation(float forwardX, float forwardY, float forwardZ,
                                             float upX, float upY, float upZ)
{
    if (!m_initialized)
        return;

    EM_ASM_({
        if (audioContext && audioContext.listener) {
            if (audioContext.listener.forwardX) {
                audioContext.listener.forwardX.value = $0;
                audioContext.listener.forwardY.value = $1;
                audioContext.listener.forwardZ.value = $2;
                audioContext.listener.upX.value = $3;
                audioContext.listener.upY.value = $4;
                audioContext.listener.upZ.value = $5;
            } else {
                audioContext.listener.setOrientation($0, $1, $2, $3, $4, $5);
            }
        }
    }, forwardX, forwardY, forwardZ, upX, upY, upZ);
}

void WebAudioEngine::ResumeContext()
{
    if (!m_initialized)
        return;

    EM_ASM({
        if (audioContext && audioContext.state === 'suspended') {
            audioContext.resume().then(function() {
                console.log("WebAudio: Audio context resumed");
            });
        }
    });
}

#endif // __EMSCRIPTEN__
