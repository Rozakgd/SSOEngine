# How to Use SSOEngine Tools

**PROFESSIONAL BUILD SYSTEM GUIDE - v1.7**

> ⚠️ **UPDATED**: Complete guide for professional-grade build system with distribution automation
> **PRODUCTION READY**: Standalone executables with zero dependencies

---

## 📁 Tools Location

All tools are located in: `01_Core/tools/`

```
01_Core/tools/
├── sso_audio.h      # Audio System with Music Management
├── sso_camera.h     # Advanced Camera with smoothing & shake
├── sso_ext.h        # System Extension & File Operations
├── sso_file.h       # File Dialogs
├── sso_logger.h     # Colored Console Logger
├── sso_math.h       # Collision & Math utilities
├── sso_physics.h    # 2D physics world helper with grounded movement
├── sso_memo.h       # Persistent Data Storage System
├── sso_ph3d.h       # Optional advanced physics helper
├── sso_provider.h   # Asset Bundle System
├── sso_splash.h     # Professional Splash Screen System
├── sso_text.h       # Advanced Text Rendering with Hyperlinks
├── sso_timer.h      # High-precision Timer with countdown/stopwatch
├── sso_ui.h         # Advanced UI System with panels & modals
├── sso_window.h     # Virtual Resolution Window Manager
└── raygui.h         # Raylib GUI Extension
```

---

## 🚀 Professional Build System (v1.7)

### Build Script Usage
```batch
# Navigate to project root
cd SSOEngine/01_Core

# Run professional build system
build.bat
```

### Build Options
1. **Windows Build** - Creates standalone EXE with static linking
2. **Android Build** - Creates signed APK with V2 signature
3. **Clean Build** - Comprehensive cleanup of build/ and dist/ folders
4. **Set Android NDK Path** - Configure Android development environment

### Production Features
- **Folder Separation**: build/ for compilation, dist/ for distribution
- **Auto-Zip Archives**: PowerShell Compress-Archive for one-click distribution
- **Standalone EXE**: Zero dependency executables with static linking
- **Build Garbage Cleanup**: Automatic cleanup of resource files
- **Post-Build Navigation**: 6-option professional menu system
- **Error Handling**: Red color display with clipboard copy feature

### Build Output Structure
```
SSOEngine/01_Core/
├── build/           # Compilation cache and temporary files
│   ├── CMakeCache.txt
│   ├── CMakeFiles/
│   ├── *.o
│   └── *.obj
├── dist/            # Final distribution package
│   ├── Game.exe
│   ├── assets.sso
│   └── Game_Release.zip
└── build.bat         # Professional build script
```

### Post-Build Menu Options
After successful build, you can:
1. **Run Game** - Launch development build from build/
2. **Open Build Folder** - Access compilation cache for debugging
3. **Open Dist Folder** - Access final distribution package
4. **Run Game from Dist** - Test production standalone build
5. **Return to Main Menu** - Build different configuration without restart
6. **Close** - Exit build system

### Error Recovery System
- **Red Color Display**: Clear error visibility with color 0C
- **Copy to Clipboard**: Automatic error logging with `type error.log | clip`
- **Error Details**: Timestamp, error code, and context
- **Recovery Options**: Return to menu or close without data loss

---

## sso_physics.h - 2D Physics Helper

### What it ACTUALLY Does
Simple 2D physics support for circle-based rigid bodies, grounded movement, and rectangle collision response. `PhysicsWorld` sekarang juga menggunakan broad-phase grid spasial untuk mempercepat deteksi tumbukan antar tubuh.

### Include
```cpp
#include "tools/sso_physics.h"
```

### Basic usage
```cpp
using namespace SSO::Physics;

PhysicsWorld world;
RigidBody player({100, 100}, 16.0f, 1.0f);
world.AddBody(player);
Rectangle floor = {0, 400, 800, 50};
world.AddStaticCollider(floor);

while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    world.Update(dt);

    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (const auto& body : world.GetBodies()) {
        DrawCircleV(body.position, body.radius, BLUE);
    }
    DrawRectangleRec(floor, DARKGRAY);
    EndDrawing();
}
```

---

## sso_memo.h - Persistent Data Storage System

### What it ACTUALLY Does
Simple key-value storage system for game save data and persistent settings. Uses std::map for efficient memory management and text files for cross-platform persistence.

### Include
```cpp
#include "tools/sso_memo.h"
```

### Available Functions

#### **Data Storage**
```cpp
// Store data with key-value pairs
saveData.write("player_score", "1500");
saveData.write("player_level", "5");
saveData.write("player_name", "Hero");
```

#### **Data Retrieval**
```cpp
// Read data by key
std::string score = saveData.read("player_score");
std::string level = saveData.read("player_level");

// Check if key exists
if (saveData.hasKey("player_health")) {
    std::string health = saveData.read("player_health");
}

// Get number of stored items
int itemCount = saveData.size();
```

#### **File Operations**
```cpp
// Save all data to file
saveData.save("save.txt");

// Load data from file
saveData.load("save.txt");

// Clear all data from memory
saveData.clear();
```

### File Format
The save system uses simple key=value format:
```
player_score=1500
player_level=5
player_name=Hero
```

### Complete Save System Example
```cpp
#include "tools/sso_memo.h"
#include "tools/sso_window.h"

struct GameData {
    int score = 0;
    int level = 1;
    std::string name = "Player";
    SSO::Memo saveData;
};

void SaveGame(GameData& game) {
    game.saveData.write("player_score", std::to_string(game.score));
    game.saveData.write("player_level", std::to_string(game.level));
    game.saveData.write("player_name", game.name);
    game.saveData.save("save.txt");
}

void LoadGame(GameData& game) {
    game.saveData.load("save.txt");
    
    if (game.saveData.hasKey("player_score")) {
        game.score = std::stoi(game.saveData.read("player_score"));
    }
    if (game.saveData.hasKey("player_level")) {
        game.level = std::stoi(game.saveData.read("player_level"));
    }
    if (game.saveData.hasKey("player_name")) {
        game.name = game.saveData.read("player_name");
    }
}

int main() {
    SSO::Window::Init(800, 600, "Save System Demo");
    
    GameData game;
    LoadGame(game);
    
    while (!WindowShouldClose()) {
        SSO::Window::BeginDrawingVirtual();
        ClearBackground(BLACK);
        
        DrawText(TextFormat("Score: %d", game.score), 10, 10, 20, WHITE);
        DrawText(TextFormat("Level: %d", game.level), 10, 40, 20, WHITE);
        DrawText(TextFormat("Name: %s", game.name.c_str()), 10, 70, 20, WHITE);
        
        DrawText("S: Save Game", 10, 120, 20, YELLOW);
        DrawText("L: Load Game", 10, 150, 20, YELLOW);
        DrawText("UP: Increase Score", 10, 180, 20, GREEN);
        
        if (IsKeyPressed(KEY_S)) {
            SaveGame(game);
        }
        if (IsKeyPressed(KEY_L)) {
            LoadGame(game);
        }
        if (IsKeyPressed(KEY_UP)) {
            game.score += 100;
        }
        
        SSO::Window::EndDrawingVirtual();
    }
    
    SSO::Window::Close();
    return 0;
}
```

### Best Practices
1. **Use string conversion** for numeric data with `std::to_string()` and `std::stoi()`
2. **Check key existence** before reading to avoid empty strings
3. **Save frequently** to prevent data loss
4. **Use descriptive keys** for easy debugging
5. **Clear data** when starting new game sessions

---

## 🎵 sso_audio.h - Audio System with Music Management

### What it ACTUALLY Does
Advanced audio system that wraps Raylib's audio functions with automatic music management, memory handling, and simplified audio operations. Perfect for games with background music and sound effects.

### Include
```cpp
#include "tools/sso_audio.h"
```

### Available Functions

#### **Sound Loading**
```cpp
// Load sound from file
Sound jumpSound = SSO::Audio::LoadSound("assets/jump.wav");

// Load sound from memory (for asset bundles)
Sound explosion = SSO::Audio::LoadSoundFromMemory("wav", audioData, dataSize);

// Load music stream from file
Music bgm = SSO::Audio::LoadMusicStream("assets/background.mp3");

// Load music stream from memory (for asset bundles)
Music bgm = SSO::Audio::LoadMusicStreamFromMemory("mp3", musicData, dataSize);
```

#### **Music Management System**
```cpp
// Register music for automatic updates
SSO::Audio::RegisterMusic(backgroundMusic, musicBuffer);

// Update all registered music streams (call in game loop)
SSO::Audio::UpdateAudio();

// Unload specific registered music by index
SSO::Audio::UnloadRegisteredMusic(0);

// Clear all registered music and free memory
SSO::Audio::ClearMusicManager();
```

#### **Audio Playback**
```cpp
// Play sounds
SSO::Audio::PlaySound(jumpSound);

// Play music
SSO::Audio::PlayMusic(backgroundMusic);

// Control music
SSO::Audio::StopMusic(backgroundMusic);
SSO::Audio::PauseMusic(backgroundMusic);
SSO::Audio::ResumeMusic(backgroundMusic);

// Check if music is playing
if (SSO::Audio::IsMusicPlaying(backgroundMusic)) {
    // Music is currently playing
}
```

#### **Audio Control**
```cpp
// Set volume (0.0 to 1.0)
SSO::Audio::SetSoundVolume(jumpSound, 0.8f);
SSO::Audio::SetMusicVolume(backgroundMusic, 0.5f);

// Set pitch (1.0 = normal, 2.0 = octave higher)
SSO::Audio::SetSoundPitch(jumpSound, 1.2f);
SSO::Audio::SetMusicPitch(backgroundMusic, 1.0f);
```

#### **Resource Cleanup**
```cpp
// Unload individual resources
SSO::Audio::UnloadSound(jumpSound);
SSO::Audio::UnloadMusic(backgroundMusic);

// Note: Registered music is automatically cleaned up with ClearMusicManager()
```

