#include "window/Window.h"

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nShowCmd) {
    VoxelEngine::Window window(false);
    
    // initialize and run window
    window.Initialize();

    // enter main loop
    window.Run();

    // Clean up window
    window.Shutdown();

    return 0;
}