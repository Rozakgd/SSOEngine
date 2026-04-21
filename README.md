<p align="center">
  <img src="02_Docs/img/banner1.png" alt="SSOEngine Banner" width="800"/>
</p>

# SSOEngine 1.6 NEW SYSTEM

**Cross-Platform 2D/3D Game Engine Built with Raylib**

> **CROSS-PLATFORM**: Windows desktop + Android mobile from the same codebase!

---

<p align="center">
  <img src="02_Docs/img/screenshot1.png" alt="SSOEngine Screenshot" width="600"/>
</p>

## 🎯 What SSOEngine Actually Is

SSOEngine is a **simplified C++ game development framework** that:

- ✅ **Wraps Raylib** - Uses Raylib as the graphics backend
- ✅ **Provides basic tools** - Asset packer, build automation
- ✅ **Simple structure** - Organized folder system for small games
- ✅ **Build automation** - One-click build with `build.bat`

## � SSOEngine vs Vanilla Raylib:

| Feature | Vanilla Raylib | SSOEngine | **Improvement** |
|---------|----------------|-----------|-----------------|
| **Window Setup** | `InitWindow()`, `BeginDrawing()`, `EndDrawing()` | `SSO::Window::Init()`, `BeginDrawingVirtual()`, `EndDrawingVirtual()` | **Virtual resolution** with automatic letterboxing |
| **Asset Loading** | `LoadTexture("assets/player.png")` | `SSO::Provider::LoadTextureFromBundle("assets.sso", "player.png")` | **Binary bundling** - assets protected in .sso format |
| **Audio Management** | Manual `UpdateMusicStream()` for each track | `SSO::Audio::RegisterMusic()` + `SSO::Audio::UpdateAudio()` | **Automatic music management** - single call updates all |
| **Camera System** | Manual `Camera2D` setup and interpolation | `SSO::Camera` with built-in smoothing & shake | **One-line camera** with professional effects |
| **Timing System** | `GetFrameTime()` - basic delta time | `SSO::Timer` with countdown/stopwatch modes | **Game-ready timers** - no manual time tracking |
| **Text Rendering** | `DrawText()` - basic text only | `SSO::Text::DrawTypewriter()`, `DrawGradient()`, `DrawOutline()`, `DrawHyperlink()` | **Professional text effects** - typewriter, gradients, outlines, interactive hyperlinks |
| **Initialization Lines** | 5+ lines for window setup | **1 line**: `SSO::Window::Init(800, 600, "Game")` | **80% code reduction** in setup |

### 🛡️ Security Features: Binary Asset Bundling

**SSOEngine's `.sso` format provides:**

- **Asset Protection**: All game assets packed into binary format
- **No Direct Access**: Files cannot be accessed or modified directly
- **Cross-Platform**: Same bundle works on Windows & Android
- **Compression**: Reduced asset size through binary packaging
- **Structure**: Custom header with metadata and offset-based file access

**How SSOPacker Works:**
```cpp
// Asset Header Structure
struct AssetHeader {
    char signature[4] = {'S', 'S', 'O', ' '}; // Magic bytes
    int version = 1;                          // Format version
    int fileCount;                           // Number of packed files
};

// File metadata for fast access
struct FileMetadata {
    char fileName[64];   // Relative path (e.g., "textures/player.png")
    long long fileSize;   // File size in bytes
    long long offset;     // Binary offset in bundle
};
```

### ⚡ Syntax Simplification Examples

**Window Setup (5 lines → 1 line):**
```cpp
// Vanilla Raylib
InitWindow(800, 600, "Game");
SetTargetFPS(60);
while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    // Game code here
    EndDrawing();
}
CloseWindow();

// SSOEngine
SSO::Window::Init(800, 600, "Game");
while (!WindowShouldClose()) {
    SSO::Window::BeginDrawingVirtual();
    ClearBackground(BLACK);
    // Game code here
    SSO::Window::EndDrawingVirtual();
}
SSO::Window::Close();
```

**Audio Management (Manual → Automatic):**
```cpp
// Vanilla Raylib - Manual tracking
Music bgm = LoadMusicStream("assets/music.mp3");
PlayMusicStream(bgm);
while (!WindowShouldClose()) {
    UpdateMusicStream(bgm); // Must call EVERY frame
    // Game code
}

// SSOEngine - Automatic management
Music bgm = SSO::Audio::LoadMusicStream("assets/music.mp3");
SSO::Audio::RegisterMusic(bgm);
SSO::Audio::PlayMusic(bgm);
while (!WindowShouldClose()) {
    SSO::Audio::UpdateAudio(); // Updates ALL registered music
    // Game code
}
```

