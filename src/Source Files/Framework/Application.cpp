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

	shaderCollection = make_unique<ShaderCollection>();
	mainScene = make_shared<Scene>();
	keyboard = make_unique<Keyboard>();
	mouse = make_unique<Mouse>();

	TextureOptions options;
	options.type = TextureType::Mixed;
	options.width = 512;
	options.height = 512;
	Color color = { 0, 0, 0, 0 };
	shared_ptr<Texture> paintTexture = make_shared<Texture>(color, options);
	
	shared_ptr<SceneObject> head = make_shared<HairSculpture>(paintTexture);
	shared_ptr<SceneObject> overlay = make_shared<TextureOverlay>(head->GetMesh());

	shared_ptr<Mesh> quadMesh = Mesh::CreateQuad(100, 100);
	shared_ptr<SceneObject> canvas = make_shared<SceneObject>(quadMesh);
	
	

	Mesh<Rendering::HairVertex> headMesh;
	RawFile vertexFile("E:/Programming/DirectX11/RTRHairModelling/ModelData/AngelinaHeadVB.raw");
	RawFile indexFile("E:/Programming/DirectX11/RTRHairModelling/ModelData/AngelinaHeadIB.raw");
	headMesh.vertices = vertexFile.InterpretAsBuffer<Rendering::HairVertex>();
	headMesh.indices = indexFile.InterpretAsBuffer<uint32_t>();
	std::shared_ptr<SceneObject> head = std::make_unique<SceneObject>(headMesh, renderer->GetVertexShader());
	mainScene->AddSceneObject(head);

	head->GetTransform().SetScale(0.5f);
	head->GetTransform().SetRotation(Vector3(0.0f, 135.0f, 0.0f));

	ShowWindow(mainWindow.WindowHandle(), nCmdShow);
}

void Application::Input()
{
	state = keyboard->GetState();
	INPUT.Update(state);

	if (INPUT.pressed.Escape)
		PostQuitMessage(0);
}

void Application::Update()
{
	mainScene->Update();
}

void Application::Render()
{
	renderer->Render(*mainScene);
}
