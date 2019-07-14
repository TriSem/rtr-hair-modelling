#include "framework.h"
#include "Window.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Window mainWindow(hInstance, L"WindowClass", 1280, 1024);
	ShowWindow(mainWindow.WindowHandle(), nCmdShow);

	MSG message = {};
	while (GetMessage(&message, nullptr, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}