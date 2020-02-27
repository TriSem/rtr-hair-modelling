#pragma once

#include "Renderer.h"
#include "framework.h"
#include <vector>
#include "Window.h"
#include <Mouse.h>
#include <Keyboard.h>
#include "SceneObject.h"
#include "RawFile.h"
#include <Vertex.h>
#include <Brush.h>
#include <ShaderCollection.h>
#include <RenderMode.h>

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

	std::unique_ptr<Rendering::Renderer> renderer = nullptr;
	std::unique_ptr<Rendering::ShaderCollection> shaderCollection = nullptr;
	std::shared_ptr<Rendering::Scene> mainScene = nullptr;
	std::shared_ptr<Canvas> canvas = nullptr;
	std::shared_ptr<Brush> brush = nullptr;

	std::unique_ptr<Keyboard> keyboard = nullptr;
	std::unique_ptr<Mouse> mouse = nullptr;
	Keyboard::State state = {};

	void Init();
	void Input();
	void Update();
	void Render();

	int nCmdShow;

	static const uint32_t WIDTH = 1920;
	static const uint32_t HEIGHT = 1080;
};