**Camera System (Complex → Simple):**
```cpp
// Vanilla Raylib - Manual camera
Camera2D camera = {0};
camera.target = playerPos;
camera.offset = {screenWidth/2, screenHeight/2};
camera.rotation = 0;
camera.zoom = 1.0f;
BeginMode2D(camera);
// Game rendering
EndMode2D();

// SSOEngine - Professional camera
SSO::Camera camera(playerPos, 1280, 720);
camera.Follow(playerPos, deltaTime); // With smoothing
camera.Shake(10.0f, 0.5f); // Screen shake
camera.Begin();
// Game rendering
camera.End();
```

### 📏 Size Optimization

**SSOEngine achieves < 1MB executables through:**

- **Static Linking**: Only required Raylib modules included
- **Asset Bundling**: External assets packed into single .sso file
- **Minimal Runtime**: No unnecessary dependencies
- **Optimized Build**: Automated MinGW compilation with size flags
- **Resource Management**: Efficient memory allocation and cleanup

**Typical Results:**
- **Windows EXE**: 800KB - 950KB (with basic game)
- **Android APK**: 2.5MB - 4MB (including Android framework)
- **Asset Bundle**: 30-70% smaller than raw files

## �🛠️ What's Actually Included

### Core Components
- **Raylib Integration** - Graphics, input, audio (via Raylib)
- **Asset Packer** - Custom `.sso` format for assets
- **Build System** - Automated MinGW compilation
- **Basic Tools** - Camera, timer, window management

### Tools Folder (`01_Core/tools/`)
```cpp
// Available tools (basic wrappers):
- sso_camera.h    // Camera management
- sso_timer.h     // Game timer
- sso_window.h    // Window setup
- sso_ui.h        // Basic UI elements
- sso_math.h      // Math utilities
- sso_logger.h    // Simple logging
- sso_3d.h       // 3D rendering API with shader support
- sso_ph3d.h      // Advanced 3D physics engine
- sso_splash.h   // Professional splash screens
- sso_container.h // Auto-stacking widgets
- sso_physics.h  // 2D physics engine
- sso_provider.h // Asset bundle system
- sso_ext.h       // System extensions
- sso_file.h      // File dialogs
- sso_text.h      // Advanced text rendering
- sso_audio.h     // Audio system with music management
- sso_utils.h     // General utilities (empty)
```

### 3D Physics Engine (sso_ph3d.h)
```cpp
// Advanced 3D Physics System
SSO::Physics3D::Init(); // Initialize physics world

// Create rigid bodies
auto floor = SSO::Physics3D::CreateStaticBody({0, -1, 0}, {50, 1, 50}, GRAY);
auto box = SSO::Physics3D::CreateBody({0, 5, 0}, {1, 1, 1}, RED, 1.0f);

// Physics simulation
SSO::Physics3D::Step(deltaTime); // Fixed timestep (1/60s)

// Rendering
SSO::Physics3D::DrawAllBodies(); // Draw all physics objects
```

**Features:**
- **RigidBody System** - Mass, velocity, acceleration, angular velocity
- **Collision Detection** - AABB collision with proper response
- **Gravity System** - Realistic gravity with ground detection
- **Impulse Forces** - Shooting, explosions, force application
- **Tumbling Effects** - Natural rotation and angular velocity
- **Fixed Timestep** - Stable physics simulation at 60 FPS

### Hyperlink Text System (sso_text.h)
```cpp
// Interactive hyperlinks in games
bool clicked = SSO::Text::DrawHyperlink("Visit GitHub", 100, 100, 20, BLUE, "https://github.com/Rozaq125/SSOEngine");

// Features:
// - Mouse hover detection with color brightening
// - Cursor change to pointing hand
// - Click to open URL in browser
// - Cross-platform support (Windows/Linux)
```

### Extended Tools (sso_ext.h)
```cpp
// System operations
SSO_Download("https://example.com/file.zip", "downloads/file.zip");
SSO_Copy("source.txt", "backup.txt");
SSO_Remove("temp_folder");
SSO_Open("https://github.com/Rozaq125/SSOEngine");

// Environment and system
std::string path = SSO_GetEnv("PATH");
SSO_SetEnv("MY_VAR", "value");
long long size = SSO_GetFileSize("game.exe");
```

### Build System (Professional Distribution)
```batch
# What build.bat actually does:
DESKTOP BUILD:
1. Check/Install Raylib libraries
2. Compile asset packer (if needed)
3. Pack assets into .sso format
4. Compile resources (.rc files)
5. Build game executable
6. Move files to dist/ folder
7. Auto-zip production archive
8. Clean build garbage

ANDROID BUILD:
1. Cross-compile with Android NDK
2. Package native libraries (lib/arm64-v8a/)
3. Create APK with proper structure
4. Sign with production keystore
5. Verify APK integrity

DISTRIBUTION FEATURES:
- Professional folder separation (build/ vs dist/)
- Auto-zip production archives
- Standalone executables (no DLL dependencies)
- One-click distribution packages
- Build garbage cleanup
- Post-build navigation menu
```

