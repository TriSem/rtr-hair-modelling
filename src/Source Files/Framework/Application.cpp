#include "Application.h"
#include <commdlg.h>
#include <HairSculpture.h>

using namespace Rendering;

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

	renderer = make_unique<Renderer>(
		mainWindow.WindowHandle(), 
		mainWindow.Width(), 
		mainWindow.Height()
	);

	mainScene = make_shared<Scene>();
	keyboard = make_unique<Keyboard>();
	mouse = make_unique<Mouse>();
	mouse->SetWindow(mainWindow.WindowHandle());

	TextureOptions options;
	options.type = TextureType::Mixed;
	options.width = 1920;
	options.height = 1080;
	Color color = { 0, 0, 0, 1 };
	shared_ptr<Texture> paintTexture = make_shared<Texture>(color, options);
	
	shared_ptr<SceneObject> head = make_shared<HairSculpture>(paintTexture);
	shared_ptr<SceneObject> overlay = make_shared<TextureOverlay>(head->GetMesh());
	shared_ptr<SceneObject> canvas = make_shared<Canvas>(paintTexture);
	shared_ptr<SceneObject> brush = make_shared<Brush>(paintTexture);

	mainScene->AddSceneObject(head);
	mainScene->AddSceneObject(brush);
	mainScene->AddSceneObject(overlay);
	mainScene->AddSceneObject(canvas);

	ShowWindow(mainWindow.WindowHandle(), nCmdShow);
}

void Application::Input()
{
	auto keyState = keyboard->GetState();
	auto mouseState = mouse->GetState();
	keyTracker.Update(keyState);
	mouseTracker.Update(mouseState);

	SceneObject::SetInput(mouseState, keyState);

	DirectX::SimpleMath::Rectangle rectangle(0, 0, WIDTH / 2, HEIGHT);
	if (rectangle.Contains(Vector2((float)mouseState.x, (float)mouseState.y)))
		mouse->SetVisible(false);
	else
		mouse->SetVisible(true);

	if (keyTracker.pressed.Escape)
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