### Complete Audio Example
```cpp
#include "tools/sso_audio.h"
#include "tools/sso_window.h"

struct AudioGame {
    Music backgroundMusic;
    Sound jumpSound;
    Sound collectSound;
};

void LoadAudioAssets(AudioGame& game) {
    // Load background music
    game.backgroundMusic = SSO::Audio::LoadMusicStream("assets/background.mp3");
    
    // Load sound effects
    game.jumpSound = SSO::Audio::LoadSound("assets/jump.wav");
    game.collectSound = SSO::Audio::LoadSound("assets/collect.wav");
    
    // Register music for automatic updates
    SSO::Audio::RegisterMusic(game.backgroundMusic);
    
    // Set volumes
    SSO::Audio::SetMusicVolume(game.backgroundMusic, 0.7f);
    SSO::Audio::SetSoundVolume(game.jumpSound, 0.8f);
    SSO::Audio::SetSoundVolume(game.collectSound, 0.9f);
    
    // Start background music
    SSO::Audio::PlayMusic(game.backgroundMusic);
}

int main() {
    SSO::Window::Init(800, 600, "Audio Demo");
    
    AudioGame game;
    LoadAudioAssets(game);
    
    while (!WindowShouldClose()) {
        // Update audio system (important for music!)
        SSO::Audio::UpdateAudio();
        
        // Handle input
        if (IsKeyPressed(KEY_SPACE)) {
            SSO::Audio::PlaySound(game.jumpSound);
        }
        
        if (IsKeyPressed(KEY_C)) {
            SSO::Audio::PlaySound(game.collectSound);
        }
        
        // Music controls
        if (IsKeyPressed(KEY_P)) {
            if (SSO::Audio::IsMusicPlaying(game.backgroundMusic)) {
                SSO::Audio::PauseMusic(game.backgroundMusic);
            } else {
                SSO::Audio::ResumeMusic(game.backgroundMusic);
            }
        }
        
        // Render
        SSO::Window::BeginDrawingVirtual();
        ClearBackground(BLACK);
        
        DrawText("SPACE: Jump Sound", 10, 10, 20, WHITE);
        DrawText("C: Collect Sound", 10, 40, 20, WHITE);
        DrawText("P: Pause/Resume Music", 10, 70, 20, WHITE);
        
        if (SSO::Audio::IsMusicPlaying(game.backgroundMusic)) {
            DrawText("Music: PLAYING", 10, 100, 20, GREEN);
        } else {
            DrawText("Music: PAUSED", 10, 100, 20, RED);
        }
        
        SSO::Window::EndDrawingVirtual();
    }
    
    // Cleanup
    SSO::Audio::ClearMusicManager(); // Cleans up registered music
    SSO::Audio::UnloadSound(game.jumpSound);
    SSO::Audio::UnloadSound(game.collectSound);
    SSO::Window::Close();
    
    return 0;
}
```

### Cross-Platform Audio Loading
```cpp
void LoadAudioForPlatform() {
#ifdef PLATFORM_ANDROID
    // Android: Load from asset bundles
    unsigned char* musicData;
    int musicSize;
    
    musicData = SSO::Provider::LoadRawDataFromBundle("audio.sso", "background.mp3", &musicSize);
    if (musicData) {
        Music bgm = SSO::Audio::LoadMusicStreamFromMemory("mp3", musicData, musicSize);
        SSO::Audio::RegisterMusic(bgm, musicData); // Buffer will be freed automatically
        SSO::Audio::PlayMusic(bgm);
    }
#else
    // Windows: Load from filesystem
    Music bgm = SSO::Audio::LoadMusicStream("assets/background.mp3");
    SSO::Audio::RegisterMusic(bgm);
    SSO::Audio::PlayMusic(bgm);
#endif
}
```

### Key Features
- **Automatic Music Management** - Register music for automatic updates
- **Memory Safety** - Automatic cleanup of registered music and buffers
- **Cross-Platform Loading** - Support both file and memory loading
- **Volume/Pitch Control** - Full audio control for sounds and music
- **Asset Bundle Integration** - Works with SSO provider system

### Best Practices
1. **Always call UpdateAudio()** in your game loop for music streaming
2. **Register music** instead of manually updating streams
3. **Use memory loading** for Android asset bundles
4. **Clean up resources** when done to prevent memory leaks
5. **Set appropriate volumes** before playing audio

---

## 📜 sso_text.h - Advanced Text System

### What is its function
If you find it difficult to create text in plain raylib language then use SSOText to make it easier!

### Include
```cpp
#include "tools/sso_text.h"
```

## Available Functions

#### **Font Management**
```cpp
void LoadFont(const char* fileName);
void UnloadFont();

// Example:
SSO::Text::LoadFont("assets/font/myfont.ttf");
```
#### **Special Effects**
```cpp
// Create typewriter dialogue
void DrawTypewriter(const char* text, int x, int y, int fontSize, float speed, float time, Color color);

// Create professional gradients
void DrawGradient(const char* text, int x, int y, int fontSize, Color top, Color bottom);

// Create text with outline
void DrawOutline(const char* text, int x, int y, int fontSize, Color color, Color outlineColor);
```

#### **Layout & Utilities**
```cpp
// Automatically wrap text within a specific width
void DrawWrapped(const char* text, int x, int y, int maxWidth, int fontSize, Color color);

// Draw text perfectly centered on screen
void DrawCentered(const char* text, int y, int fontSize, Color color);

// Create terminal-style code blocks
void DrawCodeBlock(const char* code, int x, int y, int fontSize, Color textColor, Color bgColor);
```

### Example Usage
```cpp
#include "tools/sso_text.h"

void Render() {
    SSO::Window::BeginDrawingVirtual();
    ClearBackground(BLACK);

    // 1. Title with Gradient and Outline
    SSO::Text::DrawGradient("SSO ENGINE", 50, 50, 60, YELLOW, RED);
    SSO::Text::DrawOutline("SSO ENGINE", 50, 50, 60, BLANK, WHITE);

    // 2. Automated Dialogue
    SSO::Text::DrawTypewriter("Mission Log: Connecting to base...", 50, 150, 20, 15.0f, GetTime(), LIME);

    // 3. Smart Wrap for Story/Description
    SSO::Text::DrawWrapped("This is a long description that will automatically move to the next line once it hits the maximum width limit.", 
                           50, 200, 400, 20, RAYWHITE);

    // 4. Centered Footer
    SSO::Text::DrawCentered("PRESS START TO CONTINUE", 600, 25, GRAY);

    SSO::Window::EndDrawingVirtual();
}
```

## 🎥 sso_camera.h - Advanced Camera System

### What it ACTUALLY Does
High-performance 2D Camera with smooth interpolation, zoom control, and dynamic screen shake effects.

### Include
```cpp
#include "tools/sso_camera.h"
```

### Available Functions

#### **Initialize Camera**
```cpp
SSO::Camera camera(Vector2 target, int screenWidth = 1280, int screenHeight = 720);

// Example:
SSO::Camera camera(Vector2{400, 300}, 1280, 720);
```

#### **Camera Following with Smoothing**
```cpp
void Follow(Vector2 targetPos, float dt);

// Example:
camera.Follow(playerPosition, deltaTime);
```

#### **Screen Shake Effects**
```cpp
void Shake(float intensity, float duration);

// Example:
camera.Shake(10.0f, 0.5f); // Shake for 0.5 seconds with intensity 10
```

#### **Configuration**
```cpp
void SetZoom(float zoom);
float GetZoom() const;

void SetSmoothing(float s);  // 0.0 = very smooth, 1.0 = stiff
float GetSmoothing() const;
```

#### **Camera Rendering**
```cpp
camera.Begin();  // BeginMode2D
// Draw your game objects here
camera.End();    // EndMode2D
```

### Example Usage
```cpp
#include "tools/sso_camera.h"
#include "tools/sso_timer.h"

int main() {
    SSO::Window::Init(800, 600, "Advanced Camera Demo");
    
    SSO::Camera camera(Vector2{0, 0}, 800, 600);
    SSO::Timer timer;
    
    Vector2 playerPos = {0, 0};
    
    while (!WindowShouldClose()) {
        float dt = timer.GetDeltaTime();
        
        // Player movement
        if (IsKeyDown(KEY_RIGHT)) playerPos.x += 200 * dt;
        if (IsKeyDown(KEY_LEFT)) playerPos.x -= 200 * dt;
        
        // Camera follow with smoothing
        camera.Follow(playerPos, dt);
        
        // Trigger shake on space
        if (IsKeyPressed(KEY_SPACE)) {
            camera.Shake(15.0f, 0.3f);
        }
        
        // Rendering
        SSO::Window::BeginDrawingVirtual();
        
        camera.Begin();
        DrawCircleV(playerPos, 20, BLUE);
        DrawGrid(20, 50);
        camera.End();
        
        SSO::Window::EndDrawingVirtual();
    }
    
    SSO::Window::Close();
    return 0;
}
```

---

## ⏱️ sso_timer.h - High-Precision Timer

### What it ACTUALLY Does
Universal Time Management System with high-precision delta time and flexible game timers.

### Include
```cpp
#include "tools/sso_timer.h"
```

### Available Functions

#### **Core Engine Timing**
```cpp
SSO::Timer timer;

float deltaTime = timer.GetDeltaTime();    // Frame duration
float totalTime = timer.GetTotalTime();    // Total elapsed time
```

#### **Game Timer Management**
```cpp
// Countdown timer (waktu mundur)
timer.SetValue(60.0f);  // Set 60 seconds
timer.Start();          // Start countdown
timer.UpdateCountdown(deltaTime);  // Update each frame

if (timer.IsFinished()) {
    // Timer reached zero
}

// Stopwatch timer (waktu maju)
timer.Reset();
timer.Start();
timer.UpdateStopwatch(deltaTime);  // Count up

float currentTime = timer.GetValue();  // Get current value
```

#### **Timer Control**
```cpp
timer.Start();     // Start/resume
timer.Stop();      // Pause
timer.Reset();     // Reset to zero
bool isRunning = timer.IsRunning();
```

### Example Usage
```cpp
#include "tools/sso_timer.h"
#include "tools/sso_logger.h"

int main() {
    SSO::Window::Init(800, 600, "Timer Demo");
    
    SSO::Timer engineTimer;
    SSO::Timer gameTimer;
    
    // Set 10-second countdown
    gameTimer.SetValue(10.0f);
    gameTimer.Start();
    
    while (!WindowShouldClose()) {
        float dt = engineTimer.GetDeltaTime();
        
        // Update countdown
        gameTimer.UpdateCountdown(dt);
        
        SSO::Window::BeginDrawingVirtual();
        ClearBackground(RAYWHITE);
        
        DrawText(TextFormat("Time Left: %.1f", gameTimer.GetValue()), 
                300, 250, 30, BLACK);
        
        if (gameTimer.IsFinished()) {
            DrawText("TIME'S UP!", 320, 300, 20, RED);
        }
        
        SSO::Window::EndDrawingVirtual();
    }
    
    SSO::Window::Close();
    return 0;
}
```

