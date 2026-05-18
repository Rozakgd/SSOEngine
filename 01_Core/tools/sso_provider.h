// ==============================================================================
// SSOEngine v1.7 - OFFICIAL LICENSE AND ATTRIBUTION FILE //
// ==============================================================================
// // AUTHOR & LEAD DEVELOPER: // Rozak // // CONTACT: //
// Email: spctacularstudio@gmail.com // GitHub: https://github.com/Rozaq125 //
// Website: https://ssoengine.pages.dev // // SUPPORT THE PROJECT: // Donationware
// Link: https://saweria.co/omedonation // //
// ------------------------------------------------------------------------------
// MIT LICENSE //
// ------------------------------------------------------------------------------
// Copyright (c) 2026 SSOFoundation// // Permission
// is hereby granted, free of charge, to any person obtaining a copy // of this
// software and associated documentation files (the "Software"), to deal // in the
// software without restriction, including without limitation the rights // to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell // copies of
// the Software, and to permit persons to whom the Software is // furnished to do
// so, subject to the following conditions: // // The above copyright notice and
// this permission notice shall be included in all // copies or substantial
// portions of the Software. // // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT
// WARRANTY OF ANY KIND, EXPRESS OR // IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, // FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER // LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
// THE USE OR OTHER DEALINGS IN THE // SOFTWARE. // //
// ------------------------------------------------------------------------------
// BUILT WITH & SPECIAL THANKS //
// ------------------------------------------------------------------------------
// RAYLIB: (c) Ramon Santamaria (https://www.raylib.com) // COMPILER: MinGW-w64
// Project (GCC Suite) // CONTRIBUTOR: Nathan (Feedback Provider) // // COMMUNITY
// HALL OF FAME (Reddit Feedback & Support): // u/rileygrham // u/Jimmy-M-420 //
// u/gabriel_aplok // u/InitialComfortable70 // u/ghulmar // //
// ------------------------------------------------------------------------------
// PROJECT DESCRIPTION //
// ------------------------------------------------------------------------------
// SSOEngine is a professional-grade, cross-platform 2D/3D game engine SDK //
// built on top of Raylib //
// high-precision physics, advanced camera systems, and a professional build //
// pipeline for Windows and Android distribution. // // VERSION: 1.7 //
// ==============================================================================

#ifndef SSO_PROVIDER_H
#define SSO_PROVIDER_H

#include "raylib.h"
#include <iostream>

#if defined(__ANDROID__) || defined(ANDROID)
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <android/native_window.h>

extern AAssetManager* androidAssetManager;
#endif

#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include "nlohmann/json.hpp"

namespace SSO {
    namespace Provider {

        struct AssetHeader {
            char signature[4];
            int version;
            int fileCount;
        };

        struct FileMetadata {
            char fileName[64];
            long long fileSize;
            long long offset;
        };

#if defined(__ANDROID__) || defined(ANDROID)
        inline unsigned char* LoadRawDataFromAndroidAsset(const std::string& bundlePath, const std::string& targetFile, int* dataSize) {
            if (!androidAssetManager) {
                TraceLog(LOG_ERROR, "SSO_PROVIDER: Android AssetManager initialization failed.");
                return nullptr;
            }

            AAsset* asset = AAssetManager_open(androidAssetManager, bundlePath.c_str(), AASSET_MODE_STREAMING);
            if (!asset) {
                TraceLog(LOG_ERROR, "SSO_PROVIDER: Unable to open Android asset %s", bundlePath.c_str());
                return nullptr;
            }

            AssetHeader header;
            AAsset_read(asset, &header, sizeof(AssetHeader));

            if (std::memcmp(header.signature, "SSO ", 4) != 0) {
                TraceLog(LOG_ERROR, "SSO_PROVIDER: Invalid Android bundle signature detected.");
                AAsset_close(asset);
                return nullptr;
            }

            for (int i = 0; i < header.fileCount; ++i) {
                FileMetadata meta;
                AAsset_read(asset, &meta, sizeof(FileMetadata));
                
                if (targetFile == std::string(meta.fileName)) {
                    unsigned char* buffer = (unsigned char*)RL_MALLOC(meta.fileSize);
                    AAsset_seek(asset, meta.offset, SEEK_SET);
                    AAsset_read(asset, buffer, meta.fileSize);
                    AAsset_close(asset);
                    
                    *dataSize = (int)meta.fileSize;
                    TraceLog(LOG_INFO, "SSO_PROVIDER: Successfully extracted %s from Android bundle (%d bytes)", targetFile.c_str(), *dataSize);
                    return buffer;
                }
            }

            TraceLog(LOG_WARNING, "SSO_PROVIDER: File '%s' not found in Android bundle", targetFile.c_str());
            AAsset_close(asset);
            return nullptr;
        }
#endif

