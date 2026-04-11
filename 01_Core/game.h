#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "tools/sso_text.h"
#include "tools/sso_3d.h"
#include "tools/sso_ph3d.h"

// Bullet Structure
struct Bullet {
    Vector3 position;
    Vector3 direction;
    float speed;
    bool isActive;
    float lifetime;
};

// Physics Game State
struct PhysicsGame {
    Camera3D camera;
    Vector3 playerPos;
    float cameraYaw;
    float cameraPitch;
    float mouseSensitivity;
    float gameTime;
    
    // Bullets
    Bullet bullets[50];
    int bulletCount;
    
    // Stats
    int shotsFired;
    int cubesHit;
    
    // Shader system
    int currentShader;
    Shader shaders[5];
    bool shaderLoaded[5];
};

inline PhysicsGame physicsGame;

inline void InitPhysicsGame() {
    // Initialize physics world
    SSO::Physics3D::Init();
    SSO::Physics3D::SetGravity(-9.81f);
    
    // Initialize shader system
    physicsGame.currentShader = 0;
    const char* shaderFiles[5] = {
        "assets/shaders/basic_light.fs",
        "assets/shaders/bloom_glow.fs", 
        "assets/shaders/retro_crt.fs",
        "assets/shaders/grayscale_sepia.fs",
        "assets/shaders/blur_bokeh.fs"
    };
    
    // Load all shaders
    for (int i = 0; i < 5; i++) {
        physicsGame.shaders[i] = SSO::ThreeD::LoadShaderFromBundle("assets.sso", shaderFiles[i]);
        physicsGame.shaderLoaded[i] = (physicsGame.shaders[i].id > 0);
    }
    
    // Initialize player
    physicsGame.playerPos = (Vector3){0.0f, 2.0f, 10.0f};
    physicsGame.cameraYaw = 0.0f;
    physicsGame.cameraPitch = -0.3f;
    physicsGame.mouseSensitivity = 0.003f;
    physicsGame.gameTime = 0.0f;
    
    // Setup camera
    physicsGame.camera.position = physicsGame.playerPos;
    physicsGame.camera.target = (Vector3){
        physicsGame.playerPos.x + sin(physicsGame.cameraYaw),
        physicsGame.playerPos.y + sin(physicsGame.cameraPitch),
        physicsGame.playerPos.z + cos(physicsGame.cameraYaw)
    };
    physicsGame.camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    physicsGame.camera.fovy = 60.0f;
    physicsGame.camera.projection = CAMERA_PERSPECTIVE;
    
    // Create pyramid stack using physics system
    float cubeSize = 0.8f;
    Color colors[] = {RED, GREEN, BLUE, YELLOW, ORANGE, PURPLE, PINK, LIME};
    
    // Create pyramid: 4 on bottom, 3 above, 2 above, 1 on top
    int layerSizes[] = {4, 3, 2, 1};
    float startY = cubeSize / 2.0f;
    int cubeIndex = 0;
    
    for (int layer = 0; layer < 4; layer++) {
        for (int i = 0; i < layerSizes[layer]; i++) {
            float x = (i - layerSizes[layer]/2.0f + 0.5f) * cubeSize * 1.1f;
            float y = startY + layer * cubeSize;
            float z = 0.0f;
            
            SSO::Physics3D::RigidBody* cube = SSO::Physics3D::CreateBody(
                (Vector3){x, y, z}, 
                (Vector3){cubeSize, cubeSize, cubeSize}, 
                1.0f
            );
            
            if (cube) {
                cube->restitution = 0.2f;
                cube->friction = 0.9f;
                cube->color = colors[layer % 8];
            }
        }
    }
    
    // Create ground
    SSO::Physics3D::RigidBody* ground = SSO::Physics3D::CreateStaticBody(
        (Vector3){0.0f, -0.5f, 0.0f}, 
        (Vector3){20.0f, 1.0f, 20.0f}
    );
    if (ground) {
        ground->color = GRAY;
    }
    
    // Initialize bullets
    for (int i = 0; i < 50; i++) {
        physicsGame.bullets[i].isActive = false;
    }
    physicsGame.bulletCount = 0;
    physicsGame.shotsFired = 0;
    physicsGame.cubesHit = 0;
}