---

## 🪟 sso_window.h - Virtual Resolution Manager

### What it ACTUALLY Does
Advanced window management with virtual resolution, letterboxing, and render-to-texture system.

### Include
```cpp
#include "tools/sso_window.h"
```

### Available Functions

#### **Window Initialization**
```cpp
SSO::Window::Init(width, height, title);

// Example:
SSO::Window::Init(1280, 720, "Game Title");
```

#### **Virtual Resolution System**
```cpp
// Change internal resolution (maintains aspect ratio)
SSO::Window::SetResolution(1920, 1080);

// Window resizing controls
SSO::Window::EnableResizing();
SSO::Window::DisableResizing();
```

#### **Virtual Canvas Rendering**
```cpp
SSO::Window::BeginDrawingVirtual();
// Draw everything here - it's rendered to virtual canvas
ClearBackground(BLACK);
DrawCircle(640, 360, 100, RED);
SSO::Window::EndDrawingVirtual();
// This automatically handles letterboxing and scaling
```

#### **Fullscreen Toggle**
```cpp
SSO::Window::ToggleFull();  // Proper fullscreen with resolution restore
```

#### **Developer Features**
```cpp
// Show debug border around virtual area
SSO::Window::showBorder = true;
SSO::Window::borderColor = MAROON;
SSO::Window::borderThickness = 4.0f;

// Custom watermark/icon
SSO::Window::customIcon = LoadTexture("watermark.png");
```

#### **Cleanup**
```cpp
SSO::Window::Close();  // Proper resource cleanup
```

### Example Usage
```cpp
#include "tools/sso_window.h"
#include "tools/sso_camera.h"

int main() {
    // Initialize with virtual resolution
    SSO::Window::Init(1280, 720, "Virtual Resolution Demo");
    
    // Enable debug features
    SSO::Window::showBorder = true;
    SSO::Window::borderColor = GREEN;
    
    SSO::Camera camera(Vector2{0, 0}, 1280, 720);
    
    while (!WindowShouldClose()) {
        // Handle fullscreen toggle
        if (IsKeyPressed(KEY_F11)) {
            SSO::Window::ToggleFull();
        }
        
        // Virtual canvas rendering
        SSO::Window::BeginDrawingVirtual();
        
        camera.Begin();
        DrawGrid(40, 100);
        DrawRectangle(-100, -100, 200, 200, BLUE);
        camera.End();
        
        SSO::Window::EndDrawingVirtual();
    }
    
    SSO::Window::Close();
    return 0;
}
```

---

## 🎨 sso_ui.h - Advanced UI System

### What it ACTUALLY Does
Complete UI system with styled buttons, panels, scrollable areas, modals, and status bars.

### Include
```cpp
#include "tools/sso_ui.h"
```

### Available Functions

#### **UI Styling**
```cpp
SSO::UI::UIStyle buttonStyle = {
    .baseColor = {40, 40, 40, 255},      // Normal color
    .hoverColor = {80, 80, 80, 255},     // Hover color  
    .textColor = WHITE,
    .roundness = 0.2f,
    .borderThickness = 2.0f
};
```

#### **Core UI Elements**
```cpp
// Button with custom font and style
bool clicked = SSO::UI::DrawButton(rect, "Click Me", font, buttonStyle);

// Panel with header
SSO::UI::DrawPanel(rect, "Panel Title", font);

// Health/status bar
SSO::UI::DrawHealthBar(rect, currentHealth, maxHealth, GREEN, RED);
```

#### **Scrollable Areas**
```cpp
float scrollPos = 0.0f;
float contentHeight = 1000.0f;

SSO::UI::BeginScrollArea(viewRect, &scrollPos, contentHeight);
// Draw your scrollable content here
for (int i = 0; i < 50; i++) {
    DrawText(TextFormat("Item %d", i), 20, i * 30 + scrollPos, 20, WHITE);
}
SSO::UI::EndScrollArea();
```

#### **Modal Popups**
```cpp
bool popupActive = true;

bool confirmed = SSO::UI::DrawPopUp(
    popupRect, 
    "Confirm", 
    "Are you sure you want to quit?", 
    font, 
    &popupActive
);
```

#### **Visual Effects**
```cpp
// Background texture
SSO::UI::DrawBackground(backgroundTexture, WHITE);

// Overlay for modals
SSO::UI::DrawOverlay(0.7f);

// Text with shadow
SSO::UI::DrawTextShadow(font, "Shadow Text", pos, 20, 1, WHITE);
```

### Example Usage
```cpp
#include "tools/sso_ui.h"
#include "tools/sso_window.h"

int main() {
    SSO::Window::Init(800, 600, "Advanced UI Demo");
    Font font = LoadFont("resources/font.png");
    
    Rectangle buttonRect = {300, 250, 200, 50};
    Rectangle panelRect = {200, 150, 400, 300};
    Rectangle healthBarRect = {300, 350, 200, 30};
    
    float health = 75.0f;
    bool popupActive = false;
    
    while (!WindowShouldClose()) {
        SSO::Window::BeginDrawingVirtual();
        ClearBackground(DARKGRAY);
        
        // Draw panel
        SSO::UI::DrawPanel(panelRect, "UI Demo", font);
        
        // Draw button
        if (SSO::UI::DrawButton(buttonRect, "Show Popup", font)) {
            popupActive = true;
        }
        
        // Draw health bar
        SSO::UI::DrawHealthBar(healthBarRect, health, 100.0f, GREEN, RED);
        
        // Draw popup if active
        if (popupActive) {
            Rectangle popupRect = {250, 200, 300, 150};
            if (SSO::UI::DrawPopUp(popupRect, "Popup", "Hello World!", font, &popupActive)) {
                // Popup confirmed
            }
        }
        
        SSO::Window::EndDrawingVirtual();
    }
    
    UnloadFont(font);
    SSO::Window::Close();
    return 0;
}
```

---

## 🔢 sso_math.h - Collision & Math

### What it ACTUALLY Does
Essential collision detection and math utilities for 2D games.

### Include
```cpp
#include "tools/sso_math.h"
```

### Available Functions

#### **Collision Detection**
```cpp
// AABB Collision (Rectangle vs Rectangle)
bool collision = CheckCollision(rect1, rect2);

// Circle collision (built into Raylib)
bool circleCollision = CheckCollisionCircles(circle1, radius1, circle2, radius2);
```

#### **Vector Utilities**
```cpp
// Clamp vector within boundaries
Vector2 clampedPos = ClampVector2(position, minX, minY, maxX, maxY);

// Distance calculation
float distance = GetDistance(pos1, pos2);
```

#### **Collision Resolution**
```cpp
// Solid collision resolution (prevents passing through walls)
Rectangle playerRect = {playerX, playerY, playerWidth, playerHeight};
Rectangle wallRect = {wallX, wallY, wallWidth, wallHeight};
Vector2 previousPlayerPos = {prevX, prevY};

ResolveCollision(&playerRect, wallRect, previousPlayerPos);
```

### Example Usage
```cpp
#include "tools/sso_math.h"
#include "tools/sso_window.h"

int main() {
    SSO::Window::Init(800, 600, "Collision Demo");
    
    Rectangle player = {100, 100, 40, 40};
    Rectangle wall = {300, 200, 100, 20};
    Vector2 previousPos = {100, 100};
    
    while (!WindowShouldClose()) {
        // Store previous position
        previousPos = {player.x, player.y};
        
        // Player movement
        if (IsKeyDown(KEY_RIGHT)) player.x += 5;
        if (IsKeyDown(KEY_LEFT)) player.x -= 5;
        if (IsKeyDown(KEY_UP)) player.y -= 5;
        if (IsKeyDown(KEY_DOWN)) player.y += 5;
        
        // Keep player in screen bounds
        player.x = ClampVector2({player.x, player.y}, 0, 0, 760, 560).x;
        player.y = ClampVector2({player.x, player.y}, 0, 0, 760, 560).y;
        
        // Check collision with wall
        if (CheckCollision(player, wall)) {
            // Resolve collision - move back to previous position
            ResolveCollision(&player, wall, previousPos);
        }
        
        SSO::Window::BeginDrawingVirtual();
        ClearBackground(RAYWHITE);
        
        DrawRectangleRec(player, BLUE);
        DrawRectangleRec(wall, RED);
        
        // Show collision status
        if (CheckCollision(player, wall)) {
            DrawText("COLLISION!", 10, 10, 20, RED);
        }
        
        SSO::Window::EndDrawingVirtual();
    }
    
    SSO::Window::Close();
    return 0;
}
```

---

## 📝 sso_logger.h - Colored Console Logger

### What it ACTUALLY Does
Console logging system with colored output and timestamps.

### Include
```cpp
#include "tools/sso_logger.h"
```

### Available Functions

#### **Basic Logging**
```cpp
SSO::Logger::Log(SSO::Logger::Level::INFO, "Game started");
SSO::Logger::Log(SSO::Logger::Level::WARNING, "Low memory");
SSO::Logger::Log(SSO::Logger::Level::ERROR, "Failed to load texture");
SSO::Logger::Log(SSO::Logger::Level::SUCCESS, "Level loaded");
```

#### **Output Colors**
- **INFO** - Cyan color
- **WARNING** - Yellow color  
- **ERROR** - Red color
- **SUCCESS** - Green color

#### **Timestamp Format**
```
[14:32:15] [INFO] Game started
[14:32:16] [WARNING] Low memory
[14:32:17] [SUCCESS] Level loaded
```

### Example Usage
```cpp
#include "tools/sso_logger.h"
#include "tools/sso_window.h"

int main() {
    SSO::Logger::Log(SSO::Logger::Level::INFO, "Initializing game...");
    
    SSO::Window::Init(800, 600, "Logger Demo");
    SSO::Logger::Log(SSO::Logger::Level::SUCCESS, "Window initialized");
    
    Texture2D texture = LoadTexture("player.png");
    if (texture.id <= 0) {
        SSO::Logger::Log(SSO::Logger::Level::ERROR, "Failed to load player.png");
    } else {
        SSO::Logger::Log(SSO::Logger::Level::SUCCESS, "Player texture loaded");
    }
    
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            SSO::Logger::Log(SSO::Logger::Level::INFO, "Space key pressed");
        }
        
        SSO::Window::BeginDrawingVirtual();
        ClearBackground(RAYWHITE);
        DrawText("Check console for log messages", 200, 300, 20, BLACK);
        SSO::Window::EndDrawingVirtual();
    }
    
    SSO::Logger::Log(SSO::Logger::Level::INFO, "Game shutting down...");
    SSO::Window::Close();
    return 0;
}
```

