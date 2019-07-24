#include "Application.h"
#include "ObjReader.h"
#include "SceneObject.h"

using Rendering::Renderer;
using Rendering::Scene;

Application::Application(HINSTANCE instanceHandle, int nCmdShow, std::wstring appTitle) :
	instanceHandle(instanceHandle),
	appTitle(appTitle),
	renderer(nullptr),
	mainScene(nullptr),
	nCmdShow(nCmdShow)
{
}

Application::~Application()
{
}

void Application::Run()
{
	Init();
	MSG message = {};

	OBJ::ObjReader reader;
	reader.LoadFile("C:/Users/Tristan/3D Objects/blenderMonkey.obj");
	Rendering::Mesh mesh = reader.GetObjects().at(0).ExtractMesh();
	Rendering::SceneObject model = Rendering::SceneObject(renderer->GetDevice(), mesh, renderer->GetVertexShader());
	mainScene->AddSceneObject(model);

	while (true)
	{
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			Input();
			Update();
			Render();
		}
	}
}

void Application::Init()
{
	windows.push_back(
		Window(
			instanceHandle,
			L"Main Window",
			appTitle,
			WIDTH,
			HEIGHT)
	);

	Window& mainWindow = windows.at(0);

	renderer = std::make_unique<Renderer>(
		mainWindow.WindowHandle(), 
		mainWindow.Width(), 
		mainWindow.Height()
	);

	mainScene = std::make_shared<Scene>();

	ShowWindow(mainWindow.WindowHandle(), nCmdShow);
}

void Application::Input()
{
}

void Application::Update()
{
	mainScene->Update();
}

void Application::Render()
{
	renderer->Render(*mainScene);
}
