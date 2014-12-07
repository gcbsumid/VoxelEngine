#include "GraphicsSystem.h"

using namespace VoxelEngine;

GraphicsSystem::GraphicsSystem(bool isFullScreen)
    : mFullScreen(isFullScreen)
    , mCamera(nullptr)
    , mModel(nullptr)
    , mLightShader(nullptr)
    , mLight(nullptr) {

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
        MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
        return false;
    }

    mCamera = new Camera();
    if (!mCamera) return false;
    mCamera->SetPosition(0.0f, 0.0f, -5.0f);

    mModel = new Model();
    if (!mModel) return false;
    if (!mModel->Initialize(mD3DClass->GetDevice(), L"data/seafloor.dds")) {
        MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }

    mLightShader = new LightShader();
    if (!mLightShader) return false;
    if (!mLightShader->Initialize(mD3DClass->GetDevice(), hwnd)) {
        MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
    }

    mLight = new Light();
    if (!mLight) return false;
    mLight->SetDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);
    mLight->SetDirection(0.0f, 0.0f, 1.0f);

    return true;
}

void GraphicsSystem::Shutdown() {
    if (mLight) {
        delete mLight;
        mLight = nullptr;
    }

    if (mLightShader) {
        mLightShader->Shutdown();
        delete mLightShader;
        mLightShader = nullptr;
    }

    if (mModel) {
        mModel->Shutdown();
        delete mModel;
        mModel = nullptr;
    }

    if (mCamera) {
        delete mCamera;
        mCamera = nullptr;
    }
    if (mD3DClass) {
        mD3DClass->Shutdown();
        delete mD3DClass;
    }
}

bool GraphicsSystem::ProcessFrame() {

    static float rotation = 0.0f;

    rotation += (float)D3DX_PI * 0.01f;

    if (rotation > 360.0f) {
        rotation -= 360.0f;
    }

    return Render(rotation);
}
    
bool GraphicsSystem::Render(float rotation) {
    D3DXMATRIX world, view, projection;

    // Clear the buffers to begin the scene
    mD3DClass->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    // Generate a view matrix
    mCamera->Render();
    
    // Get Matrices
    mCamera->GetViewMatrix(view);
    mD3DClass->GetWorldMatrix(world);
    mD3DClass->GetProjectionMatrix(projection);

    // rotate the world matrix so the triangle will spin
    D3DXMatrixRotationY(&world, rotation);

    // Put the model and vertex indices on the buffers on the graphics
    // pipeline to prepare them for drawing 
    mModel->Render(mD3DClass->GetDeviceContext());
    
    if (!mLightShader->Render(mD3DClass->GetDeviceContext(),
        mModel->GetIndexCount(),
        world, view, projection, 
        mModel->GetTexture(), 
        mLight->GetDirection(),
        mLight->GetDiffuseColor())) {
        return false;
    }

    // Present the rendered scene to the screen
    mD3DClass->EndScene();

    return true;
}