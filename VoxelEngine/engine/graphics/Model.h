#pragma once

#pragma warning( disable : 4005 )
#include <D3D11.h>
#include <D3DX10math.h>
#pragma warning( default : 4005 )
#include <fstream>

#include "Texture.h"

namespace VoxelEngine {
    class Model {
    public:
        Model();
        Model(const Model&);
        virtual ~Model();

        bool Initialize(ID3D11Device*, char*, WCHAR*);
        void Shutdown();
        void Render(ID3D11DeviceContext*);
        int GetIndexCount();

        ID3D11ShaderResourceView* GetTexture();

    private: 
        struct VertexType {
            D3DXVECTOR3 position;
            D3DXVECTOR2 texCoord;
            D3DXVECTOR3 normal;
        };

        struct ModelType {
            float x, y, z;
            float tu, tv;
            float nx, ny, nz;
        };

        bool InitializeBuffers(ID3D11Device*);
        void ShutdownBuffers();
        void RenderBuffers(ID3D11DeviceContext*);

        bool LoadTexture(ID3D11Device*, WCHAR*);
        void ReleaseTexture();

        bool LoadModel(char*);
        void ReleaseModel();

        ID3D11Buffer *mVertexBuffer, *mIndexBuffer;
        int mVertexCount, mIndexCount;

        Texture* mTexture;
        ModelType* mModel;
    };
}