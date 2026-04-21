#include "sso_project_manager.h"
#include <filesystem>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <chrono>

void ProjectManager::Init() {
    currentProject = "";
    documentsPath = GetDocumentsPath();
    LoadRecentProjects();
}

std::string ProjectManager::GetDocumentsPath() {
    #ifdef _WIN32
    const char* userProfile = std::getenv("USERPROFILE");
    if (userProfile) {
        std::string docsPath = std::string(userProfile) + "\\Documents\\FiveXProjects";
        if (!std::filesystem::exists(docsPath)) {
            std::system(("mkdir \"" + docsPath + "\"").c_str());
        }
        return docsPath;
    }
    #else
    const char* home = std::getenv("HOME");
    if (home) {
        std::string docsPath = std::string(home) + "/Documents/FiveXProjects";
        if (!std::filesystem::exists(docsPath)) {
            std::system(("mkdir -p \"" + docsPath + "\"").c_str());
        }
        return docsPath;
    }
    #endif
    return "FiveXProjects";
}

void ProjectManager::CreateNewProject(const std::string& projectName) {
    std::string projectPath = documentsPath + "/" + projectName;
    
    #ifdef _WIN32
    std::system(("mkdir \"" + projectPath + "\"").c_str());
    std::system(("mkdir \"" + projectPath + "/assets\"").c_str());
    std::system(("mkdir \"" + projectPath + "/src\"").c_str());
    std::system(("mkdir \"" + projectPath + "/build\"").c_str());
    #else
    std::system(("mkdir -p \"" + projectPath + "/assets\"").c_str());
    std::system(("mkdir -p \"" + projectPath + "/src\"").c_str());
    std::system(("mkdir -p \"" + projectPath + "/build\"").c_str());
    #endif
    
    isDownloading = true;
    statusMessage = "Downloading SDK...";
    downloadProgress = 0.0f;
    
    DownloadSDKAsync(projectPath);
}

