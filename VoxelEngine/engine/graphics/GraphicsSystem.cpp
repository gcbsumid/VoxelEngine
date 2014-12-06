#include "GraphicsSystem.h"

using namespace VoxelEngine;
GraphicsSystem::GraphicsSystem(bool isFullScreen)
    : mFullScreen(isFullScreen)
    , mCamera(nullptr)
    , mModel(nullptr)
    , mTextureShader(nullptr) {

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
    mCamera->SetPosition(0.0f, 0.0f, -10.0f);

    mModel = new Model();
    if (!mModel) return false;
    if (!mModel->Initialize(mD3DClass->GetDevice(), L"data/seafloor.dds")) {
        MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }

    mTextureShader = new TextureShader();
    if (!mTextureShader) return false;
    if (!mTextureShader->Initialize(mD3DClass->GetDevice(), hwnd)) {
        MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
    }

    return true;
}

void GraphicsSystem::Shutdown() {
    if (mTextureShader) {
        mTextureShader->Shutdown();
        delete mTextureShader;
        mTextureShader = nullptr;
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
    return Render();
}
    
bool GraphicsSystem::Render() {
    D3DXMATRIX world, view, projection;

    // Clear the buffers to begin the scene
    mD3DClass->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    // Generate a view matrix
    mCamera->Render();
    
    // Get Matrices
    mCamera->GetViewMatrix(view);
    mD3DClass->GetWorldMatrix(world);
    mD3DClass->GetProjectionMatrix(projection);

    // Put the model and vertex indices on the buffers on the graphics
    // pipeline to prepare them for drawing 
    mModel->Render(mD3DClass->GetDeviceContext());
    
    if (!mTextureShader->Render(mD3DClass->GetDeviceContext(),
        mModel->GetIndexCount(),
        world, view, projection, 
        mModel->GetTexture())) {
        return false;
    }

    // Present the rendered scene to the screen
    mD3DClass->EndScene();

    return true;
}