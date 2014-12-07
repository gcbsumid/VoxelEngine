#include "LightShader.h"

using namespace VoxelEngine;

LightShader::LightShader() {
    mVertexShader = nullptr;
    mPixelShader = nullptr;
    mInputLayout = nullptr;
    mMatrixBuffer = nullptr;
    mSamplerState = nullptr;
}

LightShader::LightShader(const LightShader& shader) {

}

LightShader::~LightShader() {
}

bool LightShader::Initialize(ID3D11Device* device, HWND hwnd) {
    return InitializeShader(device, hwnd, L"shaders/LightVS.hlsl", L"shaders/LightPS.hlsl");
}

void LightShader::Shutdown() {
    ShutdownShader();
}

bool LightShader::Render(ID3D11DeviceContext* deviceContext, 
                         int indexCount, 
                         D3DXMATRIX world, 
                         D3DXMATRIX view, 
                         D3DXMATRIX projection, 
                         ID3D11ShaderResourceView* texture, 
                         D3DXVECTOR3 lightDirection,
                         D3DXVECTOR4 diffuseColor) {
    if (!SetShaderParameters(deviceContext, world, view, projection, texture, lightDirection, diffuseColor)) {
        return false;
    }

    RenderShader(deviceContext, indexCount);
    return true;
}

bool LightShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) {
    HRESULT result;
    ID3D10Blob* errorMessage;
    ID3D10Blob* vertexShaderBuffer;
    ID3D10Blob* pixelShaderBuffer;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
    unsigned int numElements;
    D3D11_BUFFER_DESC matrixBufferDesc;
    D3D11_SAMPLER_DESC samplerDesc;
    D3D11_BUFFER_DESC lightBufferDesc;

    // Compile Shaders
    result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);

    if (FAILED(result)) {
        if (errorMessage) {
            OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
        } else {
            MessageBox(hwnd, vsFilename, L"Missing Vertex Shader File", MB_OK);
        }
        return false;
    }

    result = D3DX11CompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);

    if (FAILED(result)) {
        if (errorMessage) {
            OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
        } else {
            MessageBox(hwnd, psFilename, L"Missing Pixel Shader File", MB_OK);
        }
    }

    // Create shaders from buffers
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &mVertexShader);
    if (FAILED(result)) {
        return false;
    }

    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &mPixelShader);
    if (FAILED(result)) {
        return false;
    }

    // Create Vertex Input Layout
    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticName = "TEXCOORD";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

    polygonLayout[2].SemanticName = "NORMAL";
    polygonLayout[2].SemanticIndex = 0;
    polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[2].InputSlot = 0;
    polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[2].InstanceDataStepRate = 0;

    numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &mInputLayout);

    if (FAILED(result)) {
        return false;
    }

    // Release buffer shaders
    vertexShaderBuffer->Release();
    pixelShaderBuffer->Release();
    vertexShaderBuffer = nullptr;
    pixelShaderBuffer = nullptr;

    // Set up the description of the matrix buffers
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    // Create the buffer
    result = device->CreateBuffer(&matrixBufferDesc, NULL, &mMatrixBuffer);
    if (FAILED(result)) {
        return false;
    }

    // Create a texture sampler
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    result = device->CreateSamplerState(&samplerDesc, &mSamplerState);
    if (FAILED(result)) {
        return false;
    }

    lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightBufferDesc.ByteWidth = sizeof(LightBufferType);
    lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightBufferDesc.MiscFlags = 0;
    lightBufferDesc.StructureByteStride = 0;

    result = device->CreateBuffer(&lightBufferDesc, NULL, &mLightBuffer);
    if (FAILED(result)) {
        return false;
    }

    return true;
}

void LightShader::ShutdownShader() {
    if (mLightBuffer) {
        mLightBuffer->Release();
        mLightBuffer = nullptr;
    }

    if (mSamplerState) {
        mSamplerState->Release();
        mSamplerState = nullptr;
    }

    if (mMatrixBuffer) {
        mMatrixBuffer->Release();
        mMatrixBuffer = nullptr;
    }

    if (mInputLayout) {
        mInputLayout->Release();
        mInputLayout = nullptr;
    }

    if (mVertexShader) {
        mVertexShader->Release();
        mVertexShader = nullptr;
    }

    if (mPixelShader) {
        mPixelShader->Release();
        mPixelShader = nullptr;
    }
}

void LightShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName) {
    char* compileErrors = (char*)(errorMessage->GetBufferPointer());
    unsigned long buffersize = errorMessage->GetBufferSize();

    std::ofstream fout;
    fout.open("shader-error.txt");

    for (unsigned int i = 0; i < buffersize; i++) {
        fout << compileErrors[i];
    }

    fout.close();

    errorMessage->Release();
    errorMessage = nullptr;

    MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFileName, MB_OK);
}

bool LightShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, 
                                      D3DXMATRIX world, 
                                      D3DXMATRIX view, 
                                      D3DXMATRIX projection, 
                                      ID3D11ShaderResourceView* texture,
                                      D3DXVECTOR3 lightDirection,
                                      D3DXVECTOR4 diffuseColor) {
    D3DXMatrixTranspose(&world, &world);
    D3DXMatrixTranspose(&view, &view);
    D3DXMatrixTranspose(&projection, &projection);

    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    result = deviceContext->Map(mMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result)) {
        return false;
    }

    MatrixBufferType* matrixDataPtr = (MatrixBufferType*)mappedResource.pData;

    matrixDataPtr->world = world;
    matrixDataPtr->view = view;
    matrixDataPtr->projection = projection;

    deviceContext->Unmap(mMatrixBuffer, 0);

    unsigned int bufferNum = 0;
    deviceContext->VSSetConstantBuffers(bufferNum, 1, &mMatrixBuffer);
    deviceContext->PSSetShaderResources(0, 1, &texture);

    result = deviceContext->Map(mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result)) {
        return false;
    }

    LightBufferType* lightDataPtr = (LightBufferType*)mappedResource.pData;
    
    lightDataPtr->diffuseColor = diffuseColor;
    lightDataPtr->lightDirection = lightDirection;
    lightDataPtr->padding = 0.0f;

    deviceContext->Unmap(mLightBuffer, 0);

    bufferNum = 0;
    deviceContext->PSSetConstantBuffers(bufferNum, 1, &mLightBuffer);
    return true;
}

void LightShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) {
    // set the vertex input layout
    deviceContext->IASetInputLayout(mInputLayout);

    // set shaders
    deviceContext->VSSetShader(mVertexShader, NULL, 0);
    deviceContext->PSSetShader(mPixelShader, NULL, 0);

    // Set the sampler state
    deviceContext->PSSetSamplers(0, 1, &mSamplerState);
    deviceContext->DrawIndexed(indexCount, 0, 0);
}