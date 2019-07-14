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
	
}

HWND Window::WindowHandle() const
{
	return windowHandle;
}

void Window::Register()
{
	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc = Window::ProcessMessage;
	windowClass.hInstance = instanceHandle;
	windowClass.lpszClassName = windowClassName.c_str();
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

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

LRESULT CALLBACK Window::ProcessMessage(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(windowHandle, message, wParam, lParam);
}