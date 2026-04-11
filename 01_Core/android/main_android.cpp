#include <android/asset_manager.h>
#include <android_native_app_glue.h>

// Variable global untuk akses asset di Android
AAssetManager* androidAssetManager = nullptr;

// Helper function untuk asset manager access
extern "C" {
    void InitAndroidAssetManager(struct android_app* app) {
        if (app != nullptr && app->activity != nullptr) {
            androidAssetManager = app->activity->assetManager;
        }
    }
    
    // Export asset manager untuk digunakan oleh SSO::Provider
    AAssetManager* GetAndroidAssetManager() {
        return androidAssetManager;
    }
}

// Custom initialization function yang akan dipanggil dari main.cpp
void InitAndroidAsset() {
    // Asset manager sudah diinisialisasi oleh Raylib secara internal
    // Kita hanya perlu memastikan global variable ter-set
    // Raylib akan meng-handle android_main secara internal
}