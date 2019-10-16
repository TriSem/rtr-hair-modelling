#pragma once

#include "Renderer.h"
#include "framework.h"
#include <vector>
#include "Window.h"
#include <Mouse.h>
#include <Keyboard.h>
#include "SceneObject.h"
#include "Mesh.h"

using DirectX::Keyboard;
using DirectX::Mouse;

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

	std::unique_ptr<Rendering::Renderer> renderer;
	std::shared_ptr<Rendering::Scene> mainScene;

	std::unique_ptr<Keyboard> keyboard;
	std::unique_ptr<Mouse> mouse;
	Keyboard::State state;

	void Init();
	void Input();
	void Update();
	void Render();

	int nCmdShow;

	static const int WIDTH = 1920;
	static const int HEIGHT = 1080;
};

