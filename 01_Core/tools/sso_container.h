#ifndef SSO_CONTAINER_H
#define SSO_CONTAINER_H

#include "raylib.h"
#include "raymath.h"
#include "sso_window.h"
#include "sso_provider.h"
#include <string>
#include <vector>

namespace SSO {
    namespace Container {
        
        // Expanded Theme System with more colors
        enum Theme {
            THEME_LIGHT = 0,
            THEME_DARK = 1,
            THEME_SSO_BLUE = 2,
            THEME_SSO_RED = 3,
            THEME_SKY_BLUE = 4,
            THEME_CRIMSON = 5,
            THEME_EMERALD = 6,
            THEME_SLATE = 7,
            THEME_AMBER = 8,
            THEME_PURPLE = 9,
            THEME_ORANGE = 10,
            THEME_CYAN = 11
        };
        
        struct ThemeColors {
            Color background;
            Color panel;
            Color button;
            Color buttonHover;
            Color buttonPressed;
            Color text;
            Color border;
            Color accent;
        };
        
        // Widget Types
        enum WidgetType {
            WIDGET_BUTTON,
            WIDGET_LABEL,
            WIDGET_CHECKBOX,
            WIDGET_SLIDER,
            WIDGET_TEXTBOX,
            WIDGET_PROGRESSBAR,
            WIDGET_SEPARATOR,
            WIDGET_HEADER
        };
        
        struct Widget {
            WidgetType type;
            std::string label;
            Rectangle bounds;
            bool isActive;
            bool isHover;
            bool isChecked;
            float value;
            std::string text;
            
            Widget(WidgetType t, const std::string& l) 
                : type(t), label(l), isActive(false), isHover(false), isChecked(false), value(0.0f) {}
        };
        
        struct Panel {
            Rectangle area;
            std::vector<Widget> widgets;
            Theme currentTheme;
            float yOffset;
            bool isCollapsed;
            
            Panel(Rectangle a, Theme theme = THEME_DARK) 
                : area(a), currentTheme(theme), yOffset(10), isCollapsed(false) {}
        };
        
        // Global theme management
        inline ThemeColors currentThemeColors;
        inline Theme globalTheme;
        
        // Global font system
        inline Font globalFont;
        inline bool fontLoaded;
        