inline void Start() {
    InitPhysicsGame();
}

inline Vector3 GetForwardVector() {
    return (Vector3){
        sin(physicsGame.cameraYaw),
        sin(physicsGame.cameraPitch),
        cos(physicsGame.cameraYaw)
    };
}

inline void ShootBullet() {
    if (physicsGame.bulletCount >= 50) return;
    
    // Find inactive bullet slot
    for (int i = 0; i < 50; i++) {
        if (!physicsGame.bullets[i].isActive) {
            physicsGame.bullets[i].position = physicsGame.playerPos;
            physicsGame.bullets[i].direction = Vector3Normalize(GetForwardVector());
            physicsGame.bullets[i].speed = 30.0f;
            physicsGame.bullets[i].isActive = true;
            physicsGame.bullets[i].lifetime = 3.0f;
            physicsGame.bulletCount++;
            physicsGame.shotsFired++;
            
            // Apply physics force with improved raycast
            SSO::Physics3D::ShootBullet(physicsGame.playerPos, physicsGame.bullets[i].direction, 35.0f);
            
            break;
        }
    }
}

inline void UpdatePhysicsGame(float dt) {
    physicsGame.gameTime += dt;
    
    // Mouse look
    Vector2 mouseDelta = GetMouseDelta();
    physicsGame.cameraYaw -= mouseDelta.x * physicsGame.mouseSensitivity;
    physicsGame.cameraPitch -= mouseDelta.y * physicsGame.mouseSensitivity;
    
    // Clamp pitch
    if (physicsGame.cameraPitch > 1.5f) physicsGame.cameraPitch = 1.5f;
    if (physicsGame.cameraPitch < -1.5f) physicsGame.cameraPitch = -1.5f;
    
    // WASD Movement
    Vector3 forward = (Vector3){sin(physicsGame.cameraYaw), 0, cos(physicsGame.cameraYaw)};
    Vector3 right = (Vector3){cos(physicsGame.cameraYaw), 0, -sin(physicsGame.cameraYaw)};
    Vector3 movement = {0};
    
    float moveSpeed = 8.0f; // Movement speed
    
    // Fixed WASD controls
    if (IsKeyDown(KEY_W)) movement = Vector3Add(movement, forward);      // Forward
    if (IsKeyDown(KEY_S)) movement = Vector3Subtract(movement, forward); // Backward
    if (IsKeyDown(KEY_A)) movement = Vector3Subtract(movement, right);  // Left
    if (IsKeyDown(KEY_D)) movement = Vector3Add(movement, right);      // Right
    
    if (Vector3Length(movement) > 0) {
        movement = Vector3Normalize(movement);
        movement = Vector3Scale(movement, moveSpeed * dt);
        
        Vector3 newPos = Vector3Add(physicsGame.playerPos, movement);
        
        // Simple boundary check
        if (fabs(newPos.x) < 15.0f && fabs(newPos.z) < 15.0f && newPos.y > 0.5f && newPos.y < 10.0f) {
            physicsGame.playerPos = newPos;
        }
    }
    
    // Update camera
    physicsGame.camera.position = physicsGame.playerPos;
    physicsGame.camera.target = (Vector3){
        physicsGame.playerPos.x + sin(physicsGame.cameraYaw),
        physicsGame.playerPos.y + sin(physicsGame.cameraPitch),
        physicsGame.playerPos.z + cos(physicsGame.cameraYaw)
    };
    
    // Shooting
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        ShootBullet();
    }
    
    // Shader switching
    if (IsKeyPressed(KEY_ONE)) physicsGame.currentShader = 0;
    if (IsKeyPressed(KEY_TWO)) physicsGame.currentShader = 1;
    if (IsKeyPressed(KEY_THREE)) physicsGame.currentShader = 2;
    if (IsKeyPressed(KEY_FOUR)) physicsGame.currentShader = 3;
    if (IsKeyPressed(KEY_FIVE)) physicsGame.currentShader = 4;
    
    // Update physics world
    SSO::Physics3D::Step(dt);
    
    // Update bullets (visual only)
    for (int i = 0; i < 50; i++) {
        if (physicsGame.bullets[i].isActive) {
            physicsGame.bullets[i].position = Vector3Add(
                physicsGame.bullets[i].position,
                Vector3Scale(physicsGame.bullets[i].direction, physicsGame.bullets[i].speed * dt)
            );
            
            physicsGame.bullets[i].lifetime -= dt;
            if (physicsGame.bullets[i].lifetime <= 0) {
                physicsGame.bullets[i].isActive = false;
                physicsGame.bulletCount--;
            }
        }
    }
    
    // Update hit counter based on active bodies
    physicsGame.cubesHit = physicsGame.shotsFired - (SSO::Physics3D::GetBodyCount() - 1); // -1 for ground
}

