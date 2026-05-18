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
#include "raymath.h"
#include <stdlib.h>

#ifndef TextToFloat
#define TextToFloat(text) (float)atof(text)
#endif
#define RAYGUI_IMPLEMENTATION

#include "tools/sso_ui.h"
#include "tools/sso_window.h"
#include "tools/sso_camera.h"
#include "tools/sso_timer.h"
#include "tools/sso_text.h"
#include "tools/sso_splash.h"

// ============================================================
//  STRUCTS
// ============================================================
struct Car {
    Rectangle rect;
    Color color;
    float speed;
    bool active;
};

struct Particle {
    Vector2 pos;
    Vector2 vel;
    Color color;
    float life;
    bool active;
};

// ============================================================
//  PARTICLE SYSTEM
// ============================================================
static Particle particles[64] = { 0 };

static void SpawnExplosion(Vector2 pos, Color color) {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 64; j++) {
            if (!particles[j].active) {
                particles[j].pos = pos;
                particles[j].vel = { (float)GetRandomValue(-200, 200), (float)GetRandomValue(-200, 200) };
                particles[j].color = color;
                particles[j].life = 1.0f;
                particles[j].active = true;
                break;
            }
        }
    }
}

// ============================================================
//  MAIN
// ============================================================
int main() {
    SSO::Window::Init(1280, 720, "SSO-RACE PRO v1.0");
    SSO::Splash::Show(2.0f);
    Font gameFont = SSO::UI::GetDefaultFont();

    Car player = { { 615, 550, 50, 90 }, SSO::UI::Hex(0x0096FFFF), 400.0f, true };
    Car enemies[10] = { 0 };
    float roadOffset = 0.0f;
    int score = 0;
    int lives = 3;
    bool inMenu = true;
    bool inGame = false;
    bool gameOver = false;
    float gameSpeed = 500.0f;
    float spawnTimer = 0.0f;

    SSO::UI::UIStyle MenuBtn = SSO::UI::DefaultStyle;
    MenuBtn.accent = SSO::UI::Hex(0x00E676FF);
    MenuBtn.roundness = 0.4f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        SSO::Window::BeginDrawingVirtual();
        SSO::UI::DrawBackground({ 0 });

        roadOffset += gameSpeed * dt;
        if (roadOffset >= 100.0f) roadOffset = 0.0f;

        DrawRectangle(400, 0, 480, 720, SSO::UI::Hex(0x1A1A1AFF));
        for (int i = -1; i < 8; i++) {
            DrawRectangle(635, (int)(i * 100 + roadOffset), 10, 50, GRAY);
        }
        DrawRectangle(390, 0, 10, 720, LIGHTGRAY);
        DrawRectangle(880, 0, 10, 720, LIGHTGRAY);

        // ============================================================
        //  MENU SCREEN
        // ============================================================
        if (inMenu) {
            SSO::UI::DrawVignette(0.8f);
            SSO::UI::DrawTextGlow(gameFont, "SSO-RACE PRO", { 400, 200 }, 72, 2.0f, SKYBLUE, 0.8f);
            
            if (SSO::UI::DrawButton({ 540, 350, 200, 60 }, "START RACE", gameFont, MenuBtn)) {
                inMenu = false;
                inGame = true;
                score = 0;
                lives = 3;
                gameSpeed = 500.0f;
                player.rect.x = 615;
                for (int i = 0; i < 10; i++) enemies[i].active = false;
            }
            if (SSO::UI::DrawButton({ 540, 430, 200, 60 }, "EXIT", gameFont, MenuBtn)) break;
        }

        // ============================================================
        //  GAMEPLAY SCREEN
        // ============================================================
        if (inGame && !gameOver) {
            gameSpeed += 5.0f * dt;
            score += (int)(gameSpeed * 0.01f);

            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) player.rect.x -= player.speed * dt;
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) player.rect.x += player.speed * dt;

            if (player.rect.x < 400) player.rect.x = 400;
            if (player.rect.x > 880 - player.rect.width) player.rect.x = 880 - player.rect.width;

            static const float laneX[5] = { 430.0f, 500.0f, 570.0f, 640.0f, 710.0f };
            spawnTimer += dt;

            float spawnInterval = 1.2f - (gameSpeed - 500.0f) * 0.0009f;
            if (spawnInterval < 0.35f) spawnInterval = 0.35f;

            if (spawnTimer > spawnInterval) {
                int spawnCount = 1 + (gameSpeed > 850.0f ? 1 : 0);
                int spawned = 0;
                int attempts = 0;
                while (spawned < spawnCount && attempts < 12) {
                    attempts++;
                    int lane = GetRandomValue(0, 4);
                    bool laneFree = true;
                    for (int j = 0; j < 10; j++) {
                        if (enemies[j].active && fabsf(enemies[j].rect.x - laneX[lane]) < 10.0f && enemies[j].rect.y < 200.0f) {
                            laneFree = false;
                            break;
                        }
                    }
                    if (!laneFree) continue;

                    for (int i = 0; i < 10; i++) {
                        if (!enemies[i].active) {
                            enemies[i].active = true;
                            enemies[i].rect = { laneX[lane], -140.0f - GetRandomValue(0, 80), 50, 90 };
                            enemies[i].color = (GetRandomValue(0, 1)) ? RED : ORANGE;
                            enemies[i].speed = gameSpeed * 0.5f;
                            spawned++;
                            break;
                        }
                    }
                }

                spawnTimer = 0;
            }

            for (int i = 0; i < 10; i++) {
                if (!enemies[i].active) continue;
                enemies[i].rect.y += (enemies[i].speed + (gameSpeed * 0.5f)) * dt;

                if (CheckCollisionRecs(player.rect, enemies[i].rect)) {
                    enemies[i].active = false;
                    lives--;
                    SpawnExplosion({ enemies[i].rect.x + 25, enemies[i].rect.y + 45 }, RED);
                    SSO::UI::ShowToast("CRASH!", 1.5f, RED);
                    if (lives <= 0) gameOver = true;
                }

                if (enemies[i].rect.y > 720) enemies[i].active = false;

                DrawRectangleRounded(enemies[i].rect, 0.2f, 8, enemies[i].color);
                DrawRectangle(enemies[i].rect.x + 5, enemies[i].rect.y + 10, 40, 20, Fade(BLACK, 0.5f));
            }

            for (int i = 0; i < 64; i++) {
                if (!particles[i].active) continue;
                particles[i].life -= dt;
                particles[i].pos.x += particles[i].vel.x * dt;
                particles[i].pos.y += particles[i].vel.y * dt;
                if (particles[i].life <= 0) particles[i].active = false;
                DrawCircleV(particles[i].pos, 4 * particles[i].life, Fade(particles[i].color, particles[i].life));
            }

            DrawRectangleRounded(player.rect, 0.2f, 8, player.color);
            DrawRectangle(player.rect.x + 5, player.rect.y + 15, 40, 25, Fade(BLACK, 0.5f));
            SSO::UI::DrawGlow(player.rect, player.color, 0.3f);

            SSO::UI::DrawPanel({ 20, 20, 280, 120 }, "DASHBOARD", gameFont, SSO::UI::DefaultStyle);
            SSO::UI::DrawTextShadow(gameFont, TextFormat("SCORE: %06d", score), { 40, 60 }, 24, 1, WHITE);
            SSO::UI::DrawTextShadow(gameFont, "INTEGRITY", { 40, 92 }, 14, 1, Fade(GRAY, 0.7f));
            SSO::UI::DrawHealthBar({ 40, 108, 240, 12 }, (float)lives, 3.0f, RED, SSO::UI::Hex(0x320000FF));
            
            if (lives == 1) SSO::UI::DrawBadge({ 190, 25 }, "CRITICAL", gameFont, RED, BLACK);
            SSO::UI::DrawToastTick(gameFont, dt);
        }

        // ============================================================
        //  GAME OVER SCREEN
        // ============================================================
        if (gameOver) {
            static Rectangle pop = { 440, 260, 400, 200 };
            static bool popAct = true;
            static bool popDrag = false;
            char finalScore[64];
            TextCopy(finalScore, TextFormat("RACE OVER! SCORE: %06d", score));
            if (SSO::UI::DrawPopUp(&pop, "DEFEAT", finalScore, gameFont, &popAct, &popDrag)) {
                gameOver = false;
                inMenu = true;
            }
        }

        SSO::Window::EndDrawingVirtual();
    }

    SSO::Window::Close();
    return 0;
}