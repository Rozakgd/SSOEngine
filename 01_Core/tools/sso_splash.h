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
