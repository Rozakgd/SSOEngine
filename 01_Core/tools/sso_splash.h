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

#ifndef SSO_SPLASH_H
#define SSO_SPLASH_H

#include "raylib.h"

namespace SSO {
    namespace Splash {
        inline void Show(float duration, const char* title = "SSOENGINE", const char* subtitle = "Lightweight C++ Power") {
            float startTime = (float)GetTime();
            
            int titleSize = 60;
            int subSize = 20;
            Font splashFont = GetFontDefault();

            while (GetTime() - startTime < duration) {
                if (WindowShouldClose()) break;

                float elapsed = (float)GetTime() - startTime;
                float alpha = 1.0f;

                if (elapsed < 0.5f) alpha = elapsed / 0.5f;
                else if (elapsed > duration - 0.5f) alpha = (duration - elapsed) / 0.5f;

                BeginDrawing();
                ClearBackground(BLACK);

                Vector2 titleSizeVec = MeasureTextEx(splashFont, title, (float)titleSize, 1.0f);
                DrawTextEx(splashFont, title, 
                    {(GetScreenWidth() - titleSizeVec.x) / 2, (GetScreenHeight() / 2.0f) - 40.0f}, 
                    (float)titleSize, 1.0f, Fade(RAYWHITE, alpha));
                
                Vector2 subSizeVec = MeasureTextEx(splashFont, subtitle, (float)subSize, 1.0f);
                DrawTextEx(splashFont, subtitle, 
                    {(GetScreenWidth() - subSizeVec.x) / 2, (GetScreenHeight() / 2.0f) + 30.0f}, 
                    (float)subSize, 1.0f, Fade(GRAY, alpha));

                float progress = elapsed / duration;
                DrawRectangle(0, GetScreenHeight() - 5, (int)(GetScreenWidth() * progress), 5, Fade(LIME, alpha));

                EndDrawing();
            }
        }
        
        inline void ShowGame(float duration, const char* gameTitle) {
            Show(duration, gameTitle, "Powered by SSOEngine");
        }
    }
}

#endif
