#pragma once

#include "Renderer.h"
#include "framework.h"
#include <vector>
#include "Window.h"
#include "RawFile.h"
#include <Vertex.h>
#include <ShaderCollection.h>
#include <HairDrawing.h>

using DirectX::Keyboard;
using DirectX::Mouse;
using ButtonState = DirectX::Mouse::ButtonStateTracker::ButtonState;


class Application
{
public:
	void Run();

	Application(HINSTANCE instanceHandle, int nCmdShow, std::wstring appTitle);
	~Application();

	Keyboard::KeyboardStateTracker keyTracker;
	Mouse::ButtonStateTracker mouseTracker;

private:

	HINSTANCE instanceHandle;

	std::vector<Window> windows;
	std::wstring appTitle;

	std::unique_ptr<Rendering::Renderer> renderer = nullptr;
	std::unique_ptr<Rendering::ShaderCollection> shaderCollection = nullptr;
	std::shared_ptr<Rendering::Scene> mainScene = nullptr;

	std::unique_ptr<Keyboard> keyboard = nullptr;
	std::unique_ptr<Mouse> mouse = nullptr;

	void Init();
	void Input();
	void Update();
	void Render();

	int nCmdShow;

	static const uint32_t WIDTH = 1920;
	static const uint32_t HEIGHT = 1080;
};

