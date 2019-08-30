#include "Application.h"
#include <commdlg.h>
#include "DecimationModel.h"

using Rendering::Renderer;
using Rendering::Scene;
using Rendering::DirectedEdgeMesh;
using Rendering::Mesh;
using Rendering::SceneObject;

Keyboard::KeyboardStateTracker Application::INPUT;

Application::Application(HINSTANCE instanceHandle, int nCmdShow, std::wstring appTitle) :
	instanceHandle(instanceHandle),
	appTitle(appTitle),
	renderer(nullptr),
	mainScene(nullptr),
	nCmdShow(nCmdShow),
	state({})
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
			this->Update();
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

	renderer->SetRenderMode(Rendering::RenderMode::WIREFRAME);

	ShowWindow(mainWindow.WindowHandle(), nCmdShow);
}

void Application::Input()
{
	state = keyboard->GetState();
	INPUT.Update(state);

	if (INPUT.pressed.Escape)
		PostQuitMessage(0);
	if (INPUT.pressed.O)
		OpenFile();
	if (INPUT.pressed.D1)
		Decimate(0.9f);
	else if (INPUT.pressed.D2)
		Decimate(0.8f);
	else if (INPUT.pressed.D3)
		Decimate(0.7f);
	else if (INPUT.pressed.D4)
		Decimate(0.6f);
	else if (INPUT.pressed.D5)
		Decimate(0.5f);
	else if (INPUT.pressed.D6)
		Decimate(0.4f);
	else if (INPUT.pressed.D7)
		Decimate(0.3f);
	else if (INPUT.pressed.D8)
		Decimate(0.2f);
	else if (INPUT.pressed.D9)
		Decimate(0.1f);
}

void Application::Update()
{
	mainScene->Update();
}

void Application::Render()
{
	renderer->Render(*mainScene);
}

void Application::OpenFile()
{
	wchar_t fileName[100] = {};

	OPENFILENAME openFile = {};
	
	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = windows.at(0).WindowHandle();
	openFile.hInstance = instanceHandle;
	openFile.lpstrFile = fileName;
	openFile.nMaxFile = ARRAYSIZE(fileName);

	bool succeeded = GetOpenFileName(&openFile);

	if (succeeded)
	{
		reader->LoadFile(openFile.lpstrFile);
		Mesh mesh = Mesh(reader->GetObjects().at(0).ExtractMesh());
		auto model = std::make_shared<DecimationModel>(renderer->GetDevice(), mesh, renderer->GetVertexShader());
		mainScene->GetSceneObjects().clear();
		mainScene->AddSceneObject(model);
	}
}

void Application::Decimate(float decimationPercentage)
{	
	if (decimationPercentage < 0.01f || decimationPercentage > 0.99f || mainScene->GetSceneObjects().empty())
		return;

	auto object = mainScene->GetSceneObjects().at(0);
	Rendering::Transform transform = object->GetTransform();
	auto decimatedMesh = std::make_unique<DirectedEdgeMesh>(object->GetMesh());
	decimatedMesh->Decimate(decimatedMesh->FaceCount() * decimationPercentage);
	Mesh mesh = decimatedMesh->ExtractBasicMesh();
	auto model = std::make_shared<DecimationModel>(renderer->GetDevice(), mesh, renderer->GetVertexShader());
	model->GetTransform() = transform;
	mainScene->GetSceneObjects().at(0) = model;
}
