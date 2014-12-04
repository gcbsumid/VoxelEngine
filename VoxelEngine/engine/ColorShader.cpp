#include "ColorShader.h"

using namespace VoxelEngine;

ColorShader::ColorShader() {
    mVertexShader = nullptr;
    mPixelShader = nullptr;
    mInputLayout = nullptr;
    mMatrixBuffer = nullptr;
}

ColorShader::ColorShader(const ColorShader& other) {}

ColorShader::~ColorShader() {}

bool ColorShader::Initialize(ID3D11Device* device, HWND hwnd) {
    bool result = false;

    // Initialize the Vertex and Pixel Shaders;
    result = InitializeShader(device, hwnd, L"engine/ColorVertexShader.hlsl", L"engine/ColorPixelShader.hlsl");
    if (!result) return false;
    return true;
}

void ColorShader::Shutdown() {
    ShutdownShader();
}

bool ColorShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DMATRIX world, D3DMATRIX view, D3DMATRIX projection) {
    if (!SetShaderParameters(deviceContext, world, view, projection))
        return false;

    RenderShader(deviceContext, indexCount);
    return true;
}

bool ColorShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) {
    HRESULT result;
    ID3D10Blob* errorMessage;
    ID3D10Blob* vertexShaderBuffer;
    ID3D10Blob* pixelShaderBuffer;
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    unsigned int numElements;
    D3D11_BUFFER_DESC matrixBufferDesc;

    // Initialize the pointers this function wll use to null
    errorMessage = nullptr;
    vertexShaderBuffer = nullptr;
    pixelShaderBuffer = nullptr;

    // Compile the vertex shader
    result = D3DX11CompileFromFile(vsFilename,
                                   NULL, NULL,
                                   "ColorVertexShader",
                                   "vs_5_0",
                                   D3D10_SHADER_ENABLE_STRICTNESS,
                                   0, NULL,
                                   &vertexShaderBuffer,
                                   &errorMessage,
                                   NULL);

    if (FAILED(result)) {
        // if there is a message
        if (errorMessage) {
            OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
        } else {
            MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
        }
        return false;
    }

    // Compile the pixel shader
    result = D3DX11CompileFromFile(psFilename,
                                   NULL, NULL,
                                   "ColorPixelShader",
                                   "ps_5_0",
                                   D3D10_SHADER_ENABLE_STRICTNESS,
                                   0, NULL,
                                   &pixelShaderBuffer,
                                   &errorMessage,
                                   NULL);
    if (FAILED(result)) {
        if (errorMessage) {
            OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
        } else {
            MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
        }
        return false;
    }

    // Create the vertex and pixel shader from the buffer
    result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
                                        vertexShaderBuffer->GetBufferSize(),
                                        NULL, &mVertexShader);
    if (FAILED(result)) return false;

    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
                                       pixelShaderBuffer->GetBufferSize(),
                                       NULL, &mPixelShader);
    if (FAILED(result)) return false;

    // Set up the layout of the data that goes into the shader
    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticName = "COLOR";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

     numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

     result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &mInputLayout);
    if (FAILED(result)) return false;

    vertexShaderBuffer->Release();
    vertexShaderBuffer = nullptr;
    pixelShaderBuffer->Release();
    pixelShaderBuffer = nullptr;

    // Set up the description of the dynamic matrix constant buffer in
    // the vertex shader
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;
    
    result = device->CreateBuffer(&matrixBufferDesc, NULL, &mMatrixBuffer);
    if (FAILED(result)) return false;
    return true;
}

void ColorShader::ShutdownShader() {
    // Release all interfaces
    if (mMatrixBuffer) {
        mMatrixBuffer->Release();
        mMatrixBuffer = nullptr;
    }

    if (mMatrixBuffer) {
        mMatrixBuffer->Release();
        mMatrixBuffer = nullptr;
    }

    if (mPixelShader) {
        mPixelShader->Release();
        mPixelShader = nullptr;
    }

    if (mVertexShader) {
        mVertexShader->Release();
        mVertexShader = nullptr;
    }
}

void ColorShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage,
                                           HWND hwnd,
                                           WCHAR* shaderFilename) {
    char* compileErrors;
    unsigned long bufferSize;
    std::ofstream fout;

    // Get Pointer to the error message and length
    compileErrors = (char*)(errorMessage->GetBufferPointer());
    bufferSize = errorMessage->GetBufferSize();

    // Ope a file to write the error to
    fout.open("ShaderErr.log");

    for (int i = 0; i < bufferSize; i++) {
        fout << compileErrors[i];
    }

    fout.close();

    errorMessage->Release();
    errorMessage = nullptr;

    MessageBox(hwnd, L"Error compiling shader. Check ShaderErr.log for message.", shaderFilename, MB_OK);
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection) {
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    MatrixBufferType* dataPtr;
    unsigned int bufferNumber;

    // Transpose the matrices to prepare them for the shader
    D3DXMatrixTranspose(&world, &world);
    D3DXMatrixTranspose(&view, &view);
    D3DXMatrixTranspose(&projection, &projection);

    result = deviceContext->Map(mMatrixBuffer, 0,
                                D3D11_MAP_WRITE_DISCARD,
                                0, &mappedResource);
    if (FAILED(result)) return false;
    
    dataPtr = (MatrixBufferType*)mappedResource.pData;

    // copy matrices into constant buffer
    dataPtr->worldMatrix = world;
    dataPtr->viewMatrix = view;
    dataPtr->projectionMatrix = projection;

    deviceContext->Unmap(mMatrixBuffer, 0);

    // Set the constant buffer in the vertex shader to the updated values
    bufferNumber = 0;
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &mMatrixBuffer);

    return true;
}

void ColorShader::RenderShader(ID3D11DeviceContext* deviceContext,
                               int indexCount) {
    // Set the vertex input layout
    deviceContext->IASetInputLayout(mInputLayout);

    // Set the vertex and pixel shaders that will be used to render this triangle
    deviceContext->VSSetShader(mVertexShader, NULL, 0);
    deviceContext->PSSetShader(mPixelShader, NULL, 0);

    // Render Triangle
    // deviceContext->DrawIndexed(indexCount, 0, 0);
    deviceContext->Draw(3, 0);
}