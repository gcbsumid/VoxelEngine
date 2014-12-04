#ifndef COLOR_SHADER_H
#define COLOR_SHADER_H

#pragma warning( disable : 4005 )
#include <D3D11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#pragma warning( default : 4005 )

#include <fstream>

namespace VoxelEngine {
    class ColorShader {
    public:
        ColorShader();
        ColorShader(const ColorShader&);
        virtual ~ColorShader();

        bool Initialize(ID3D11Device*, HWND);
        void Shutdown();
        bool Render(ID3D11DeviceContext*, int, D3DMATRIX, D3DMATRIX, D3DMATRIX);

    private:
        bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
        void ShutdownShader();
        void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

        bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
        void RenderShader(ID3D11DeviceContext*, int);

    private:
        ID3D11VertexShader* mVertexShader;
        ID3D11PixelShader* mPixelShader;
        ID3D11InputLayout* mInputLayout;
        ID3D11Buffer* mMatrixBuffer;

        struct MatrixBufferType {
            D3DXMATRIX worldMatrix;
            D3DXMATRIX viewMatrix;
            D3DXMATRIX projectionMatrix;
        };
    };
}

#endif