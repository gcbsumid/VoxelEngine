#include "Engine.h"

namespace VoxelEngine {
    Engine::Engine(UINT width, UINT height)
        : mHeight(height)
        , mWidth(width) {

    }

    Engine::~Engine() {
        CleanD3D();
    }

    void Engine::Init(HWND hwnd) {
        ZeroMemory(&mScd, sizeof(DXGI_SWAP_CHAIN_DESC));

        mScd.BufferCount = 1;                                   // One back buffer
        mScd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32 bit color
        mScd.BufferDesc.Width = mWidth;
        mScd.BufferDesc.Height = mHeight;
        mScd.   BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
        mScd.OutputWindow = hwnd;                            // window to be used
        mScd.SampleDesc.Count = 4;                              // how many multisamples
        mScd.Windowed = TRUE;                                   // Windowed/Full screen mode
        mScd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // Allow full-screen switching

        D3D11CreateDeviceAndSwapChain(NULL,
                                      D3D_DRIVER_TYPE_HARDWARE,
                                      NULL,
                                      NULL,
                                      NULL,
                                      NULL,
                                      D3D11_SDK_VERSION,
                                      &mScd,
                                      &mSwapChain,
                                      &mDevice,
                                      NULL,
                                      &mDevContext);

        // Get address of the back buffer
        ID3D11Texture2D *pBackBuffer;
        mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

        // use the back buffer address to create the render target 
        mDevice->CreateRenderTargetView(pBackBuffer, NULL, &mBackBuffer);
        pBackBuffer->Release();

        // set the render target as the back buffer
        mDevContext->OMSetRenderTargets(1, &mBackBuffer, NULL);

        // Set the viewport
        ZeroMemory(&mViewport, sizeof(D3D11_VIEWPORT));

        mViewport.TopLeftX = 0;
        mViewport.TopLeftY = 0;
        mViewport.Height = (float)mHeight;
        mViewport.Width = (float)mWidth;

        mDevContext->RSSetViewports(1, &mViewport);
    }

    void Engine::CleanD3D() {
        mSwapChain->SetFullscreenState(FALSE, NULL);

        mVertexShader->Release();
        mPixelShader->Release();
        mBuffer->Release();

        mSwapChain->Release();
        mBackBuffer->Release();
        mDevice->Release();
        mDevContext->Release();
    }

    void Engine::Render() {
        // Clear the color
        mDevContext->ClearRenderTargetView(mBackBuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

        // Do rendering here

        // switch the back buffer to the front
        mSwapChain->Present(0, 0);  
    }

    void Engine::InitPipeline() {
        // load and compile shaders
        ID3D10Blob *vshader, *pshader;
        D3DX11CompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &vshader, 0, 0);
        D3DX11CompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &pshader, 0, 0);

        // Create shader objects
        mDevice->CreateVertexShader(vshader->GetBufferPointer(), vshader->GetBufferSize(), NULL, &mVertexShader);
        mDevice->CreatePixelShader(pshader->GetBufferPointer(), pshader->GetBufferSize(), NULL, &mPixelShader);

        // set the shader objects
        mDevContext->VSSetShader(mVertexShader, 0, 0);
        mDevContext->PSSetShader(mPixelShader, 0, 0);
    }

    void Engine::InitGraphics() {
        // Creating a triangle
        Vertex outVerts[] =
        {
            { 0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
            { 0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
            { -0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) }
        };

        // Create the buffer
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DYNAMIC;             // Write access by gpu & cpu
        bd.ByteWidth = sizeof(Vertex) * 3;          // size of our buffer
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;    // used as a vertex buffer
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // allow cpu to write to the buffer

        mDevice->CreateBuffer(&bd, NULL, &mBuffer);

        // Fill in the vertex buffer
        D3D11_MAPPED_SUBRESOURCE ms;
        mDevContext->Map(mBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, outVerts, sizeof(outVerts));
        mDevContext->Unmap(mBuffer, NULL);
    }
}