        // Theme Setup - Expanded with new colors
        inline void SSO_SetTheme(int themeID) {
            globalTheme = (Theme)themeID;
            
            switch (themeID) {
                case THEME_LIGHT:
                    currentThemeColors = {
                        .background = {240, 240, 240, 255},
                        .panel = {255, 255, 255, 255},
                        .button = {200, 200, 200, 255},
                        .buttonHover = {180, 180, 180, 255},
                        .buttonPressed = {160, 160, 160, 255},
                        .text = {0, 0, 0, 255},
                        .border = {150, 150, 150, 255},
                        .accent = {0, 120, 215, 255}
                    };
                    break;
                    
                case THEME_DARK:
                    currentThemeColors = {
                        .background = {30, 30, 30, 255},
                        .panel = {45, 45, 45, 255},
                        .button = {60, 60, 60, 255},
                        .buttonHover = {80, 80, 80, 255},
                        .buttonPressed = {100, 100, 100, 255},
                        .text = {255, 255, 255, 255},
                        .border = {80, 80, 80, 255},
                        .accent = {0, 162, 232, 255}
                    };
                    break;
                    
                case THEME_SSO_BLUE:
                    currentThemeColors = {
                        .background = {15, 25, 35, 255},
                        .panel = {25, 40, 55, 255},
                        .button = {40, 65, 85, 255},
                        .buttonHover = {55, 85, 110, 255},
                        .buttonPressed = {70, 105, 135, 255},
                        .text = {220, 240, 255, 255},
                        .border = {60, 95, 125, 255},
                        .accent = {0, 200, 255, 255}
                    };
                    break;
                    
                case THEME_SSO_RED:
                    currentThemeColors = {
                        .background = {35, 15, 15, 255},
                        .panel = {55, 25, 40, 255},
                        .button = {85, 40, 65, 255},
                        .buttonHover = {110, 55, 85, 255},
                        .buttonPressed = {135, 70, 105, 255},
                        .text = {255, 220, 220, 255},
                        .border = {125, 60, 95, 255},
                        .accent = {255, 0, 100, 255}
                    };
                    break;
                    
                case THEME_SKY_BLUE:
                    currentThemeColors = {
                        .background = {15, 35, 55, 255},
                        .panel = {25, 50, 70, 255},
                        .button = {40, 75, 95, 255},
                        .buttonHover = {55, 95, 120, 255},
                        .buttonPressed = {70, 115, 145, 255},
                        .text = {220, 255, 255, 255},
                        .border = {95, 125, 60, 255},
                        .accent = {0, 150, 255, 255}
                    };
                    break;
                    
                case THEME_CRIMSON:
                    currentThemeColors = {
                        .background = {55, 15, 15, 255},
                        .panel = {70, 25, 40, 255},
                        .button = {95, 40, 65, 255},
                        .buttonHover = {120, 55, 85, 255},
                        .buttonPressed = {145, 70, 105, 255},
                        .text = {255, 220, 220, 255},
                        .border = {125, 60, 95, 255},
                        .accent = {220, 0, 100, 255}
                    };
                    break;
                    
                case THEME_EMERALD:
                    currentThemeColors = {
                        .background = {15, 55, 15, 255},
                        .panel = {25, 70, 40, 255},
                        .button = {40, 95, 65, 255},
                        .buttonHover = {55, 120, 85, 255},
                        .buttonPressed = {70, 145, 105, 255},
                        .text = {220, 255, 220, 255},
                        .border = {60, 125, 95, 255},
                        .accent = {0, 255, 100, 255}
                    };
                    break;
                    
                case THEME_SLATE:
                    currentThemeColors = {
                        .background = {35, 35, 55, 255},
                        .panel = {50, 50, 70, 255},
                        .button = {65, 75, 95, 255},
                        .buttonHover = {85, 95, 120, 255},
                        .buttonPressed = {105, 115, 145, 255},
                        .text = {255, 255, 255, 255},
                        .border = {125, 95, 60, 255},
                        .accent = {100, 150, 255, 255}
                    };
                    break;
                    
                case THEME_AMBER:
                    currentThemeColors = {
                        .background = {55, 35, 15, 255},
                        .panel = {70, 50, 40, 255},
                        .button = {95, 65, 85, 255},
                        .buttonHover = {120, 85, 110, 255},
                        .buttonPressed = {145, 105, 135, 255},
                        .text = {255, 255, 220, 255},
                        .border = {125, 60, 95, 255},
                        .accent = {255, 200, 0, 255}
                    };
                    break;
                    
                case THEME_PURPLE:
                    currentThemeColors = {
                        .background = {35, 15, 55, 255},
                        .panel = {50, 25, 70, 255},
                        .button = {65, 40, 95, 255},
                        .buttonHover = {85, 55, 120, 255},
                        .buttonPressed = {105, 70, 145, 255},
                        .text = {255, 220, 255, 255},
                        .border = {95, 60, 125, 255},
                        .accent = {200, 0, 255, 255}
                    };
                    break;
                    
                case THEME_ORANGE:
                    currentThemeColors = {
                        .background = {55, 35, 15, 255},
                        .panel = {70, 50, 40, 255},
                        .button = {95, 65, 85, 255},
                        .buttonHover = {120, 85, 110, 255},
                        .buttonPressed = {145, 105, 135, 255},
                        .text = {255, 255, 220, 255},
                        .border = {125, 60, 95, 255},
                        .accent = {255, 150, 0, 255}
                    };
                    break;
                    
                case THEME_CYAN:
                    currentThemeColors = {
                        .background = {15, 55, 55, 255},
                        .panel = {25, 70, 70, 255},
                        .button = {40, 95, 95, 255},
                        .buttonHover = {55, 120, 120, 255},
                        .buttonPressed = {70, 145, 145, 255},
                        .text = {255, 255, 255, 255},
                        .border = {60, 125, 95, 255},
                        .accent = {0, 255, 255, 255}
                    };
                    break;
            }
        }
        
