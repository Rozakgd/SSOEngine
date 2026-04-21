#include "tools/sso_window.h"
#include "tools/sso_ui.h"
#include "tools/sso_splash.h"
#include "sso_project_manager.h"

int main() {
    SSO::Window::Init(1280, 720, "FiveX Engine Launcher");
    
    SSO::Splash::Show(2.0f, "FiveX Engine", "Project Manager v1.0");
    
    Font gameFont = SSO::UI::GetDefaultFont();
    SetTextureFilter(gameFont.texture, TEXTURE_FILTER_BILINEAR);
    
    ProjectManager projectManager;
    projectManager.Init();
    
    Rectangle newProjectBtn = {500, 200, 280, 50};
    Rectangle openProjectBtn = {500, 270, 280, 50};
    Rectangle recentProjectsArea = {200, 350, 880, 300};
    
    char projectNameInput[256] = {0};
    bool showNewProjectDialog = false;
    bool projectNameActive = false;
    bool showLoadingScreen = false;
    float loadingStartTime = 0.0f;
    
    while (!WindowShouldClose()) {
        SSO::Window::BeginDrawingVirtual();
        ClearBackground({15, 15, 20, 255});
        
        if (showLoadingScreen) {
            float currentTime = (float)GetTime();
            float elapsed = currentTime - loadingStartTime;
            
            DrawRectangle(0, 0, 1280, 720, {15, 15, 20, 255});
            SSO::UI::DrawTextShadow(gameFont, "FiveX Engine", {50, 300}, 48, 1, WHITE);
            
            if (projectManager.isDownloading.load()) {
                SSO::UI::DrawTextShadow(gameFont, TextFormat("Downloading SDK... %.0f%%", projectManager.downloadProgress.load()), {50, 370}, 24, 1, GRAY);
            } else if (projectManager.isExtracting.load()) {
                SSO::UI::DrawTextShadow(gameFont, TextFormat("Extracting SDK... %.0f%%", projectManager.downloadProgress.load()), {50, 370}, 24, 1, GRAY);
            } else {
                SSO::UI::DrawTextShadow(gameFont, "Loading Project...", {50, 370}, 24, 1, GRAY);
            }
            
            float progress = projectManager.downloadProgress.load() / 100.0f;
            DrawRectangle(50, 420, 1180, 10, {30, 30, 35, 255});
            DrawRectangle(50, 420, (int)(1180 * progress), 10, {70, 130, 180, 255});
            
            if (elapsed > 3.0f && !projectManager.isDownloading.load() && !projectManager.isExtracting.load()) {
                showLoadingScreen = false;
            }
        } else {
            DrawRectangle(0, 0, 1280, 150, {25, 25, 35, 255});
            SSO::UI::DrawTextShadow(gameFont, "FiveX Engine", {50, 30}, 48, 1, WHITE);
            SSO::UI::DrawTextShadow(gameFont, "Project Manager", {50, 90}, 24, 1, GRAY);
        
            if (SSO::UI::DrawButton(newProjectBtn, "Create New Project", gameFont)) {
                showNewProjectDialog = true;
                projectNameActive = true;
            }
            
            if (SSO::UI::DrawButton(openProjectBtn, "Open Project", gameFont)) {
                #ifdef _WIN32
                std::system(("explorer \"" + projectManager.documentsPath + "\"").c_str());
                #else
                std::system(("xdg-open \"" + projectManager.documentsPath + "\"").c_str());
                #endif
            }
            
            DrawRectangleLinesEx(recentProjectsArea, 2, {70, 70, 80, 255});
            SSO::UI::DrawTextShadow(gameFont, "Recent Projects", {220, 370}, 20, 1, WHITE);
            
            std::vector<Project> recentProjects = projectManager.GetRecentProjects();
            for (size_t i = 0; i < recentProjects.size() && i < 8; ++i) {
                Rectangle projectItem = {220, 410.0f + (float)i * 40.0f, 840, 35};
                
                bool hovering = CheckCollisionPointRec(SSO::Window::GetVirtualMouse(), projectItem);
                if (hovering) {
                    DrawRectangle(projectItem.x, projectItem.y, projectItem.width, projectItem.height, {45, 45, 50, 255});
                }
                
                std::string displayName = recentProjects[i].name;
                if (displayName.length() > 25) {
                    displayName = displayName.substr(0, 22) + "...";
                }
                DrawTextEx(gameFont, displayName.c_str(), {projectItem.x + 10, projectItem.y + 10}, 16, 1, WHITE);
                
                std::string displayPath = recentProjects[i].path;
                if (displayPath.length() > 30) {
                    displayPath = "..." + displayPath.substr(displayPath.length() - 27);
                }
                DrawTextEx(gameFont, displayPath.c_str(), {projectItem.x + 250, projectItem.y + 10}, 14, 1, GRAY);
                DrawTextEx(gameFont, recentProjects[i].lastModified.c_str(), {projectItem.x + 700, projectItem.y + 12}, 12, 1, GRAY);
                
                if (hovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    std::string projectFilePath = recentProjects[i].path + "/" + recentProjects[i].name + ".fivex";
                    projectManager.OpenProject(projectFilePath);
                    showLoadingScreen = true;
                    loadingStartTime = (float)GetTime();
                }
            }
            
            if (showNewProjectDialog) {
                DrawRectangle(300, 250, 680, 220, {30, 30, 35, 250});
                DrawRectangleLinesEx({300, 250, 680, 220}, 2, {70, 70, 80, 255});
                SSO::UI::DrawTextShadow(gameFont, "Create New Project", {320, 270}, 24, 1, WHITE);
                SSO::UI::DrawTextShadow(gameFont, "Project Name:", {320, 320}, 16, 1, GRAY);
                
                Rectangle inputBox = {320, 350, 640, 40};
                DrawRectangleRec(inputBox, {20, 20, 25, 255});
                DrawRectangleLinesEx(inputBox, 1, {60, 60, 70, 255});
                
                if (projectNameActive) {
                    DrawRectangleLinesEx(inputBox, 2, SKYBLUE);
                }
                
                SSO::UI::DrawTextShadow(gameFont, projectNameInput, {inputBox.x + 10, inputBox.y + 10}, 16, 1, WHITE);
                
                Rectangle createBtn = {520, 410, 160, 40};
                Rectangle cancelBtn = {350, 410, 160, 40};
                
                if (SSO::UI::DrawButton(createBtn, "Create", gameFont)) {
                    if (strlen(projectNameInput) > 0) {
                        projectManager.CreateNewProject(projectNameInput);
                        memset(projectNameInput, 0, sizeof(projectNameInput));
                        showNewProjectDialog = false;
                        projectNameActive = false;
                        showLoadingScreen = true;
                        loadingStartTime = (float)GetTime();
                    }
                }
                
                if (SSO::UI::DrawButton(cancelBtn, "Cancel", gameFont)) {
                    memset(projectNameInput, 0, sizeof(projectNameInput));
                    showNewProjectDialog = false;
                    projectNameActive = false;
                }
                
                if (projectNameActive) {
                    int key = GetCharPressed();
                    while (key > 0) {
                        if ((key >= 32) && (key <= 125) && (strlen(projectNameInput) < 255)) {
                            size_t len = strlen(projectNameInput);
                            projectNameInput[len] = (char)key;
                            projectNameInput[len + 1] = '\0';
                        }
                        key = GetCharPressed();
                    }
                    
                    if (IsKeyPressed(KEY_BACKSPACE)) {
                        if (strlen(projectNameInput) > 0) {
                            projectNameInput[strlen(projectNameInput) - 1] = '\0';
                        }
                    }
                }
            }
            
            SSO::Text::DrawHyperlink("GitHub: FiveX Engine", 1050, 680, 14, BLUE, "https://github.com/Rozaq125/SSOEngine");
        }
        
        SSO::Window::EndDrawingVirtual();
    }
    
    SSO::Window::Close();
    return 0;
}
