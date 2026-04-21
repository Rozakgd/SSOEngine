#include "sso_editor.h"

void EditorController::Init() {
    SSO::Physics3D::Init();
    SSO::Physics3D::CreateStaticBody({0, -1, 0}, {50, 1, 50}, GRAY);
}

void EditorController::Update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Ray ray = GetMouseRay(GetMousePosition(), editor.camera);
        float closestDistance = 1000.0f;
        SSO::Physics3D::RigidBody* closestBody = nullptr;
        
        for (auto& body : SSO::Physics3D::world.bodies) {
            if (!body.isStatic) {
                Vector3 center = body.position;
                float distance = Vector3Distance(ray.position, center);
                
                if (distance < closestDistance && distance < 10.0f) {
                    closestDistance = distance;
                    closestBody = &body;
                }
            }
        }
        
        editor.selectedBody = closestBody;
    }
    
    if (IsKeyPressed(KEY_SPACE)) {
        Vector3 spawnPos = {
            editor.camera.target.x + (float)GetRandomValue(-100, 100) / 100.0f,
            editor.camera.target.y,
            editor.camera.target.z + (float)GetRandomValue(-100, 100) / 100.0f
        };
        
        Color randomColor = {
            (unsigned char)GetRandomValue(100, 255),
            (unsigned char)GetRandomValue(100, 255),
            (unsigned char)GetRandomValue(100, 255),
            255
        };
        
        SSO::Physics3D::RigidBody* newBody = SSO::Physics3D::CreateBody(spawnPos, {1, 1, 1}, randomColor, 1.0f);
        newBody->isStatic = false;
        newBody->isActive = true;
        newBody->color = randomColor;
    }
    
    if (IsKeyPressed(KEY_S)) {
        SaveSceneToJSON();
    }
    
    if (IsKeyPressed(KEY_L)) {
        LoadSceneFromJSON();
    }
    
    if (IsKeyPressed(KEY_BACKSPACE)) {
        for (auto it = SSO::Physics3D::world.bodies.begin(); it != SSO::Physics3D::world.bodies.end(); ++it) {
            if (!it->isStatic) {
                it = SSO::Physics3D::world.bodies.erase(it);
                if (it == SSO::Physics3D::world.bodies.end()) break;
            }
        }
        editor.selectedBody = nullptr;
    }
    
    SSO::ThreeD::SSO_UpdateCamera(&editor.camera, CAMERA_ORBITAL);
}

void EditorController::SaveSceneToJSON() {
    std::ofstream file("scene.json");
    if (file.is_open()) {
        file << "{\n";
        file << "  \"objects\": [\n";
        
        bool first = true;
        for (const auto& body : SSO::Physics3D::world.bodies) {
            if (!body.isStatic) {
                if (!first) file << ",\n";
                file << "    {\n";
                file << "      \"position\": [" << body.position.x << ", " << body.position.y << ", " << body.position.z << "],\n";
                file << "      \"size\": [" << body.size.x << ", " << body.size.y << ", " << body.size.z << "],\n";
                file << "      \"color\": [" << (int)body.color.r << ", " << (int)body.color.g << ", " << (int)body.color.b << ", " << (int)body.color.a << "]\n";
                file << "    }";
                first = false;
            }
        }
        
        file << "\n  ]\n";
        file << "}\n";
        file.close();
    }
}

void EditorController::LoadSceneFromJSON() {
    std::ifstream file("scene.json");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("\"position\"") != std::string::npos) {
                size_t start = line.find("[");
                size_t end = line.find("]");
                if (start != std::string::npos && end != std::string::npos) {
                    std::string posStr = line.substr(start + 1, end - start - 1);
                    float x = 0, y = 0, z = 0;
                    sscanf(posStr.c_str(), "%f, %f, %f", &x, &y, &z);
                    
                    std::getline(file, line);
                    start = line.find("[");
                    end = line.find("]");
                    std::string sizeStr = line.substr(start + 1, end - start - 1);
                    float sx = 1, sy = 1, sz = 1;
                    sscanf(sizeStr.c_str(), "%f, %f, %f", &sx, &sy, &sz);
                    
                    std::getline(file, line);
                    start = line.find("[");
                    end = line.find("]");
                    std::string colorStr = line.substr(start + 1, end - start - 1);
                    int r = 255, g = 255, b = 255, a = 255;
                    sscanf(colorStr.c_str(), "%d, %d, %d, %d", &r, &g, &b, &a);
                    
                    SSO::Physics3D::RigidBody* newBody = SSO::Physics3D::CreateBody({x, y, z}, {sx, sy, sz}, {(unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a}, 1.0f);
                    newBody->isStatic = false;
                    newBody->isActive = true;
                    newBody->color = {(unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a};
                }
            }
        }
        file.close();
    }
}
