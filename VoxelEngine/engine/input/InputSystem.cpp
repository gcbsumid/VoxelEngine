#include "InputSystem.h"

using namespace VoxelEngine;

InputSystem::InputSystem() {}

InputSystem::InputSystem(const InputSystem& other) {}

InputSystem::~InputSystem() {}

bool InputSystem::Initialize() {
    for (int i = 0; i < 256; i++) {
        mKeys.push_back(false);
    }
    return true;
}

void InputSystem::KeyDown(unsigned int key) {
    mKeys.at(key) = true;
}

void InputSystem::KeyUp(unsigned int key) {
    mKeys.at(key) = false;
}

bool InputSystem::IsKeyDown(unsigned int key) {
    return mKeys.at(key);
}