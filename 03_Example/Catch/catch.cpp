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

#include "raylib.h"
#include <stdlib.h>

// RayGUI Implementation & Float Fallback
#ifndef TextToFloat
#define TextToFloat(text) (float)atof(text)
#endif
#define RAYGUI_IMPLEMENTATION

// Engine Includes
#include "tools/sso_ui.h"
#include "tools/sso_window.h"
#include "tools/sso_camera.h"
#include "tools/sso_timer.h"
#include "tools/sso_text.h"
#include "tools/sso_splash.h"

// ============================================================
//  STRUCTS
// ============================================================
struct Ball {
    Vector2 position;
    Vector2 velocity;
    Color   color;
    bool    active;
    float   trailAlpha; // for fade trail effect
};

struct Player {
    Rectangle rect;
    Vector2   velocity;
    float     speed;
    float     flash;     // white flash on catch
    float     catchPulse;// scale pulse on catch
};

struct Star {
    Vector2 pos;
    float   speed;
    float   bright;
    float   size;
};

struct Particle {
    Vector2 pos;
    Vector2 vel;
    Color   color;
    float   life;
    float   maxLife;
    bool    active;
};

// ============================================================
//  PARTICLE SYSTEM
// ============================================================
static Particle particles[128] = {};

static void SpawnParticles(Vector2 origin, Color color, int count = 10) {
    for (int i = 0; i < count; i++) {
        for (auto& p : particles) {
            if (!p.active) {
                p.pos     = origin;
                p.vel     = { (float)GetRandomValue(-120, 120), (float)GetRandomValue(-160, -40) };
                p.color   = color;
                p.maxLife = (float)GetRandomValue(30, 60) / 100.0f;
                p.life    = p.maxLife;
                p.active  = true;
                break;
            }
        }
    }
}

static void UpdateDrawParticles(float dt) {
    for (auto& p : particles) {
        if (!p.active) continue;
        p.life -= dt;
        if (p.life <= 0) { p.active = false; continue; }
        p.vel.y += 280.0f * dt; // gravity
        p.pos.x += p.vel.x * dt;
        p.pos.y += p.vel.y * dt;
        float alpha = p.life / p.maxLife;
        float r     = 4.0f * alpha;
        DrawCircleV(p.pos, r, Fade(p.color, alpha));
    }
}

