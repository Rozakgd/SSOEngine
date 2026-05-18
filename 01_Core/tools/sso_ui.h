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

#ifndef SSO_UI_H
#define SSO_UI_H

#include "raylib.h"
#include "raygui.h"
#include "sso_window.h"
#include "sso_provider.h"
#include <cmath>   // sinf, fabsf
#include <cstring> // strlen

namespace SSO {
    namespace UI {

        // ======================================================================
        //  HEX COLOR HELPER
        //  Usage: SSO::UI::Hex(0xFF5500FF) -> RGBA Color
        // ======================================================================
        inline Color Hex(unsigned int hexValue) {
            return GetColor(hexValue);
        }

        // ======================================================================
        //  COLOR UTILITIES
        // ======================================================================

        // Linearly interpolate between two colors
        inline Color ColorLerp(Color a, Color b, float t) {
            if (t < 0.0f) t = 0.0f;
            if (t > 1.0f) t = 1.0f;
            return {
                (unsigned char)(a.r + (b.r - a.r) * t),
                (unsigned char)(a.g + (b.g - a.g) * t),
                (unsigned char)(a.b + (b.b - a.b) * t),
                (unsigned char)(a.a + (b.a - a.a) * t)
            };
        }

        // Brighten a color by factor (1.0 = no change, 1.3 = 30% brighter)
        inline Color ColorBrighten(Color c, float factor) {
            auto clamp = [](int v) -> unsigned char { return v > 255 ? 255 : (unsigned char)v; };
            return { clamp((int)(c.r * factor)), clamp((int)(c.g * factor)), clamp((int)(c.b * factor)), c.a };
        }

        // ======================================================================
        //  UI STYLE DEFINITION
        // ======================================================================
        struct UIStyle {
            Color accent;
            Color background;
            Color border;
            Color text;
            float roundness;
            float shadowOpacity;
            int   borderThickness;
        };

        inline UIStyle DefaultStyle = {
            Hex(0x0096FFFF), // SSO Blue
            Hex(0x19191EFF), // Deep Dark
            Hex(0x3C3C46FF), // Steel
            WHITE,
            0.20f, 0.4f, 2
        };

        // ======================================================================
        //  FONT MANAGEMENT
        // ======================================================================
        inline Font JetBrainsFont = {0};
        inline bool FontLoaded    = false;

        inline void LoadUIFont() {
            if (!FontLoaded) {
                JetBrainsFont = SSO::Provider::LoadFontFromBundle("assets.sso", "font/JetBrainsMono-Bold.ttf", 24);
                if (JetBrainsFont.texture.id <= 0) JetBrainsFont = GetFontDefault();
                GuiSetFont(JetBrainsFont);
                FontLoaded = true;
            }
        }

        inline Font GetDefaultFont() {
            if (!FontLoaded) LoadUIFont();
            return JetBrainsFont;
        }

        // ======================================================================
        //  SHADOW — 3-layer for a softer, deeper look
        // ======================================================================
        inline void DrawControlShadow(Rectangle rect, float opacity) {
            if (opacity <= 0.0f) return;
            // Layer 1: Soft wide blur
            DrawRectangleRounded({ rect.x + 4, rect.y + 4, rect.width + 2, rect.height + 2 }, 0.2f, 36, Fade(BLACK, opacity * 0.18f));
            // Layer 2: Mid shadow
            DrawRectangleRounded({ rect.x + 2, rect.y + 3, rect.width + 1, rect.height + 1 }, 0.2f, 36, Fade(BLACK, opacity * 0.35f));
            // Layer 3: Sharp drop shadow
            DrawRectangleRounded({ rect.x + 1, rect.y + 2, rect.width, rect.height },         0.2f, 36, Fade(BLACK, opacity * 0.5f));
        }

        // ======================================================================
        //  GLOW — draws a colored halo around a rectangle
        // ======================================================================
        inline void DrawGlow(Rectangle rect, Color color, float intensity) {
            if (intensity <= 0.0f) return;
            for (int i = 4; i >= 1; i--) {
                float spread = (float)i * 2.5f;
                Rectangle gr = { rect.x - spread, rect.y - spread, rect.width + spread * 2, rect.height + spread * 2 };
                DrawRectangleRounded(gr, 0.3f, 36, Fade(color, intensity * (0.06f * (5 - i))));
            }
        }