---

## � sso_ext.h - System Extension & File Operations

### What it ACTUALLY Does
Cross-platform system operations including file management, downloads, git operations, and environment variables.

### Include
```cpp
#include "tools/sso_ext.h"
```

### Available Functions

#### **System Commands**
```cpp
// Execute system command
int result = SSO_System("dir");

// Get command output as string
std::string output = SSO_GetOutput("ls -la");
```

#### **Tool Detection**
```cpp
// Check if tool exists on system
bool hasGit = SSO_HasTool("git");
bool hasCurl = SSO_HasTool("curl");
```

#### **File & Directory Operations**
```cpp
// Download file (auto-detects curl/wget/powershell)
SSO_Download("https://example.com/file.zip", "downloads/file.zip");

// Copy/Move/Remove files and folders
SSO_Copy("source.txt", "backup.txt");
SSO_Move("old.txt", "new.txt");
SSO_Remove("temp_folder");

// Create directories
SSO_MakeFolder("data/assets/textures");
```

#### **Git Operations**
```cpp
// Execute git commands
SSO_Git("clone https://github.com/user/repo.git");
SSO_Git("pull origin main");
SSO_Git("add .");
SSO_Git("commit -m 'Update files'");
```

#### **System Integration**
```cpp
// Open files/folders with default application
SSO_Open("index.html");        // Opens in browser
SSO_Open("folder");            // Opens file explorer

// Kill processes
SSO_KillProcess("notepad.exe");

// Environment variables
std::string path = SSO_GetEnv("PATH");
SSO_SetEnv("MY_VAR", "value");
```

#### **File Utilities**
```cpp
// Get file size
long long size = SSO_GetFileSize("large_file.txt");

// Check if file exists (returns -1 if not found)
if (size > 0) {
    // File exists and has size
}
```

### Example Usage
```cpp
#include "tools/sso_ext.h"
#include "tools/sso_logger.h"

void SetupProject() {
    // Create project structure
    SSO_MakeFolder("assets");
    SSO_MakeFolder("assets/textures");
    SSO_MakeFolder("assets/sounds");
    
    // Download required assets
    if (SSO_HasTool("curl")) {
        SSO::Logger::Log(SSO::Logger::Level::INFO, "Downloading assets...");
        SSO_Download("https://example.com/textures.png", "assets/textures/player.png");
    }
    
    // Initialize git repository
    if (SSO_HasTool("git")) {
        SSO_Git("init");
        SSO_Git("add .");
        SSO_Git("commit -m 'Initial commit'");
    }
    
    // Open project folder
    SSO_Open(".");
}
```

---

## 📁 sso_file.h - Windows File Dialogs

### What it ACTUALLY Does
Native Windows file dialogs for opening files, saving files, and browsing folders.

### Include
```cpp
#include "tools/sso_file.h"
```

### Available Functions

#### **Open File Dialog**
```cpp
std::string filePath = SSO_OpenFile(
    "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0", 
    "Select a text file"
);

if (!filePath.empty()) {
    // User selected a file
}
```

#### **Save File Dialog**
```cpp
std::string savePath = SSO_SaveFile(
    "PNG Files (*.png)\0*.png\0JPEG Files (*.jpg)\0*.jpg\0", 
    "Save screenshot"
);

if (!savePath.empty()) {
    // User chose save location
}
```

#### **Browse Folder Dialog**
```cpp
std::string folderPath = SSO_BrowseFolder("Select project folder");

if (!folderPath.empty()) {
    // User selected a folder
}
```

### Common File Filters
```cpp
// Images
"Image Files\0*.png;*.jpg;*.jpeg;*.bmp\0All Files\0*.*\0"

// Audio
"Audio Files\0*.wav;*.mp3;*.ogg\0All Files\0*.*\0"

// Documents
"Documents\0*.txt;*.doc;*.pdf\0All Files\0*.*\0"

// All supported formats
"Supported Files\0*.png;*.jpg;*.txt;*.json\0All Files\0*.*\0"
```

### Example Usage
```cpp
#include "tools/sso_file.h"
#include "tools/sso_window.h"
#include "raylib.h"

void HandleFileOperations() {
    // Load texture from file dialog
    if (IsKeyPressed(KEY_L)) {
        std::string texturePath = SSO_OpenFile(
            "Image Files\0*.png;*.jpg;*.jpeg\0All Files\0*.*\0",
            "Load Texture"
        );
        
        if (!texturePath.empty()) {
            Texture2D tex = LoadTexture(texturePath.c_str());
            // Use texture...
        }
    }
    
    // Save screenshot
    if (IsKeyPressed(KEY_S)) {
        std::string savePath = SSO_SaveFile(
            "PNG Files\0*.png\0All Files\0*.*\0",
            "Save Screenshot"
        );
        
        if (!savePath.empty()) {
            TakeScreenshot(savePath.c_str());
        }
    }
    
    // Select project folder
    if (IsKeyPressed(KEY_O)) {
        std::string projectFolder = SSO_BrowseFolder("Select Project Folder");
        if (!projectFolder.empty()) {
            // Change working directory or load project
        }
    }
}
```

---

## 🌊 sso_splash.h - Splash Screen System

### What it ACTUALLY Does
Professional splash screen with fade in/out effects and progress bar for game startup.

### Include
```cpp
#include "tools/sso_splash.h"
```

### Available Functions

#### **Show Splash Screen**
```cpp
SSO::Splash::Show(float duration);

// Example:
SSO::Splash::Show(3.0f); // Show for 3 seconds
```

### Visual Features
- **Fade In/Out** - Smooth alpha transitions
- **Centered Text** - "SSOENGINE" title
- **Subtitle** - "Lightweight C++ Power"
- **Progress Bar** - Bottom loading indicator
- **Early Exit** - Can be closed with window close button

### Example Usage
```cpp
#include "tools/sso_splash.h"
#include "tools/sso_window.h"

int main() {
    // Show splash screen before window initialization
    SSO::Splash::Show(2.5f);
    
    // Initialize main game window
    SSO::Window::Init(1280, 720, "My Game");
    
    // Game loop...
    
    SSO::Window::Close();
    return 0;
}
```

---

## 📦 sso_provider.h - Asset Bundle System

### What it ACTUALLY Does
Custom asset bundle format (.sso) for packaging game assets into single files with fast loading.

### Include
```cpp
#include "tools/sso_provider.h"
```

### Available Functions

#### **Load Raw Data from Bundle**
```cpp
int dataSize;
unsigned char* data = SSO::Provider::LoadRawDataFromBundle(
    "assets.sso", 
    "textures/player.png", 
    &dataSize
);

if (data) {
    // Use data...
    free(data); // Don't forget to free!
}
```

#### **Load Raylib Resources from Bundle**
```cpp
// Load texture
Texture2D texture = SSO::Provider::LoadTextureFromBundle("assets.sso", "player.png");

// Load sound
Sound sound = SSO::Provider::LoadSoundFromBundle("assets.sso", "jump.wav");

// Load music
Music music = SSO::Provider::LoadMusicFromBundle("assets.sso", "background.ogg");
```

#### **Bundle Structure**
```
SSO Bundle Format:
├── Header (SSO + version + file count)
├── File Metadata Array
│   ├── filename (64 bytes)
│   ├── file size (8 bytes)
│   └── offset (8 bytes)
└── Raw File Data
```

### Example Usage
```cpp
#include "tools/sso_provider.h"
#include "tools/sso_window.h"

struct GameAssets {
    Texture2D playerTexture;
    Texture2D enemyTexture;
    Sound jumpSound;
    Music backgroundMusic;
};

bool LoadAssets(GameAssets& assets) {
    // Load all assets from bundle
    assets.playerTexture = SSO::Provider::LoadTextureFromBundle("game.sso", "player.png");
    assets.enemyTexture = SSO::Provider::LoadTextureFromBundle("game.sso", "enemy.png");
    assets.jumpSound = SSO::Provider::LoadSoundFromBundle("game.sso", "jump.wav");
    assets.backgroundMusic = SSO::Provider::LoadMusicFromBundle("game.sso", "background.ogg");
    
    // Check if all loaded successfully
    return (assets.playerTexture.id > 0 && 
            assets.enemyTexture.id > 0 && 
            assets.jumpSound.frameCount > 0);
}
```

---

## � sso_ph3d.h - Optional Physics Helper

### What it ACTUALLY Does
Optional physics helper for advanced rigid body dynamics, collision detection, gravity simulation, and angular velocity. This header is included for experimental cases and is not required by the core 2D engine.

### Include
```cpp
#include "tools/sso_ph3d.h"
```

### Available Functions

#### **Physics World Management**
```cpp
// Initialize physics world
SSO::Physics3D::Init();
SSO::Physics3D::SetGravity(-29.43f); // 3x Earth gravity for realistic falling

// Step physics simulation
SSO::Physics3D::Step(deltaTime);

// Get physics statistics
int bodyCount = SSO::Physics3D::GetBodyCount();

// Cleanup physics world
SSO::Physics3D::Cleanup();
```

#### **Rigid Body Creation**
```cpp
// Create dynamic physics body
SSO::Physics3D::RigidBody* cube = SSO::Physics3D::CreateBody(
    position,    // Vector3 position
    size,        // Vector3 dimensions
    mass         // float mass
);

// Create static body (immovable)
SSO::Physics3D::RigidBody* ground = SSO::Physics3D::CreateStaticBody(
    position,    // Vector3 position
    size         // Vector3 dimensions
);

// Configure body properties
cube->restitution = 0.2f;  // Bounciness (0.0 = no bounce, 1.0 = super bouncy)
cube->friction = 0.9f;       // Surface friction
cube->color = BLUE;          // Visual color
```

#### **Forces & Impulses**
```cpp
// Apply continuous force
SSO::Physics3D::ApplyForce(body, forceVector);

// Apply instant impulse (for explosions, shooting)
SSO::Physics3D::ApplyImpulse(body, impulseVector);

// Bullet shooting with raycast collision
SSO::Physics3D::ShootBullet(position, direction, force);

// Explosion effect
SSO::Physics3D::Explode(center, force, radius);
```

