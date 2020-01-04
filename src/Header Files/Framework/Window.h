#pragma once
#include "framework.h"

class Window
{
public:
	
	static LRESULT CALLBACK ProcessMessage(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

	Window(HINSTANCE instanceHandle, std::wstring windowClassName, std::wstring title, int width, int height);
	~Window();
	HWND WindowHandle() const;
	int Width() const;
	int Height() const;

private:

	void Register();

	HWND windowHandle;
	HINSTANCE instanceHandle;

	std::wstring windowClassName;
	std::wstring title;
	int width;
	int height;
};