        // Panel Management
        inline Panel* currentPanel = nullptr;
        
        // Input scaling and coordinate sync functions
        inline Vector2 GetScaledMousePos() {
            Vector2 rawMouse = GetMousePosition();
            Vector2 virtualMouse = SSO::Window::GetVirtualMouse();
            
            // Use virtual mouse coordinates for proper scaling
            return virtualMouse;
        }
        
        inline Rectangle GetScaledBounds(const Rectangle& originalBounds) {
            float scaleX = (float)GetScreenWidth() / 1280.0f;
            float scaleY = (float)GetScreenHeight() / 720.0f;
            
            return {
                originalBounds.x * scaleX,
                originalBounds.y * scaleY,
                originalBounds.width * scaleX,
                originalBounds.height * scaleY
            };
        }
        
        inline void SSO_BeginPanel(Rectangle area) {
            if (!area.width || !area.height) return;
            
            static Panel panel(area, globalTheme);
            currentPanel = &panel;
            panel.widgets.clear();
            panel.yOffset = 10;
            
            // Draw panel background with proper scaling
            Rectangle scaledArea = GetScaledBounds(area);
            DrawRectangleRec(scaledArea, currentThemeColors.panel);
            DrawRectangleLinesEx(scaledArea, 2.0f, currentThemeColors.border);
        }
        
        // Widget System - Auto-stacking with proper scaling
        inline void SSO_PushWidget(const char* label) {
            if (!currentPanel) return;
            
            Widget widget(WIDGET_BUTTON, label);
            Rectangle scaledBounds = GetScaledBounds({
                currentPanel->area.x + 10,
                currentPanel->area.y + currentPanel->yOffset,
                currentPanel->area.width - 20,
                30
            });
            widget.bounds = scaledBounds;
            
            currentPanel->widgets.push_back(widget);
            currentPanel->yOffset += 40;
        }
        
        inline void SSO_PushLabel(const char* text) {
            if (!currentPanel) return;
            
            Widget widget(WIDGET_LABEL, text);
            Rectangle scaledBounds = GetScaledBounds({
                currentPanel->area.x + 10,
                currentPanel->area.y + currentPanel->yOffset,
                currentPanel->area.width - 20,
                20
            });
            widget.bounds = scaledBounds;
            
            currentPanel->widgets.push_back(widget);
            currentPanel->yOffset += 25;
        }
        
        inline void SSO_PushCheckbox(const char* label, bool checked = false) {
            if (!currentPanel) return;
            
            Widget widget(WIDGET_CHECKBOX, label);
            widget.isChecked = checked;
            Rectangle scaledBounds = GetScaledBounds({
                currentPanel->area.x + 10,
                currentPanel->area.y + currentPanel->yOffset,
                currentPanel->area.width - 20,
                25
            });
            widget.bounds = scaledBounds;
            
            currentPanel->widgets.push_back(widget);
            currentPanel->yOffset += 30;
        }
        
        inline void SSO_PushSlider(const char* label, float value = 0.0f, float min = 0.0f, float max = 100.0f) {
            if (!currentPanel) return;
            
            Widget widget(WIDGET_SLIDER, label);
            widget.value = value;
            Rectangle scaledBounds = GetScaledBounds({
                currentPanel->area.x + 10,
                currentPanel->area.y + currentPanel->yOffset,
                currentPanel->area.width - 20,
                30
            });
            widget.bounds = scaledBounds;
            
            currentPanel->widgets.push_back(widget);
            currentPanel->yOffset += 40;
        }
        
