#pragma once

#pragma warning( disable : 4005 )
#include <D3DX10math.h>
#pragma warning( default : 4005 )

namespace VoxelEngine {
    class Camera {
    public:
        Camera();
        Camera(const Camera&);
        virtual ~Camera();

        void SetPosition(float, float, float);
        void SetRotation(float, float, float);

        D3DXVECTOR3 GetPosition();
        D3DXVECTOR3 GetRotation();

        void Render();
        void GetViewMatrix(D3DXMATRIX&);

    private:
        float mPositionX, mPositionY, mPositionZ;
        float mRotationX, mRotationY, mRotationZ;
        D3DXMATRIX mView;
    };
}

