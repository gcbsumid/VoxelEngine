#include "Window.h"

namespace VoxelEngine {
    Window::Window(bool isFullScreen)
        : mFullScreen(isFullScreen){

    }

    Window::~Window() {
    }

    void Window::Shutdown() {
        if (mGraphics) {
            mGraphics->Shutdown();
            delete mGraphics;
        }

        if (mInput) {
            delete mInput;
        }

        ShutdownWindow();
    }

    void Window::ShutdownWindow() {
        ShowCursor(true);

        if (mFullScreen) {
            ChangeDisplaySettings(NULL, 0);
        }

        DestroyWindow(mWindow);
        mWindow = NULL;

        UnregisterClass(mAppName, mInstance);
        mInstance = NULL;
    }

    bool Window::Initialize() {
        mInstance = GetModuleHandle(NULL);

        ApplicationHandle = this;
        InitializeWindows(mWidth, mHeight);

        // Create Input object
        mInput = new InputSystem();
        if (!mInput->Initialize()) {
            return false;
        }

        // Create the D3D11 Device
        mGraphics = new GraphicsSystem(mFullScreen);
        if (!mGraphics->Initialize(mWindow, mWidth, mHeight)) {
            return false;
        }
        return true;
    }

    void Window::Run() {
        MSG msg;
        ZeroMemory(&msg, sizeof(MSG));

        while (true) {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                // Translate message
                TranslateMessage(&msg);

                //send message to windows proc
                DispatchMessage(&msg);
            }

            if (msg.message == WM_QUIT) {
                break;
            } else {
                if (!ProcessFrame()) {
                    break;
                }
            }
        }

        return;
    }

    bool Window::ProcessFrame() {
        if (mInput->IsKeyDown(VK_ESCAPE)) {
            return false;
        }

        return mGraphics->ProcessFrame();
    }

    LRESULT CALLBACK Window::MessageHandler(HWND hwnd,
                                            UINT message,
                                            WPARAM wParam,
                                            LPARAM lParam) {
        switch (message) {
            case WM_KEYDOWN: 
                mInput->KeyDown((unsigned int)wParam);
                return 0;
            case WM_KEYUP:
                mInput->KeyUp((unsigned int)wParam);
                return 0;
            default:
                return DefWindowProc(hwnd, message, wParam, lParam);
        }
    }

    void Window::InitializeWindows(int &width, int &height) {
        WNDCLASSEX wc;
        DEVMODE dmScreenSettings;
        int posX, posY;

        // Zero out the memory
        ZeroMemory(&wc, sizeof(WNDCLASSEX));

        // Get external pointer to this.
        mAppName = L"Voxel Engine";

        // Set up windows class with defaults
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc = WindowProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = mInstance;
        wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
        wc.hIconSm = wc.hIcon;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.lpszMenuName = NULL;
        wc.lpszClassName = mAppName;
        wc.cbSize = sizeof(WNDCLASSEX);

        // Register Class
        RegisterClassEx(&wc);

        // Determine the resolution of the client's desktop screen
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);

        if (mFullScreen) {
            // Set the screen to max size of the user's desktop and 32 bit
            memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
            dmScreenSettings.dmSize = sizeof(dmScreenSettings);
            dmScreenSettings.dmPelsWidth = (unsigned long)width;
            dmScreenSettings.dmPelsHeight = (unsigned long)height;
            dmScreenSettings.dmBitsPerPel = 32;
            dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            // Change display setting to full screen
            ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
            
            // Set the position of the window to the top left corner
            posX = posY = 0;
        } else {
            // if windowed then set it to 800 x 600 resolution
            width = SCREEN_WIDTH;
            height = SCREEN_HEIGHT;

            // Place window in the middle of the screen
            posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
            posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
        }

        // Create the window up on the screen and set it as main focus
        mWindow = CreateWindowEx(WS_EX_APPWINDOW,
                                 mAppName, mAppName,
                                 WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
                                 WS_POPUP,
                                 posX, posY,
                                 width, height,
                                 NULL, NULL,
                                 mInstance, NULL);

        // Bring the window to the screen and set it as the main focus
        ShowWindow(mWindow, SW_SHOW);
        SetForegroundWindow(mWindow);
        SetFocus(mWindow);

        // Hide the mouse cursor
        ShowCursor(false);
    }

    LRESULT CALLBACK WindowProc(HWND hWnd,
                                UINT message,
                                WPARAM wParam,
                                LPARAM lParam) {
        switch (message) {
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
            case WM_CLOSE:
            {
                PostQuitMessage(0);
                return 0;
            }
            default:
                return ApplicationHandle->MessageHandler(hWnd, message, wParam, lParam);
        }   
    }
}