        // ======================================================================
        //  BUTTON — gradient fill + glow on hover + gloss shine
        // ======================================================================
        inline bool DrawButton(Rectangle rect, const char* text, Font font, UIStyle style = DefaultStyle) {
            Vector2 mouse    = SSO::Window::GetVirtualMouse();
            bool isHover     = CheckCollisionPointRec(mouse, rect);
            bool isPressed   = isHover && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
            float pressShift = isPressed ? 1.0f : 0.0f;

            // Shadow (suppressed while pressed for "press-down" feel)
            if (!isPressed)
                DrawControlShadow(rect, style.shadowOpacity);

            // Glow on hover
            if (isHover)
                DrawGlow(rect, style.accent, 0.55f);

            // Gradient fill: dark bottom -> accent top
            Color topColor    = isPressed ? ColorLerp(style.accent, BLACK, 0.25f)
                              : isHover   ? style.accent
                                          : ColorLerp(style.background, style.accent, 0.15f);
            Color bottomColor = isPressed ? ColorLerp(style.accent, BLACK, 0.40f)
                              : isHover   ? ColorLerp(style.accent, BLACK, 0.30f)
                                          : style.background;

            // Draw background
            DrawRectangleRounded({rect.x, rect.y + pressShift, rect.width, rect.height}, style.roundness, 36, bottomColor);

            // Gradient simulation: thin top stripe
            Rectangle topStripe = { rect.x + 2, rect.y + pressShift + 2, rect.width - 4, rect.height * 0.5f - 2 };
            DrawRectangleRounded(topStripe, style.roundness, 36, Fade(topColor, 0.7f));

            // Gloss highlight (top inner shine)
            Rectangle gloss = { rect.x + 4, rect.y + pressShift + 3, rect.width - 8, rect.height * 0.28f };
            DrawRectangleRounded(gloss, style.roundness, 36, Fade(WHITE, isHover ? 0.18f : 0.10f));

            // Border
            DrawRectangleRoundedLines({rect.x, rect.y + pressShift, rect.width, rect.height},
                style.roundness, 36, (float)style.borderThickness,
                isHover ? ColorBrighten(style.accent, 1.4f) : style.border);

            // Text
            float   fontSize = rect.height * 0.42f;
            Vector2 size     = MeasureTextEx(font, text, fontSize, 1.2f);
            Vector2 pos      = { rect.x + (rect.width / 2 - size.x / 2),
                                 rect.y + pressShift + (rect.height / 2 - size.y / 2) };

            // Shadow then text
            DrawTextEx(font, text, { pos.x + 1, pos.y + 1 }, fontSize, 1.2f, Fade(BLACK, 0.5f));
            DrawTextEx(font, text, pos, fontSize, 1.2f, style.text);

            return (isHover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON));
        }

        // ======================================================================
        //  PANEL — accent header with subtle inner gradient
        // ======================================================================
        inline void DrawPanel(Rectangle rect, const char* title, Font font, UIStyle style = DefaultStyle) {
            DrawControlShadow(rect, style.shadowOpacity * 1.4f);

            // Body gradient (top slightly lighter)
            DrawRectangleRounded(rect, style.roundness, 36, style.background);
            Rectangle bodyTop = { rect.x + 2, rect.y + 35, rect.width - 4, rect.height * 0.45f };
            DrawRectangleRounded(bodyTop, 0.1f, 36, Fade(WHITE, 0.025f));

            DrawRectangleRoundedLines(rect, style.roundness, 36, (float)style.borderThickness, style.border);

            // Header bar
            Rectangle header = { rect.x, rect.y, rect.width, 35 };
            DrawRectangleRounded(header, style.roundness, 36, Fade(style.accent, 0.92f));
            // Cover rounded bottom corners of header
            DrawRectangle((int)rect.x, (int)(rect.y + 25), (int)rect.width, 10, Fade(style.accent, 0.92f));
            // Gloss on header
            Rectangle hGloss = { rect.x + 4, rect.y + 3, rect.width - 8, 12 };
            DrawRectangleRounded(hGloss, 0.5f, 36, Fade(WHITE, 0.20f));
            // Accent bottom border line
            DrawLineEx({ rect.x, rect.y + 35 }, { rect.x + rect.width, rect.y + 35 }, 1.0f, Fade(WHITE, 0.12f));

            DrawTextEx(font, title, { rect.x + 15, rect.y + 8 }, 18, 1, WHITE);
        }