## 🚀 Quick Start (Realistic)

### Prerequisites
- **MinGW-w64** - Required for Windows compilation
- **Android SDK/NDK** - Required for Android builds
- **Windows** - Development environment
- **Basic C++ knowledge** - This is not a no-code tool

### Installation
```bash
# Clone and build
git clone https://github.com/Rozaq125/SSOEngine.git
cd SSOEngine/01_Core
build.bat

# Select build option:
# [1] Windows Build - Creates .exe
# [2] Android Build - Creates .apk

# This will:
# 1. Download Raylib automatically
# 2. Build your game (Windows or Android)
# 3. Create executable (.exe) or APK (.apk)
```

### Basic Game Structure (v1.6)
```cpp
#include "tools/sso_window.h"
#include "tools/sso_camera.h"
#include "tools/sso_splash.h"

int main() {
    // Show splash screen
    SSO::Splash::Show(2.0f);
    
    // Initialize window
    SSO::Window::Init(800, 600, "My Game");
    
    // Main game loop
    while (!WindowShouldClose()) {
        SSO::Window::BeginDrawingVirtual();
        ClearBackground(RAYWHITE);
        // Your game code here
        SSO::Window::EndDrawingVirtual();
    }
    
    SSO::Window::Close();
    return 0;
}
```

## Project Structure (SSOEngine v1.6)

```
SSOEngine/
|-- 00_BuildTools/     # Asset Packer source code
|-- 01_Core/          # Engine core & game code
|   |-- game/         # User game code (Multi-file support)
|   |   |-- main.cpp  # Game entry point
|   |   |-- scripts/  # Game-specific headers (.h, .hpp)
|   |   |-- *.cpp     # Additional game source files
|   |-- tools/        # Engine core headers & source
|   |-- assets/       # Game resources (Tetap di sini!)
|   |-- android/      # Android project files
|   |   |-- AndroidManifest.xml
|   |   |-- main_android.cpp
|   |   |-- src/     # Java source
|   |-- build.bat     # Build script
|   |-- CMakeLists.txt # Build configuration
|-- 02_Docs/          # Documentation
|-- Web/Website/      # Web-based documentation
|-- include/          # Raylib headers
|-- lib/             # Raylib libraries
```

### v1.6 Changes
- **Professional Build System**: Production-grade distribution with folder separation
- **Auto-Zip Archives**: PowerShell Compress-Archive for one-click distribution
- **Build Garbage Cleanup**: Automatic cleanup of resource files and build cache
- **Enhanced Navigation**: Post-build menu with 6 professional options
- **Standalone EXE**: Static linking for zero-dependency executables
- **Game Folder Isolation**: User code separated from engine core
- **Multi-file Support**: Add multiple .cpp/.h files to `01_Core/game/`
- **Asset Location**: Assets remain in `01_Core/assets/` for compatibility
- **Cross-platform File Dialog**: New `sso_file.h` with platform-specific implementations

## Audio System (sso_audio.h)

### Features
- **Automatic Music Management**: Register once, update all with single call
- **Cross-platform Audio**: Works on Windows and Android
- **Sound Effects**: Support for multiple sound effects
- **Volume Control**: Per-channel volume management

### Usage Examples
```cpp
#include "tools/sso_audio.h"

// Load and play background music
Music bgm = SSO::Audio::LoadMusicStream("assets/music.mp3");
SSO::Audio::RegisterMusic(bgm);
SSO::Audio::PlayMusic(bgm);

// Load sound effects
Sound shoot = SSO::Audio::LoadSound("assets/shoot.wav");
Sound explosion = SSO::Audio::LoadSound("assets/explosion.wav");

// In game loop
while (!WindowShouldClose()) {
    SSO::Audio::UpdateAudio(); // Updates ALL registered music
    
    if (IsKeyPressed(KEY_SPACE)) {
        SSO::Audio::PlaySound(shoot);
    }
    
    // Game code...
}
```

## Asset Management (sso_provider.h)

### Features
- **Binary Asset Bundling**: Protect assets in .sso format
- **JSON Loading**: Load configuration from bundles
- **Cross-platform**: Same bundle works on Windows & Android
- **Compression**: Reduced asset sizes

### Usage Examples
```cpp
#include "tools/sso_provider.h"

// Load texture from asset bundle
Texture2D playerTex = SSO::Provider::LoadTextureFromBundle("assets.sso", "player.png");

// Load configuration from JSON
nlohmann::json config = SSO::Provider::LoadJsonFromBundle("assets.sso", "config.json");
float playerSpeed = config["player"]["speed"];

// Load audio from bundle
Music bgm = SSO::Provider::LoadMusicFromBundle("assets.sso", "background.mp3");
```

## File System (sso_file.h)

