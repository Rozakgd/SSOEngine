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

#ifndef SSO_CAMERA_H
#define SSO_CAMERA_H

#include "../include/raylib.h"
#include "../include/raymath.h"

namespace SSO {
    /**
     * @brief High-performance 2D Camera for SSOEngine.
     * Features: Smooth Interpolation, Zoom Control, and Dynamic Screen Shake.
     */
    class Camera {
    private:
        Camera2D m_Camera;
        float m_Smoothing;       // 0.0 to 1.0 (Higher = Stiffer, Lower = Smoother)
        float m_ShakeIntensity;  // Current shake strength
        float m_ShakeDuration;   // Remaining shake time in seconds

    public:
        /**
         * @brief Constructor for the Camera.
         * @param target Initial focal point (usually player center).
         * @param screenWidth Base resolution width.
         * @param screenHeight Base resolution height.
         */
        Camera(Vector2 target, int screenWidth = 1280, int screenHeight = 720) {
            m_Camera.target = target;
            m_Camera.offset = { (float)screenWidth / 2.0f, (float)screenHeight / 2.0f };
            m_Camera.rotation = 0.0f;
            m_Camera.zoom = 1.0f;
            
            m_Smoothing = 0.1f; 
            m_ShakeIntensity = 0.0f;
            m_ShakeDuration = 0.0f;
        }

        /**
         * @brief Updates camera position to follow a target with smoothing and shake effects.
         */
        void Follow(Vector2 targetPos, float dt) {
            // 1. Smooth Linear Interpolation (Lerp) for cinematic movement
            m_Camera.target.x += (targetPos.x - m_Camera.target.x) * m_Smoothing;
            m_Camera.target.y += (targetPos.y - m_Camera.target.y) * m_Smoothing;

            // 2. Screen Shake Logic
            if (m_ShakeDuration > 0) {
                // Apply random offset based on intensity
                m_Camera.offset.x = (1280.0f / 2.0f) + (float)GetRandomValue(-(int)m_ShakeIntensity, (int)m_ShakeIntensity);
                m_Camera.offset.y = (720.0f / 2.0f) + (float)GetRandomValue(-(int)m_ShakeIntensity, (int)m_ShakeIntensity);
                m_ShakeDuration -= dt;
            } else {
                // Reset to standard center offset
                m_Camera.offset = { 1280.0f / 2.0f, 720.0f / 2.0f };
            }
        }

        /**
         * @brief Triggers a Screen Shake effect (Impact/Explosion).
         */
        void Shake(float intensity, float duration) {
            m_ShakeIntensity = intensity;
            m_ShakeDuration = duration;
        }

        // --- Configuration Getters/Setters ---
        void SetZoom(float zoom) { m_Camera.zoom = zoom; }
        float GetZoom() const { return m_Camera.zoom; }
        
        void SetSmoothing(float s) { m_Smoothing = s; }
        float GetSmoothing() const { return m_Smoothing; }

        // --- Engine Lifecycle Integration ---
        void Begin() { BeginMode2D(m_Camera); }
        void End()   { EndMode2D(); }
        
        Camera2D& GetInternalCamera() { return m_Camera; }
    };
}

#endif