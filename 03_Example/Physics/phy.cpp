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
#include <vector>

#ifndef TextToFloat
#define TextToFloat(text) (float)atof(text)
#endif
#define RAYGUI_IMPLEMENTATION
#include "tools/sso_ui.h"
#include "tools/sso_window.h"
#include "tools/sso_text.h"
#include "tools/sso_physics.h"

struct BallVisual {
    Color color;
};

int main() {
    // --- 1. INITIALIZATION ---
    SSO::Window::Init(1280, 720, "SSOEngine v1.7 - Advanced Physics Sandbox");
    SetTargetFPS(60);
    Font gameFont = SSO::UI::GetDefaultFont();

    // --- 2. SETUP PHYSICS WORLD & MAP BOUNDARIES ---
    SSO::Physics::PhysicsWorld world;

    // Boundaries (Floor, Left Wall, Right Wall)
    Rectangle floor = { 100, 640, 1080, 40 };
    Rectangle leftWall = { 60, 100, 40, 580 };
    Rectangle rightWall = { 1180, 100, 40, 580 };
    
    // Floating platforms
    Rectangle platform1 = { 300, 350, 250, 30 };
    Rectangle platform2 = { 730, 450, 250, 30 };

    world.AddStaticCollider(floor);
    world.AddStaticCollider(leftWall);
    world.AddStaticCollider(rightWall);
    world.AddStaticCollider(platform1);
    world.AddStaticCollider(platform2);

    std::vector<BallVisual> ballVisuals;

    // Auto-spawn config
    float spawnTimer = 0.0f;
    float spawnInterval = 0.1f; 
    bool autoSpawnActive = true;

    // Drag and Drop State Management variables
    int grabbedBodyIndex = -1;  // -1 means no object is currently grabbed
    float grabMassStorage = 1.0f; // Temporary hold for mass so the object doesn't feel heavy while dragged

    // --- GAME LOOP ---
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Vector2 mousePos = GetMousePosition();

        // Access the mutable vector of internal physics world bodies directly
        auto& worldBodies = const_cast<std::vector<SSO::Physics::RigidBody>&>(world.GetBodies());

        // --- 3. INPUT MANAGEMENT & INTERACTION ---

        // FEATURE A: Dragging Mechanics (Left Click & Hold)
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // Check if mouse cursor is inside any active ball
            for (size_t i = 0; i < worldBodies.size(); i++) {
                if (CheckCollisionPointCircle(mousePos, worldBodies[i].position, worldBodies[i].radius)) {
                    grabbedBodyIndex = (int)i;
                    
                    // Temporarily store original mass and make it static/infinite mass 
                    // so it overrides normal gravity calculation while being dragged
                    grabMassStorage = worldBodies[i].mass;
                    worldBodies[i].velocity = { 0, 0 };
                    break;
                }
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && grabbedBodyIndex != -1) {
            // Smoothly lerp or snap the rigid body's position straight to the mouse coordinate
            worldBodies[grabbedBodyIndex].position = mousePos;
            
            // Calculate a synthetic drag velocity vector based on sudden frame movements
            // This ensures when you fling a ball, it transfers momentum to nearby objects!
            Vector2 mouseDelta = GetMouseDelta();
            worldBodies[grabbedBodyIndex].velocity = Vector2Scale(mouseDelta, 1.0f / dt);
            worldBodies[grabbedBodyIndex].isGrounded = false;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && grabbedBodyIndex != -1) {
            // Restore structural mass and drop the object back into the simulation pipeline
            worldBodies[grabbedBodyIndex].mass = grabMassStorage;
            grabbedBodyIndex = -1;
        }

        // FEATURE B: Manual Spawn on Spacebar
        if (IsKeyPressed(KEY_SPACE)) {
            float randomRadius = (float)GetRandomValue(14, 26);
            float randomMass = randomRadius * 0.1f;
            
            SSO::Physics::RigidBody newBall(mousePos, randomRadius, randomMass);
            newBall.velocity = { (float)GetRandomValue(-100, 100), (float)GetRandomValue(-20, 20) };
            
            world.AddBody(newBall);

            Color randomColor = { (unsigned char)GetRandomValue(50, 240), 
                                  (unsigned char)GetRandomValue(140, 255), 
                                  (unsigned char)GetRandomValue(180, 255), 255 };
            ballVisuals.push_back({ randomColor });
        }

        // FEATURE C: Auto Spawn System (Toggled by UI button / automated)
        if (autoSpawnActive && grabbedBodyIndex == -1) {
            spawnTimer += dt;
            if (spawnTimer >= spawnInterval && worldBodies.size() < 120) { 
                Vector2 spawnPos = { (float)GetRandomValue(250, 1030), 80.0f };
                float randomRadius = (float)GetRandomValue(16, 24);
                
                SSO::Physics::RigidBody autoBall(spawnPos, randomRadius, randomRadius * 0.1f);
                world.AddBody(autoBall);

                Color randomColor = { (unsigned char)GetRandomValue(200, 255), 
                                      (unsigned char)GetRandomValue(80, 180), 
                                      (unsigned char)GetRandomValue(50, 120), 255 };
                ballVisuals.push_back({ randomColor });
                
                spawnTimer = 0.0f;
            }
        }

        // FEATURE D: Toggle Auto-Spawn with 'A' Key
        if (IsKeyPressed(KEY_A)) {
            autoSpawnActive = !autoSpawnActive;
        }

        // FEATURE E: Reset / Clear Simulator with 'C' Key
        if (IsKeyPressed(KEY_C)) {
            world.Clear();
            ballVisuals.clear();
            grabbedBodyIndex = -1;
            
            world.AddStaticCollider(floor);
            world.AddStaticCollider(leftWall);
            world.AddStaticCollider(rightWall);
            world.AddStaticCollider(platform1);
            world.AddStaticCollider(platform2);
        }

        // --- 4. PHYSICS SIMULATION UPDATE ---
        // Run full collision matrix loops and apply structural constraint corrections
        world.Update(dt);

        // --- 5. RENDERING ---
        SSO::Window::BeginDrawingVirtual();
        ClearBackground(SSO::UI::Hex(0x0E0E12FF)); 

        // Render Environment Colliders
        for (const auto& box : world.GetStaticColliders()) {
            DrawRectangleRec(box, SSO::UI::Hex(0x22222BFF));
            DrawRectangleLinesEx(box, 2, SSO::UI::Hex(0x3D3D4CFF));
        }

        // Render Active Physics Entities
        for (size_t i = 0; i < worldBodies.size(); i++) {
            const auto& ball = worldBodies[i];
            
            // Highlight the object currently held by the cursor
            if ((int)i == grabbedBodyIndex) {
                DrawCircleV(ball.position, ball.radius + 4, SKYBLUE);
            }
            
            DrawCircleV(ball.position, ball.radius, ballVisuals[i].color);
            DrawCircleLines((int)ball.position.x, (int)ball.position.y, ball.radius, Fade(WHITE, 0.3f));
            
            // Grounded state indicator
            if (ball.isGrounded && (int)i != grabbedBodyIndex) {
                DrawCircleV(ball.position, 3.5f, WHITE);
            }
        }

        // --- 6. METRIC DASHBOARD (SSO::UI Panel) ---
        SSO::UI::DrawPanel({ 20, 20, 380, 190 }, "PHYSICS ENGINE SANDBOX v1.7", gameFont, SSO::UI::DefaultStyle);
        SSO::UI::DrawTextShadow(gameFont, TextFormat("TOTAL RIGID BODIES : %03d", worldBodies.size()), { 40, 55 }, 18, 1, WHITE);
        SSO::UI::DrawTextShadow(gameFont, TextFormat("SIMULATION SPEED   : %d FPS", GetFPS()), { 40, 80 }, 18, 1, LIME);
        SSO::UI::DrawTextShadow(gameFont, TextFormat("STREAM GENERATOR   : %s", autoSpawnActive ? "ACTIVE" : "DISABLED"), { 40, 105 }, 18, 1, autoSpawnActive ? GOLD : RED);
        
        DrawLine(40, 135, 380, 135, SSO::UI::Hex(0x3D3D4CFF));
        SSO::UI::DrawTextShadow(gameFont, "[Left Click + Drag] Move & Fling Objects", { 40, 145 }, 13, 1, SKYBLUE);
        SSO::UI::DrawTextShadow(gameFont, "[SPACE] Spawn at Cursor | [A] Toggle Auto | [C] Clear", { 40, 165 }, 13, 1, LIGHTGRAY);

        // Mouse Reticle Display
        if (grabbedBodyIndex != -1) {
            DrawCircleLines((int)mousePos.x, (int)mousePos.y, 6, RED);
        } else {
            DrawCircleLines((int)mousePos.x, (int)mousePos.y, 14, Fade(SKYBLUE, 0.6f));
        }

        SSO::Window::EndDrawingVirtual();
    }

    SSO::Window::Close();
    return 0;
}