#ifndef SSO_AUDIO_H
#define SSO_AUDIO_H

#include "raylib.h"
#include <vector>
#include <string>

namespace SSO {
    namespace Audio {

        struct ManagedMusic {
            Music music;
            bool active;
            unsigned char* buffer;
        };

        inline std::vector<ManagedMusic> musicManager;

        inline Sound LoadSound(const char* fileName) {
            return ::LoadSound(fileName);
        }

        inline Sound LoadSoundFromMemory(const char* fileType, const unsigned char* data, int dataSize) {
            Wave wave = LoadWaveFromMemory(fileType, data, dataSize);
            Sound sound = LoadSoundFromWave(wave);
            UnloadWave(wave);
            return sound;
        }

        inline Music LoadMusicStream(const char* fileName) {
            return ::LoadMusicStream(fileName);
        }

        inline Music LoadMusicStreamFromMemory(const char* fileType, const unsigned char* data, int dataSize) {
            return ::LoadMusicStreamFromMemory(fileType, data, dataSize);
        }

        inline void RegisterMusic(Music music, unsigned char* buffer = nullptr) {
            ManagedMusic managed;
            managed.music = music;
            managed.active = true;
            managed.buffer = buffer;
            musicManager.push_back(managed);
        }

        inline void UpdateAudio() {
            for (auto& managed : musicManager) {
                if (managed.active) {
                    UpdateMusicStream(managed.music);
                }
            }
        }

        inline void UnloadSound(Sound sound) {
            ::UnloadSound(sound);
        }

        inline void UnloadMusic(Music music) {
            ::UnloadMusicStream(music);
        }

        inline void UnloadRegisteredMusic(int index) {
            if (index >= 0 && index < musicManager.size()) {
                UnloadMusic(musicManager[index].music);
                if (musicManager[index].buffer) {
                    RL_FREE(musicManager[index].buffer);
                }
                musicManager[index].active = false;
            }
        }

        inline void ClearMusicManager() {
            for (auto& managed : musicManager) {
                if (managed.active) {
                    UnloadMusic(managed.music);
                    if (managed.buffer) {
                        RL_FREE(managed.buffer);
                    }
                }
            }
            musicManager.clear();
        }

        inline void PlaySound(Sound sound) {
            ::PlaySound(sound);
        }

        inline void PlayMusic(Music music) {
            ::PlayMusicStream(music);
        }

        inline void StopMusic(Music music) {
            ::StopMusicStream(music);
        }

        inline void PauseMusic(Music music) {
            ::PauseMusicStream(music);
        }

        inline void ResumeMusic(Music music) {
            ::ResumeMusicStream(music);
        }

        inline void SetSoundVolume(Sound sound, float volume) {
            ::SetSoundVolume(sound, volume);
        }

        inline void SetMusicVolume(Music music, float volume) {
            ::SetMusicVolume(music, volume);
        }

        inline void SetSoundPitch(Sound sound, float pitch) {
            ::SetSoundPitch(sound, pitch);
        }

        inline void SetMusicPitch(Music music, float pitch) {
            ::SetMusicPitch(music, pitch);
        }

        inline bool IsMusicPlaying(Music music) {
            return ::IsMusicStreamPlaying(music);
        }
    }
}

#endif
