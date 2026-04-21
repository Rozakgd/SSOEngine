#pragma once
#include "tools/sso_window.h"
#include "tools/sso_ui.h"
#include "tools/sso_ph3d.h"
#include "tools/sso_3d.h"
#include "tools/sso_text.h"
#include <fstream>
#include <string>

struct SceneEditor {
    Camera3D camera;
    SSO::Physics3D::RigidBody* selectedBody;
    
    SceneEditor() {
        camera = {{10, 10, 10}, {0, 0, 0}, {0, 1, 0}, 45.0f, CAMERA_ORBITAL};
        selectedBody = nullptr;
    }
};

class EditorController {
public:
    SceneEditor editor;
    
    void Init();
    void Update();
    void SaveSceneToJSON();
    void LoadSceneFromJSON();
};