        inline unsigned char* LoadRawDataFromPC(const std::string& bundlePath, const std::string& targetFile, int* dataSize) {
            std::ifstream file(bundlePath, std::ios::binary);
            if (!file) {
                TraceLog(LOG_ERROR, "SSO_PROVIDER: Failed to open bundle at %s", bundlePath.c_str());
                return nullptr;
            }

            AssetHeader header;
            file.read(reinterpret_cast<char*>(&header), sizeof(AssetHeader));

            if (std::memcmp(header.signature, "SSO ", 4) != 0) {
                TraceLog(LOG_ERROR, "SSO_PROVIDER: Invalid bundle signature detected.");
                file.close();
                return nullptr;
            }

            for (int i = 0; i < header.fileCount; ++i) {
                FileMetadata meta;
                file.read(reinterpret_cast<char*>(&meta), sizeof(FileMetadata));
                
                if (targetFile == std::string(meta.fileName)) {
                    unsigned char* buffer = (unsigned char*)RL_MALLOC(meta.fileSize);
                    file.seekg(meta.offset);
                    file.read(reinterpret_cast<char*>(buffer), meta.fileSize);
                    file.close();
                    
                    *dataSize = (int)meta.fileSize;
                    TraceLog(LOG_INFO, "SSO_PROVIDER: Successfully extracted %s from bundle (%d bytes)", targetFile.c_str(), *dataSize);
                    return buffer;
                }
            }

            TraceLog(LOG_WARNING, "SSO_PROVIDER: File '%s' not found in bundle", targetFile.c_str());
            file.close();
            return nullptr;
        }

        inline unsigned char* LoadRawDataFromBundle(const std::string& bundlePath, const std::string& targetFile, int* dataSize) {
#if !defined(__ANDROID__)
            return LoadRawDataFromPC(bundlePath, targetFile, dataSize);
#else
            return LoadRawDataFromAndroidAsset(bundlePath, targetFile, dataSize);
#endif
        }

        inline nlohmann::json LoadJSONFromBundle(const std::string& bPath, const std::string& fName) {
            int size = 0;
            unsigned char* data = LoadRawDataFromBundle(bPath, fName, &size);
            if (!data) return nullptr;

            std::string jsonStr(reinterpret_cast<char*>(data), size);
            nlohmann::json jsonData = nlohmann::json::parse(jsonStr, nullptr, false);
            RL_FREE(data);
            
            if (jsonData.is_discarded()) {
                TraceLog(LOG_ERROR, "SSO_PROVIDER: Failed to parse JSON from %s", fName.c_str());
                return nullptr;
            }
            
            TraceLog(LOG_INFO, "SSO_PROVIDER: Successfully loaded JSON from %s", fName.c_str());
            return jsonData;
        }

        
        inline Texture2D LoadTextureFromBundle(const std::string& bPath, const std::string& fName) {
            int size = 0;
            unsigned char* data = LoadRawDataFromBundle(bPath, fName, &size);
            if (!data) return { 0 };

            Image img = LoadImageFromMemory(GetFileExtension(fName.c_str()), data, size);
            Texture2D tex = LoadTextureFromImage(img);
            UnloadImage(img);
            if (data) RL_FREE(data);
            return tex;
        }

