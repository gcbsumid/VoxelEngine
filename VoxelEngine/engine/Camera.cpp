#include "Camera.h"

using namespace VoxelEngine;

Camera::Camera() : mPositionX(0.0f), 
                   mPositionY(0.0f), 
                   mPositionZ(0.0f),
                   mRotationX(0.0f),
                   mRotationY(0.0f),
                   mRotationZ(0.0f) {}

Camera::Camera(const Camera& other) {}

Camera::~Camera() {}

void Camera::SetPosition(float x, float y, float z) {
    mPositionX = x; 
    mPositionY = y;
    mPositionZ = z;
}

void Camera::SetRotation(float x, float y, float z) {
    mRotationX = x;
    mRotationY = y;
    mRotationZ = z;
}

D3DXVECTOR3 Camera::GetPosition() {
    return D3DXVECTOR3(mPositionX, mPositionY, mPositionZ);
}

D3DXVECTOR3 Camera::GetRotation() {
    return D3DXVECTOR3(mRotationX, mRotationY, mRotationZ);
}

void Camera::Render() {
    D3DXVECTOR3 up, position, lookAt;
    float yaw, pitch, roll;
    D3DXMATRIX rotationMatrix;

    // set up vectors;
	// Setup the vector that points upwards.
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;

    // Setup the position of the camera in the world.
    position.x = mPositionX;
    position.y = mPositionY;
    position.z = mPositionZ;

    // Setup where the camera is looking by default.
    lookAt.x = 0.0f;
    lookAt.y = 0.0f;
    lookAt.z = 1.0f;

    pitch = mRotationX * 0.0174532925f;
    yaw = mRotationY * 0.0174532925f;
    roll = mRotationZ * 0.0174532925f;

    // create the rotation matrix
    D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

    // Transform the lookat and up vector by the rotation matrix so 
    // that the view is correct
    D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
    D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

    // Translate the rotated camera position
    lookAt = position + lookAt;

    // Create view matrix
    D3DXMatrixLookAtLH(&mView, &position, &lookAt, &up);
}

void Camera::GetViewMatrix(D3DXMATRIX& view) {
    view = mView;
}