        inline void SSO_PushProgressBar(float value, const char* label = "") {
            if (!currentPanel) return;
            
            Widget widget(WIDGET_PROGRESSBAR, label);
            widget.value = value;
            Rectangle scaledBounds = GetScaledBounds({
                currentPanel->area.x + 10,
                currentPanel->area.y + currentPanel->yOffset,
                currentPanel->area.width - 20,
                25
            });
            widget.bounds = scaledBounds;
            
            currentPanel->widgets.push_back(widget);
            currentPanel->yOffset += 35;
        }
        
        inline void SSO_PushSeparator() {
            if (!currentPanel) return;
            
            Widget widget(WIDGET_SEPARATOR, "");
            Rectangle scaledBounds = GetScaledBounds({
                currentPanel->area.x + 10,
                currentPanel->area.y + currentPanel->yOffset,
                currentPanel->area.width - 20,
                2
            });
            widget.bounds = scaledBounds;
            
            currentPanel->widgets.push_back(widget);
            currentPanel->yOffset += 8;
        }
        
        inline void SSO_PushHeader(const char* text) {
            if (!currentPanel) return;
            
            Widget widget(WIDGET_HEADER, text);
            Rectangle scaledBounds = GetScaledBounds({
                currentPanel->area.x + 10,
                currentPanel->area.y + currentPanel->yOffset,
                currentPanel->area.width - 20,
                30
            });
            widget.bounds = scaledBounds;
            
            currentPanel->widgets.push_back(widget);
            currentPanel->yOffset += 40;
        }
        
        // Widget Interaction with proper input scaling
        inline bool SSO_IsButtonClicked(const char* label) {
            if (!currentPanel) return false;
            
            Vector2 mousePos = GetScaledMousePos();
            for (auto& widget : currentPanel->widgets) {
                if (widget.type == WIDGET_BUTTON && widget.label == label) {
                    bool isHover = CheckCollisionPointRec(mousePos, widget.bounds);
                    return isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
                }
            }
            return false;
        }
        
        inline bool SSO_IsCheckboxChecked(const char* label) {
            if (!currentPanel) return false;
            
            for (auto& widget : currentPanel->widgets) {
                if (widget.type == WIDGET_CHECKBOX && widget.label == label) {
                    return widget.isChecked;
                }
            }
            return false;
        }
        
        inline float SSO_GetSliderValue(const char* label) {
            if (!currentPanel) return 0.0f;
            
            for (auto& widget : currentPanel->widgets) {
                if (widget.type == WIDGET_SLIDER && widget.label == label) {
                    return widget.value;
                }
            }
            return 0.0f;
        }
        