#### **Physics Properties**
```cpp
struct RigidBody {
    Vector3 position;          // Current position
    Vector3 velocity;          // Linear velocity
    Vector3 acceleration;      // Current acceleration
    Vector3 angularVelocity;   // Rotation speed
    Vector3 size;             // Dimensions
    float mass;               // Object mass
    float restitution;        // Bounciness
    float friction;           // Surface friction
    bool isStatic;            // Immovable object
    bool isGrounded;          // Touching ground
    bool isActive;            // Physics enabled
    Color color;              // Visual color
};
```

#### **Rendering Helpers**
```cpp
// Draw all physics bodies
SSO::Physics3D::DrawAllBodies();

// Draw single body with wireframe
SSO::Physics3D::DrawBody(body);

// Get bounding box for collision
Vector3 minBounds = SSO::Physics3D::GetBoundingBoxMin(body);
Vector3 maxBounds = SSO::Physics3D::GetBoundingBoxMax(body);
```

### Physics Features
- **Realistic Gravity** - Earth gravity simulation with customizable strength
- **Collision Detection** - AABB collision with penetration depth calculation
- **Collision Response** - Impulse-based resolution with mass consideration
- **Angular Velocity** - Objects tumble and rotate realistically
- **Chain Reactions** - Force propagation through connected objects
- **Ground Collision** - Proper bouncing and settling on surfaces
- **Sub-stepping** - Stable simulation at 60 FPS with multiple physics steps

### Example Usage
```cpp
#include "tools/sso_ph3d.h"
#include "tools/sso_window.h"

void InitPhysicsDemo() {
    // Initialize physics world
    SSO::Physics3D::Init();
    SSO::Physics3D::SetGravity(-29.43f); // 3x Earth gravity
    
    // Create ground
    SSO::Physics3D::CreateStaticBody({0, -0.5f, 0}, {20, 1, 20});
    
    // Create pyramid of cubes
    for (int layer = 0; layer < 4; layer++) {
        for (int i = 0; i < (4 - layer); i++) {
            Vector3 pos = {(float)i * 1.1f - 1.65f, layer * 1.1f + 1.0f, 0.0f};
            SSO::Physics3D::RigidBody* cube = SSO::Physics3D::CreateBody(pos, {1.0f, 1.0f, 1.0f}, 1.0f);
            cube->restitution = 0.2f; // Low bounciness
            cube->friction = 0.9f;    // High friction
            cube->color = colors[layer];
        }
    }
}

void UpdatePhysics(float dt) {
    // Step physics simulation
    SSO::Physics3D::Step(dt);
    
    // Handle shooting
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector3 shootPos = camera.position;
        Vector3 shootDir = GetCameraForward(camera);
        SSO::Physics3D::ShootBullet(shootPos, shootDir, 35.0f);
    }
}

void RenderPhysics() {
    // Draw all physics bodies
    SSO::Physics3D::DrawAllBodies();
    
    // Draw physics info
    DrawText(TextFormat("Bodies: %d", SSO::Physics3D::GetBodyCount()), 10, 10, 20, WHITE);
    DrawText("Gravity: -29.43 m/s²", 10, 40, 20, WHITE);
}
```

---

##  Android Build System - Cross-Platform Development

### What it ACTUALLY Does
Complete Android build system that converts your SSOEngine game into a production-ready APK with proper native library packaging, signing, and deployment.

### Build System Components

#### **1. Automated Build Script**
```batch
build.bat
```
- **Auto-detects** Android SDK and build-tools
- **Cross-compiles** for ARM64 architecture
- **Packages** native libraries correctly
- **Signs** APK with production keystore
- **Verifies** APK integrity

#### **2. Android Project Structure**
```
01_Core/android/
|-- AndroidManifest.xml    # App configuration
|-- main_android.cpp       # Android entry point
|-- src/                   # Java source
|   `-- com/ssogames/ssoengine/
|       |-- MainActivity.java
|       `-- R.java
|-- res/                   # Android resources
|   `-- values/
|       `-- strings.xml
`-- libs/                  # Native libraries (auto-generated)
```

### Quick Start - Build Your Android APK

#### **Prerequisites**
- Android SDK with build-tools
- Android NDK (for cross-compilation)
- Raylib Android library

#### **Build Process**
```batch
# Run the build script
build.bat

# Select option [2] for Android build
[2] Android Build

# Watch the magic happen:
# 1. CMake cross-compilation
# 2. Native library creation
# 3. APK packaging
# 4. Signing and verification
```

#### **Output Files**
```
android/bin/SSOEngine.apk    # Final production APK
build/libSSOEngine.so        # Native library
```

### Android-Specific Features

#### **Native Activity Integration**
```xml
<!-- AndroidManifest.xml -->
<activity android:name="android.app.NativeActivity"
          android:hasCode="false"
          android:extractNativeLibs="true">
    <meta-data android:name="android.app.lib_name" 
               android:value="SSOEngine" />
</activity>
```

#### **Asset Manager Integration**
```cpp
// main_android.cpp
extern "C" {
    AAssetManager* GetAndroidAssetManager() {
        return androidAssetManager;
    }
}
```

#### **Cross-Platform Asset Loading**
```cpp
// Automatic platform detection
#ifdef PLATFORM_ANDROID
    // Load from Android assets
    Texture2D tex = SSO::Provider::LoadTextureFromBundle("assets.sso", "player.png");
#else
    // Load from PC filesystem
    Texture2D tex = LoadTexture("assets/player.png");
#endif
```

### APK Structure Verification

Your final APK contains:
```
APK ROOT:
|-- AndroidManifest.xml
|-- classes.dex
|-- lib/
|   `-- arm64-v8a/
|       `-- libSSOEngine.so    # Your game!
|-- res/
|   `-- values/
|       `-- strings.xml
`-- resources.arsc
```

### Troubleshooting Common Issues

#### **"Unable to find native library"**
```xml
<!-- Ensure these are in AndroidManifest.xml -->
android:extractNativeLibs="true"
android:hasCode="false"
```

#### **Force Close on Startup**
```cpp
// Check main_android.cpp has asset manager setup
AAssetManager* androidAssetManager = nullptr;

// Ensure no duplicate symbols
// Raylib handles android_main internally
```

#### **APK Installation Issues**
```batch
# Verify APK signature
build-tools/apksigner verify --verbose android/bin/SSOEngine.apk

# Check APK contents
build-tools/aapt list android/bin/SSOEngine.apk
```

### Advanced Configuration

#### **Custom Keystore**
```batch
# Build script auto-creates ssoengine.keystore
# Password: rozak123
# Alias: ssoengine
```

#### **Target Android Versions**
```xml
<uses-sdk android:minSdkVersion="29"
          android:targetSdkVersion="33" />
```

#### **Permissions**
```xml
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
```

### Deployment Ready

Your APK is production-ready with:
- **Proper signing** (V2/V3 schemes)
- **Optimized libraries** (ARM64)
- **Correct structure** (lib/arm64-v8a/)
- **Manifest configuration** (NativeActivity)
- **Asset integration** (Bundle system)

### Example: Cross-Platform Game Loop

```cpp
#include "tools/sso_window.h"
#include "tools/sso_camera.h"
#include "tools/sso_provider.h"

int main() {
    // Platform-agnostic initialization
    SSO::Window::Init(1280, 720, "SSOEngine Game");
    
    // Load assets (cross-platform)
    #ifdef PLATFORM_ANDROID
        Texture2D player = SSO::Provider::LoadTextureFromBundle("game.sso", "player.png");
    #else
        Texture2D player = LoadTexture("assets/player.png");
    #endif
    
    SSO::Camera camera({0, 0}, 1280, 720);
    
    while (!WindowShouldClose()) {
        // Game logic works on both platforms!
        SSO::Window::BeginDrawingVirtual();
        
        camera.Begin();
        DrawTexture(player, 0, 0, WHITE);
        camera.End();
        
        SSO::Window::EndDrawingVirtual();
    }
    
    SSO::Window::Close();
    return 0;
}
```

**Result:** Same codebase, Windows executable AND Android APK! 

### Advanced Android Examples

#### **Complete Cross-Platform Game**
```cpp
#include "tools/sso_window.h"
#include "tools/sso_camera.h"
#include "tools/sso_provider.h"
#include "tools/sso_text.h"

struct Game {
    Texture2D player;
    Texture2D enemy;
    Sound jumpSound;
    SSO::Camera camera;
};

// Cross-platform asset loading
bool LoadGameAssets(Game& game) {
#ifdef PLATFORM_ANDROID
    // Android: Load from asset bundle
    game.player = SSO::Provider::LoadTextureFromBundle("game.sso", "player.png");
    game.enemy = SSO::Provider::LoadTextureFromBundle("game.sso", "enemy.png");
    game.jumpSound = SSO::Provider::LoadSoundFromBundle("game.sso", "jump.wav");
#else
    // Windows: Load from filesystem
    game.player = LoadTexture("assets/player.png");
    game.enemy = LoadTexture("assets/enemy.png");
    game.jumpSound = LoadSound("assets/jump.wav");
#endif
    
    return (game.player.id > 0 && game.enemy.id > 0);
}

int main() {
    // Cross-platform initialization
    SSO::Window::Init(1280, 720, "Cross-Platform Game");
    
    Game game;
    if (!LoadGameAssets(game)) {
        TraceLog(LOG_ERROR, "Failed to load game assets");
        return -1;
    }
    
    game.camera = SSO::Camera({0, 0}, 1280, 720);
    
    while (!WindowShouldClose()) {
        // Input handling (works on both platforms)
        if (IsKeyPressed(KEY_SPACE)) {
            PlaySound(game.jumpSound);
        }
        
        // Camera follow player (example)
        Vector2 playerPos = {400, 300};
        game.camera.Follow(playerPos, GetFrameTime());
        
        // Cross-platform rendering
        SSO::Window::BeginDrawingVirtual();
        ClearBackground(BLACK);
        
        game.camera.Begin();
        DrawTexture(game.player, 400, 300, WHITE);
        DrawTexture(game.enemy, 600, 300, WHITE);
        game.camera.End();
        
        // UI overlay
        SSO::Text::DrawCentered("Cross-Platform SSOEngine Game", 50, 30, WHITE);
        
        SSO::Window::EndDrawingVirtual();
    }
    
    // Cleanup
    UnloadTexture(game.player);
    UnloadTexture(game.enemy);
    UnloadSound(game.jumpSound);
    SSO::Window::Close();
    
    return 0;
}
```

