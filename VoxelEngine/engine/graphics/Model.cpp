#include "Model.h"

using namespace VoxelEngine;

Model::Model() {
    mVertexBuffer = nullptr;
    mIndexBuffer = nullptr;
    mTexture = nullptr;
    mModel = nullptr;
}

Model::Model(const Model& model) {}

Model::~Model() {}

bool Model::Initialize(ID3D11Device* device, char* modelFileName, WCHAR* textureFileName) {
    if (!LoadModel(modelFileName)) {
        return false;
    }

    if (!InitializeBuffers(device)) {
        return false;
    }

    if (!LoadTexture(device, textureFileName)) {
        return false;
    }

    return true;
}

void Model::Shutdown() {
    ReleaseTexture();
    ShutdownBuffers();
    ReleaseModel();
}

void Model::Render(ID3D11DeviceContext* deviceContext) {
    RenderBuffers(deviceContext);
}

int Model::GetIndexCount() {
    return mIndexCount;
}

bool Model::InitializeBuffers(ID3D11Device* device) {
    VertexType* vertices;
    unsigned long* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    // Create the vertex array and index array
    vertices = new VertexType[mVertexCount];
    if (!vertices) return false;

    indices = new unsigned long[mIndexCount];
    if (!indices) return false;

    // Load the vertex array with data.
    // Load the index array with data.
    for (int i = 0; i < mVertexCount; i++) {
        vertices[i].position = D3DXVECTOR3(mModel[i].x, mModel[i].y, mModel[i].z);  // Bottom left.
        vertices[i].texCoord = D3DXVECTOR2(mModel[i].tu, mModel[i].tv);
        vertices[i].normal = D3DXVECTOR3(mModel[i].nx, mModel[i].ny, mModel[i].nz);

        indices[i] = i;
    }

    // Set up description of vertex buffer
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Give the subroutine structure a pointer to the data
    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // Create the buffer data
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &mVertexBuffer);
    if (FAILED(result)) return false;

    // Set up description for the index buffer
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Give the subroutine structure a pointer to the data
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch= 0;

    result = device->CreateBuffer(&indexBufferDesc, &indexData, &mIndexBuffer);
    if (FAILED(result)) return false;

    delete[] vertices;
    delete[] indices;
    vertices = nullptr;
    indices = nullptr;

    return true;
}

void Model::ShutdownBuffers() {
    if (mIndexBuffer) {
        mIndexBuffer->Release();
        mIndexBuffer = nullptr;
    }

    if (mVertexBuffer) {
        mVertexBuffer->Release();
        mVertexBuffer = nullptr;
    }
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext) {
    unsigned int stride = sizeof(VertexType);
    unsigned int offset = 0;

    // Set the vertex buffer as active
    deviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);

    // Set the index buffer as active
    deviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // set the type of primitive that should be rendered 
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

ID3D11ShaderResourceView* Model::GetTexture() {
    return mTexture->GetTexture();
}

bool Model::LoadTexture(ID3D11Device* device, WCHAR* textureFileName) {
    mTexture = new Texture();
    if (!mTexture) {
        return false;
    }

    return mTexture->Initialize(device, textureFileName);
}

void Model::ReleaseTexture() {
    if (mTexture) {
        mTexture->Shutdown();
        delete mTexture;
        mTexture = nullptr;
    }
}

bool Model::LoadModel(char* filename) {
    std::ifstream stream;

    stream.open(filename);

    if (stream.fail()) {
        return false;
    }

    char input;
    stream.get(input);
    while (input != ':') {
        stream.get(input);
    }

    stream >> mVertexCount;
    mIndexCount = mVertexCount;

    mModel = new ModelType[mVertexCount];
    if (!mModel) {
        return false;
    }

    stream.get(input);
    while (input != ':') {
        stream.get(input);
    }
    stream.get(input);
    stream.get(input);
    // Read in the vertex data.
    for (int i = 0; i < mVertexCount; i++) {
        stream >> mModel[i].x >> mModel[i].y >> mModel[i].z;
        stream >> mModel[i].tu >> mModel[i].tv;
        stream >> mModel[i].nx >> mModel[i].ny >> mModel[i].nz;
    }

    // Close the model file.
    stream.close();

    return true;
}

void Model::ReleaseModel() { 
    if (mModel) {
        delete[] mModel;
        mModel = nullptr;
    }
}