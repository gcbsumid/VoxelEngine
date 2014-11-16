#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <vector>

namespace VoxelEngine {
    class InputSystem {
    public:
        InputSystem();
        InputSystem(const InputSystem&);
        ~InputSystem();

        bool Initialize();

        void KeyDown(unsigned int);
        void KeyUp(unsigned int);

        bool IsKeyDown(unsigned int);

    private:
        std::vector<bool> mKeys;
    };
}

#endif