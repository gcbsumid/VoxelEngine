#include "GraphicsSystem.h"

namespace VoxelEngine {
    GraphicsSystem::GraphicsSystem(bool isFullScreen)
        : mFullScreen(isFullScreen) {

    }

    GraphicsSystem::~GraphicsSystem() {

    }

    bool GraphicsSystem::Initialize(HWND hwnd, int width, int height) {
        mD3DClass = new D3DClass();
        if (!mD3DClass) {
            return false;
        }

        if (!mD3DClass->Initialize(width,
            height,
            VsyncEnable,
            hwnd,
            mFullScreen,
            ScreenDepth,
            ScreenNear)) {
            return false;
        }

        return true;
    }

    void GraphicsSystem::Shutdown() {
        if (mD3DClass) {
            mD3DClass->Shutdown();
            delete mD3DClass;
        }
    }

    bool GraphicsSystem::ProcessFrame() {
        return Render();
    }
    
    bool GraphicsSystem::Render() {
        // Clear the buffers to begin the scene
        mD3DClass->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);


        // Present the rendered scene to the screen
        mD3DClass->EndScene();

        return true;
    }
}