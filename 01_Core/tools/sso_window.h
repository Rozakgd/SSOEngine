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

        static void Init(int width, int height, const char* title) {
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
            SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
            SetTargetFPS(60);
        }

        static void SetResolution(int width, int height) {
            virtualWidth = width;
            virtualHeight = height;
            if (target.id > 0) UnloadRenderTexture(target);
            target = LoadRenderTexture(virtualWidth, virtualHeight);
            SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
#if !defined(PLATFORM_ANDROID)
            SetWindowSize(virtualWidth, virtualHeight);
#endif
        }

        static void DisableResizing() {
#if !defined(PLATFORM_ANDROID)
            ClearWindowState(FLAG_WINDOW_RESIZABLE);
#endif
        }

        static void EnableResizing() {
#if !defined(PLATFORM_ANDROID)
            SetWindowState(FLAG_WINDOW_RESIZABLE);
#endif
        }

        static void BeginDrawingVirtual() {
            BeginTextureMode(target);
            ClearBackground(BLACK);
        }

        static void EndDrawingVirtual() {
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
            float scale = fminf(sw / (float)virtualWidth, sh / (float)virtualHeight);

            Rectangle destRec = {
                (sw - ((float)virtualWidth * scale)) * 0.5f,
                (sh - ((float)virtualHeight * scale)) * 0.5f,
                (float)virtualWidth * scale, 
                (float)virtualHeight * scale
            };

            DrawTexturePro(target.texture,
                { 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                destRec, { 0, 0 }, 0.0f, WHITE);

            EndDrawing();
        }

        static Vector2 GetVirtualMouse() {
            float sw = (float)GetScreenWidth();
            float sh = (float)GetScreenHeight();
            float scale = fminf(sw / (float)virtualWidth, sh / (float)virtualHeight);

            Vector2 mouse = GetMousePosition();
            Vector2 vMouse = { 0 };

            vMouse.x = (mouse.x - (sw - ((float)virtualWidth * scale)) * 0.5f) / scale;
            vMouse.y = (mouse.y - (sh - ((float)virtualHeight * scale)) * 0.5f) / scale;

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