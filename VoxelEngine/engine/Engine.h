#ifndef ENGINE_H
#define ENGINE_H

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

    class Engine {
    public:
        Engine(UINT width, UINT height);
        ~Engine();

        void Init(HWND hwnd);
        void Render();

    private:
        void CleanD3D();
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
    };
}

#endif