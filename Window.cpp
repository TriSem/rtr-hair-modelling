#include "Window.h"
#include "DirectXTK/Inc/Mouse.h"
#include "DirectXTK/Inc/Keyboard.h"

Window::Window(HINSTANCE instanceHandle, std::wstring windowClassName, std::wstring title, int width, int height) :
	instanceHandle(instanceHandle),
	windowClassName(windowClassName),
	title(title),
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

int Window::Width() const
{
	return width;
}

int Window::Height() const
{
	return height;
}

void Window::Register()
{
	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc = Window::ProcessMessage;
	windowClass.hInstance = instanceHandle;
	windowClass.lpszClassName = windowClassName.c_str();
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClass(&windowClass))
	{
		SendErrorMessage(GetLastError(), L"RegisterClass failed.");
	}

	windowHandle = CreateWindowEx(
		0,
		windowClassName.c_str(),
		title.c_str(),
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
		case WM_CLOSE:
		{
			DestroyWindow(windowHandle);
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			//DirectX::Mouse::ProcessMessage(message, wParam, lParam);
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			//DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
			break;
	}
	return DefWindowProc(windowHandle, message, wParam, lParam);
}