void ProjectManager::DownloadSDKAsync(const std::string& projectPath) {
    if (downloadThread.joinable()) {
        downloadThread.join();
    }
    
    downloadThread = std::thread([this, projectPath]() {
        for (int i = 0; i < 90; i++) {
            downloadProgress = (float)i;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        
        if (DownloadSDK(projectPath)) {
            isDownloading = false;
            isExtracting = true;
            statusMessage = "Extracting SDK...";
            downloadProgress = 0.0f;
            
            ExtractSDKAsync(projectPath, projectPath + "/SSOEngine1.6.EXPERIMENTAL.PLUGIN.zip");
        }
    });
}

bool ProjectManager::DownloadSDK(const std::string& projectPath) {
    std::string url = "https://github.com/Rozaq125/SSOEngine/releases/download/1.6/SSOEngine1.6.EXPERIMENTAL.PLUGIN.zip";
    std::string outputPath = projectPath + "/SSOEngine1.6.EXPERIMENTAL.PLUGIN.zip";
    
    #ifdef _WIN32
    std::string command = "curl -L -o \"" + outputPath + "\" \"" + url + "\"";
    int result = std::system(command.c_str());
    return result == 0;
    #else
    std::string command = "curl -L -o \"" + outputPath + "\" \"" + url + "\"";
    int result = std::system(command.c_str());
    return result == 0;
    #endif
}

void ProjectManager::ExtractSDKAsync(const std::string& projectPath, const std::string& zipPath) {
    if (extractThread.joinable()) {
        extractThread.join();
    }
    
    extractThread = std::thread([this, projectPath, zipPath]() {
        if (ExtractSDK(projectPath, zipPath)) {
            std::string projectFile = projectPath + "/project.fivex";
            std::ofstream file(projectFile);
            if (file.is_open()) {
                file << GenerateProjectFile("NewProject");
                file.close();
            }
            
            AddToRecent("NewProject", projectPath);
            currentProject = projectPath;
        }
        
        isExtracting = false;
        downloadProgress = 100.0f;
        statusMessage = "";
    });
}

bool ProjectManager::ExtractSDK(const std::string& projectPath, const std::string& zipPath) {
    #ifdef _WIN32
    std::string command = "powershell -Command \"Expand-Archive -Path '" + zipPath + "' -DestinationPath '" + projectPath + "' -Force\"";
    int result = std::system(command.c_str());
    #else
    std::string command = "unzip -o '" + zipPath + "' -d '" + projectPath + "'";
    int result = std::system(command.c_str());
    #endif
    
    std::filesystem::remove(zipPath);
    return result == 0;
}

void ProjectManager::OpenProject(const std::string& projectPath) {
    Project project;
    if (LoadProjectFile(projectPath, project)) {
        currentProject = projectPath;
        AddToRecent(project.name, projectPath);
        SaveContext();
        LaunchEditor(projectPath);
    }
}

void ProjectManager::LoadRecentProjects() {
    std::string recentPath = documentsPath + "/recent.json";
    std::ifstream file(recentPath);
    if (file.is_open()) {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        
        size_t pos = 0;
        while ((pos = content.find("\"name\"", pos)) != std::string::npos) {
            Project project;
            
            size_t nameStart = content.find("\"", pos + 5) + 1;
            size_t nameEnd = content.find("\"", nameStart);
            if (nameStart != std::string::npos && nameEnd != std::string::npos) {
                project.name = content.substr(nameStart, nameEnd - nameStart);
            }
            
            size_t pathPos = content.find("\"path\"", nameEnd);
            size_t pathStart = content.find("\"", pathPos + 6) + 1;
            size_t pathEnd = content.find("\"", pathStart);
            if (pathStart != std::string::npos && pathEnd != std::string::npos) {
                project.path = content.substr(pathStart, pathEnd - pathStart);
            }
            
            size_t modPos = content.find("\"lastModified\"", pathEnd);
            size_t modStart = content.find("\"", modPos + 15) + 1;
            size_t modEnd = content.find("\"", modStart);
            if (modStart != std::string::npos && modEnd != std::string::npos) {
                project.lastModified = content.substr(modStart, modEnd - modStart);
            }
            
            project.isRecent = true;
            recentProjects.push_back(project);
            pos = nameEnd + 1;
        }
    }
}

void ProjectManager::SaveRecentProjects() {
    std::string recentPath = documentsPath + "/recent.json";
    std::ofstream file(recentPath);
    if (file.is_open()) {
        file << "{\n";
        file << "  \"projects\": [\n";
        
        for (size_t i = 0; i < recentProjects.size(); ++i) {
            file << "    {\n";
            file << "      \"name\": \"" << recentProjects[i].name << "\",\n";
            file << "      \"path\": \"" << recentProjects[i].path << "\",\n";
            file << "      \"lastModified\": \"" << recentProjects[i].lastModified << "\"\n";
            file << "    }";
            if (i < recentProjects.size() - 1) file << ",";
            file << "\n";
        }
        
        file << "  ]\n";
        file << "}\n";
        file.close();
    }
}

void ProjectManager::AddToRecent(const std::string& projectName, const std::string& projectPath) {
    auto it = std::find_if(recentProjects.begin(), recentProjects.end(),
        [&projectPath](const Project& p) { return p.path == projectPath; });
    
    if (it != recentProjects.end()) {
        recentProjects.erase(it);
    }
    
    Project newProject;
    newProject.name = projectName;
    newProject.path = projectPath;
    newProject.isRecent = true;
    
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    newProject.lastModified = oss.str();
    
    recentProjects.insert(recentProjects.begin(), newProject);
    
    if (recentProjects.size() > 10) {
        recentProjects.resize(10);
    }
    
    SaveRecentProjects();
}

std::vector<Project> ProjectManager::GetRecentProjects() {
    return recentProjects;
}

bool ProjectManager::ProjectExists(const std::string& projectPath) {
    return std::filesystem::exists(projectPath);
}

std::string ProjectManager::GenerateProjectFile(const std::string& projectName) {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    
    std::string content = "{\n";
    content += "  \"name\": \"" + projectName + "\",\n";
    content += "  \"version\": \"1.0.0\",\n";
    content += "  \"engine\": \"FiveX Engine v1.0\",\n";
    content += "  \"created\": \"" + oss.str() + "\",\n";
    content += "  \"lastModified\": \"" + oss.str() + "\",\n";
    content += "  \"settings\": {\n";
    content += "    \"targetPlatform\": \"Desktop\",\n";
    content += "    \"graphics\": \"OpenGL\",\n";
    content += "    \"resolution\": \"1280x720\",\n";
    content += "    \"fullscreen\": false\n";
    content += "  },\n";
    content += "  \"folders\": {\n";
    content += "    \"assets\": \"assets/\",\n";
    content += "    \"source\": \"src/\",\n";
    content += "    \"build\": \"build/\"\n";
    content += "  }\n";
    content += "}";
    
    return content;
}

bool ProjectManager::LoadProjectFile(const std::string& filePath, Project& project) {
    std::ifstream file(filePath);
    if (!file.is_open()) return false;
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("\"name\"") != std::string::npos) {
            size_t start = line.find("\"") + 1;
            size_t end = line.find("\"", start);
            if (start != std::string::npos && end != std::string::npos) {
                project.name = line.substr(start, end - start);
            }
        }
        if (line.find("\"lastModified\"") != std::string::npos) {
            size_t start = line.find("\"") + 1;
            size_t end = line.find("\"", start);
            if (start != std::string::npos && end != std::string::npos) {
                project.lastModified = line.substr(start, end - start);
            }
        }
    }
    
    project.path = filePath.substr(0, filePath.find_last_of("/\\"));
    file.close();
    return true;
}

void ProjectManager::SaveContext() {
    std::string contextPath = documentsPath + "/session.tmp";
    std::ofstream file(contextPath);
    if (file.is_open()) {
        file << "current_project=" << currentProject << "\n";
        file.close();
    }
}

void ProjectManager::LaunchEditor(const std::string& projectPath) {
    #ifdef _WIN32
    std::string command = "FiveX-Editor.exe \"" + projectPath + "\"";
    std::system(command.c_str());
    #else
    std::string command = "./FiveX-Editor \"" + projectPath + "\"";
    std::system(command.c_str());
    #endif
}