### Features
- **Cross-platform Dialog**: Windows and Android support
- **No Windows.h Conflicts**: Clean header, platform-specific implementations
- **File Operations**: Open, save, browse folder dialogs

### Usage Examples
```cpp
#include "tools/sso_file.h"

// Open file dialog
std::string filePath = SSO_OpenFile("Text Files (*.txt)\0*.txt\0", "Open Text File");
if (!filePath.empty()) {
    // Process selected file
}

// Save file dialog
std::string savePath = SSO_SaveFile("All Files (*.*)\0*.*\0", "Save As");
if (!savePath.empty()) {
    // Save to selected path
}

// Browse folder dialog
std::string folderPath = SSO_BrowseFolder("Select Project Folder");
if (!folderPath.empty()) {
    // Use selected folder
}
```

## 🎮 What You Can Actually Build

### ✅ Supported Features
- **2D Graphics** - Sprites, shapes, text
- **3D Graphics** - Model loading, basic shapes, collision detection
- **3D Physics** - Advanced rigid body physics with realistic collision
- **Shader System** - 5 professional shader effects (Bloom, CRT, Grayscale, Blur, Basic Light)
- **Input Handling** - Keyboard, mouse, WASD movement
- **Asset Management** - Custom .sso format with shader loading
- **Window Management** - Resizable windows
- **Basic UI** - Simple buttons and text
- **Android Support** - Full APK building and deployment
- **Cross-Platform** - Windows + Android from same codebase

### ❌ NOT Supported (Yet)
- **Advanced 3D** - Support shaders,news feature!
- **Networking** - Single player only
- **iOS Support** - Android only for now
- **Visual Editor** - Code-based development
- **Advanced AI** - Basic game logic only

## 🔧 Development Tools

### Asset Packer
```bash
# Converts assets to .sso format
sso_packer.exe

# Input: assets/ folder
# Output: build/assets.sso
```

### Build System
```bash
build.bat

# Features:
- Auto Raylib installation
- Asset packing
- Resource compilation
- Executable generation
```

## 📚 Learning Resources

### For Beginners
1. **Learn C++ first** - This requires programming knowledge
2. **Raylib documentation** - SSOEngine uses Raylib functions
3. **Basic game dev concepts** - Game loops, update/draw patterns

### Recommended Tutorials
- [Raylib Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [C++ Game Development](https://www.learncpp.com/)
- [Basic Game Programming](https://github.com/raysan5/raylibgames)

## 🤝 Contributing

### What We Need
- **Bug fixes** - Report and fix issues
- **Documentation** - Improve tutorials
- **Examples** - Simple game examples
- **Tools** - Better utility functions

### How to Contribute
1. Fork the repository
2. Make your changes
3. Test thoroughly
4. Submit a pull request

### Coding Standards
- Use C++17 features
- Follow existing code style
- Add comments for complex logic
- Include examples for new features

### Bug Reports
- Use GitHub Issues
- Provide detailed steps
- Include error messages
- Attach screenshots if applicable

---

## 📄 License & Support

### License
SSOEngine is **Donationware** - Free for personal & commercial use!

### 🎁 Support the Project
💝 **Donate on Saweria:** [https://saweria.co/omedonation](https://saweria.co/omedonation)

Your support helps us:
- ✅ Maintain and improve the engine
- ✅ Add new features and tools
- ✅ Provide better documentation
- ✅ Create more examples and tutorials

### Community
- **GitHub**: [Rozaq125/SSOEngine](https://github.com/Rozak125/SSOEngine)
- **Website**: [SSOEngine Web](https://ssoengine.pages.dev)

---

## 🏆 Showcase

### Games Made with SSOEngine
*Have you made a game with SSOEngine? Let us know and we'll feature it here!*

### Features in Development
- 🌐 Networking support
- 🎨 Particle system
- 🎮 Input system enhancements

---

## 📞 Contact

### Lead Developer
**Rozak** - Game Engine Architect
- GitHub: [@Rozaq125](https://github.com/Rozaq125)
- Email: spctacularstudio@gmail.com


---

## 📚 Additional Resources

### Learning Resources
- [Raylib Documentation](https://www.raylib.com/)
- [C++ Game Development Tutorials](https://github.com/topics/cpp-game-dev)
- [Game Programming Patterns](https://gameprogrammingpatterns.com/)

### Tools & Software
- **IDE**: Visual Studio Code, CLion
- **Graphics**: Aseprite, Photoshop
- **Audio**: Audacity, FMOD
- **Version Control**: Git, GitHub Desktop

---
**THANK YOU FOR:**
- **RAYLIB**
- **C++**
- **GITHUB**
- **ALL ON CREDITS.md**
---

**© 2026 SSOGames Studio | Lead Developer: Rozak**

*Built with passion for indie game developers everywhere.*
