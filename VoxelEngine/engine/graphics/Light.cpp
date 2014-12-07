#include "Light.h"

using namespace VoxelEngine;

Light::Light() {}

Light::Light(const Light&) {}

Light::~Light() {}

void Light::SetDiffuseColor(float r, float g, float b, float a) {
    mDiffuseColor = D3DXVECTOR4(r, g, b, a);
}

void Light::SetDirection(float x, float y, float z) {
    mDirection = D3DXVECTOR3(x, y, z);
}

D3DXVECTOR4 Light::GetDiffuseColor() {
    return mDiffuseColor;
}

D3DXVECTOR3 Light::GetDirection() {
    return mDirection;
}