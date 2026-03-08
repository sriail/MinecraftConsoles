#pragma once

#ifdef __EMSCRIPTEN__

#include <emscripten.h>

// Maximum number of simultaneously loaded sounds
#define WEBAUDIO_MAX_SOUNDS 512

// Maximum number of simultaneous sound sources
#define WEBAUDIO_MAX_SOURCES 32

// ============================================================================
// Web Audio API Backend
// ============================================================================

class WebAudioEngine
{
public:
    static WebAudioEngine& GetInstance();

    // Lifecycle
    bool Init();
    void Shutdown();

    // Sound loading
    void LoadSound(const char* path, int soundId);
    void UnloadSound(int soundId);
    bool IsSoundLoaded(int soundId) const;

    // Sound playback
    void PlaySound(int soundId, float volume, float pitch);
    void PlaySound3D(int soundId, float x, float y, float z, float volume, float pitch);
    void StopAllSounds();

    // Streaming music
    void PlayMusic(const char* path, float volume, bool loop);
    void StopMusic();
    void SetMusicVolume(float volume);
    bool IsMusicPlaying() const;

    // Global controls
    void SetMasterVolume(float volume);
    void SetSoundEffectVolume(float volume);
    void SetMusicMasterVolume(float volume);

    // Listener (for 3D audio)
    void SetListenerPosition(float x, float y, float z);
    void SetListenerOrientation(float forwardX, float forwardY, float forwardZ,
                                float upX, float upY, float upZ);

    // Resume audio context (required after user interaction on some browsers)
    void ResumeContext();

private:
    WebAudioEngine();
    ~WebAudioEngine();
    WebAudioEngine(const WebAudioEngine&) = delete;
    WebAudioEngine& operator=(const WebAudioEngine&) = delete;

    bool m_initialized;
    float m_masterVolume;
    float m_soundEffectVolume;
    float m_musicVolume;
    bool m_musicPlaying;
    bool m_soundLoaded[WEBAUDIO_MAX_SOUNDS];
};

#endif // __EMSCRIPTEN__
