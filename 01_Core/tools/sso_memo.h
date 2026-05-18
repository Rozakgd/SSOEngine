#pragma once
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdlib>
#include <vector>
#include <algorithm>

namespace SSO {
class Memo {
private:
    std::map<std::string, std::string> data;
    std::string basePath;

    std::string getDocumentsPath() {
        #ifdef _WIN32
        const char* userProfile = std::getenv("USERPROFILE");
        if (userProfile) {
            return std::string(userProfile) + "\\Documents\\SSOEngine";
        }
        #else
        const char* home = std::getenv("HOME");
        if (home) {
            return std::string(home) + "/Documents/SSOEngine";
        }
        #endif
        return "SSOEngine";
    }

    void createFolder(const std::string& path) {
        if (!std::filesystem::exists(path)) {
            std::filesystem::create_directories(path);
        }
    }

    std::vector<unsigned char> compressData(const std::string& text) {
        std::vector<unsigned char> compressed;
        compressed.reserve(text.size() * 2);
        
        for (char c : text) {
            compressed.push_back(static_cast<unsigned char>(c ^ 0xAA));
            compressed.push_back(static_cast<unsigned char>((c >> 4) ^ 0x55));
        }
        
        return compressed;
    }

    std::string decompressData(const std::vector<unsigned char>& compressed) {
        std::string decompressed;
        decompressed.reserve(compressed.size() / 2);
        
        for (size_t i = 0; i < compressed.size(); i += 2) {
            if (i + 1 < compressed.size()) {
                unsigned char byte1 = compressed[i] ^ 0xAA;
                unsigned char byte2 = compressed[i + 1] ^ 0x55;
                decompressed += static_cast<char>((byte2 << 4) | (byte1 & 0x0F));
            }
        }
        
        return decompressed;
    }

public:
    Memo() {
        basePath = getDocumentsPath();
    }

    void write(const std::string& key, const std::string& value) {
        data[key] = value;
    }

    std::string read(const std::string& key) {
        auto it = data.find(key);
        if (it != data.end()) {
            return it->second;
        }
        return "";
    }

    void save(const std::string& filename) {
        std::string fullPath = basePath + "\\" + filename + ".sml";
        std::string folderPath = basePath;
        
        createFolder(folderPath);
        
        std::string textData;
        for (const auto& pair : data) {
            textData += pair.first + "=" + pair.second + "\n";
        }
        
        std::vector<unsigned char> compressed = compressData(textData);
        
        std::ofstream file(fullPath, std::ios::binary);
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(compressed.data()), compressed.size());
            file.close();
        }
    }

    void saveToPath(const std::string& customPath, const std::string& filename) {
        std::string fullPath = customPath + "\\" + filename + ".sml";
        std::string folderPath = customPath;
        
        createFolder(folderPath);
        
        std::string textData;
        for (const auto& pair : data) {
            textData += pair.first + "=" + pair.second + "\n";
        }
        
        std::vector<unsigned char> compressed = compressData(textData);
        
        std::ofstream file(fullPath, std::ios::binary);
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(compressed.data()), compressed.size());
            file.close();
        }
    }

    void load(const std::string& filename) {
        std::string fullPath = basePath + "\\" + filename + ".sml";
        
        std::ifstream file(fullPath, std::ios::binary);
        if (file.is_open()) {
            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);
            
            std::vector<unsigned char> compressed(size);
            file.read(reinterpret_cast<char*>(compressed.data()), size);
            file.close();
            
            std::string textData = decompressData(compressed);
            
            std::istringstream iss(textData);
            std::string line;
            while (std::getline(iss, line)) {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    data[key] = value;
                }
            }
        }
    }

    void loadFromPath(const std::string& customPath, const std::string& filename) {
        std::string fullPath = customPath + "\\" + filename + ".sml";
        
        std::ifstream file(fullPath, std::ios::binary);
        if (file.is_open()) {
            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);
            
            std::vector<unsigned char> compressed(size);
            file.read(reinterpret_cast<char*>(compressed.data()), size);
            file.close();
            
            std::string textData = decompressData(compressed);
            
            std::istringstream iss(textData);
            std::string line;
            while (std::getline(iss, line)) {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string key = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    data[key] = value;
                }
            }
        }
    }

    void clear() {
        data.clear();
    }

    bool hasKey(const std::string& key) {
        return data.find(key) != data.end();
    }

    int size() {
        return data.size();
    }

    std::string getBasePath() {
        return basePath;
    }
};
}