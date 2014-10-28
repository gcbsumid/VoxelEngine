#include "Window.h"

namespace VoxelEngine {
    Window::Window(UINT width, UINT height)
        : mHeight(height)
        , mWidth(width) {

    }

    Window::~Window() {
        delete mEngine;
    }

    void Window::Initialize(HINSTANCE hInstance,
                            int nShowCmd) {
        // Zero out the memory
        ZeroMemory(&mWndClass, sizeof(WNDCLASSEX));

        // Fill in WNDCLASSEX
        mWndClass.cbSize = sizeof(WNDCLASSEX);
        mWndClass.style = CS_HREDRAW | CS_VREDRAW;
        mWndClass.lpfnWndProc = Window::WindowProc;
        mWndClass.hInstance = hInstance;
        mWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        //mWndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
        mWndClass.lpszClassName = L"Voxel Engine";

        RegisterClassEx(&mWndClass);

        RECT rect = { 0, 0, mWidth, mHeight };
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        // Create the window
        mWindow = CreateWindowEx(NULL,
                                L"Voxel Engine",
                                L"Voxel Engine Program",
                                WS_OVERLAPPEDWINDOW,
                                300, 300,
                                (int)mWidth, (int)mHeight,
                                NULL,
                                NULL,
                                hInstance,
                                NULL);

        // Create the D3D11 Device
        mEngine = new Engine(mWidth, mHeight);
        mEngine->Init(mWindow);

        // Display window on the screen
        ShowWindow(mWindow, nShowCmd);
    }

    LRESULT CALLBACK Window::WindowProc(HWND hWnd, 
                                        UINT message, 
                                        WPARAM wParam, 
                                        LPARAM lParam) {
        switch (message) {
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    int Window::Run() {
        MSG msg = { 0 };

        while (true) {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                // Translate message
                TranslateMessage(&msg);

                //send message to windows proc
                DispatchMessage(&msg);

                if (msg.message == WM_QUIT) {
                    break;
                }
            }

            mEngine->Render();
        }

        return msg.wParam;
    }
}

