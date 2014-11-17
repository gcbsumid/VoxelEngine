#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include "D3DClass.h"

namespace VoxelEngine {
    class GraphicsSystem {
    public:
        GraphicsSystem(bool isFullScreen);
        ~GraphicsSystem();

        bool Initialize(HWND hwnd, int width, int height);
        void Shutdown();
        bool ProcessFrame();

    private:
        void InitPipeline();
        void InitGraphics();
        bool Render();
        
        int mWidth;
        int mHeight;

        bool mFullScreen;
        const bool VsyncEnable = true;
        const float ScreenDepth = 1000.0f;
        const float ScreenNear = 0.1f;

        D3DClass* mD3DClass;
    };
}

#endif