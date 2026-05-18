// ==============================================================================
// SSOEngine v1.7 - OFFICIAL LICENSE AND ATTRIBUTION FILE //
// ==============================================================================
// // AUTHOR & LEAD DEVELOPER: // Rozak // // CONTACT: //
// Email: spctacularstudio@gmail.com // GitHub: https://github.com/Rozaq125 //
// Website: https://ssoengine.pages.dev // // SUPPORT THE PROJECT: // Donationware
// Link: https://saweria.co/omedonation // //
// ------------------------------------------------------------------------------
// MIT LICENSE //
// ------------------------------------------------------------------------------
// Copyright (c) 2026 SSOFoundation// // Permission
// is hereby granted, free of charge, to any person obtaining a copy // of this
// software and associated documentation files (the "Software"), to deal // in the
// software without restriction, including without limitation the rights // to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell // copies of
// the Software, and to permit persons to whom the Software is // furnished to do
// so, subject to the following conditions: // // The above copyright notice and
// this permission notice shall be included in all // copies or substantial
// portions of the Software. // // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT
// WARRANTY OF ANY KIND, EXPRESS OR // IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, // FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER // LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
// THE USE OR OTHER DEALINGS IN THE // SOFTWARE. // //
// ------------------------------------------------------------------------------
// BUILT WITH & SPECIAL THANKS //
// ------------------------------------------------------------------------------
// RAYLIB: (c) Ramon Santamaria (https://www.raylib.com) // COMPILER: MinGW-w64
// Project (GCC Suite) // CONTRIBUTOR: Nathan (Feedback Provider) // // COMMUNITY
// HALL OF FAME (Reddit Feedback & Support): // u/rileygrham // u/Jimmy-M-420 //
// u/gabriel_aplok // u/InitialComfortable70 // u/ghulmar // //
// ------------------------------------------------------------------------------
// PROJECT DESCRIPTION //
// ------------------------------------------------------------------------------
// SSOEngine is a professional-grade, cross-platform 2D/3D game engine SDK //
// built on top of Raylib //
// high-precision physics, advanced camera systems, and a professional build //
// pipeline for Windows and Android distribution. // // VERSION: 1.7 //
// ==============================================================================

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
