#include "D3DClass.h"

using namespace VoxelEngine;

D3DClass::D3DClass() {
    mSwapChain = nullptr;
    mDevice = nullptr;
    mDeviceContext = nullptr;
    mRenderTargetView = nullptr;
    mDepthStencilBuffer = nullptr;
    mDepthStencilState = nullptr;
    mDepthStencilView = nullptr;
    mRasterState = nullptr;
}

D3DClass::D3DClass(const D3DClass& other) {}

D3DClass::~D3DClass() {}

bool D3DClass::Initialize(int width,
                          int height,
                          bool isVsyncEnabled,
                          HWND hwnd,
                          bool fullScreen,
                          float screenDepth,
                          float screenNear) {
    HRESULT result;
    IDXGIFactory* factory;
    IDXGIAdapter* adapter;
    IDXGIOutput* adapterOutput;
    unsigned int numModes, numerator, denominator, strLength;
    DXGI_MODE_DESC* displayModeList;
    DXGI_ADAPTER_DESC adapterDesc;

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    D3D_FEATURE_LEVEL featureLevel;
    ID3D11Texture2D* backBufferPtr;

    // Set up the depth buffer description
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

    D3D11_RASTERIZER_DESC rasterDesc;
    D3D11_VIEWPORT viewport;
    float fieldOfView, screenAspect;
    int error;

    // Store Vsync Setting
    mVsyncEnabled = isVsyncEnabled;

    // Create a DirectX Graphics Interface Factory
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if (FAILED(result)) {
        return false;
    }

    // create an adapter for he primary graphics interface (video card)
    result = factory->EnumAdapters(0, &adapter);
    if (FAILED(result)) {
        return false;
    }

    // enumerate primary adapter output (monitor)
    result = adapter->EnumOutputs(0, &adapterOutput);
    if (FAILED(result)) {
        return false;
    }

    // Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format
    // for the adapter output (monitor)
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
    if (FAILED(result)) {
        return false;
    }

    // create a list to hold all the possilble display modes for this monitor/video 
    // card combination
    displayModeList = new DXGI_MODE_DESC[numModes];
    if (!displayModeList) {
        return false;
    }

    // Now fill the display mode list structures.
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
    if (FAILED(result)) {
        return false;
    }

    // Now go through all the display modes and find the one that matches the screen 
    // width and height. When a match is found store the numerator and denominator of 
    // the refresh rate for that monitor.
    for (unsigned int i = 0; i < numModes; i++) {
        if ((displayModeList[i].Width == (unsigned int)width) &&
            (displayModeList[i].Height == (unsigned int)height)) {
            numerator = displayModeList[i].RefreshRate.Numerator;
            denominator = displayModeList[i].RefreshRate.Denominator;
        }
    }

    // Get the adapter (video card) description.
    result = adapter->GetDesc(&adapterDesc);
    if (FAILED(result)) {
        return false;
    }

    // Store the dedicated video card memory in megabytes.
    mVideoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

    // Convert the name of the video card to a character array and store it.
    error = wcstombs_s(&strLength, mVideoCardDescription, 128, adapterDesc.Description, 128);
    if (error != 0) {
        return false;
    }

    // Clean up
    delete[] displayModeList;
    adapterOutput->Release();
    adapter->Release();
    factory->Release();
    adapterOutput = nullptr;
    adapter = nullptr;
    factory = nullptr;
    displayModeList = nullptr;

    // Initialize the swap chain desc
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    if (mVsyncEnabled) {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
    } else {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }

    // set the usage of the backbuffer
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    if (fullScreen) {
        swapChainDesc.Windowed = false;
    } else {
        swapChainDesc.Windowed = true;
    }

    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // Discard back buffer contents after presenting
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = 0;

    featureLevel = D3D_FEATURE_LEVEL_11_0;

    // Create swapchain, device, and device context
    result = D3D11CreateDeviceAndSwapChain(NULL,
                                           D3D_DRIVER_TYPE_HARDWARE,
                                           NULL, 0,
                                           &featureLevel,
                                           1, D3D11_SDK_VERSION,
                                           &swapChainDesc,
                                           &mSwapChain,
                                           &mDevice,
                                           NULL,
                                           &mDeviceContext);
    if (FAILED(result)) {
        return false;
    }

    // Get pointer to the back buffer
    result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
    if (FAILED(result)) {
        return false;
    }

    // Create the render target view with the back buffer pointer
    result = mDevice->CreateRenderTargetView(backBufferPtr, NULL, &mRenderTargetView);
    if (FAILED(result)) {
        return false;
    }

    // Release the pointer to the back buffer
    backBufferPtr->Release();
    backBufferPtr = nullptr;

    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    // Set up depth buffer description
    depthBufferDesc.Width = width;
    depthBufferDesc.Height = height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // Create the texture for the depth buffer
    result = mDevice->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilBuffer);
    if (FAILED(result)) {
        return false;
    }

    // Set up depth stencil description
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    
    // Set up stencil state description
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil Operations if pixel is front-facing
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Create depth stencil state
    result = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
    if (FAILED(result)) {
        return false;
    }

    // Set the depth stencil state
    mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

    // Initialize Depth Stencil View
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // create the depth stencil view
    result = mDevice->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
    if (FAILED(result)) {
        return false;
    }

    // Bind the render target view and the depth stencil buffer 
    // to the output render pipeline
    mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    // Create the rasterizer state from the description we just 
    // filled out
    result = mDevice->CreateRasterizerState(&rasterDesc, &mRasterState);
    if (FAILED(result)) {
        return false;
    }

    // Now set the rasterizer state
    mDeviceContext->RSSetState(mRasterState);

    // Set up the viewport for rendering
    viewport.Width = (float)width;
    viewport.Height = (float)height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    // Create the viewport
    mDeviceContext->RSSetViewports(1, &viewport);

    // Set up the projection matrix
    fieldOfView = (float)D3DX_PI / 4.0f;
    screenAspect = (float)width / (float)height;
    
    // create the projection matrix 
    D3DXMatrixPerspectiveFovLH(&mProjectionMatrix, 
                               fieldOfView, 
                               screenAspect, 
                               screenNear, 
                               screenDepth);

    // Initialize the world matrix to the identity matrix
    D3DXMatrixIdentity(&mWorldMatrix);

    // Create an orthgraphic projection matrix for 2D rendering
    D3DXMatrixOrthoLH(&mOrthoMatrix,
                      (float)width,
                      (float)height,
                      screenNear,
                      screenDepth);

    return true;
}

