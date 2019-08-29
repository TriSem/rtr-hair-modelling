#pragma once

#include "Renderer.h"
#include "framework.h"
#include <vector>
#include "Window.h"
#include "DirectXTK/Inc/Mouse.h"
#include "DirectXTK/Inc/Keyboard.h"
#include "ObjReader.h"
#include "SceneObject.h"
#include "GeometryDataStructs.h"

using DirectX::Keyboard;

class Application
{
public:
	void Run();

	Application(HINSTANCE instanceHandle, int nCmdShow, std::wstring appTitle);
	~Application();

	static Keyboard::KeyboardStateTracker INPUT;

private:

	HINSTANCE instanceHandle;

	std::vector<Window> windows;
	std::wstring appTitle;

	std::unique_ptr<OBJ::ObjReader> reader;
	std::unique_ptr<Rendering::Renderer> renderer;
	std::shared_ptr<Rendering::Scene> mainScene;

	std::unique_ptr<Keyboard> keyboard;
	Keyboard::State state;

	void Init();
	void Input();
	void Update();
	void Render();
	void OpenFile(HWND windowHandle);

	int nCmdShow;

	static const int WIDTH = 1920;
	static const int HEIGHT = 1080;
};

