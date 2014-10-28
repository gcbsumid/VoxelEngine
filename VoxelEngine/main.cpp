#include "window/Window.h"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nShowCmd) {
    VoxelEngine::Window window(500, 400);
    
    // initialize and run window
    window.Initialize(hInstance, nShowCmd);

    // enter main loop
    return window.Run();
}