// ============================================================
//  MAIN
// ============================================================
int main() {
    // 1. Initialization
    SSO::Window::Init(1280, 720, "SSO-Catch Pro v1.8");
    SSO::Splash::Show(2.0f);
    Font gameFont = SSO::UI::GetDefaultFont();

    // 2. Custom Styles
    SSO::UI::UIStyle PlayBtn    = SSO::UI::DefaultStyle;
    PlayBtn.accent              = SSO::UI::Hex(0x00E676FF); // Emerald Green
    PlayBtn.background          = SSO::UI::Hex(0x0A2D1EFF); // Dark Forest
    PlayBtn.roundness           = 0.4f;

    SSO::UI::UIStyle ExitBtn    = SSO::UI::DefaultStyle;
    ExitBtn.accent              = SSO::UI::Hex(0xFF2E5BFF); // Crimson Red
    ExitBtn.background          = SSO::UI::Hex(0x2D0A0FFF); // Dark Wine
    ExitBtn.roundness           = 0.4f;

    SSO::UI::UIStyle HUDStyle   = SSO::UI::DefaultStyle;
    HUDStyle.background         = SSO::UI::Hex(0x141419CC); // Transparent Slate
    HUDStyle.accent             = SSO::UI::Hex(0x0096FFFF); // SSO Blue

    SSO::UI::UIStyle ScoreCard  = SSO::UI::DefaultStyle;
    ScoreCard.background        = SSO::UI::Hex(0x0D0D12EE);
    ScoreCard.border            = SSO::UI::Hex(0xFFD700AA);
    ScoreCard.roundness         = 0.35f;

    // 3. Game Variables
    Player player   = { {580, 640, 130, 25}, {0, 0}, 1600.0f, 0, 0 };
    Ball   balls[10]= {};
    Star   stars[80];
    int    score    = 0, lives = 3, highScore = 0, combo = 0;
    float  comboTimer   = 0.0f;
    bool   inMenu       = true;
    bool   inGame       = false;
    bool   gameOver     = false;
    bool   dragging     = false;
    float  spawnTimer   = 0;
    float  spawnRate    = 0.85f;  // decreases over time (gets harder)
    float  diffTimer    = 0.0f;
    float  gameTime     = 0.0f;
    float  menuTime     = 0.0f;
    bool   toastShown   = false;  // guard for one-shot toasts

    for (int i = 0; i < 80; i++) {
        stars[i] = {
            { (float)GetRandomValue(0, 1280), (float)GetRandomValue(0, 720) },
            (float)GetRandomValue(30, 110),
            (float)GetRandomValue(8, 25) / 25.0f,
            (float)GetRandomValue(1, 3)
        };
    }

    // --------------------------------------------------------
    //  MAIN LOOP
    // --------------------------------------------------------
    while (!WindowShouldClose()) {
        float dt      = GetFrameTime();
        float curTime = (float)GetTime();

        SSO::Window::BeginDrawingVirtual();
        SSO::UI::DrawBackground({0});

        // ---- STARFIELD (shared across all screens) ----
        for (int i = 0; i < 80; i++) {
            stars[i].pos.y += stars[i].speed * dt;
            if (stars[i].pos.y > 730) {
                stars[i].pos.y = -10;
                stars[i].pos.x = (float)GetRandomValue(0, 1280);
            }
            float twinkle = 0.5f + 0.5f * sinf(curTime * stars[i].speed * 0.08f + i);
            DrawCircleV(stars[i].pos, stars[i].size * 0.6f, Fade(WHITE, stars[i].bright * twinkle));
        }

        // ============================================================
        //  MENU SCREEN
        // ============================================================
        if (inMenu) {
            menuTime += dt;

            // Vignette for atmosphere
            SSO::UI::DrawVignette(0.7f);

            // Animated title with glow
            float titleY = 150.0f + sinf(menuTime * 1.2f) * 5.0f;
            SSO::UI::DrawTextGlow(gameFont, "SSO-CATCH PRO", {390, titleY}, 72, 2.0f,
                                  SSO::UI::Hex(0x0096FFFF), 0.8f);
            SSO::Text::DrawGradient("SSO-CATCH PRO", 390, (int)titleY, 72, SKYBLUE, BLUE);

            // Subtitle pulse
            SSO::UI::DrawTextPulse(gameFont, "THE PRO-SHARP UPDATE v1.8",
                                   {450, titleY + 85}, 20, 1, GRAY, menuTime, 1.8f);

            // High Score badge under subtitle
            if (highScore > 0) {
                char hsBuf[32];
                TextFormat("BEST: %04d", highScore);
                SSO::UI::DrawBadge({548, titleY + 120},
                                   TextFormat("BEST: %04d", highScore),
                                   gameFont,
                                   SSO::UI::Hex(0xFFD700CC),
                                   BLACK);
            }

            // Buttons
            if (SSO::UI::DrawButton({540, 330, 200, 60}, "PLAY GAME", gameFont, PlayBtn)) {
                inMenu     = false;
                inGame     = true;
                score      = 0;
                lives      = 3;
                combo      = 0;
                spawnRate  = 0.85f;
                diffTimer  = 0.0f;
                gameTime   = 0.0f;
                toastShown = false;
                for (auto& b : balls) b.active = false;
                for (auto& p : particles) p.active = false;
            }
            if (SSO::UI::DrawButton({540, 410, 200, 60}, "EXIT", gameFont, ExitBtn)) break;

            // How-to card
            SSO::UI::DrawCard({395, 490, 490, 100}, ScoreCard);
            SSO::UI::DrawTextShadow(gameFont, "A / D  or  LEFT / RIGHT  to move", {415, 506}, 17, 1, LIGHTGRAY);
            SSO::UI::DrawTextShadow(gameFont, "Catch the orbs — don't miss any!", {415, 534}, 17, 1, LIGHTGRAY);
            SSO::UI::DrawTextShadow(gameFont, "Combo x2, x3... for bonus points", {415, 562}, 17, 1, LIGHTGRAY);

            SSO::Text::DrawHyperlink("GITHUB: ROZAQ125", 535, 668, 18, SKYBLUE, "https://github.com/Rozaq125");
        }

        // ============================================================
        //  GAMEPLAY SCREEN
        // ============================================================
        if (inGame && !gameOver) {
            gameTime  += dt;
            diffTimer += dt;

            // Gradually increase difficulty every 10 seconds
            if (diffTimer >= 10.0f) {
                diffTimer = 0.0f;
                if (spawnRate > 0.35f) spawnRate -= 0.08f;
                if (!toastShown && spawnRate < 0.50f) {
                    SSO::UI::ShowToast("Speed UP!", 2.0f, SSO::UI::Hex(0xFF9800FF));
                    toastShown = false; // allow next toast
                }
            }

            // Combo timer decay
            if (combo > 0) {
                comboTimer -= dt;
                if (comboTimer <= 0) { combo = 0; }
            }

            // ---- PLAYER INPUT ----
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  player.velocity.x -= player.speed * dt;
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) player.velocity.x += player.speed * dt;
            player.velocity.x *= 0.92f;
            player.rect.x     += player.velocity.x * dt;
            if (player.rect.x < 0)                        player.rect.x = 0;
            if (player.rect.x > 1280 - player.rect.width) player.rect.x = 1280 - player.rect.width;

            // ---- SPAWN ORBS ----
            spawnTimer += dt;
            if (spawnTimer > spawnRate) {
                for (auto& b : balls) {
                    if (!b.active) {
                        b.active     = true;
                        b.position   = { (float)GetRandomValue(40, 1240), -30 };
                        b.velocity   = { 0, (float)GetRandomValue(350, 600) };
                        b.color      = (GetRandomValue(0, 1))
                                       ? SSO::UI::Hex(0x00E676FF)   // Green orb
                                       : SSO::UI::Hex(0xFFD700FF);  // Gold orb
                        b.trailAlpha = 0.0f;
                        spawnTimer   = 0;
                        break;
                    }
                }
            }

            // ---- UPDATE & DRAW ORBS ----
            for (auto& b : balls) {
                if (!b.active) continue;
                b.position.y += b.velocity.y * dt;

                // Trail
                for (int t = 1; t <= 4; t++) {
                    float ty    = b.position.y - b.velocity.y * dt * t * 0.3f;
                    float alpha = 0.18f - t * 0.04f;
                    DrawCircleV({ b.position.x, ty }, 15 - t * 2.0f, Fade(b.color, alpha));
                }

                // Catch collision
                if (CheckCollisionCircleRec(b.position, 15, player.rect)) {
                    b.active       = false;
                    combo++;
                    comboTimer     = 2.5f;
                    int pts        = 10 * (combo >= 3 ? 3 : combo >= 2 ? 2 : 1);
                    score         += pts;
                    player.flash   = 0.12f;
                    player.catchPulse = 0.12f;

                    SpawnParticles(b.position, b.color, 12);

                    if (combo == 2) SSO::UI::ShowToast("COMBO x2!", 1.5f, SSO::UI::Hex(0x00E676FF));
                    if (combo == 3) SSO::UI::ShowToast("COMBO x3! MAX!", 2.0f, SSO::UI::Hex(0xFFD700FF));
                }
                // Missed
                else if (b.position.y > 760) {
                    b.active = false;
                    lives--;
                    combo    = 0;
                    SSO::UI::ShowToast("Missed!", 1.2f, SSO::UI::Hex(0xFF2E5BFF));
                    if (lives <= 0) gameOver = true;
                }

                // Draw orb + glow
                if (b.active) {
                    SSO::UI::DrawGlow({ b.position.x - 15, b.position.y - 15, 30, 30 }, b.color, 0.5f);
                    DrawCircleV(b.position, 15, b.color);
                    DrawCircleV({ b.position.x - 4, b.position.y - 4 }, 5, Fade(WHITE, 0.35f)); // gloss
                    DrawCircleLines((int)b.position.x, (int)b.position.y, 16, Fade(WHITE, 0.5f));
                }
            }

            // ---- PARTICLES ----
            UpdateDrawParticles(dt);

            // ---- PLAYER RENDER ----
            float pw = player.rect.width  * (1.0f + player.catchPulse * 0.15f);
            float ph = player.rect.height * (1.0f + player.catchPulse * 0.15f);
            Rectangle drawRect = {
                player.rect.x - (pw - player.rect.width) * 0.5f,
                player.rect.y - (ph - player.rect.height) * 0.5f,
                pw, ph
            };
            Color playerColor = player.flash > 0
                ? WHITE
                : SSO::UI::Hex(0x0096FFFF);

            SSO::UI::DrawGlow(drawRect, playerColor, 0.4f);
            DrawRectangleRounded(drawRect, 0.4f, 36, playerColor);
            // Gloss on paddle
            DrawRectangleRounded(
                { drawRect.x + 4, drawRect.y + 3, drawRect.width - 8, drawRect.height * 0.4f },
                0.4f, 36, Fade(WHITE, 0.20f));
            DrawRectangleRoundedLines(drawRect, 0.4f, 36, 2, Fade(WHITE, 0.6f));

            if (player.flash       > 0) player.flash        -= dt;
            if (player.catchPulse  > 0) player.catchPulse   -= dt * 3.0f;

            // ---- HUD PANEL ----
            SSO::UI::DrawPanel({1010, 20, 250, 130}, "DASHBOARD", gameFont, HUDStyle);

            // Score with glow when high
            Color scoreColor = score >= 100 ? SSO::UI::Hex(0xFFD700FF) : SSO::UI::Hex(0x0096FFFF);
            if (score >= 100)
                SSO::UI::DrawTextGlow(gameFont, TextFormat("SCORE: %04d", score),
                                      {1030, 60}, 22, 1.0f, scoreColor, 0.6f);
            else
                SSO::UI::DrawTextShadow(gameFont, TextFormat("SCORE: %04d", score),
                                        {1030, 60}, 22, 1, scoreColor);

            // Lives bar
            SSO::UI::DrawHealthBar({1030, 95, 210, 18}, (float)lives, 3.0f,
                                   SSO::UI::Hex(0xFF2E5BFF), SSO::UI::Hex(0x320000FF));

            // Combo badge
            if (combo >= 2) {
                Color comboColor = combo >= 3 ? SSO::UI::Hex(0xFFD700FF) : SSO::UI::Hex(0x00E676FF);
                SSO::UI::DrawBadge({1030, 120}, TextFormat("COMBO x%d", combo),
                                   gameFont, comboColor, BLACK);
            }

            // Difficulty progress bar (top of screen, thin)
            float diffProgress = 1.0f - ((spawnRate - 0.35f) / 0.50f);
            if (diffProgress < 0) diffProgress = 0; if (diffProgress > 1) diffProgress = 1;
            SSO::UI::DrawProgressBar({500, 8, 280, 8}, diffProgress,
                                     SSO::UI::Hex(0xFF6B35FF), SSO::UI::Hex(0x2A1A10FF), curTime);
            SSO::UI::DrawTextShadow(gameFont, "DIFFICULTY", {500, 18}, 13, 1, Fade(GRAY, 0.7f));

            // Game time (top left)
            SSO::UI::DrawCard({10, 10, 130, 44}, HUDStyle);
            SSO::UI::DrawTextShadow(gameFont, TextFormat("%.1fs", gameTime), {22, 20}, 20, 1, LIGHTGRAY);

            // Toast notifications
            SSO::UI::DrawToastTick(gameFont, dt);
        }

        // ============================================================
        //  GAME OVER POPUP
        // ============================================================
        if (gameOver) {
            if (score > highScore) highScore = score;

            static Rectangle pop     = {390, 240, 500, 240};
            static bool      popAct  = true;
            static bool      popDrag = false;
            popAct = true;

            char msg[64];
            TextCopy(msg, TextFormat("Score: %04d   Best: %04d", score, highScore));

            if (SSO::UI::DrawPopUp(&pop, "SESSION ENDED", msg, gameFont, &popAct, &popDrag)) {
                inMenu   = true;
                inGame   = false;
                gameOver = false;
                popAct   = true;
            }
        }

        SSO::Window::EndDrawingVirtual();
    }

    SSO::Window::Close();
    return 0;
}