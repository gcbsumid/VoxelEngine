#pragma once

#pragma warning( disable : 4005 )
#include <D3D11.h>
#include <D3DX11tex.h>
#pragma warning( default : 4005 )

namespace VoxelEngine {
    class Texture {
    public:
        Texture();
        Texture(const Texture&);
        virtual ~Texture();

        bool Initialize(ID3D11Device*, WCHAR*);
        void Shutdown();

        ID3D11ShaderResourceView* GetTexture();

    private:
        ID3D11ShaderResourceView* mTexture;
    };
}