inline void Update(float dt) {
    UpdatePhysicsGame(dt);
}

inline void RenderPhysicsWorld() {
    // Draw ground
    SSO::ThreeD::SSO_DrawPlane((Vector3){0, 0, 0}, (Vector2){20, 20}, (Color){40, 40, 40, 255});
    
    // Draw grid
    SSO::ThreeD::SSO_DrawGrid(20, 1.0f);
    
    // Apply current shader if loaded
    if (physicsGame.shaderLoaded[physicsGame.currentShader]) {
        Shader currentShader = physicsGame.shaders[physicsGame.currentShader];
        Vector3 lightPos = (Vector3){5.0f, 10.0f, 5.0f};
        Vector3 lightColor = (Vector3){1.0f, 0.8f, 0.6f};
        Vector3 viewPos = physicsGame.camera.position;
        Vector2 resolution = (Vector2){1280, 720};
        
        // Apply shader-specific settings
        switch (physicsGame.currentShader) {
            case 0: // Basic Light
                SSO::ThreeD::ApplyBasicLight(currentShader, lightPos, lightColor, viewPos);
                break;
            case 1: // Bloom Glow
                SSO::ThreeD::ApplyBasicLight(currentShader, lightPos, lightColor, viewPos);
                SSO::ThreeD::ApplyBloomGlow(currentShader, physicsGame.gameTime, resolution, 0.8f, 2.0f);
                break;
            case 2: // Retro CRT
                SSO::ThreeD::ApplyBasicLight(currentShader, lightPos, lightColor, viewPos);
                SSO::ThreeD::ApplyRetroCRT(currentShader, physicsGame.gameTime, resolution, 0.1f, 0.15f);
                break;
            case 3: // Grayscale Sepia
                SSO::ThreeD::ApplyBasicLight(currentShader, lightPos, lightColor, viewPos);
                SSO::ThreeD::ApplyGrayscaleSepia(currentShader, physicsGame.gameTime, resolution, 1.0f, 1);
                break;
            case 4: // Blur Bokeh
                SSO::ThreeD::ApplyBasicLight(currentShader, lightPos, lightColor, viewPos);
                SSO::ThreeD::ApplyBlurBokeh(currentShader, physicsGame.gameTime, resolution, 2.0f, 10.0f);
                break;
        }
        
        BeginShaderMode(currentShader);
    }
    
    // Draw all physics bodies with shader
    SSO::Physics3D::DrawAllBodies();
    
    if (physicsGame.shaderLoaded[physicsGame.currentShader]) {
        EndShaderMode();
    }
    
    // Draw bullets (without shader for contrast)
    for (int i = 0; i < 50; i++) {
        if (physicsGame.bullets[i].isActive) {
            SSO::ThreeD::SSO_DrawSphere(physicsGame.bullets[i].position, 0.1f, YELLOW);
        }
    }
    
    // Draw shooting direction indicator
    Vector3 forward = GetForwardVector();
    Vector3 endPos = Vector3Add(physicsGame.playerPos, Vector3Scale(forward, 2.0f));
    SSO::ThreeD::SSO_DrawRay((Ray){physicsGame.playerPos, forward}, WHITE);
}

