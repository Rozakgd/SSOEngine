#pragma once
#include "tools/sso_window.h"
#include "tools/sso_ui.h"
#include "tools/sso_ph3d.h"
#include "tools/sso_3d.h"
#include "tools/sso_text.h"
#include "sso_editor.h"

class EditorRenderer {
public:
    void DrawScene(SceneEditor& editor);
    void DrawEditorUI(SceneEditor& editor, Font gameFont);
};
