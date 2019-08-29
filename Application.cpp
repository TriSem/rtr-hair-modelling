#include "Application.h"
#include <commdlg.h>

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

	reader = std::make_unique<OBJ::ObjReader>();
	mainScene = std::make_shared<Scene>();
	keyboard = std::make_unique<Keyboard>();

	ShowWindow(mainWindow.WindowHandle(), nCmdShow);
}

void Application::Input()
{
	auto state = keyboard->GetState();

	if (state.Escape)
	{
		PostQuitMessage(0);
	}
	if (state.O)
	{
		OpenFile(windows.at(0).WindowHandle());
	}
}

void Application::Update()
{
	mainScene->Update();
}

void Application::Render()
{
	renderer->Render(*mainScene);
}

void Application::OpenFile(HWND windowHandle)
{
	wchar_t fileName[100] = {};

	OPENFILENAME openFile = {};
	
	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = windowHandle;
	openFile.hInstance = instanceHandle;
	openFile.lpstrFile = fileName;
	openFile.nMaxFile = 100;

	bool succeeded = GetOpenFileName(&openFile);

	if (succeeded)
	{
		reader->LoadFile(openFile.lpstrFile);
		Mesh mesh = Mesh(reader->GetObjects().at(0).ExtractMesh());
		std::unique_ptr<DirectedEdgeMesh> decimatedMesh = std::make_unique<DirectedEdgeMesh>(mesh);
		mesh = decimatedMesh->ExtractBasicMesh();
		SceneObject model = SceneObject(renderer->GetDevice(), mesh, renderer->GetVertexShader());
		mainScene->GetSceneObjects().clear();
		mainScene->AddSceneObject(model);
	}
}