#### **Android-Specific Features**
```cpp
// Android asset manager integration
#include "tools/sso_provider.h"

void AndroidSpecificFeatures() {
#ifdef PLATFORM_ANDROID
    // Get Android asset manager
    AAssetManager* assetMgr = GetAndroidAssetManager();
    
    if (assetMgr) {
        TraceLog(LOG_INFO, "Android asset manager loaded successfully");
        
        // Load Android-specific assets
        Texture2D androidTex = SSO::Provider::LoadTextureFromBundle("android.sso", "icon.png");
        
        // Use Android-specific features
        // - Touch input handling
        // - Screen orientation
        // - Back button handling
    }
#else
    TraceLog(LOG_INFO, "Running on desktop platform");
#endif
}
```

#### **Build Automation Script**
```batch
@echo off
echo ========================================
echo    SSOEngine Android Builder
echo ========================================

:: Check prerequisites
if not exist "%ANDROID_HOME%" (
    echo [ERROR] ANDROID_HOME not set!
    echo Please install Android SDK and set ANDROID_HOME
    pause
    exit /b 1
)

if not exist "%ANDROID_NDK_HOME%" (
    echo [ERROR] ANDROID_NDK_HOME not set!
    echo Please install Android NDK and set ANDROID_NDK_HOME
    pause
    exit /b 1
)

:: Build options
echo.
echo [1] Windows Build
echo [2] Android Build
echo [3] Clean Build
echo.
set /p choice="Select build option: "

if "%choice%"=="1" goto windows_build
if "%choice%"=="2" goto android_build
if "%choice%"=="3" goto clean_build

:android_build
echo [INFO] Starting Android build...
call build_android.bat
goto end

:windows_build
echo [INFO] Starting Windows build...
call build_windows.bat
goto end

:clean_build
echo [INFO] Cleaning build directories...
rmdir /s /q build
rmdir /s /q android\bin
rmdir /s /q lib
goto end

:end
echo.
echo [SUCCESS] Build completed!
pause
```

### Android Deployment Guide

#### **Install APK on Device**
```bash
# Install via ADB
adb install android/bin/SSOEngine.apk

# Install and run
adb install android/bin/SSOEngine.apk && adb shell am start -n com.ssogames.ssoengine/.MainActivity

# Debug with logcat
adb logcat | grep SSOEngine
```

#### **Release Checklist**
- [ ] **APK Signed** with production keystore
- [ ] **Native Libraries** in lib/arm64-v8a/
- [ ] **Manifest** has android:extractNativeLibs="true"
- [ ] **Assets** bundled in .sso format
- [ ] **Permissions** minimal and necessary
- [ ] **Target SDK** compatible with device
- [ ] **Tested** on actual Android device

#### **Common Android Issues & Solutions**

**Issue: "Unable to find native library"**
```xml
<!-- Solution: Add to AndroidManifest.xml -->
<application android:extractNativeLibs="true">
    <activity android:hasCode="false">
        <meta-data android:name="android.app.lib_name" android:value="SSOEngine" />
    </activity>
</application>
```

**Issue: "Force close on startup"**
```cpp
// Solution: Check main_android.cpp
extern "C" {
    AAssetManager* GetAndroidAssetManager() {
        return androidAssetManager;  // Must be global
    }
}
```

**Issue: "Assets not loading"**
```cpp
// Solution: Use platform-specific loading
#ifdef PLATFORM_ANDROID
    Texture2D tex = SSO::Provider::LoadTextureFromBundle("game.sso", "player.png");
#else
    Texture2D tex = LoadTexture("assets/player.png");
#endif
```

---

## 🎬 sso_splash.h - Professional Splash Screen System

### What it ACTUALLY Does
Professional splash screen system with fade-in/fade-out animations, progress bar, and customizable duration. Perfect for giving your game a professional startup experience.

### Include
```cpp
#include "tools/sso_splash.h"
```

### Available Functions

#### **Show Splash Screen**
```cpp
// Show splash screen for specified duration (in seconds)
SSO::Splash::Show(2.0f);  // Show for 2 seconds
SSO::Splash::Show(3.5f);  // Show for 3.5 seconds
SSO::Splash::Show(1.0f);  // Show for 1 second
```

### Features
- **Automatic Fade Effects** - Smooth fade-in and fade-out animations
- **Progress Bar** - Visual loading progress indicator
- **Professional Branding** - "SSOENGINE" title with "Lightweight C++ Power" subtitle
- **Customizable Duration** - Set any duration in seconds
- **Early Exit** - Can be closed with window close button
- **Smooth Performance** - Optimized rendering loop

### Visual Elements
- **Title**: "SSOENGINE" (large white text, 60px)
- **Subtitle**: "Lightweight C++ Power" (gray text, 20px)
- **Progress Bar**: Green bar at bottom showing elapsed time
- **Background**: Black with fade effects

### Example Usage
```cpp
#include "tools/sso_splash.h"
#include "tools/sso_window.h"

int main() {
    // Show professional splash screen first
    SSO::Splash::Show(2.0f);
    
    // Initialize your game after splash
    SSO::Window::Init(1280, 720, "My Game");
    
    // Your game code here
    while (!WindowShouldClose()) {
        SSO::Window::BeginDrawingVirtual();
        ClearBackground(BLACK);
        
        DrawText("Game Started!", 10, 10, 20, WHITE);
        
        SSO::Window::EndDrawingVirtual();
    }
    
    SSO::Window::Close();
    return 0;
}
```

### Integration Tips
```cpp
// Use at the very beginning of main()
int main() {
    // Professional startup
    SSO::Splash::Show(2.0f);
    
    // Initialize systems
    SSO::Window::Init(1280, 720, "Game");
    // ... other initialization
    
    // Game loop
    while (!WindowShouldClose()) {
        // ... game logic
    }
    
    // Cleanup
    SSO::Window::Close();
    return 0;
}
```

### Customization Options
The splash screen is designed to be consistent but you can modify the source if needed:
- Change title text in `sso_splash.h`
- Modify colors (currently BLACK background, WHITE title, GRAY subtitle)
- Adjust animation timing (0.5s fade duration)
- Change progress bar color (currently LIME)

---

## ⚛️ sso_physics.h - Complete 2D Physics Engine

#### **Physics Constants**
```cpp
constexpr float GRAVITY = 980.0f;        // pixels/second^2
constexpr float AIR_RESISTANCE = 0.99f; // velocity dampening
constexpr float BOUNCE_DAMPING = 0.7f;   // energy loss on bounce
```

#### **RigidBody Structure**
```cpp
SSO::Physics::RigidBody body;
body.position = {x, y};
body.velocity = {vx, vy};
body.acceleration = {ax, ay};
body.radius = radius;
body.mass = mass;
body.isGrounded = false;
body.isStatic = false;
```

#### **Forces & Impulses**
```cpp
// Apply continuous force
SSO::Physics::ApplyForce(body, {forceX, forceY});

// Apply instant impulse
SSO::Physics::ApplyImpulse(body, {impulseX, impulseY});

// Apply gravity
body.acceleration = {0, SSO::Physics::GRAVITY};
```

#### **Collision Detection**
```cpp
// Circle vs Circle
bool collision = SSO::Physics::CheckCircleCollision(circle1, circle2);

// Circle vs Rectangle
bool collision = SSO::Physics::CheckCircleRectCollision(circle, rect);

// Rectangle vs Rectangle
bool collision = SSO::Physics::CheckRectCollision(rect1, rect2);
```

#### **Collision Resolution**
```cpp
// Resolve collision between bodies
SSO::Physics::ResolveCollision(body1, body2);

// Resolve collision with static object
SSO::Physics::ResolveStaticCollision(body, staticRect);
```

### Example Usage
```cpp
#include "tools/sso_physics.h"
#include "tools/sso_window.h"
#include "tools/sso_timer.h"

struct Ball {
    SSO::Physics::RigidBody body;
    Color color;
};

int main() {
    SSO::Window::Init(1280, 720, "Physics Demo");
    SSO::Timer timer;
    
    std::vector<Ball> balls;
    
    // Create some balls
    for (int i = 0; i < 10; i++) {
        Ball ball;
        ball.body.position = {100.0f + i * 50.0f, 100.0f};
        ball.body.velocity = {(float)(rand() % 200 - 100), 0};
        ball.body.radius = 20.0f;
        ball.body.mass = 1.0f;
        ball.color = {rand() % 255, rand() % 255, rand() % 255, 255};
        balls.push_back(ball);
    }
    
    while (!WindowShouldClose()) {
        float dt = timer.GetDeltaTime();
        
        // Update physics
        for (auto& ball : balls) {
            // Apply gravity
            ball.body.acceleration = {0, SSO::Physics::GRAVITY};
            
            // Update velocity and position
            ball.body.velocity.x += ball.body.acceleration.x * dt;
            ball.body.velocity.y += ball.body.acceleration.y * dt;
            ball.body.position.x += ball.body.velocity.x * dt;
            ball.body.position.y += ball.body.velocity.y * dt;
            
            // Apply air resistance
            ball.body.velocity.x *= SSO::Physics::AIR_RESISTANCE;
            ball.body.velocity.y *= SSO::Physics::AIR_RESISTANCE;
            
            // Ground collision
            if (ball.body.position.y + ball.body.radius >= 650) {
                ball.body.position.y = 650 - ball.body.radius;
                ball.body.velocity.y *= -SSO::Physics::BOUNCE_DAMPING;
            }
        }
        
        // Ball-to-ball collisions
        for (size_t i = 0; i < balls.size(); i++) {
            for (size_t j = i + 1; j < balls.size(); j++) {
                if (SSO::Physics::CheckCircleCollision(balls[i].body.position, balls[i].body.radius,
                                                       balls[j].body.position, balls[j].body.radius)) {
                    SSO::Physics::ResolveCollision(balls[i].body, balls[j].body);
                }
            }
        }
        
        // Rendering
        SSO::Window::BeginDrawingVirtual();
        ClearBackground(BLACK);
        
        for (const auto& ball : balls) {
            DrawCircleV(ball.body.position, ball.body.radius, ball.color);
        }
        
        SSO::Window::EndDrawingVirtual();
    }
    
    SSO::Window::Close();
    return 0;
}
```

---

## 📦 sso_container.h - Panel & Widget System

### What it ACTUALLY Does
Modern UI container system with auto-stacking widgets, theme management, and two development paths: Classic Game (manual positioning) and App (container-based).