        // Rendering System with proper clipping and state management
        inline void SSO_RenderPanel() {
            if (!currentPanel) return;
            
            Vector2 mousePos = GetScaledMousePos();
            bool isWindowFocused = IsWindowFocused();
            
            // Enable scissor mode for panel clipping with proper scaling
            Rectangle scaledArea = GetScaledBounds(currentPanel->area);
            BeginScissorMode(
                (int)scaledArea.x,
                (int)scaledArea.y,
                (int)scaledArea.width,
                (int)scaledArea.height
            );
            
            for (auto& widget : currentPanel->widgets) {
                bool isHover = CheckCollisionPointRec(mousePos, widget.bounds);
                
                // Reset hover state if window lost focus
                if (!isWindowFocused) {
                    widget.isHover = false;
                    widget.isActive = false;
                } else {
                    widget.isHover = isHover;
                }
                
                switch (widget.type) {
                    case WIDGET_BUTTON:
                        {
                            Color buttonColor = currentThemeColors.button;
                            if (widget.isHover) {
                                buttonColor = currentThemeColors.buttonHover;
                                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                                    buttonColor = currentThemeColors.buttonPressed;
                                    widget.isActive = true;
                                } else {
                                    widget.isActive = false;
                                }
                            } else {
                                widget.isActive = false;
                            }
                            
                            DrawRectangleRec(widget.bounds, buttonColor);
                            DrawRectangleLinesEx(widget.bounds, 1.0f, currentThemeColors.border);
                            
                            int textWidth = MeasureText(widget.label.c_str(), 10);
                            Font fontToUse = fontLoaded ? globalFont : GetFontDefault();
                            Vector2 textSize = MeasureTextEx(fontToUse, widget.label.c_str(), 12.0f, 1.0f);
                            DrawTextEx(fontToUse, widget.label.c_str(), 
                                   {widget.bounds.x + (widget.bounds.width - textSize.x) / 2,
                                    widget.bounds.y + (widget.bounds.height - textSize.y) / 2},
                                   12.0f, 1.0f, currentThemeColors.text);
                        }
                        break;
                        
                    case WIDGET_LABEL:
                        {
                            Font fontToUse = fontLoaded ? globalFont : GetFontDefault();
                            DrawTextEx(fontToUse, widget.label.c_str(), 
                                   {widget.bounds.x, widget.bounds.y},
                                   10.0f, 1.0f, currentThemeColors.text);
                        }
                        break;
                        
                    case WIDGET_CHECKBOX:
                        {
                            Rectangle checkRect = widget.bounds;
                            DrawRectangleRec(checkRect, currentThemeColors.button);
                            DrawRectangleLinesEx(checkRect, 1.0f, currentThemeColors.border);
                            
                            if (widget.isChecked) {
                                Font fontToUse = fontLoaded ? globalFont : GetFontDefault();
                                DrawTextEx(fontToUse, "✓", {checkRect.x + 2, checkRect.y}, 12.0f, 1.0f, currentThemeColors.accent);
                            }
                            
                            if (widget.isHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                                widget.isChecked = !widget.isChecked;
                            }
                            
                            Font fontToUse = fontLoaded ? globalFont : GetFontDefault();
                            DrawTextEx(fontToUse, widget.label.c_str(),
                                   {checkRect.x + 25, checkRect.y + 2},
                                   10.0f, 1.0f, currentThemeColors.text);
                        }
                        break;
                        
                    case WIDGET_SLIDER:
                        {
                            Rectangle sliderTrack = widget.bounds;
                            sliderTrack.y += 10;
                            sliderTrack.height = 10;
                            sliderTrack.width -= 40;
                            
                            Rectangle sliderThumb = {
                                sliderTrack.x + (widget.value / 100.0f) * (sliderTrack.width - 20),
                                sliderTrack.y - 5,
                                20, 20
                            };
                            
                            DrawRectangleRec(sliderTrack, currentThemeColors.button);
                            DrawRectangleRec(sliderThumb, widget.isHover ? currentThemeColors.accent : currentThemeColors.buttonHover);
                            
                            if (widget.isHover && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                                Vector2 mousePos = GetScaledMousePos();
                                float relativeX = mousePos.x - sliderTrack.x;
                                widget.value = (relativeX / sliderTrack.width) * 100.0f;
                                widget.value = Clamp(widget.value, 0.0f, 100.0f);
                            }
                            
                            Font fontToUse = fontLoaded ? globalFont : GetFontDefault();
                            DrawTextEx(fontToUse, widget.label.c_str(),
                                   {widget.bounds.x, widget.bounds.y},
                                   10.0f, 1.0f, currentThemeColors.text);
                        }
                        break;
                        
                    case WIDGET_PROGRESSBAR:
                        {
                            DrawRectangleRec(widget.bounds, currentThemeColors.button);
                            DrawRectangleLinesEx(widget.bounds, 1.0f, currentThemeColors.border);
                            
                            Rectangle fillRect = widget.bounds;
                            fillRect.x += 2;
                            fillRect.y += 2;
                            fillRect.width -= 4;
                            fillRect.height -= 4;
                            fillRect.width = (fillRect.width * widget.value) / 100.0f;
                            fillRect.height = fillRect.height;
                            DrawRectangleRec(fillRect, currentThemeColors.accent);
                            
                            if (!widget.label.empty()) {
                                Font fontToUse = fontLoaded ? globalFont : GetFontDefault();
                                Vector2 textSize = MeasureTextEx(fontToUse, widget.label.c_str(), 8.0f, 1.0f);
                                DrawTextEx(fontToUse, widget.label.c_str(),
                                       {widget.bounds.x + 5, widget.bounds.y + (widget.bounds.height - textSize.y) / 2},
                                       8.0f, 1.0f, currentThemeColors.text);
                            }
                        }
                        break;
                        
                    case WIDGET_SEPARATOR:
                        {
                            float centerX = widget.bounds.x + widget.bounds.width / 2;
                            DrawLine(widget.bounds.x, widget.bounds.y + 1, centerX - 10, widget.bounds.y + 1, currentThemeColors.border);
                            DrawLine(centerX + 10, widget.bounds.y + 1, widget.bounds.x + widget.bounds.width, widget.bounds.y + 1, currentThemeColors.border);
                        }
                        break;
                        
                    case WIDGET_HEADER:
                        {
                            Color headerBg = {
                                (unsigned char)(currentThemeColors.panel.r * 0.8f),
                                (unsigned char)(currentThemeColors.panel.g * 0.8f),
                                (unsigned char)(currentThemeColors.panel.b * 0.8f),
                                255
                            };
                            DrawRectangleRec(widget.bounds, headerBg);
                            DrawRectangleLinesEx(widget.bounds, 1.0f, currentThemeColors.border);
                            
                            Font fontToUse = fontLoaded ? globalFont : GetFontDefault();
                            DrawTextEx(fontToUse, widget.label.c_str(),
                                   {widget.bounds.x + 8, widget.bounds.y + 8},
                                   14.0f, 1.0f, currentThemeColors.accent);
                        }
                        break;
                }
            }
            