        // ======================================================================
        //  CARD — borderless floating panel (no header), for info blocks
        // ======================================================================
        inline void DrawCard(Rectangle rect, UIStyle style = DefaultStyle) {
            DrawControlShadow(rect, style.shadowOpacity * 1.2f);
            DrawRectangleRounded(rect, style.roundness * 1.5f, 36, style.background);
            // Subtle top-edge highlight
            DrawRectangleRounded({ rect.x + 1, rect.y + 1, rect.width - 2, 8 }, 0.5f, 36, Fade(WHITE, 0.06f));
            DrawRectangleRoundedLines(rect, style.roundness * 1.5f, 36, (float)style.borderThickness, style.border);
        }

        // ======================================================================
        //  HEALTH / PROGRESS BAR — segmented cells + glow on fill
        // ======================================================================
        inline void DrawHealthBar(Rectangle rect, float current, float max, Color fill, Color bg) {
            float ratio = (max > 0) ? (current / max) : 0;
            if (ratio < 0) ratio = 0;
            if (ratio > 1) ratio = 1;

            // Track
            DrawRectangleRounded(rect, 0.5f, 36, bg);
            DrawRectangleRoundedLines(rect, 0.5f, 36, 1.0f, Fade(WHITE, 0.12f));

            if (ratio > 0.01f) {
                Rectangle fillRect = { rect.x + 2, rect.y + 2, (rect.width - 4) * ratio, rect.height - 4 };

                // Color shift: green -> yellow -> red based on ratio
                Color dynFill = ratio > 0.5f
                    ? ColorLerp({ 255, 200, 0, 255 }, fill, (ratio - 0.5f) * 2.0f)
                    : ColorLerp({ 255, 40, 40, 255 }, { 255, 200, 0, 255 }, ratio * 2.0f);
                (void)dynFill; // use 'fill' param as-is if caller wants custom color

                DrawRectangleRounded(fillRect, 0.5f, 36, fill);

                // Gloss shine
                DrawRectangle((int)fillRect.x, (int)fillRect.y,
                              (int)fillRect.width, (int)(fillRect.height * 0.35f),
                              Fade(WHITE, 0.15f));

                // Segmented dividers (every ~20px)
                int segments = (int)(rect.width / 20);
                for (int i = 1; i < segments; i++) {
                    float sx = rect.x + 2 + (rect.width - 4) / segments * i;
                    if (sx < fillRect.x + fillRect.width)
                        DrawLineEx({ sx, rect.y + 2 }, { sx, rect.y + rect.height - 2 }, 1.0f, Fade(BLACK, 0.20f));
                }

                // Glow at fill edge
                DrawGlow({ fillRect.x + fillRect.width - 6, fillRect.y, 8, fillRect.height }, fill, 0.6f);
            }

            // Outer border
            DrawRectangleRoundedLines(rect, 0.5f, 36, 1.5f, Fade(WHITE, 0.18f));
        }

        // ======================================================================
        //  FANCY PROGRESS BAR — with animated shine sweep
        //  Call each frame; pass GetTime() as 'time' for animation.
        // ======================================================================
        inline void DrawProgressBar(Rectangle rect, float progress, Color fill, Color bg, float time) {
            if (progress < 0) progress = 0;
            if (progress > 1) progress = 1;

            DrawRectangleRounded(rect, 0.5f, 36, bg);

            if (progress > 0.01f) {
                Rectangle fillRect = { rect.x + 2, rect.y + 2, (rect.width - 4) * progress, rect.height - 4 };
                DrawRectangleRounded(fillRect, 0.5f, 36, fill);

                // Animated shine sweep
                float sweepX = fillRect.x + fmodf(time * 80.0f, fillRect.width + 30.0f) - 20.0f;
                Rectangle shine = { sweepX, fillRect.y, 18, fillRect.height };
                BeginScissorMode((int)fillRect.x, (int)fillRect.y, (int)fillRect.width, (int)fillRect.height);
                DrawRectangleRounded(shine, 0.5f, 16, Fade(WHITE, 0.28f));
                EndScissorMode();

                // Gloss
                DrawRectangle((int)fillRect.x, (int)fillRect.y,
                              (int)fillRect.width, (int)(fillRect.height * 0.38f),
                              Fade(WHITE, 0.12f));
            }
            DrawRectangleRoundedLines(rect, 0.5f, 36, 1.5f, Fade(WHITE, 0.18f));
        }

