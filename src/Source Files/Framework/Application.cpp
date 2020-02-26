#include "Application.h"
#include <commdlg.h>

using Rendering::Renderer;
using Rendering::Scene;
using Rendering::Mesh;
using Rendering::SceneObject;
using Rendering::Vertex;

Keyboard::KeyboardStateTracker Application::INPUT;

Application::Application(HINSTANCE instanceHandle, int nCmdShow, std::wstring appTitle) :
	instanceHandle(instanceHandle),
	appTitle(appTitle),
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

	mainScene = std::make_shared<Scene>();
	keyboard = std::make_unique<Keyboard>();
	mouse = std::make_unique<Mouse>();

	canvas = std::make_shared<Canvas>(WIDTH, HEIGHT);
	brush = std::make_shared<Brush>(canvas);

	Mesh<Rendering::HairVertex> headMesh;
	RawFile vertexFile("E:/Programming/DirectX11/RTRHairModelling/ModelData/AngelinaHeadVB.raw");
	RawFile indexFile("E:/Programming/DirectX11/RTRHairModelling/ModelData/AngelinaHeadIB.raw");
	headMesh.vertices = vertexFile.InterpretAsBuffer<Rendering::HairVertex>();
	headMesh.indices = indexFile.InterpretAsBuffer<uint32_t>();
	std::shared_ptr<SceneObject> head = std::make_unique<SceneObject>(headMesh, renderer->GetVertexShader());
	mainScene->AddSceneObject(head);

	head->GetTransform().SetScale(0.5f);
	head->GetTransform().SetRotation(Vector3(0.0f, 180.0f, 0.0f));

	ShowWindow(mainWindow.WindowHandle(), nCmdShow);
}

void Application::Input()
{
	state = keyboard->GetState();
	INPUT.Update(state);

	if (INPUT.pressed.Escape)
		PostQuitMessage(0);
	else if (INPUT.pressed.S)
	{
		// Save hairstyle
	}
	else if (INPUT.pressed.X)
	{
		// Load hairstyle
	}
	else if (INPUT.pressed.N)
	{
		// Load next predefined hairstyle
	}
	else if (INPUT.pressed.L)
	{
		brush->SetPaintChannel(PaintChannel::Length);
	}
	else if (INPUT.pressed.C)
	{
		brush->SetPaintChannel(PaintChannel::Curl);
	}
	else if (INPUT.pressed.T)
	{
		brush->SetPaintChannel(PaintChannel::Twist);
	}
	else if (INPUT.pressed.R)
	{
		canvas->Clear();
	}
	else if (INPUT.pressed.D)
	{
		// toggle overlay
	}
	else if (INPUT.pressed.D1 || INPUT.pressed.D2 || INPUT.pressed.D3 || INPUT.pressed.D4 || INPUT.pressed.D5)
	{
		// change hair color
	}

	if (state.LeftShift)
	{
		// Inverse bend angle
	}
	else if (state.OemPlus)
	{
		// Increase brush size
	}
	else if (state.OemMinus)
	{
		// decrease brush size
	}
	else if (state.P)
	{
		brush->IncreaseStrength();
	}
	else if (state.M)
	{
		brush->DecreaseStrength();
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
