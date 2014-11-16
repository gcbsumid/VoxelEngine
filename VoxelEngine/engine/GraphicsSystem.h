#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

#pragma warning( disable : 4005 )
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#pragma warning( default : 4005 )

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "dxgi.lib")

namespace VoxelEngine {
    struct Vertex {
        float x, y, z;
        D3DXCOLOR color;
    };

    class GraphicsSystem {
    public:
        GraphicsSystem(int width, int height, bool isFullScreen);
        ~GraphicsSystem();

        bool Initialize(HWND hwnd);
        void Shutdown();
        bool ProcessFrame();

    private:
        void InitPipeline();
        void InitGraphics();
        
        UINT mWidth;
        UINT mHeight;

        IDXGISwapChain* mSwapChain;         // Swap Chain Interface
        ID3D11Device* mDevice;              // Direct3D device interface
        ID3D11DeviceContext* mDevContext;   // Direct3D device context
        ID3D11RenderTargetView* mBackBuffer;
        D3D11_VIEWPORT mViewport;

        // Struct to hold information about the swap chain
        DXGI_SWAP_CHAIN_DESC mScd;

        // Shader Object
        ID3D11VertexShader* mVertexShader;
        ID3D11PixelShader* mPixelShader;

        // Buffer Object
        ID3D11Buffer* mBuffer;

        // Input Layout Object
        ID3D11InputLayout* mInputLayout;

        bool mFullScreen;
        const bool VsyncEnable = true;
        const float ScreenDepth = 1000.0f;
        const float ScreenNear = 0.1f;
    };
}

#endif