        // ======================================================================
        //  BADGE — small colored label (e.g. score tags, status chips)
        // ======================================================================
        inline void DrawBadge(Vector2 pos, const char* label, Font font, Color bg, Color textColor) {
            float   fs    = 14.0f;
            Vector2 size  = MeasureTextEx(font, label, fs, 1.0f);
            Rectangle rect = { pos.x, pos.y, size.x + 16, size.y + 8 };

            DrawGlow(rect, bg, 0.35f);
            DrawRectangleRounded(rect, 0.6f, 36, bg);
            DrawRectangleRoundedLines(rect, 0.6f, 36, 1.0f, Fade(WHITE, 0.25f));
            // Gloss
            DrawRectangleRounded({ rect.x + 2, rect.y + 2, rect.width - 4, rect.height * 0.4f }, 0.6f, 36, Fade(WHITE, 0.18f));
            DrawTextEx(font, label, { rect.x + 8, rect.y + 4 }, fs, 1.0f, textColor);
        }

        // ======================================================================
        //  TOAST NOTIFICATION
        //  Call once to schedule; DrawToastTick() each frame to render & advance.
        //  Slides in from the bottom-right, auto-dismisses after 'duration' seconds.
        // ======================================================================
        struct ToastState {
            char    msg[128];
            float   duration;
            float   elapsed;
            bool    active;
            Color   accentColor;
        };
        inline ToastState _toast = {};

        inline void ShowToast(const char* msg, float duration = 2.5f, Color accent = {0, 150, 255, 255}) {
            int len = 0;
            while (msg[len] && len < 127) { _toast.msg[len] = msg[len]; len++; }
            _toast.msg[len] = '\0';
            _toast.duration    = duration;
            _toast.elapsed     = 0.0f;
            _toast.active      = true;
            _toast.accentColor = accent;
        }

        inline void DrawToastTick(Font font, float dt) {
            if (!_toast.active) return;
            _toast.elapsed += dt;
            if (_toast.elapsed >= _toast.duration) { _toast.active = false; return; }

            float progress  = _toast.elapsed / _toast.duration;
            // Slide in during first 15%, slide out during last 15%
            float alpha = 1.0f;
            if (progress < 0.15f)      alpha = progress / 0.15f;
            else if (progress > 0.85f) alpha = 1.0f - (progress - 0.85f) / 0.15f;

            float slideY = (1.0f - alpha) * 40.0f;

            float vW  = (float)SSO::Window::virtualWidth;
            float vH  = (float)SSO::Window::virtualHeight;
            float tW  = 280, tH = 52;
            Rectangle rect = { vW - tW - 20, vH - tH - 20 + slideY, tW, tH };

            // Shadow
            DrawControlShadow({ rect.x, rect.y, rect.width, rect.height }, 0.5f * alpha);
            // Bg
            DrawRectangleRounded(rect, 0.4f, 36, Fade(Hex(0x19191EFF), alpha * 0.95f));
            // Accent left strip
            DrawRectangleRounded({ rect.x, rect.y, 5, tH }, 0.4f, 36, Fade(_toast.accentColor, alpha));
            // Border
            DrawRectangleRoundedLines(rect, 0.4f, 36, 1.0f, Fade(_toast.accentColor, alpha * 0.6f));
            // Glow
            DrawGlow(rect, _toast.accentColor, 0.3f * alpha);
            // Text
            float fs   = 16.0f;
            Vector2 ts = MeasureTextEx(font, _toast.msg, fs, 1.0f);
            DrawTextEx(font, _toast.msg,
                { rect.x + 16, rect.y + (tH - ts.y) * 0.5f },
                fs, 1.0f, Fade(WHITE, alpha));

            // Countdown bar at bottom of toast
            DrawProgressBar({ rect.x + 2, rect.y + tH - 4, tW - 4, 3 },
                1.0f - progress,
                Fade(_toast.accentColor, alpha * 0.8f),
                Fade(BLACK, 0.0f),
                _toast.elapsed);
        }

