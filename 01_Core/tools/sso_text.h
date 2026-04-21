#ifndef SSO_TEXT_H
#define SSO_TEXT_H

#include "raylib.h"
#include <string>
#include <vector>

namespace SSO {
    namespace Text {
        inline Font currentFont;
        inline bool isFontLoaded = false;

        inline void LoadFont(const char* fileName) {
            currentFont = ::LoadFont(fileName);
            isFontLoaded = true;
        }

        inline void UnloadFont() {
            if (isFontLoaded) {
                ::UnloadFont(currentFont);
                isFontLoaded = false;
            }
        }

        inline Font GetActiveFont() {
            return isFontLoaded ? currentFont : GetFontDefault();
        }

        inline void DrawShadow(const char* text, int x, int y, int fontSize, Color color, int offset) {
            Vector2 pos = {(float)x, (float)y};
            DrawTextEx(GetActiveFont(), text, {pos.x + offset, pos.y + offset}, (float)fontSize, 1, BLACK);
            DrawTextEx(GetActiveFont(), text, pos, (float)fontSize, 1, color);
        }

        inline void DrawCentered(const char* text, int y, int fontSize, Color color) {
            Vector2 textSize = MeasureTextEx(GetActiveFont(), text, (float)fontSize, 1);
            Vector2 pos = {(float)GetScreenWidth() / 2 - textSize.x / 2, (float)y};
            DrawTextEx(GetActiveFont(), text, pos, (float)fontSize, 1, color);
        }

        inline void DrawCodeBlock(const char* code, int x, int y, int fontSize, Color textColor, Color bgColor) {
            int padding = 10;
            Vector2 textSize = MeasureTextEx(GetActiveFont(), code, (float)fontSize, 1);
            DrawRectangle(x - padding, y - padding, (int)textSize.x + (padding * 2), fontSize + (padding * 2), bgColor);
            DrawRectangleLines(x - padding, y - padding, (int)textSize.x + (padding * 2), fontSize + (padding * 2), DARKGRAY);
            DrawTextEx(GetActiveFont(), code, {(float)x, (float)y}, (float)fontSize, 1, textColor);
        }

        inline void DrawTypewriter(const char* text, int x, int y, int fontSize, float speed, float time, Color color) {
            int length = (int)(time * speed);
            int totalLen = (int)TextLength(text);
            if (length > totalLen) length = totalLen;
            DrawTextEx(GetActiveFont(), TextSubtext(text, 0, length), {(float)x, (float)y}, (float)fontSize, 1, color);
        }

        inline void DrawWrapped(const char* text, int x, int y, int maxWidth, int fontSize, Color color) {
            float spacing = 1.0f;
            int currentY = y;
            std::string textStr = text;
            std::string line = "";
            std::string word = "";
            size_t i = 0;

            while (i <= textStr.length()) {
                if (i < textStr.length() && textStr[i] != ' ') {
                    word += textStr[i];
                } else {
                    std::string testLine = line + (line.empty() ? "" : " ") + word;
                    Vector2 textSize = MeasureTextEx(GetActiveFont(), testLine.c_str(), (float)fontSize, spacing);

                    if (textSize.x > maxWidth) {
                        DrawTextEx(GetActiveFont(), line.c_str(), {(float)x, (float)currentY}, (float)fontSize, spacing, color);
                        line = word;
                        currentY += fontSize + 5;
                    } else {
                        line = testLine;
                    }
                    word = "";
                }
                i++;
            }
            if (!line.empty()) DrawTextEx(GetActiveFont(), line.c_str(), {(float)x, (float)currentY}, (float)fontSize, spacing, color);
        }

        inline void DrawOutline(const char* text, int x, int y, int fontSize, Color color, Color outlineColor) {
            float fSize = (float)fontSize;
            Vector2 pos = {(float)x, (float)y};
            DrawTextEx(GetActiveFont(), text, {pos.x - 1, pos.y - 1}, fSize, 1, outlineColor);
            DrawTextEx(GetActiveFont(), text, {pos.x + 1, pos.y - 1}, fSize, 1, outlineColor);
            DrawTextEx(GetActiveFont(), text, {pos.x - 1, pos.y + 1}, fSize, 1, outlineColor);
            DrawTextEx(GetActiveFont(), text, {pos.x + 1, pos.y + 1}, fSize, 1, outlineColor);
            DrawTextEx(GetActiveFont(), text, pos, fSize, 1, color);
        }

        inline void DrawVertical(const char* text, int x, int y, int fontSize, Color color) {
            int len = TextLength(text);
            for (int i = 0; i < len; i++) {
                DrawTextEx(GetActiveFont(), TextSubtext(text, i, 1), {(float)x, (float)(y + (i * fontSize))}, (float)fontSize, 1, color);
            }
        }

        inline void DrawGradient(const char* text, int x, int y, int fontSize, Color topColor, Color bottomColor) {
            for (int i = 0; i < fontSize; i++) {
                float factor = (float)i / (float)fontSize;
                Color lineCol = {
                    (unsigned char)(topColor.r + (bottomColor.r - topColor.r) * factor),
                    (unsigned char)(topColor.g + (bottomColor.g - topColor.g) * factor),
                    (unsigned char)(topColor.b + (bottomColor.b - topColor.b) * factor),
                    255
                };
                BeginScissorMode(x, y + i, GetScreenWidth(), 1);
                DrawTextEx(GetActiveFont(), text, {(float)x, (float)y}, (float)fontSize, 1, lineCol);
                EndScissorMode();
            }
        }

        inline bool DrawHyperlink(const char* text, int x, int y, int fontSize, Color color, const char* url) {
            Vector2 textSize = MeasureTextEx(GetActiveFont(), text, (float)fontSize, 1);
            Rectangle linkArea = {(float)x, (float)y, textSize.x, textSize.y};
            
            Vector2 mousePos = GetMousePosition();
            float scale = fminf((float)GetScreenWidth() / (float)SSO::Window::virtualWidth, (float)GetScreenHeight() / (float)SSO::Window::virtualHeight);
            
            Vector2 virtualMouse = {
                (mousePos.x - (GetScreenWidth() - (SSO::Window::virtualWidth * scale)) * 0.5f) / scale,
                (mousePos.y - (GetScreenHeight() - (SSO::Window::virtualHeight * scale)) * 0.5f) / scale
            };
            
            bool isHovered = CheckCollisionPointRec(virtualMouse, linkArea);
            
            if (isHovered) {
                Color hoverColor = {
                    (unsigned char)(color.r + (255 - color.r) * 0.3f),
                    (unsigned char)(color.g + (255 - color.g) * 0.3f),
                    (unsigned char)(color.b + (255 - color.b) * 0.3f),
                    color.a
                };
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                DrawTextEx(GetActiveFont(), text, {(float)x, (float)y}, (float)fontSize, 1, hoverColor);
                
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    #ifdef _WIN32
                    std::system(("start \"\" \"" + std::string(url) + "\"").c_str());
                    #else
                    std::system(("xdg-open \"" + std::string(url) + "\"").c_str());
                    #endif
                    return true;
                }
            } else {
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                DrawTextEx(GetActiveFont(), text, {(float)x, (float)y}, (float)fontSize, 1, color);
            }
            
            return false;
        }
    }
}

#endif