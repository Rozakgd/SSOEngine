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

#ifndef SSO_TIMER_H
#define SSO_TIMER_H

#include <chrono>

namespace SSO {
    /**
     * @brief Universal Time Management System for SSOEngine.
     * Provides high-precision Delta Time and flexible game timers.
     */
    class Timer {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTime;
        float m_CurrentValue = 0.0f;
        bool m_IsActive = false;

    public:
        Timer() {
            m_StartTime = std::chrono::high_resolution_clock::now();
            m_LastTime = m_StartTime;
        }

        // --- Core Engine Timing ---
        
        /**
         * @return Frame duration in seconds (Delta Time).
         */
        float GetDeltaTime() {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count();
            m_LastTime = currentTime;
            return deltaTime;
        }

        /**
         * @return Total time elapsed since the application started.
         */
        float GetTotalTime() const {
            auto currentTime = std::chrono::high_resolution_clock::now();
            return std::chrono::duration<float>(currentTime - m_StartTime).count();
        }

        // --- Flexible Gameplay Timing ---

        void SetValue(float value) { m_CurrentValue = value; }
        float GetValue() const { return m_CurrentValue; }
        
        void Start() { m_IsActive = true; }
        void Stop()  { m_IsActive = false; }
        void Reset() { m_CurrentValue = 0.0f; m_IsActive = false; }

        /**
         * @brief Logic for Countdown (Waktu Mundur)
         */
        void UpdateCountdown(float dt) {
            if (m_IsActive && m_CurrentValue > 0) m_CurrentValue -= dt;
            if (m_CurrentValue < 0) { m_CurrentValue = 0; m_IsActive = false; }
        }

        /**
         * @brief Logic for Stopwatch (Waktu Maju)
         */
        void UpdateStopwatch(float dt) {
            if (m_IsActive) m_CurrentValue += dt;
        }

        bool IsFinished() const { return m_CurrentValue <= 0 && !m_IsActive; }
        bool IsRunning() const { return m_IsActive; }
    };
}

#endif