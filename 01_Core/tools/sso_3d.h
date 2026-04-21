#ifndef SSO_3D_H
#define SSO_3D_H

#include "raylib.h"
#include "raymath.h"
#include "sso_provider.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace SSO {
    namespace ThreeD {
        
        struct Model3D {
            Model model;
            Texture2D texture;
            Vector3 position;
            Vector3 rotation;
            Vector3 scale;
            bool loaded;
            Color tint;
            
            Model3D() {
                model = {0};
                texture = {0};
                position = {0, 0, 0};
                rotation = {0, 0, 0};
                scale = {1, 1, 1};
                loaded = false;
                tint = WHITE;
            }
        };
        
        struct Shader3D {
            Shader shader;
            bool loaded;
            std::string name;
            
            Shader3D() {
                shader = {0};
                loaded = false;
                name = "";
            }
        };
        
        inline std::unordered_map<std::string, Model3D> models;
        inline std::unordered_map<std::string, Shader3D> shaders;
        
        inline void SSO_DrawCube(Vector3 position, float size, Color color) {
            DrawCube(position, size, size, size, color);
        }
        
        inline void SSO_DrawCubeWires(Vector3 position, float size, Color color) {
            DrawCubeWires(position, size, size, size, color);
        }
        
        inline void SSO_DrawSphere(Vector3 position, float radius, Color color) {
            DrawSphere(position, radius, color);
        }
        
        inline void SSO_DrawCylinder(Vector3 position, float radiusTop, float radiusBottom, float height, Color color) {
            DrawCylinder(position, radiusTop, radiusBottom, height, 16, color);
        }
        
        inline void SSO_BeginMode3D(Camera3D camera) {
            BeginMode3D(camera);
        }
        
        inline void SSO_EndMode3D() {
            EndMode3D();
        }
        
        inline void SSO_UpdateCamera(Camera3D* camera, int mode) {
            UpdateCamera(camera, mode);
        }
        
        inline void SSO_DrawGrid(int slices, float spacing) {
            DrawGrid(slices, spacing);
        }
        
        inline Model3D* LoadModel3D(const std::string& name, const char* modelPath, const char* texturePath = nullptr) {
            Model3D model3D;
            
            model3D.loaded = true;
            
            if (texturePath) {
                model3D.texture = LoadTexture(texturePath);
                model3D.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = model3D.texture;
            }
            
            models[name] = model3D;
            return &models[name];
        }
        
        inline Model3D* LoadModel3DFromBundle(const std::string& name, const char* bundlePath, const char* modelPath, const char* texturePath = nullptr) {
            Model3D model3D;
            
            model3D.loaded = true;
            
            if (texturePath) {
                int dataSize;
                unsigned char* textureData = SSO::Provider::LoadRawDataFromBundle(bundlePath, texturePath, &dataSize);
                if (textureData) {
                    Image img = LoadImageFromMemory(".png", textureData, dataSize);
                    model3D.texture = LoadTextureFromImage(img);
                    UnloadImage(img);
                    model3D.model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = model3D.texture;
                }
            }
            
            models[name] = model3D;
            return &models[name];
        }
        
        inline Model3D* GetModel3D(const std::string& name) {
            auto it = models.find(name);
            if (it != models.end()) {
                return &it->second;
            }
            return nullptr;
        }
        
        inline void DrawModel3D(const std::string& name) {
            Model3D* model3D = GetModel3D(name);
            if (model3D && model3D->loaded) {
                DrawModel(model3D->model, model3D->position, 1.0f, model3D->tint);
            }
        }
        
        inline void DrawModel3DEx(const std::string& name, Vector3 position, Vector3 rotation, Vector3 scale, Color tint) {
            Model3D* model3D = GetModel3D(name);
            if (model3D && model3D->loaded) {
                Matrix transform = MatrixMultiply(
                    MatrixMultiply(
                        MatrixScale(scale.x, scale.y, scale.z),
                        MatrixRotateXYZ(rotation)
                    ),
                    MatrixTranslate(position.x, position.y, position.z)
                );
                DrawModel(model3D->model, position, 1.0f, tint);
            }
        }
        
        inline Shader3D* LoadShader3D(const std::string& name, const char* vsPath, const char* fsPath) {
            Shader3D shader3D;
            shader3D.shader = LoadShader(vsPath, fsPath);
            shader3D.loaded = shader3D.shader.id > 0;
            shader3D.name = name;
            
            shaders[name] = shader3D;
            return &shaders[name];
        }
        
        inline Shader3D* GetShader3D(const std::string& name) {
            auto it = shaders.find(name);
            if (it != shaders.end()) {
                return &it->second;
            }
            return nullptr;
        }
        
        inline void ApplyShader3D(const std::string& name) {
            Shader3D* shader3D = GetShader3D(name);
            if (shader3D && shader3D->loaded) {
                BeginShaderMode(shader3D->shader);
            }
        }
        
        inline void EndShader3D() {
            EndShaderMode();
        }
        
        inline void Cleanup3D() {
            for (auto& pair : models) {
                if (pair.second.loaded) {
                    UnloadModel(pair.second.model);
                    if (pair.second.texture.id > 0) {
                        UnloadTexture(pair.second.texture);
                    }
                }
            }
            models.clear();
            
            for (auto& pair : shaders) {
                if (pair.second.loaded) {
                    UnloadShader(pair.second.shader);
                }
            }
            shaders.clear();
        }
    }
}

#endif