void D3DClass::Shutdown() {
    // Before Shutting down, set to windowed mode (otherwise, 
    // releasing the swapchain will throw an exception
    if (mSwapChain) {
        mSwapChain->SetFullscreenState(false, NULL);
    }

    if (mRasterState) {
        mRasterState->Release();
        mRasterState = nullptr;
    }

    if (mDepthStencilView) {
        mDepthStencilView->Release();
        mDepthStencilView = nullptr;
    }

    if (mDepthStencilState) {
        mDepthStencilState->Release();
        mDepthStencilState = nullptr;
    }

    if (mDepthStencilBuffer) {
        mDepthStencilBuffer->Release();
        mDepthStencilBuffer = nullptr;
    }

    if (mRenderTargetView) {
        mRenderTargetView->Release();
        mRenderTargetView = nullptr;
    }

    if (mDeviceContext) {
        mDeviceContext->Release();
        mDeviceContext = nullptr;
    }

    if (mDevice) {
        mDevice->Release();
        mDevice = nullptr;
    }

    if (mSwapChain) {
        mSwapChain->Release();
        mSwapChain = nullptr;
    }
}

void D3DClass::BeginScene(float r, float g, float b, float alpha) {
    float color[4];

    // Setup the color to clear the buffer
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = alpha;

    // Clear the back buffer.
    mDeviceContext->ClearRenderTargetView(mRenderTargetView, color);

    // Clear the depth buffer.
    mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DClass::EndScene() {
    if (mVsyncEnabled) {
        // Locked to the screen refresh rate
        mSwapChain->Present(1, 0);
    } else {
        // Present as fast as possible
        mSwapChain->Present(0, 0);
    }
}

ID3D11Device* D3DClass::GetDevice() {
    return mDevice;
}


ID3D11DeviceContext* D3DClass::GetDeviceContext() {
    return mDeviceContext;
}

void D3DClass::GetProjectionMatrix(D3DXMATRIX& projectionMatrix) {
    projectionMatrix = mProjectionMatrix;
}


void D3DClass::GetWorldMatrix(D3DXMATRIX& worldMatrix) {
    worldMatrix = mWorldMatrix;
}


void D3DClass::GetOrthoMatrix(D3DXMATRIX& orthoMatrix) {
    orthoMatrix = mOrthoMatrix;
}

void D3DClass::GetVideoGardInfo(char* cardName, int& memory) {
    strcpy_s(cardName, 128, mVideoCardDescription);
    memory = mVideoCardMemory;
}