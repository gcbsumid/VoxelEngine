#ifndef MODEL_H
#define MODEL_H

#pragma warning( disable : 4005 )
#include <D3D11.h>
#include <D3DX10math.h>
#pragma warning( default : 4005 )

namespace VoxelEngine {
    class Model {
    public:
        Model();
        Model(const Model&);
        virtual ~Model();

        bool Initialize(ID3D11Device*);
        void Shutdown();
        void Render(ID3D11DeviceContext*);
        int GetIndexCount();

    private: 
        struct VertexType {
            D3DXVECTOR3 position;
            D3DXVECTOR4 color;
        };

        bool InitializeBuffers(ID3D11Device*);
        void ShutdownBuffers();
        void RenderBuffers(ID3D11DeviceContext*);

        ID3D11Buffer *mVertexBuffer, *mIndexBuffer;
        int mVertexCount, mIndexCount;
    };
}

#endif