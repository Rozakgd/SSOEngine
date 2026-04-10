# [<img src="https://raw.githubusercontent.com/SSOEngine/1.5.png" alt="SSOEngine v1.5 COMING SOON" width="100%">](https://github.com/SSOEngine/01_Core/releases)

# SSOEngine

**Simple 2D/3D Game Engine Built with Raylib**

> ⚠️ **REALISTIC VERSION**: This is a basic game engine wrapper around Raylib with 2D focus and emerging 3D capabilities, not a full commercial engine.

---

## 🎯 What SSOEngine Actually Is

SSOEngine is a **simplified C++ game development framework** that:

- ✅ **Wraps Raylib** - Uses Raylib as the graphics backend
- ✅ **Provides basic tools** - Asset packer, build automation
- ✅ **Simple structure** - Organized folder system for small games
- ✅ **Build automation** - One-click build with `build.bat`

## 🛠️ What's Actually Included

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
- sso_utils.h     // General utilities (empty)
```

### Build System
```batch
# What build.bat actually does:
1. Check/Install Raylib libraries
2. Compile asset packer (if needed)
3. Pack assets into .sso format
4. Compile resources (.rc files)
5. Build game executable
```

## 🚀 Quick Start (Realistic)

### Prerequisites
- **MinGW-w64** - Required for compilation
- **Windows** - Currently Windows-only
- **Basic C++ knowledge** - This is not a no-code tool

### Installation
```bash
# Clone and build
git clone https://github.com/Rozaq125/SSOEngine
cd SSOEngine/01_Core
build.bat

# This will:
# 1. Download Raylib automatically
# 2. Build your game
# 3. Create .exe in build/ folder
```

### Basic Game Structure
```cpp
#include "tools/sso_window.h"
#include "tools/sso_camera.h"

int main() {
    // Initialize window
    InitWindow(800, 600, "My Game");
    
    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        // Your game code here
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
```

## 📁 Project Structure

```
SSOEngine/
├── 00_BuildTools/     # Asset packer source
├── 01_Core/          # Your game code
│   ├── tools/        # Engine tools (C++ headers)
│   ├── scripts/      # Game scripts
│   ├── assets/       # Game assets
│   ├── build.bat     # Build script
│   └── main.cpp      # Game entry point
├── 02_Docs/          # Documentation
├── include/          # Raylib headers
└── lib/             # Raylib libraries
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

### ❌ NOT Supported (Yet)
- **Advanced 3D** - Support shaders,news feature!
- **Networking** - Single player only
- **Mobile Platforms** - Windows only
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
- **Website**: [SSOEngine Webs](https://ssoengine.pages.dev)

---

## 🏆 Showcase

### Games Made with SSOEngine
*Have you made a game with SSOEngine? Let us know and we'll feature it here!*

### Features in Development
- 🎮 3D rendering improvements
- 🎬 Enhanced splash screen system
- 🌐 Networking support
- 🎨 Particle system
- 📱 Mobile export options
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
- **NATHAN**
- **RAYLIB**
- **C++**
- **GITHUB**
---

**© 2026 SSOGames Studio | Lead Developer: Rozak**

*Built with passion for indie game developers everywhere.*
