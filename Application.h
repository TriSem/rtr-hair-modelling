#pragma once

#include "Renderer.h"
#include "framework.h"
#include <vector>
#include "Window.h"
#include "DirectXTK/Inc/Mouse.h"
#include "DirectXTK/Inc/Keyboard.h"

using DirectX::Mouse;
using DirectX::Keyboard;

class Application
{
public:
	void Run();

	Application(HINSTANCE instanceHandle, int nCmdShow, std::wstring appTitle);
	~Application();

private:

	HINSTANCE instanceHandle;

	std::vector<Window> windows;
	std::wstring appTitle;

	std::unique_ptr<Rendering::Renderer> renderer;
	std::unique_ptr<Rendering::Scene> mainScene;

	void Init();
	void Input();
	void Update();
	void Render();

	int nCmdShow;

	static const int WIDTH = 1920;
	static const int HEIGHT = 1080;
};

