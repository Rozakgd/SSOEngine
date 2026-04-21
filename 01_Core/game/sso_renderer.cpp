#include "sso_renderer.h"

void EditorRenderer::DrawScene(SceneEditor& editor) {
    SSO::ThreeD::SSO_BeginMode3D(editor.camera);
    SSO::ThreeD::SSO_DrawGrid(20, 1.0f);
    
    for (auto& body : SSO::Physics3D::world.bodies) {
        Color drawColor = body.color;
        if (&body == editor.selectedBody) {
            drawColor = YELLOW;
        }
        SSO::ThreeD::SSO_DrawCube(body.position, body.size.x, drawColor);
        SSO::ThreeD::SSO_DrawCubeWires(body.position, body.size.x, BLACK);
    }
    
    SSO::ThreeD::SSO_EndMode3D();
}

void EditorRenderer::DrawEditorUI(SceneEditor& editor, Font gameFont) {
    DrawRectangle(10, 10, 300, 120, {30, 30, 35, 200});
    DrawRectangleLines(10, 10, 300, 120, {70, 70, 80, 255});
    SSO::UI::DrawTextShadow(gameFont, "SCENE EDITOR", {20, 30}, 20, 1, WHITE);
    SSO::UI::DrawTextShadow(gameFont, "SPACE: Spawn | S: Save | L: Load", {20, 55}, 16, 1, GRAY);
    SSO::UI::DrawTextShadow(gameFont, "Click: Select | BACKSPACE: Clear", {20, 75}, 16, 1, GRAY);
    SSO::UI::DrawTextShadow(gameFont, "Mouse drag: Rotate camera", {20, 95}, 16, 1, GRAY);
    
    if (editor.selectedBody) {
        DrawRectangle(970, 10, 300, 120, {30, 30, 35, 200});
        DrawRectangleLines(970, 10, 300, 120, {70, 70, 80, 255});
        SSO::UI::DrawTextShadow(gameFont, "OBJECT INSPECTOR", {980, 30}, 20, 1, WHITE);
        SSO::UI::DrawTextShadow(gameFont, "Type: RigidBody", {980, 55}, 16, 1, GRAY);
        SSO::UI::DrawTextShadow(gameFont, TextFormat("Pos: [%.1f, %.1f, %.1f]", 
            editor.selectedBody->position.x, 
            editor.selectedBody->position.y, 
            editor.selectedBody->position.z), {980, 75}, 16, 1, GRAY);
        SSO::UI::DrawTextShadow(gameFont, TextFormat("Mass: %.1f", editor.selectedBody->mass), {980, 95}, 16, 1, GRAY);
    }
    
    int objectCount = 0;
    for (const auto& body : SSO::Physics3D::world.bodies) {
        if (!body.isStatic) objectCount++;
    }
    
    SSO::UI::DrawTextShadow(gameFont, TextFormat("Objects: %d", objectCount), {10, 680}, 16, 1, WHITE);
    
    SSO::Text::DrawHyperlink("GitHub: Rozaq125", 1100, 680, 14, BLUE, "https://github.com/Rozaq125/SSOEngine");
}
