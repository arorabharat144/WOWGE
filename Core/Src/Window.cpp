#include "Precompiled.h"
#include "Window.h"

#include "Debug.h"

#include "Input\Inc\Input.h"

using namespace WOWGE;
using namespace WOWGE::Core;

LRESULT CALLBACK WinProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	Input::InputSystem::ProcessMessage(handle, message, wParam, lParam);

	switch (message)
	{
		case WM_SIZE:
		{
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			//Graphics::GraphicsSystem::OnResize(width, height);
			}
			break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProcA(handle, message, wParam, lParam);
}

Window::Window():
	mInstance(nullptr),
	mWindow(nullptr)
{

}

Window::~Window()
{

}


void Window::Initialize(HINSTANCE instance, LPCSTR appName, uint32_t width, uint32_t height)
{
	mInstance = instance;
	mAppName = appName;

	// Every Windows Window requires at least one window object. Three things are involved:
	// 1) Register a window class.
	// 2) Create a window object.
	// 3) Retrieve and dispatch messages

	// Register class
	WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(WNDCLASSEXA);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WinProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = appName;
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	VERIFY(RegisterClassExA(&wcex), "[Core::Window] Failed to register window class.");

	// Compute the correct window dimensions
	RECT rc = { 0,0,(LONG)width,(LONG)height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int winWidth = rc.right - rc.left;
	const int winHeight = rc.bottom - rc.top;
	const int left = (screenWidth - winWidth) >> 1;
	const int top = (screenHeight - winHeight) >> 1;

	// Create window
	mWindow = CreateWindowA
	(
		appName, appName,
		WS_OVERLAPPEDWINDOW,
		left, top,
		winWidth, winHeight,
		nullptr, nullptr,
		instance, nullptr
	);

	ASSERT(mWindow != nullptr, "[Core::Window] Failed to create window");

	ShowWindow(mWindow, true);

	VERIFY(SetCursorPos(screenWidth >> 1, screenHeight >> 1), "[Core::Window] Failed to set cursor position");
}

void Window::Terminate()
{
	if (mWindow)
	{
		DestroyWindow(mWindow);
		VERIFY(UnregisterClassA(mAppName.c_str(), mInstance), "[Core::Window] Failed to unregister window class");
		mWindow = nullptr;
		mInstance = nullptr;
	}
}

bool Window::ProcessMessage()
{
	MSG msg = {};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (WM_QUIT == msg.message);
}