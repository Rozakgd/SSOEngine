#include "tools/sso_window.h"
#include "tools/sso_audio.h"
#include "tools/sso_provider.h"
#include "tools/sso_ui.h"
#include "tools/sso_text.h"
#include "tools/sso_timer.h"
#include "tools/sso_math.h"
#include "tools/sso_camera.h"
#include "tools/sso_logger.h"
#include "raylib.h"
#include <vector>

struct Player {
    Vector2 position;
    Vector2 velocity;
    float width = 40;
    float height = 60;
    bool isJumping = false;
    float speed = 300.0f;
    float jumpPower = 400.0f;
};

struct Obstacle {
    Vector2 position;
    float width = 30;
    float height = 50;
    float speed = 200.0f;
};

struct RedBall {
    Vector2 position;
    Vector2 velocity;
    float radius = 20;
    float speed = 150.0f;
};

int main() {
    SSO::Logger::Log(SSO::Logger::Level::INFO, "Initializing Running Game...");
    
    // Inisialisasi Window (Di Android, 1280x720 akan jadi Virtual Resolution)
    SSO::Window::Init(1280, 720, "SSOEngine Running Game");
    InitAudioDevice();

    Music myMusic = { 0 };
    Font gameFont = { 0 };

    // Load font - Tambahkan fallback path jika diperlukan
    gameFont = LoadFont("assets/font/jetpilot.ttf");
    
    // Load music dari Bundle SSO
    auto [music, buffer] = SSO::Provider::LoadMusicFromBundle("assets.sso", "musicbg.mp3");
    myMusic = music;
    SSO::Audio::RegisterMusic(myMusic, buffer);
    SSO::Audio::PlayMusic(myMusic);

    // Initialize game objects
    Player player = {{100, 300}, {0, 0}}; 
    std::vector<Obstacle> obstacles;
    RedBall redBall = {{300, 400}, {0, 0}};
    
    int score = 0;
    float health = 100.0f;
    bool gameOver = false;
    bool gameStarted = false;
    
    SSO::Timer gameTimer;
    SSO::Timer obstacleTimer;
    SSO::Timer difficultyTimer;
    
    obstacleTimer.SetValue(2.0f);
    obstacleTimer.Start();
    difficultyTimer.SetValue(10.0f);
    difficultyTimer.Start();
    
    SSO::Camera camera(Vector2{400, 300}, 1280, 720);
    
    Rectangle startButton = {540, 350, 200, 50};
    Rectangle restartButton = {540, 400, 200, 50};
    Rectangle healthBar = {50, 50, 200, 25};
    
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime(); // Lebih stabil untuk cross-platform
        SSO::Audio::UpdateAudio();

        // --- INPUT HANDLING (PC & ANDROID) ---
        bool moveRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
        bool moveLeft = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
        bool jumpAction = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP);

        // Tambahan input Touch untuk Android
        if (GetTouchPointCount() > 0) {
            Vector2 touchPos = GetTouchPosition(0);
            // Jika sentuh layar bagian atas/tengah, anggap loncat
            if (GetGestureDetected() == GESTURE_TAP || GetGestureDetected() == GESTURE_DOUBLETAP) jumpAction = true;
            // Jika sentuh kanan layar, gerak kanan
            if (touchPos.x > GetScreenWidth() / 2) moveRight = true;
            else moveLeft = true;
        }

        if (!gameStarted && !gameOver) {
            SSO::Window::BeginDrawingVirtual();
            ClearBackground({20, 20, 30, 255});
            
            SSO::Text::DrawGradient("RUNNING GAME", 400, 150, 60, YELLOW, RED);
            if (SSO::UI::DrawButton(startButton, "START GAME", gameFont) || (GetTouchPointCount() > 0 && CheckCollisionPointRec(GetTouchPosition(0), startButton))) {
                gameStarted = true;
                gameOver = false;
                score = 0;
                health = 100.0f;
                player.position = {100, 300};
                obstacles.clear();
                obstacleTimer.Start();
                difficultyTimer.Start();
            }
            SSO::Window::EndDrawingVirtual();
            continue;
        }

        if (gameOver) {
            SSO::Window::BeginDrawingVirtual();
            ClearBackground({30, 20, 20, 255});
            SSO::Text::DrawGradient("GAME OVER", 450, 200, 60, RED, DARKPURPLE);
            if (SSO::UI::DrawButton(restartButton, "RESTART", gameFont)) {
                gameOver = false;
                gameStarted = true;
                health = 100.0f;
                score = 0;
                obstacles.clear();
            }
            SSO::Window::EndDrawingVirtual();
            continue;
        }

        // --- LOGIC ---
        if (gameStarted && !gameOver) {
            difficultyTimer.UpdateCountdown(deltaTime);
            if (difficultyTimer.IsFinished()) {
                redBall.speed += 20.0f;
                difficultyTimer.Reset();
                difficultyTimer.Start();
            }

            if (moveRight) player.velocity.x = player.speed;
            else if (moveLeft) player.velocity.x = -player.speed;
            else player.velocity.x *= 0.8f;

            if (jumpAction && !player.isJumping) {
                player.velocity.y = -player.jumpPower;
                player.isJumping = true;
            }

            player.velocity.y += 800.0f * deltaTime;
            player.position.x += player.velocity.x * deltaTime;
            player.position.y += player.velocity.y * deltaTime;

            if (player.position.y >= 500) {
                player.position.y = 500;
                player.velocity.y = 0;
                player.isJumping = false;
            }

            // Ball AI & Obstacles
            redBall.position = Vector2MoveTowards(redBall.position, player.position, redBall.speed * deltaTime);

            obstacleTimer.UpdateCountdown(deltaTime);
            if (obstacleTimer.IsFinished()) {
                obstacles.push_back({{1300, 450}, 30, 50, 200.0f + (score / 10) * 20.0f});
                obstacleTimer.Reset();
                obstacleTimer.Start();
            }

            for (auto it = obstacles.begin(); it != obstacles.end();) {
                it->position.x -= it->speed * deltaTime;
                if (CheckCollisionRecs({player.position.x, player.position.y, player.width, player.height}, {it->position.x, it->position.y, it->width, it->height})) {
                    health -= 10.0f;
                    it = obstacles.erase(it);
                } else if (it->position.x < -50) {
                    it = obstacles.erase(it);
                    score += 10;
                } else ++it;
            }

            if (CheckCollisionCircles(player.position, 20, redBall.position, redBall.radius)) health -= 0.5f;
            if (health <= 0) gameOver = true;

            camera.Follow(player.position, deltaTime);
        }

        // --- RENDER ---
        SSO::Window::BeginDrawingVirtual();
        ClearBackground({15, 15, 25, 255});
        camera.Begin();
            DrawRectangle(0, 560, 2000, 200, {60, 40, 30, 255}); // Ground
            DrawRectangleRec({player.position.x, player.position.y, player.width, player.height}, BLUE);
            for (const auto& obs : obstacles) DrawRectangleRec({obs.position.x, obs.position.y, obs.width, obs.height}, RED);
            DrawCircleV(redBall.position, redBall.radius, RED);
        camera.End();

        SSO::UI::DrawHealthBar(healthBar, health, 100.0f, GREEN, RED);
        DrawText(TextFormat("SCORE: %d", score), 50, 20, 24, YELLOW);
        SSO::Window::EndDrawingVirtual();
    }

    UnloadFont(gameFont);
    CloseAudioDevice();
    SSO::Window::Close();
    return 0;
}