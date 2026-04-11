#ifndef SSO_UI_H
#define SSO_UI_H

#include "raylib.h"
#include "sso_window.h"
#include <vector>
#include <string>
#include <math.h> 

namespace SSO {
    namespace UI {

        struct UIStyle {
            Color baseColor;
            Color hoverColor;
            Color textColor;
            Color borderColor;
        };

        inline UIStyle DefaultButtonStyle = { { 45, 45, 48, 255 }, { 65, 65, 70, 255 }, WHITE, { 100, 100, 110, 255 } };
        inline UIStyle PanelStyle = { { 30, 30, 35, 250 }, { 30, 30, 35, 250 }, WHITE, { 70, 70, 80, 255 } };

        inline void DrawBackground(Texture2D tex, Color tint = WHITE) {
            if (tex.id <= 0 || tex.width <= 0 || tex.height <= 0) {
                ClearBackground({ 15, 15, 20, 255 });
                return;
            }
            if (!IsTextureValid(tex)) {
                ClearBackground({ 15, 15, 20, 255 });
                return;
            }
            DrawTexturePro(tex, { 0, 0, (float)tex.width, (float)tex.height }, 
                           { 0, 0, (float)SSO::Window::virtualWidth, (float)SSO::Window::virtualHeight }, 
                           { 0, 0 }, 0.0f, tint);
        }

        inline void DrawOverlay(float alpha = 0.5f) {
            DrawRectangle(0, 0, SSO::Window::virtualWidth, SSO::Window::virtualHeight, Fade(BLACK, alpha));
        }

        inline void DrawTextShadow(Font font, const char* text, Vector2 pos, float size, float spacing, Color color) {
            DrawTextEx(font, text, { pos.x + 2, pos.y + 2 }, size, spacing, BLACK);
            DrawTextEx(font, text, pos, size, spacing, color);
        }

        inline bool DrawButton(Rectangle rect, const char* text, Font font, UIStyle style = DefaultButtonStyle) {
            Vector2 mouse = SSO::Window::GetVirtualMouse();
            bool hovering = CheckCollisionPointRec(mouse, rect);
            bool pressed = hovering && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
            bool clicked = hovering && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
            
            Color bg;
            if (pressed) {
                bg = { (unsigned char)(style.baseColor.r - 10), (unsigned char)(style.baseColor.g - 10), (unsigned char)(style.baseColor.b - 10), style.baseColor.a };
            } else if (hovering) {
                bg = style.hoverColor;
            } else {
                bg = style.baseColor;
            }
            Color border = hovering ? SKYBLUE : style.borderColor;

            DrawRectangleRec(rect, bg);
            DrawRectangleLinesEx(rect, 1, border);

            float fontSize = rect.height * 0.4f;
            Vector2 textSize = MeasureTextEx(font, text, fontSize, 1);
            Vector2 textPos = { rect.x + (rect.width / 2) - (textSize.x / 2), rect.y + (rect.height / 2) - (textSize.y / 2) };
            
            DrawTextEx(font, text, textPos, fontSize, 1, style.textColor);
            return clicked;
        }

        inline void DrawPanel(Rectangle rect, const char* title, Font font, UIStyle style = PanelStyle) {
            DrawRectangleRec(rect, style.baseColor);
            DrawRectangleLinesEx(rect, 1, style.borderColor);
            Rectangle header = { rect.x, rect.y, rect.width, 30 };
            DrawRectangleRec(header, { 100, 30, 45, 255 }); 
            DrawRectangleLinesEx(header, 1, style.borderColor);
            DrawTextEx(font, title, { rect.x + 10, rect.y + 7 }, 16, 1, WHITE);
        }

        inline void BeginScrollArea(Rectangle rect, float* scrollPos, float contentHeight) {
            DrawRectangleRec(rect, { 25, 25, 30, 255 });
            *scrollPos += GetMouseWheelMove() * 30.0f;
            
            if (*scrollPos > 0) *scrollPos = 0;
            if (contentHeight > rect.height) {
                if (*scrollPos < -(contentHeight - rect.height)) *scrollPos = -(contentHeight - rect.height);
            } else {
                *scrollPos = 0;
            }

            BeginScissorMode((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height);
        }

        inline void EndScrollArea() {
            EndScissorMode();
        }

        inline void HandleDrag(Rectangle* rect, bool* dragging) {
            Vector2 mouse = SSO::Window::GetVirtualMouse();
            Rectangle header = { rect->x, rect->y, rect->width, 30 };
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, header)) *dragging = true;
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) *dragging = false;
            
            if (*dragging) {
                Vector2 delta = GetMouseDelta();
                rect->x += delta.x;
                rect->y += delta.y;
            }
        }

        inline bool DrawPopUp(Rectangle* rect, const char* title, const char* msg, Font font, bool* active, bool* dragging) {
            if (!*active) return false;
            DrawOverlay(0.6f);
            HandleDrag(rect, dragging);
            
            DrawPanel(*rect, title, font);
            DrawTextEx(font, msg, { rect->x + 20, rect->y + 60 }, 18, 1, LIGHTGRAY);
            
            Rectangle btn = { rect->x + rect->width/2 - 50, rect->y + rect->height - 45, 100, 30 };
            if (DrawButton(btn, "OK", font)) {
                *active = false;
                *dragging = false;
                return true;
            }
            return false;
        }

        inline void DrawHealthBar(Rectangle rect, float current, float max, Color fill, Color bg) {
            float perc = (max > 0) ? (current / max) : 0;
            if (perc < 0) perc = 0; if (perc > 1) perc = 1;

            DrawRectangleRec(rect, bg);
            if (perc > 0) {
                DrawRectangleRec({ rect.x + 1, rect.y + 1, (rect.width - 2) * perc, rect.height - 2 }, fill);
            }
            DrawRectangleLinesEx(rect, 1, Fade(WHITE, 0.2f));
        }
    }
}

#endif