#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include "D3DClass.h"
//#include "../d3dclass.h"
#include "Camera.h"
#include "Model.h"
#include "ColorShader.h"
//#include "../cameraclass.h"
//#include "../modelclass.h"
//#include "../colorshaderclass.h"

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
        Camera* mCamera;
        Model* mModel;
        ColorShader* mColorShader;
        //CameraClass* mCamera;
        //ModelClass* mModel;
        //ColorShaderClass* mColorShader;
    };
}

#endif