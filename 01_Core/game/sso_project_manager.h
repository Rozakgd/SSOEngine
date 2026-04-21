#pragma once
#include "tools/sso_window.h"
#include "tools/sso_ui.h"
#include "tools/sso_text.h"
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <atomic>

struct Project {
    std::string name;
    std::string path;
    std::string lastModified;
    bool isRecent;
    
    Project() {
        name = "";
        path = "";
        lastModified = "";
        isRecent = false;
    }
};

class ProjectManager {
public:
    std::vector<Project> recentProjects;
    std::string currentProject;
    std::string documentsPath;
    std::atomic<bool> isDownloading;
    std::atomic<bool> isExtracting;
    std::atomic<float> downloadProgress;
    std::string statusMessage;
    std::thread downloadThread;
    std::thread extractThread;
    
    void Init();
    void CreateNewProject(const std::string& projectName);
    void OpenProject(const std::string& projectPath);
    void LoadRecentProjects();
    void SaveRecentProjects();
    void AddToRecent(const std::string& projectName, const std::string& projectPath);
    std::vector<Project> GetRecentProjects();
    bool ProjectExists(const std::string& projectPath);
    std::string GenerateProjectFile(const std::string& projectName);
    bool LoadProjectFile(const std::string& filePath, Project& project);
    void SaveContext();
    void LaunchEditor(const std::string& projectPath);
    std::string GetDocumentsPath();
    void DownloadSDKAsync(const std::string& projectPath);
    void ExtractSDKAsync(const std::string& projectPath, const std::string& zipPath);
    bool DownloadSDK(const std::string& projectPath);
    bool ExtractSDK(const std::string& projectPath, const std::string& zipPath);
};
