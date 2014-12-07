#pragma once

#pragma warning( disable : 4005 )
#include <D3DX10math.h>
#pragma warning( default : 4005 )

namespace VoxelEngine {
    class Light {
    public:
        Light();
        Light(const Light&);
        ~Light();

        void SetDiffuseColor(float, float, float, float);
        void SetDirection(float, float, float);

        D3DXVECTOR4 GetDiffuseColor();
        D3DXVECTOR3 GetDirection();

    private:
        D3DXVECTOR4 mDiffuseColor;
        D3DXVECTOR3 mDirection;
    };
}