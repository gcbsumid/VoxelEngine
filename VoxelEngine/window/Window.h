#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <windowsx.h>
#include "../engine/GraphicsSystem.h"
#include "../engine/InputSystem.h"

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 800

namespace VoxelEngine {
    class Window {
    public:
        Window(bool isFullscreen);
        ~Window();

        bool Initialize(HINSTANCE hInstance);
        void Run();
        void Shutdown();

        LRESULT CALLBACK MessageHandler(HWND hwnd,
                                        UINT message,
                                        WPARAM wParam,
                                        LPARAM lParam);

    private:

        bool ProcessFrame();
        void InitializeWindows(int &width, int &height);
        void ShutdownWindow();
        
        InputSystem* mInput;
        GraphicsSystem* mGraphics;

        int mWidth;
        int mHeight;
        bool mFullScreen;

        HWND mWindow;
        HINSTANCE mInstance;
        LPCWSTR  mAppName;
    };

    static LRESULT CALLBACK WindowProc(HWND hwnd,
                                       UINT message,
                                       WPARAM wParam,
                                       LPARAM lParam);

    static Window* ApplicationHandle = 0;
}

#endif