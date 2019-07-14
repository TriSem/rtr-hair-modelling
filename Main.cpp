#include "framework.h"
#include "Window.h"
#include "Renderer.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Window mainWindow(hInstance, L"WindowClass", 1280, 1024);
	ShowWindow(mainWindow.WindowHandle(), nCmdShow);
	Renderer renderer = Renderer(mainWindow.WindowHandle());

	MSG message = {};

	while (true)
	{
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			renderer.Render();
		}
	}
	

	return 0;
 }

