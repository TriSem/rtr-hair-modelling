#pragma once
#include "framework.h"

class Window
{
public:
	
	static LRESULT CALLBACK ProcessMessage(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);


	Window(HINSTANCE instanceHandle, std::wstring windowClassName, int width, int height);
	~Window();
	HWND WindowHandle() const;

private:

	void Register();

	HWND windowHandle;
	HINSTANCE instanceHandle;

	std::wstring windowClassName;

	int width;
	int height;
};