inline void RenderCrosshair() {
    int centerX = 1280 / 2;
    int centerY = 720 / 2;
    int crossSize = 10;
    
    DrawLine(centerX - crossSize, centerY, centerX + crossSize, centerY, WHITE);
    DrawLine(centerX, centerY - crossSize, centerX, centerY + crossSize, WHITE);
    DrawCircle(centerX, centerY, 2, WHITE);
}

inline void RenderUI() {
    // Title
    const char* title = "SSOEngine 3D Physics Test";
    int titleWidth = MeasureText(title, 30);
    DrawText(title, (1280 - titleWidth) / 2, 20, 30, WHITE);
    
    // Instructions
    DrawText("WASD: Move | Mouse: Look | Click: Shoot | ESC: Exit", 20, 680, 16, GRAY);
    
    // Stats
    DrawText(TextFormat("Shots: %d", physicsGame.shotsFired), 20, 20, 20, WHITE);
    DrawText(TextFormat("Hits: %d", physicsGame.cubesHit), 20, 50, 20, WHITE);
    DrawText(TextFormat("Accuracy: %.1f%%", physicsGame.shotsFired > 0 ? 
                        (float)physicsGame.cubesHit / physicsGame.shotsFired * 100 : 0), 20, 80, 20, WHITE);
    
    // Active cubes
    int activeCubes = SSO::Physics3D::GetBodyCount() - 1; // -1 for ground
    DrawText(TextFormat("Cubes: %d/20", activeCubes), 20, 110, 20, WHITE);
    
    // Physics info
    DrawText(TextFormat("Bodies: %d", SSO::Physics3D::GetBodyCount()), 20, 140, 20, WHITE);
    DrawText("Gravity: -29.43 m/s²", 20, 170, 20, WHITE); // 9.81 * 3
    
    // Shader info
    const char* shaderNames[5] = {
        "Basic Light", "Bloom Glow", "Retro CRT", 
        "Grayscale Sepia", "Blur Bokeh"
    };
    DrawText(TextFormat("Shader: %s", shaderNames[physicsGame.currentShader]), 20, 200, 20, YELLOW);
    DrawText("Press 1-5 for effects", 20, 230, 16, GRAY);
    
    // Debug shader loading status
    int loadedCount = 0;
    for (int i = 0; i < 5; i++) {
        if (physicsGame.shaderLoaded[i]) loadedCount++;
    }
    DrawText(TextFormat("Shaders loaded: %d/5", loadedCount), 20, 260, 16, loadedCount == 5 ? GREEN : RED);
    
    if (physicsGame.shaderLoaded[physicsGame.currentShader]) {
        DrawText("Current shader: ACTIVE", 20, 280, 16, GREEN);
    } else {
        DrawText("Current shader: NOT LOADED", 20, 280, 16, RED);
    }
    
    // SSO Text effects
    SSO::Text::DrawGradient("PHYSICS ENGINE", 1280 - 250, 30, 25, VIOLET, BLUE);
    SSO::Text::DrawCodeBlock("PHYSICS_ACTIVE", 1280 - 200, 60, 15, GREEN, BLACK);
    SSO::Text::DrawTypewriter("Realistic 3D physics with shaders...", 1280 - 280, 90, 15, 10.0f, physicsGame.gameTime, WHITE);
}

inline void Render() {
    RenderCrosshair();
    RenderUI();
}

inline void Shutdown() {
    // Unload all shaders
    for (int i = 0; i < 5; i++) {
        if (physicsGame.shaderLoaded[i]) {
            UnloadShader(physicsGame.shaders[i]);
        }
    }
    
    // Cleanup physics
    SSO::Physics3D::Cleanup();
}

#endif