        // ======================================================================
        //  POPUP DIALOG — draggable, with glow border
        // ======================================================================
        inline bool DrawPopUp(Rectangle* rect, const char* title, const char* msg, Font font, bool* active, bool* dragging) {
            if (!*active) return false;

            // Dimmed backdrop
            DrawRectangle(0, 0, SSO::Window::virtualWidth, SSO::Window::virtualHeight, Fade(BLACK, 0.72f));

            Vector2 mouse = SSO::Window::GetVirtualMouse();
            Rectangle headerHit = { rect->x, rect->y, rect->width, 35 };

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, headerHit))
                *dragging = true;
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                *dragging = false;
            if (*dragging) {
                Vector2 delta = GetMouseDelta();
                rect->x += delta.x * ((float)SSO::Window::virtualWidth  / GetScreenWidth());
                rect->y += delta.y * ((float)SSO::Window::virtualHeight / GetScreenHeight());
            }

            // Glow border behind panel
            DrawGlow(*rect, DefaultStyle.accent, 0.45f);
            DrawPanel(*rect, title, font);
            DrawTextEx(font, msg, { rect->x + 20, rect->y + 60 }, 20, 1, LIGHTGRAY);

            if (DrawButton({ rect->x + rect->width / 2 - 60, rect->y + rect->height - 50, 120, 35 }, "OK", font)) {
                *active   = false;
                *dragging = false;
                return true;
            }
            return false;
        }

        // ======================================================================
        //  TEXT HELPERS
        // ======================================================================

        // Shadow text
        inline void DrawTextShadow(Font font, const char* text, Vector2 pos, float size, float spacing, Color color) {
            DrawTextEx(font, text, { pos.x + 1, pos.y + 1 }, size, spacing, Fade(BLACK, 0.55f));
            DrawTextEx(font, text, pos, size, spacing, color);
        }

        // Glowing text (draws blurred halo behind text)
        inline void DrawTextGlow(Font font, const char* text, Vector2 pos, float size, float spacing, Color color, float glowIntensity = 0.5f) {
            for (int i = 3; i >= 1; i--) {
                float o = (float)i;
                DrawTextEx(font, text, { pos.x - o, pos.y }, size, spacing, Fade(color, glowIntensity * 0.12f));
                DrawTextEx(font, text, { pos.x + o, pos.y }, size, spacing, Fade(color, glowIntensity * 0.12f));
                DrawTextEx(font, text, { pos.x, pos.y - o }, size, spacing, Fade(color, glowIntensity * 0.12f));
                DrawTextEx(font, text, { pos.x, pos.y + o }, size, spacing, Fade(color, glowIntensity * 0.12f));
            }
            DrawTextEx(font, text, pos, size, spacing, color);
        }

        // Pulsing text (scale breathing effect via alpha pulse)
        inline void DrawTextPulse(Font font, const char* text, Vector2 pos, float size, float spacing, Color color, float time, float speed = 2.0f) {
            float alpha = 0.65f + 0.35f * sinf(time * speed);
            DrawTextShadow(font, text, pos, size, spacing, Fade(color, alpha));
        }

        // ======================================================================
        //  BACKGROUND & OVERLAY
        // ======================================================================
        inline void DrawBackground(Texture2D tex, Color tint = WHITE) {
            if (tex.id <= 0) { ClearBackground(Hex(0x0A0A0FFF)); return; }
            DrawTexturePro(tex,
                { 0, 0, (float)tex.width, (float)tex.height },
                { 0, 0, (float)SSO::Window::virtualWidth, (float)SSO::Window::virtualHeight },
                { 0, 0 }, 0.0f, tint);
        }

        // Flat dark overlay
        inline void DrawOverlay(float alpha = 0.5f) {
            DrawRectangle(0, 0, SSO::Window::virtualWidth, SSO::Window::virtualHeight, Fade(BLACK, alpha));
        }

        // Vignette overlay (darkens edges, keeps center bright)
        inline void DrawVignette(float strength = 0.55f) {
            int vW = SSO::Window::virtualWidth;
            int vH = SSO::Window::virtualHeight;
            // Simulate with 4 gradient rectangles from each edge
            int depth = (int)(vW * 0.28f);
            for (int i = 0; i < depth; i++) {
                float a = strength * (1.0f - (float)i / depth) * 0.012f;
                DrawRectangle(i, 0, 1, vH, Fade(BLACK, a));
                DrawRectangle(vW - 1 - i, 0, 1, vH, Fade(BLACK, a));
            }
            int hdepth = (int)(vH * 0.28f);
            for (int i = 0; i < hdepth; i++) {
                float a = strength * (1.0f - (float)i / hdepth) * 0.012f;
                DrawRectangle(0, i, vW, 1, Fade(BLACK, a));
                DrawRectangle(0, vH - 1 - i, vW, 1, Fade(BLACK, a));
            }
        }

    } // namespace UI
} // namespace SSO

#endif // SSO_UI_H