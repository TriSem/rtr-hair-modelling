#include "Application.h"
#include <commdlg.h>

using namespace Rendering;
using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;

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

	renderer = make_unique<Renderer>(
		mainWindow.WindowHandle(), 
		mainWindow.Width(), 
		mainWindow.Height()
	);

	shaderCollection = make_unique<ShaderCollection>();
	mainScene = make_shared<Scene>();
	keyboard = make_unique<Keyboard>();
	mouse = make_unique<Mouse>();

	canvas = make_shared<Canvas>(WIDTH, HEIGHT);
	brush = make_shared<Brush>(canvas);

	RawFile vertexFile("E:/Programming/DirectX11/RTRHairModelling/ModelData/AngelinaHeadVB.raw");
	RawFile indexFile("E:/Programming/DirectX11/RTRHairModelling/ModelData/AngelinaHeadIB.raw");
	shared_ptr<Mesh> headMesh = make_shared<Mesh>(vertexFile.InterpretAsBuffer<Vertex>(), indexFile.InterpretAsBuffer<uint32_t>());

	TextureOptions options = { TextureType::ShaderResource, 2048, 2048 };
	shared_ptr<Texture> diffuseTexture = make_shared<Texture>(
		"E:/Programming/DirectX11/RTRHairModelling/ModelData/AngelinaDiffuseTex2048.raw", options);

	options.type = TextureType::Mixed;
	options.width = 512;
	options.height = 512;
	Rendering::Color color = { 0, 0, 0, 0 };
	shared_ptr<Texture> paintTexture = make_shared<Texture>(color, options);
	
	shared_ptr<SceneObject> head = make_shared<SceneObject>(headMesh);
	head->outputViewport = 1;
	Material diffuseMaterial;
	diffuseMaterial.vertexShader = shaderCollection->standardVertexShader;
	diffuseMaterial.geometryShader = shaderCollection->standardGeometryShader;
	diffuseMaterial.pixelShader = shaderCollection->litPixelShader;
	diffuseMaterial.SetTexture(diffuseTexture);

	Material hairMaterial;
	hairMaterial.vertexShader = shaderCollection->hairVertexShader;
	hairMaterial.geometryShader = shaderCollection->hairGeometryShader;
	hairMaterial.pixelShader = shaderCollection->litLinesPixelShader;
	hairMaterial.SetTexture(paintTexture);
	head->materials.push_back(diffuseMaterial);
	head->materials.push_back(hairMaterial);

	shared_ptr<Mesh> quadMesh = Mesh::CreateQuad(100, 100);
	shared_ptr<SceneObject> canvas = make_shared<SceneObject>(quadMesh);
	
	shared_ptr<SceneObject> overlay = make_shared<SceneObject>(headMesh);
	Material overlayMaterial;
	overlayMaterial.vertexShader = shaderCollection->flatVertexShader;
	overlayMaterial.geometryShader = shaderCollection->standardGeometryShader;
	overlayMaterial.pixelShader = shaderCollection->monoColorPixelShader;
	//overlayMaterial.SetTexture(diffuseTexture);
	overlay->SetRenderMode(RenderMode::WireFrame);
	overlay->materials.push_back(overlayMaterial);

	mainScene->AddSceneObject(head);
	mainScene->AddSceneObject(overlay);

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
