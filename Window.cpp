#include "Window.h"

Window::Window(HINSTANCE instanceHandle, std::wstring windowClassName, int width, int height) :
	instanceHandle(instanceHandle),
	windowClassName(windowClassName),
	width(width),
	height(height),
	windowHandle(nullptr)
{
	Register();
}

Window::~Window()
{
	if (windowHandle != nullptr)
	{
		UnregisterClass(windowClassName.c_str(), instanceHandle);
		DestroyWindow(windowHandle);
	}
}

HWND Window::WindowHandle() const
{
	return windowHandle;
}

void Window::Register()
{
	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc = ProcessMessage;
	windowClass.hInstance = instanceHandle;
	windowClass.lpszClassName = windowClassName.c_str();

	RegisterClass(&windowClass);

	windowHandle = CreateWindowEx(
		0,
		windowClassName.c_str(),
		L"Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		nullptr,
		nullptr,
		instanceHandle,
		nullptr
	);

	if (windowHandle == nullptr)
	{
		SendErrorMessage(GetLastError(), L"CreateWindowEx failed.");
	}
}

LRESULT CALLBACK ProcessMessage(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(windowHandle, message, wParam, lParam);
}