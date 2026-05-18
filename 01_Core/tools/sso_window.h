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

#ifndef SSO_WINDOW_H
#define SSO_WINDOW_H

#include "../include/raylib.h"
#include <math.h>

namespace SSO {
    class Window {
    public:
        static inline int virtualWidth = 1280;
        static inline int virtualHeight = 720;
        static inline RenderTexture2D target = { 0 };

        static inline bool showBorder = false;
        static inline Color borderColor = MAROON;
        static inline float borderThickness = 4.0f;
        static inline Texture2D customIcon = { 0 };

        static inline void Init(int width, int height, const char* title) {
            virtualWidth = width;
            virtualHeight = height;

#if defined(PLATFORM_ANDROID)
            SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
            InitWindow(0, 0, NULL);
#else
            SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
            InitWindow(virtualWidth, virtualHeight, title);
            SetWindowMinSize(640, 360);
#endif

            target = LoadRenderTexture(virtualWidth, virtualHeight);
            SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
            SetTargetFPS(60);
        }

        static inline void SetResolution(int width, int height) {
            virtualWidth = width;
            virtualHeight = height;
            if (target.id > 0) UnloadRenderTexture(target);
            target = LoadRenderTexture(virtualWidth, virtualHeight);
            SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
#if !defined(PLATFORM_ANDROID)
            SetWindowSize(virtualWidth, virtualHeight);
#endif
        }

        static inline void DisableResizing() {
#if !defined(PLATFORM_ANDROID)
            ClearWindowState(FLAG_WINDOW_RESIZABLE);
#endif
        }

        static inline void EnableResizing() {
#if !defined(PLATFORM_ANDROID)
            SetWindowState(FLAG_WINDOW_RESIZABLE);
#endif
        }

        static inline void BeginDrawingVirtual() {
            BeginTextureMode(target);
            ClearBackground(BLACK);
        }

        static inline void EndDrawingVirtual() {
            if (showBorder) {
                DrawRectangleLinesEx({ 0, 0, (float)virtualWidth, (float)virtualHeight }, 
                                     borderThickness, borderColor);
            }
            if (customIcon.id > 0) {
                DrawTextureEx(customIcon, { (float)virtualWidth - customIcon.width - 20, 20 }, 0, 1.0f, WHITE);
            }
            EndTextureMode();

            BeginDrawing();
            ClearBackground(BLACK);

            float sw = (float)GetScreenWidth();
            float sh = (float)GetScreenHeight();
            float aspectRatio = (float)virtualWidth / (float)virtualHeight;
            float screenAspectRatio = sw / sh;

            Rectangle sourceRec = { 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height };
            Rectangle destRec;

            if (screenAspectRatio > aspectRatio) {
                float scaledWidth = sh * aspectRatio;
                destRec = {
                    (sw - scaledWidth) * 0.5f,
                    0.0f,
                    scaledWidth,
                    sh
                };
            } else {
                float scaledHeight = sw / aspectRatio;
                destRec = {
                    0.0f,
                    (sh - scaledHeight) * 0.5f,
                    sw,
                    scaledHeight
                };
            }

            DrawTexturePro(target.texture, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);

            EndDrawing();
        }

        static inline Vector2 GetVirtualMouse() {
            float sw = (float)GetScreenWidth();
            float sh = (float)GetScreenHeight();
            float aspectRatio = (float)virtualWidth / (float)virtualHeight;
            float screenAspectRatio = sw / sh;

            Vector2 mouse = GetMousePosition();
            Vector2 vMouse = { 0 };

            Rectangle destRec;
            if (screenAspectRatio > aspectRatio) {
                float scaledWidth = sh * aspectRatio;
                float offsetX = (sw - scaledWidth) * 0.5f;
                
                vMouse.x = (mouse.x - offsetX) / scaledWidth * (float)virtualWidth;
                vMouse.y = mouse.y / sh * (float)virtualHeight;
            } else {
                float scaledHeight = sw / aspectRatio;
                float offsetY = (sh - scaledHeight) * 0.5f;
                
                vMouse.x = mouse.x / sw * (float)virtualWidth;
                vMouse.y = (mouse.y - offsetY) / scaledHeight * (float)virtualHeight;
            }

            if (vMouse.x < 0.0f) vMouse.x = 0.0f;
            if (vMouse.x > (float)virtualWidth) vMouse.x = (float)virtualWidth;
            if (vMouse.y < 0.0f) vMouse.y = 0.0f;
            if (vMouse.y > (float)virtualHeight) vMouse.y = (float)virtualHeight;

            return vMouse;
        }

#if !defined(PLATFORM_ANDROID)
        static void ToggleFull() {
            if (!IsWindowFullscreen()) {
                int monitor = GetCurrentMonitor();
                SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
                ToggleFullscreen();
            } else {
                ToggleFullscreen();
                SetWindowSize(virtualWidth, virtualHeight);
            }
        }
#endif

        static void Close() {
            if (customIcon.id > 0) UnloadTexture(customIcon);
            UnloadRenderTexture(target);
            CloseWindow();
        }
    };
}

#endif
