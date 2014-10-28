#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <windowsx.h>
#include "../engine/Engine.h"

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 800

namespace VoxelEngine {
    class Window {
    public:
        Window(UINT width = SCREEN_WIDTH, UINT height = SCREEN_HEIGHT);
        ~Window();

        void Initialize(HINSTANCE hInstance,
                        int nShowCmd);

        int Run();

    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd,
                                           UINT message,
                                           WPARAM wParam,
                                           LPARAM lParam);
        UINT mWidth;
        UINT mHeight;

        HWND mWindow;
        WNDCLASSEX mWndClass;
        
        Engine* mEngine;
    };
}

#endif