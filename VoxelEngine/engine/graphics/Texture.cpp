#include "Texture.h"

using namespace VoxelEngine;

Texture::Texture() {
    mTexture = nullptr;
}

Texture::Texture(const Texture& tex) {
}

Texture::~Texture() {
}

bool Texture::Initialize(ID3D11Device* device, WCHAR* filename) {
    HRESULT result;

    result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &mTexture, NULL);
    if (FAILED(result)) {
        return false;
    }
    return true;
}

void Texture::Shutdown() {
    if (mTexture) {
        mTexture->Release();
        mTexture = nullptr;
    }
}

ID3D11ShaderResourceView* Texture::GetTexture() {
    return mTexture;
}