            // Disable scissor mode
            EndScissorMode();
        }
        
        // Classic Game Drawing Functions (Jalur Game)
        inline void SSO_DrawSprite(Texture2D texture, Vector2 pos, Color tint = WHITE) {
            DrawTextureV(texture, pos, tint);
        }
        
        inline void SSO_DrawRectangle(Rectangle rect, Color color) {
            DrawRectangleRec(rect, color);
        }
        
        inline void SSO_DrawRectangleLines(Rectangle rect, Color color) {
            DrawRectangleLinesEx(rect, 1.0f, color);
        }
        
        inline void SSO_DrawCircle(Vector2 center, float radius, Color color) {
            DrawCircleV(center, radius, color);
        }
        
        inline void SSO_DrawCircleLines(Vector2 center, float radius, Color color) {
            DrawCircleLines(center.x, center.y, radius, color);
        }
        
        inline void SSO_DrawText(const char* text, Vector2 pos, int fontSize, Color color) {
            DrawText(text, pos.x, pos.y, fontSize, color);
        }
        
        inline void SSO_DrawLine(Vector2 start, Vector2 end, Color color) {
            DrawLineV(start, end, color);
        }
        
        // Font initialization with proper memory management
        inline void SSO_InitFont() {
            if (fontLoaded) {
                UnloadFont(globalFont);
                fontLoaded = false;
            }
            
            try {
                globalFont = SSO::Provider::LoadFontFromBundle("assets.sso", "font/JetBrainsMono-Regular.ttf", 12);
                fontLoaded = (globalFont.texture.id > 0);
            } catch (...) {
                globalFont = GetFontDefault();
                fontLoaded = false;
            }
            
            if (fontLoaded) {
                TraceLog(LOG_INFO, "SSO_CONTAINER: JetBrains Mono font loaded successfully");
            } else {
                TraceLog(LOG_WARNING, "SSO_CONTAINER: Failed to load JetBrains Mono, using default font");
            }
        }
        
        // Initialize theme system
        inline void SSO_InitContainer() {
            SSO_SetTheme(THEME_DARK);
            SSO_InitFont();
        }
        
        // Cleanup font with proper memory management
        inline void SSO_CleanupFont() {
            if (fontLoaded) {
                UnloadFont(globalFont);
                fontLoaded = false;
            }
        }
    }
}

#endif // SSO_CONTAINER_H
