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
    
    Music myMusic = { 0 };
    Font gameFont = { 0 };

    SSO::Window::Init(1280, 720, "SSOEngine Running Game");
    InitAudioDevice();

    // Load font
    gameFont = LoadFont("assets/font/jetpilot.ttf");
    if (gameFont.texture.id <= 0) {
        SSO::Logger::Log(SSO::Logger::Level::WARNING, "Failed to load jetpilot.ttf, using default font");
    }

    // Load music
    auto [music, buffer] = SSO::Provider::LoadMusicFromBundle("assets.sso", "musicbg.mp3");
    myMusic = music;
    SSO::Audio::RegisterMusic(myMusic, buffer);
    SSO::Audio::PlayMusic(myMusic);

    // Initialize game objects
    Player player = {{100, 300}, {0, 0}}; // Start higher
    std::vector<Obstacle> obstacles;
    RedBall redBall = {{300, 400}, {0, 0}};
    
    // Add some initial obstacles for testing
    for (int i = 0; i < 3; i++) {
        Obstacle obs;
        obs.position = {400.0f + i * 200.0f, 450};
        obs.speed = 0; // Static obstacles for testing
        obstacles.push_back(obs);
    }
    
    // Game state
    int score = 0;
    float health = 100.0f;
    bool gameOver = false;
    bool gameStarted = false;
    
    // Timers
    SSO::Timer gameTimer;
    SSO::Timer obstacleTimer;
    SSO::Timer difficultyTimer;
    
    obstacleTimer.SetValue(2.0f);
    obstacleTimer.Start();
    difficultyTimer.SetValue(10.0f);
    difficultyTimer.Start();
    
    // Camera
    SSO::Camera camera(Vector2{400, 300}, 1280, 720);
    camera.SetZoom(1.0f);
    
    // UI Elements
    Rectangle startButton = {540, 350, 200, 50};
    Rectangle restartButton = {540, 400, 200, 50};
    Rectangle healthBar = {50, 50, 200, 25};
    
    SSO::Logger::Log(SSO::Logger::Level::SUCCESS, "Game initialized successfully");

    while (!WindowShouldClose()) {
        float deltaTime = gameTimer.GetDeltaTime();
        SSO::Audio::UpdateAudio();

        if (!gameStarted && !gameOver) {
            // Start Screen
            SSO::Window::BeginDrawingVirtual();
            ClearBackground({20, 20, 30, 255});
            
            // Title with gradient effect
            if (gameFont.texture.id > 0) {
                SSO::Text::DrawGradient("RUNNING GAME", 400, 150, 60, YELLOW, RED);
            } else {
                DrawText("RUNNING GAME", 400, 150, 60, YELLOW);
            }
            
            // Start button
            if (SSO::UI::DrawButton(startButton, "START GAME", gameFont)) {
                gameStarted = true;
                gameOver = false;
                score = 0;
                health = 100.0f;
                player.position = {100, 300};
                player.velocity = {0, 0};
                obstacles.clear();
                redBall.position = {300, 400};
                
                // Add initial obstacles
                for (int i = 0; i < 3; i++) {
                    Obstacle obs;
                    obs.position = {400.0f + i * 200.0f, 450};
                    obs.speed = 0;
                    obstacles.push_back(obs);
                }
                
                obstacleTimer.Reset();
                obstacleTimer.Start();
                difficultyTimer.Reset();
                difficultyTimer.Start();
                SSO::Logger::Log(SSO::Logger::Level::INFO, "Game started");
            }
            
            SSO::Window::EndDrawingVirtual();
            continue;
        }

        if (gameOver) {
            // Game Over Screen
            SSO::Window::BeginDrawingVirtual();
            ClearBackground({30, 20, 20, 255});
            
            if (gameFont.texture.id > 0) {
                SSO::Text::DrawGradient("GAME OVER", 450, 200, 60, RED, DARKPURPLE);
                SSO::Text::DrawCentered(TextFormat("Final Score: %d", score), 300, 30, WHITE);
            } else {
                DrawText("GAME OVER", 450, 200, 60, RED);
                DrawText(TextFormat("Final Score: %d", score), 500, 300, 30, WHITE);
            }
            
            if (SSO::UI::DrawButton(restartButton, "RESTART", gameFont)) {
                gameOver = false;
                gameStarted = true;
                score = 0;
                health = 100.0f;
                player.position = {100, 300};
                player.velocity = {0, 0};
                obstacles.clear();
                redBall.position = {300, 400};
                
                // Add initial obstacles
                for (int i = 0; i < 3; i++) {
                    Obstacle obs;
                    obs.position = {400.0f + i * 200.0f, 450};
                    obs.speed = 0;
                    obstacles.push_back(obs);
                }
                
                obstacleTimer.Reset();
                obstacleTimer.Start();
                difficultyTimer.Reset();
                difficultyTimer.Start();
                SSO::Logger::Log(SSO::Logger::Level::INFO, "Game restarted");
            }
            
            SSO::Window::EndDrawingVirtual();
            continue;
        }

        // Update game logic
        if (gameStarted && !gameOver) {
            // Update difficulty
            difficultyTimer.UpdateCountdown(deltaTime);
            if (difficultyTimer.IsFinished()) {
                redBall.speed += 20.0f;
                difficultyTimer.Reset();
                difficultyTimer.Start();
                SSO::Logger::Log(SSO::Logger::Level::INFO, "Difficulty increased!");
            }

            // Player movement
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                player.velocity.x = player.speed;
            } else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                player.velocity.x = -player.speed;
            } else {
                player.velocity.x *= 0.8f; // Friction
            }

            // Jump
            if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) && !player.isJumping) {
                player.velocity.y = -player.jumpPower;
                player.isJumping = true;
            }

            // Apply gravity
            player.velocity.y += 800.0f * deltaTime;
            
            // Update player position
            player.position.x += player.velocity.x * deltaTime;
            player.position.y += player.velocity.y * deltaTime;

            // Ground collision (ground at y=500)
            if (player.position.y >= 500) {
                player.position.y = 500;
                player.velocity.y = 0;
                player.isJumping = false;
            }

            // Keep player in bounds
            player.position.x = ClampVector2(player.position, 0, 0, 1240, 0).x;

            // Update red ball AI
            Vector2 directionToPlayer = {
                player.position.x - redBall.position.x,
                player.position.y - redBall.position.y
            };
            float distance = GetDistance(redBall.position, player.position);
            
            if (distance > 0.1f) {
                directionToPlayer.x /= distance;
                directionToPlayer.y /= distance;
                redBall.velocity.x = directionToPlayer.x * redBall.speed;
                redBall.velocity.y = directionToPlayer.y * redBall.speed;
            }
            
            redBall.position.x += redBall.velocity.x * deltaTime;
            redBall.position.y += redBall.velocity.y * deltaTime;

            // Spawn obstacles
            obstacleTimer.UpdateCountdown(deltaTime);
            if (obstacleTimer.IsFinished()) {
                Obstacle newObstacle;
                newObstacle.position = {1280, 450}; // Spawn on ground
                newObstacle.speed = 200.0f + (score / 10) * 20.0f; // Increase speed with score
                obstacles.push_back(newObstacle);
                obstacleTimer.Reset();
                obstacleTimer.Start();
                SSO::Logger::Log(SSO::Logger::Level::INFO, "New obstacle spawned!");
            }

            // Update obstacles
            for (auto it = obstacles.begin(); it != obstacles.end(); ) {
                it->position.x -= it->speed * deltaTime;
                
                // Remove off-screen obstacles and add score
                if (it->position.x < -50) {
                    it = obstacles.erase(it);
                    score += 10;
                } else {
                    ++it;
                }
            }

            // Check collisions
            Rectangle playerRect = {player.position.x, player.position.y, player.width, player.height};
            
            // Check obstacle collisions
            for (const auto& obstacle : obstacles) {
                Rectangle obstacleRect = {obstacle.position.x, obstacle.position.y, obstacle.width, obstacle.height};
                if (CheckCollision(playerRect, obstacleRect)) {
                    health -= 20.0f;
                    camera.Shake(10.0f, 0.3f);
                    SSO::Logger::Log(SSO::Logger::Level::WARNING, "Player hit obstacle!");
                }
            }

            // Check red ball collision
            if (CheckCollisionCircles(player.position, 20, redBall.position, redBall.radius)) {
                health -= 0.5f; // Continuous damage when touching red ball
                camera.Shake(5.0f, 0.1f);
            }

            // Check game over
            if (health <= 0) {
                gameOver = true;
                SSO::Logger::Log(SSO::Logger::Level::INFO, "Game Over!");
            }

            // Update camera to follow player
            camera.Follow(player.position, deltaTime);
        }

        // Render game
        SSO::Window::BeginDrawingVirtual();
        ClearBackground({15, 15, 25, 255});

        camera.Begin();

        // Draw ground
        DrawRectangle(0, 560, 2000, 200, {60, 40, 30, 255}); // Ground at bottom
        DrawLine(0, 560, 2000, 560, GRAY);
        
        // Draw some ground details
        for (int i = 0; i < 20; i++) {
            DrawRectangle(i * 100, 570, 80, 10, {80, 60, 40, 255});
        }

        // Draw player
        Rectangle playerRect = {player.position.x, player.position.y, player.width, player.height};
        DrawRectangleRec(playerRect, BLUE);
        DrawRectangleLinesEx(playerRect, 2, DARKBLUE);

        // Draw obstacles
        for (const auto& obstacle : obstacles) {
            Rectangle obstacleRect = {obstacle.position.x, obstacle.position.y, obstacle.width, obstacle.height};
            DrawRectangleRec(obstacleRect, RED);
            DrawRectangleLinesEx(obstacleRect, 2, MAROON);
        }

        // Draw red ball
        DrawCircleV(redBall.position, redBall.radius, RED);
        DrawCircleLines(redBall.position.x, redBall.position.y, redBall.radius, MAROON);

        camera.End();

        // Draw UI
        if (gameFont.texture.id > 0) {
            SSO::UI::DrawTextShadow(gameFont, TextFormat("SCORE: %d", score), {50, 20}, 24, 1, YELLOW);
            SSO::UI::DrawHealthBar(healthBar, health, 100.0f, GREEN, RED);
            SSO::UI::DrawTextShadow(gameFont, "HEALTH", {50, 80}, 18, 1, WHITE);
        } else {
            DrawText(TextFormat("SCORE: %d", score), 50, 20, 24, YELLOW);
            DrawText("HEALTH", 50, 80, 18, WHITE);
            SSO::UI::DrawHealthBar(healthBar, health, 100.0f, GREEN, RED);
        }

        // Draw controls
        if (gameFont.texture.id > 0) {
            SSO::Text::DrawWrapped("Controls: ARROW KEYS/WASD to move, SPACE to jump", 50, 650, 400, 16, LIGHTGRAY);
        } else {
            DrawText("Controls: ARROW KEYS/WASD to move, SPACE to jump", 50, 650, 16, LIGHTGRAY);
        }

        SSO::Window::EndDrawingVirtual();
    }

    // Cleanup
    if (gameFont.texture.id > 0) UnloadFont(gameFont);
    SSO::Audio::ClearMusicManager();
    CloseAudioDevice();
    SSO::Window::Close();
    
    SSO::Logger::Log(SSO::Logger::Level::INFO, "Game shutdown complete");

    return 0;
}