### Include
```cpp
#include "tools/sso_container.h"
```

### Available Functions

#### **Theme System**
```cpp
// Set global theme
SSO::Container::SSO_SetTheme(SSO::Container::THEME_DARK);    // Dark theme
SSO::Container::SSO_SetTheme(SSO::Container::THEME_LIGHT);   // Light theme
SSO::Container::SSO_SetTheme(SSO::Container::THEME_SSO_BLUE); // SSO Blue theme
```

#### **Panel System (Jalur App)**
```cpp
// Begin panel area
SSO::Container::SSO_BeginPanel({10, 10, 300, 400});

// Auto-stacking widgets
SSO::Container::SSO_PushWidget("Click Me");        // Button
SSO::Container::SSO_PushLabel("Information");      // Label
SSO::Container::SSO_PushCheckbox("Enable", true);  // Checkbox
SSO::Container::SSO_PushSlider("Volume", 50.0f);  // Slider

// Render panel
SSO::Container::SSO_RenderPanel();
```

#### **Widget Interaction**
```cpp
// Check button click
if (SSO::Container::SSO_IsButtonClicked("Click Me")) {
    // Button was clicked
}

// Get checkbox state
bool enabled = SSO::Container::SSO_IsCheckboxChecked("Enable");

// Get slider value
float volume = SSO::Container::SSO_GetSliderValue("Volume");
```

#### **Classic Game Functions (Jalur Game)**
```cpp
// Manual positioning - free coordinate system
Vector2 playerPos = {100, 200};
SSO::Container::SSO_DrawSprite(playerTexture, playerPos);

SSO::Container::SSO_DrawRectangle({50, 50, 100, 100}, RED);
SSO::Container::SSO_DrawCircle({400, 300}, 50, BLUE);
SSO::Container::SSO_DrawText("Score: 100", {10, 10}, 20, WHITE);
```

### Theme Options
- **THEME_DARK** - Dark gray background with blue accents
- **THEME_LIGHT** - Light background with subtle shadows
- **THEME_SSO_BLUE** - Professional blue theme

### Example Usage
```cpp
#include "tools/sso_container.h"
#include "tools/sso_window.h"

int main() {
    SSO::Window::Init(1280, 720, "Container Demo");
    SSO::Container::SSO_InitContainer();
    
    // Set theme
    SSO::Container::SSO_SetTheme(SSO::Container::THEME_SSO_BLUE);
    
    while (!WindowShouldClose()) {
        SSO::Window::BeginDrawingVirtual();
        ClearBackground(BLACK);
        
        // Jalur App - Container System
        SSO::Container::SSO_BeginPanel({50, 50, 300, 400});
        SSO::Container::SSO_PushLabel("Application Settings");
        SSO::Container::SSO_PushCheckbox("Enable Sound", true);
        SSO::Container::SSO_PushSlider("Master Volume", 75.0f);
        SSO::Container::SSO_PushWidget("Apply Settings");
        SSO::Container::SSO_PushWidget("Reset to Default");
        SSO::Container::SSO_RenderPanel();
        
        // Jalur Game - Classic Manual Positioning
        Vector2 playerPos = {500, 300};
        SSO::Container::SSO_DrawRectangle(playerPos, {50, 50}, GREEN);
        SSO::Container::SSO_DrawText("Player", playerPos.x, playerPos.y - 20, 12, WHITE);
        
        // Handle interactions
        if (SSO::Container::SSO_IsButtonClicked("Apply Settings")) {
            // Apply settings logic
        }
        
        if (SSO::Container::SSO_IsButtonClicked("Reset to Default")) {
            // Reset logic
        }
        
        SSO::Window::EndDrawingVirtual();
    }
    
    SSO::Window::Close();
    return 0;
}
```

---


### Status
This file exists but is currently empty (only contains a blank line).

---

## 🚀 Complete Game Example

Here's how to use ALL the advanced tools together:

```cpp
#include "tools/sso_window.h"
#include "tools/sso_camera.h"
#include "tools/sso_timer.h"
#include "tools/sso_ui.h"
#include "tools/sso_math.h"
#include "tools/sso_logger.h"
#include "tools/sso_physics.h"
#include "tools/sso_ext.h"
#include "tools/sso_file.h"
#include "tools/sso_splash.h"
#include "tools/sso_text.h"
#include "tools/sso_provider.h"

int main() {
    // Show professional splash screen
    SSO::Splash::Show(2.0f);
    
    // Initialize systems with logging
    SSO::Logger::Log(SSO::Logger::Level::INFO, "Starting game...");
    
    // Window setup with virtual resolution
    SSO::Window::Init(1280, 720, "Advanced Game Demo");
    SSO::Window::showBorder = true;
    SSO::Logger::Log(SSO::Logger::Level::SUCCESS, "Window initialized");
    
    // Camera setup
    SSO::Camera camera(Vector2{0, 0}, 1280, 720);
    camera.SetZoom(1.0f);
    camera.SetSmoothing(0.1f);
    
    // Timer setup
    SSO::Timer engineTimer;
    SSO::Timer gameTimer;
    gameTimer.SetValue(120.0f); // 2 minute timer
    gameTimer.Start();
    
    // Physics world
    std::vector<SSO::Physics::RigidBody> physicsObjects;
    
    // Create player physics body
    SSO::Physics::RigidBody player;
    player.position = {0, 0};
    player.velocity = {0, 0};
    player.radius = 20.0f;
    player.mass = 1.0f;
    player.isGrounded = false;
    player.isStatic = false;
    physicsObjects.push_back(player);
    
    // Create some physics obstacles
    for (int i = 0; i < 5; i++) {
        SSO::Physics::RigidBody obstacle;
        obstacle.position = {200.0f + i * 150.0f, -100.0f};
        obstacle.velocity = {0, 0};
        obstacle.radius = 15.0f;
        obstacle.mass = 2.0f;
        obstacle.isGrounded = false;
        obstacle.isStatic = false;
        physicsObjects.push_back(obstacle);
    }
    
    // UI setup
    Font font = GetFontDefault();
    Rectangle buttonRect = {550, 20, 150, 40};
    Rectangle loadButtonRect = {550, 70, 150, 40};
    Rectangle saveButtonRect = {550, 120, 150, 40};
    Rectangle healthBarRect = {550, 170, 150, 20};
    float health = 100.0f;
    bool showPopup = false;
    
    // Check if git is available
    bool hasGit = SSO_HasTool("git");
    if (hasGit) {
        SSO::Logger::Log(SSO::Logger::Level::INFO, "Git is available");
    }
    
    // Try to load assets from bundle
    Texture2D playerTexture = {0};
    if (SSO::Provider::LoadTextureFromBundle("assets.sso", "player.png").id > 0) {
        playerTexture = SSO::Provider::LoadTextureFromBundle("assets.sso", "player.png");
        SSO::Logger::Log(SSO::Logger::Level::SUCCESS, "Loaded assets from bundle");
    } else {
        SSO::Logger::Log(SSO::Logger::Level::WARNING, "Using fallback textures");
    }
    
    SSO::Logger::Log(SSO::Logger::Level::INFO, "Game loop started");
    
    while (!WindowShouldClose()) {
        float dt = engineTimer.GetDeltaTime();
        
        // Update game timer
        gameTimer.UpdateCountdown(dt);
        
        // Player movement with physics
        Vector2 previousPos = physicsObjects[0].position;
        if (IsKeyDown(KEY_RIGHT)) physicsObjects[0].velocity.x = 200;
        else if (IsKeyDown(KEY_LEFT)) physicsObjects[0].velocity.x = -200;
        else physicsObjects[0].velocity.x = 0;
        
        if (IsKeyDown(KEY_UP) && physicsObjects[0].isGrounded) {
            physicsObjects[0].velocity.y = -400; // Jump
        }
        
        // Update all physics objects
        for (auto& body : physicsObjects) {
            if (body.isStatic) continue;
            
            // Apply gravity
            body.acceleration = {0, SSO::Physics::GRAVITY};
            
            // Update velocity and position
            body.velocity.x += body.acceleration.x * dt;
            body.velocity.y += body.acceleration.y * dt;
            body.position.x += body.velocity.x * dt;
            body.position.y += body.velocity.y * dt;
            
            // Apply air resistance
            body.velocity.x *= 0.98f;
            body.velocity.y *= 0.98f;
            
            // Ground collision
            if (body.position.y + body.radius >= 300) {
                body.position.y = 300 - body.radius;
                body.velocity.y *= -SSO::Physics::BOUNCE_DAMPING;
                if (fabs(body.velocity.y) < 50) {
                    body.isGrounded = true;
                    body.velocity.y = 0;
                }
            } else {
                body.isGrounded = false;
            }
            
            // Wall boundaries
            if (body.position.x - body.radius < -400) body.position.x = -400 + body.radius;
            if (body.position.x + body.radius > 400) body.position.x = 400 - body.radius;
        }
        
        // Physics collisions between objects
        for (size_t i = 0; i < physicsObjects.size(); i++) {
            for (size_t j = i + 1; j < physicsObjects.size(); j++) {
                if (SSO::Physics::CheckCircleCollision(physicsObjects[i].position, physicsObjects[i].radius,
                                                       physicsObjects[j].position, physicsObjects[j].radius)) {
                    SSO::Physics::ResolveCollision(physicsObjects[i], physicsObjects[j]);
                }
            }
        }
        
        // Camera follow with smoothing
        camera.Follow(physicsObjects[0].position, dt);
        
        // Handle UI interactions
        if (SSO::UI::DrawButton(buttonRect, "Shake", font)) {
            camera.Shake(20.0f, 0.5f);
            SSO::Logger::Log(SSO::Logger::Level::INFO, "Camera shake triggered");
        }
        
        // Load texture from file dialog
        if (SSO::UI::DrawButton(loadButtonRect, "Load Tex", font)) {
            std::string texturePath = SSO_OpenFile(
                "Image Files\0*.png;*.jpg;*.jpeg\0All Files\0*.*\0",
                "Load Texture"
            );
            if (!texturePath.empty()) {
                if (playerTexture.id > 0) UnloadTexture(playerTexture);
                playerTexture = LoadTexture(texturePath.c_str());
                SSO::Logger::Log(SSO::Logger::Level::SUCCESS, "Texture loaded from file");
            }
        }
        
        // Save screenshot
        if (SSO::UI::DrawButton(saveButtonRect, "Save", font)) {
            std::string savePath = SSO_SaveFile(
                "PNG Files\0*.png\0All Files\0*.*\0",
                "Save Screenshot"
            );
            if (!savePath.empty()) {
                TakeScreenshot(savePath.c_str());
                SSO::Logger::Log(SSO::Logger::Level::SUCCESS, "Screenshot saved");
            }
        }
        
        // Health system
        if (gameTimer.IsFinished()) {
            health -= dt * 10; // Lose health over time
            if (health <= 0) {
                showPopup = true;
                health = 0;
            }
        }
        
        // Rendering
        SSO::Window::BeginDrawingVirtual();
        ClearBackground(DARKGRAY);
        
        // Game world (with camera)
        camera.Begin();
        DrawGrid(40, 100);
        
        // Draw ground
        DrawRectangle(-400, 300, 800, 20, BROWN);
        
        // Draw physics objects
        for (size_t i = 0; i < physicsObjects.size(); i++) {
            Color color = (i == 0) ? BLUE : RED;
            DrawCircleV(physicsObjects[i].position, physicsObjects[i].radius, color);
            
            // Draw texture if loaded
            if (playerTexture.id > 0 && i == 0) {
                DrawTexture(playerTexture, 
                           physicsObjects[i].position.x - playerTexture.width/2,
                           physicsObjects[i].position.y - playerTexture.height/2, 
                           WHITE);
            }
        }
        
        camera.End();
        
        // UI (no camera)
        DrawText(TextFormat("Time: %.1f", gameTimer.GetValue()), 10, 10, 20, WHITE);
        DrawText(TextFormat("FPS: %d", GetFPS()), 10, 40, 20, WHITE);
        DrawText(TextFormat("Objects: %zu", physicsObjects.size()), 10, 70, 20, WHITE);
        
        // Advanced text rendering
        SSO::Text::DrawGradient("PHYSICS DEMO", 10, 120, 30, YELLOW, RED);
        SSO::Text::DrawTypewriter("Advanced physics simulation with SSOEngine tools...", 
                                10, 160, 15, 20.0f, GetTime(), LIME);
        
        // UI elements
        SSO::UI::DrawHealthBar(healthBarRect, health, 100.0f, GREEN, RED);
        
        // Popup
        if (showPopup) {
            Rectangle popupRect = {400, 250, 400, 150};
            if (SSO::UI::DrawPopUp(popupRect, "Game Over", "Time's up! Click OK to restart.", font, &showPopup)) {
                // Restart game
                gameTimer.Reset();
                gameTimer.Start();
                health = 100.0f;
                physicsObjects[0].position = {0, 0};
                physicsObjects[0].velocity = {0, 0};
                SSO::Logger::Log(SSO::Logger::Level::INFO, "Game restarted");
            }
        }
        
        SSO::Window::EndDrawingVirtual();
    }
    
    // Cleanup
    if (playerTexture.id > 0) UnloadTexture(playerTexture);
    SSO::Logger::Log(SSO::Logger::Level::INFO, "Game ending...");
    SSO::Window::Close();
    return 0;
}
```

