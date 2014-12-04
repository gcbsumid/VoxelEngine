#ifndef D3D_CLASS_H
#define D3D_CLASS_H

#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#pragma warning( disable : 4005 )
#include <DXGI.h>
#include <D3Dcommon.h>
#include <D3D11.h>
#include <D3DX10math.h>
//#include <d3d11.h>
//#include <d3dx11.h>
//#include <d3dx10.h>
#pragma warning( default : 4005 )

namespace VoxelEngine {
    class D3DClass {
    public :
        D3DClass();
        D3DClass(const D3DClass&);
        virtual ~D3DClass();

        bool Initialize(int width,
                        int height,
                        bool isVsyncEnabled,
                        HWND hwnd,
                        bool fullScreen,
                        float screenDepth,
                        float screenNear);

        void Shutdown();

        void BeginScene(float r, float g, float b, float alpha);
        void EndScene();

        // Helper Functions I think
        ID3D11Device* GetDevice();
        ID3D11DeviceContext* GetDeviceContext();

        void GetProjectionMatrix(D3DXMATRIX& projectionMatrix);
        void GetWorldMatrix(D3DXMATRIX& worldMatrix);
        void GetOrthoMatrix(D3DXMATRIX& orthoMatrix);

        void GetVideoGardInfo(char*, int&);

    private:
        bool mVsyncEnabled;
        int mVideoCardMemory;
        char mVideoCardDescription[128];

        // DirectX Interfaces
        IDXGISwapChain* mSwapChain;         // Swap Chain Interface
        ID3D11Device* mDevice;              // Direct3D device interface
        ID3D11DeviceContext* mDeviceContext;   // Direct3D device context
        ID3D11RenderTargetView* mRenderTargetView;
        ID3D11Texture2D* mDepthStencilBuffer;
        ID3D11DepthStencilState* mDepthStencilState;
        ID3D11DepthStencilView* mDepthStencilView;
        ID3D11RasterizerState* mRasterState;

        // Matrices
        D3DXMATRIX mProjectionMatrix;
        D3DXMATRIX mWorldMatrix;
        D3DXMATRIX mOrthoMatrix;

        //D3D11_VIEWPORT mViewport;

        //// Struct to hold information about the swap chain
        //DXGI_SWAP_CHAIN_DESC mScd;

        //// Shader Object
        //ID3D11VertexShader* mVertexShader;
        //ID3D11PixelShader* mPixelShader;

        //// Buffer Object
        //ID3D11Buffer* mBuffer;

        //// Input Layout Object
        //ID3D11InputLayout* mInputLayout;
    };
}

#endif