        inline Font LoadFontFromBundle(const std::string& bPath, const std::string& fName, int fontSize) {
            int size = 0;
            unsigned char* data = LoadRawDataFromBundle(bPath, fName, &size);
            if (!data) return GetFontDefault();

            Font font = LoadFontFromMemory(GetFileExtension(fName.c_str()), data, size, fontSize, NULL, 0);
            if (data) RL_FREE(data);
            return font;
        }

        inline Wave LoadWaveFromBundle(const std::string& bPath, const std::string& fName) {
            int size = 0;
            unsigned char* data = LoadRawDataFromBundle(bPath, fName, &size);
            if (!data) return { 0 };

            Wave wave = LoadWaveFromMemory(GetFileExtension(fName.c_str()), data, size);
            if (data) RL_FREE(data);
            return wave;
        }

        inline std::pair<Music, unsigned char*> LoadMusicFromBundle(const std::string& bPath, const std::string& fName, int* dataSize = nullptr) {
            int size = 0;
            unsigned char* data = LoadRawDataFromBundle(bPath, fName, &size);
            if (!data) return { { 0 }, nullptr };

            Music music = LoadMusicStreamFromMemory(GetFileExtension(fName.c_str()), data, size);
            if (dataSize) *dataSize = size;
            return { music, data };
        }

        inline Model LoadModelFromBundle(const std::string& bPath, const std::string& fName) {
            int size = 0;
            unsigned char* data = LoadRawDataFromBundle(bPath, fName, &size);
            if (!data) return { 0 };

            const char* tempFileName = "temp_model.obj";
            const char* tempMtlFileName = "materials.mtl";
            
            SaveFileData(tempFileName, data, size);
            
            FILE* mtlFile = fopen(tempMtlFileName, "w");
            if (mtlFile) {
                fprintf(mtlFile, "# Dummy MTL file to prevent TINYOBJ errors\n");
                fprintf(mtlFile, "newmtl default\n");
                fprintf(mtlFile, "Ka 1.0 1.0 1.0\n");
                fprintf(mtlFile, "Kd 1.0 1.0 1.0\n");
                fprintf(mtlFile, "Ks 0.0 0.0 0.0\n");
                fprintf(mtlFile, "Ns 0\n");
                fprintf(mtlFile, "illum 2\n");
                fclose(mtlFile);
            }
            
            Model model = LoadModel(tempFileName);
            
            std::remove(tempFileName);
            std::remove(tempMtlFileName);
            RL_FREE(data);
            
            TraceLog(LOG_INFO, "SSO_PROVIDER: Successfully loaded Model from %s", fName.c_str());
            return model;
        }

        inline Mesh LoadMeshFromBundle(const std::string& bPath, const std::string& fName) {
            TraceLog(LOG_WARNING, "SSO_PROVIDER: LoadMeshFromBundle - Not Supported Yet");
            return { 0 };
        }

        inline Material LoadMaterialFromBundle(const std::string& bPath, const std::string& fName) {
            TraceLog(LOG_WARNING, "SSO_PROVIDER: LoadMaterialFromBundle - Not Supported Yet");
            return LoadMaterialDefault();
        }

        inline ModelAnimation LoadAnimationFromBundle(const std::string& bPath, const std::string& fName, int* animCount) {
            TraceLog(LOG_WARNING, "SSO_PROVIDER: LoadAnimationFromBundle - Not Supported Yet");
            *animCount = 0;
            return { 0 };
        }

        inline unsigned char* LoadVideoDataFromBundle(const std::string& bPath, const std::string& fName, int* size) {
            return LoadRawDataFromBundle(bPath, fName, size);
        }
    }
}

#endif