---

**sso_audio.h**

**Reason:** The audio system was removed to simplify the engine and focus on core graphics and gameplay features.

**Migration:** Use Raylib's built-in audio functions directly:
```cpp
// Instead of SSO::Audio::SSO_LoadMusic()
Music music = LoadMusic("assets/background.mp3");

// Instead of SSO::Audio::SSO_PlayMusic()
PlayMusicStream(music);

// Instead of SSO::Audio::SSO_UpdateMusic()
UpdateMusicStream(music);
```

**Files Affected:**
- `main.cpp` - Audio loading and controls removed
- `game.h` - Audio status indicators removed  
- `main_backup.cpp` - Audio functionality removed
- `audio_example.cpp` - Entire file deleted

---

## ⚠️ Important Notes

### What These Tools ACTUALLY Are
- ✅ **Advanced camera system** - With smoothing and shake effects
- ✅ **High-precision timer** - With countdown/stopwatch functionality
- ✅ **Virtual resolution manager** - With letterboxing and scaling
- ✅ **Complete UI system** - With panels, modals, and styling
- ✅ **Collision utilities** - Essential math functions
- ✅ **Colored logger** - With timestamps and colors
- ✅ **System extensions** - Cross-platform file operations and downloads
- ✅ **File dialogs** - Native Windows open/save dialogs
- ✅ **Physics engine** - Complete 2D physics with collision detection
- ✅ **Asset bundles** - Custom .sso format for asset packaging
- ✅ **Splash screens** - Professional startup screens with fade effects and progress bar
- ✅ **Advanced text** - Typewriter effects, gradients, and wrapping

## Optional Physics Helper (sso_ph3d.h)

### What it ACTUALLY Does
Optional physics helper with rigid body simulation, collision detection, and realistic physics responses. This header is included for advanced or experimental cases and is not required by the core 2D engine.

### Include
```cpp
#include "tools/sso_ph3d.h"
```

### Core Physics System
```cpp
// Initialize physics world
SSO::Physics3D::Init();

// Create static floor
auto floor = SSO::Physics3D::CreateStaticBody({0, -1, 0}, {50, 1, 50}, GRAY);

// Create dynamic box
auto box = SSO::Physics3D::CreateBody({0, 5, 0}, {1, 1, 1}, RED, 1.0f);

// Physics simulation (call in game loop)
SSO::Physics3D::Step(deltaTime);

// Render all physics objects
SSO::Physics3D::DrawAllBodies();

// Cleanup
SSO::Physics3D::Cleanup();
```

### Physics Features
- **RigidBody System** - Mass, velocity, acceleration, angular velocity
- **Collision Detection** - AABB collision with proper response
- **Gravity System** - Realistic gravity with ground detection
- **Impulse Forces** - Shooting, explosions, force application
- **Tumbling Effects** - Natural rotation and angular velocity
- **Fixed Timestep** - Stable physics simulation at 60 FPS

### Force Application
```cpp
// Apply force to body
SSO::Physics3D::ApplyForce(body, {0, 10, 0});

// Apply impulse (instant velocity change)
SSO::Physics3D::ApplyImpulse(body, {5, 0, 0});

// Shooting with raycast
SSO::Physics3D::ShootBullet(cameraPos, direction, 100.0f);

// Explosion effect
SSO::Physics3D::Explode({0, 0, 0}, 100.0f, 10.0f);
```

---

## Hyperlink Text System (sso_text.h) - Interactive Text

### What it ACTUALLY Does
Advanced text rendering system with typewriter effects, gradients, outlines, and interactive hyperlinks that can open URLs when clicked.

### Include
```cpp
#include "tools/sso_text.h"
```

### Hyperlink Function
```cpp
// Draw interactive hyperlink
bool clicked = SSO::Text::DrawHyperlink("Visit GitHub", 100, 100, 20, BLUE, "https://github.com/Rozaq125/SSOEngine");

// Features:
// - Mouse hover detection with color brightening
// - Cursor change to pointing hand
// - Click to open URL in browser
// - Cross-platform support (Windows/Linux)
```

### Text Effects
```cpp
// Shadow text
SSO::Text::DrawShadow("Game Title", 100, 100, 30, WHITE, 2);

// Gradient text
SSO::Text::DrawGradient("Awesome", 100, 150, 30, RED, BLUE);

// Outline text
SSO::Text::DrawOutline("BORDER", 100, 200, 30, WHITE, BLACK);

// Typewriter effect
SSO::Text::DrawTypewriter("Loading...", 100, 250, 20, 10.0f, time, WHITE);

// Centered text
SSO::Text::DrawCentered("CENTERED", 300, 30, WHITE);

// Code block
SSO::Text::DrawCodeBlock("printf(\"Hello\");", 100, 350, 16, WHITE, {40, 40, 40, 255});
```

---

## Extended Tools (sso_ext.h) - System Operations

### What it ACTUALLY Does
System extension tools for file operations, downloads, environment variables, and cross-platform system integration.

### Include
```cpp
#include "tools/sso_ext.h"
```

### File Operations
```cpp
// Download files (curl/wget/powershell)
SSO_Download("https://example.com/file.zip", "downloads/file.zip");

// Copy files and folders
SSO_Copy("source.txt", "backup.txt");
SSO_Copy("project/", "backup/");

// Move files
SSO_Move("old.txt", "new.txt");

// Remove files and folders
SSO_Remove("temp_folder");

// Create folders
SSO_MakeFolder("data/levels");
```

### System Operations
```cpp
// Open URLs and files
SSO_Open("https://github.com/Rozaq125/SSOEngine");
SSO_Open("document.pdf");

// Execute system commands
int result = SSO_System("dir");

// Get command output
std::string output = SSO_GetOutput("git status");

// Check if tool exists
bool hasGit = SSO_HasTool("git");
```

### Environment Variables
```cpp
// Get environment variable
std::string path = SSO_GetEnv("PATH");

// Set environment variable
SSO_SetEnv("MY_VAR", "value");
```

### File Information
```cpp
// Get file size
long long size = SSO_GetFileSize("game.exe");

// Kill process
SSO_KillProcess("notepad.exe");
```

---

### Limitations
- **sso_utils.h is empty** - Currently contains no functions
- **sso_file.h is Windows-only** - Uses Windows API for file dialogs
- **sso_ext.h requires C++17** - Uses filesystem library
- **sso_provider.h needs manual memory management** - Free raw data when done

### Best Practices
1. **Use SSO::Window::BeginDrawingVirtual()** - Not BeginDrawing()
2. **Always call SSO::Window::Close()** - Proper resource cleanup
3. **Use namespace SSO::UI** - For UI functions
4. **Check timer IsFinished()** - Before using timer values

### When to Use These Tools
- ✅ **2D games with advanced camera** - Platformers, top-down games
- ✅ **3D games and prototypes** - 3D demos, mixed 2D/3D games
- ✅ **Games with professional presentation** - Commercial-quality games with splash screens
- ✅ **Games with complex UI** - RPGs, strategy games
- ✅ **Games with timing mechanics** - Puzzles, arcade games
- ✅ **Games needing collision** - Action games, platformers
- ✅ **Games with physics simulation** - Puzzle games, sandbox games
- ✅ **Games needing file operations** - Level editors, asset managers
- ✅ **Games requiring asset bundles** - Large games with many assets

---

**These tools are actually quite advanced!** They provide features you'd expect from a serious game engine, not just basic wrappers. The virtual resolution system, advanced camera with shake, and complete UI system are particularly impressive. 🎮✨
