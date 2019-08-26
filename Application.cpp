#include "Application.h"

using Rendering::Renderer;
using Rendering::Scene;
using Rendering::DirectedEdgeMesh;
using Rendering::Mesh;
using Rendering::SceneObject;

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
	reader.LoadFile("C:/Users/Tristan/3D Objects/bunny.obj");
	Mesh mesh = Mesh(reader.GetObjects().at(0).ExtractMesh());
	std::unique_ptr<DirectedEdgeMesh> decimatedMesh = std::make_unique<DirectedEdgeMesh>(mesh);
	//decimatedMesh->Decimate(500);
	mesh = decimatedMesh->ExtractBasicMesh();
	SceneObject model = SceneObject(renderer->GetDevice(), mesh, renderer->GetVertexShader());
	model.GetTransform().SetScale(0.6f);
	mainScene->AddSceneObject(model);

	while (message.message != WM_QUIT)
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

	//renderer->SetRenderMode(Rendering::RenderMode::WIREFRAME);

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
