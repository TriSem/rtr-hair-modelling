#include "Renderer.h"
#include "DirectXTK/Inc/GeometricPrimitive.h"
#include <DirectXMath.h>
#include "GeometryDataStructs.h"

using namespace DirectX::SimpleMath;
using DirectX::GeometricPrimitive;
using DirectX::XM_PI;


Renderer::Renderer(HWND windowHandle, int width, int height) :
	windowHandle(windowHandle),
	multisampleCount(4),
	width(width),
	height(height),
	device(nullptr),
	context(nullptr),
	swapChain(nullptr),
	depthStencilBuffer(nullptr),
	backBufferView(nullptr),
	depthStencilView(nullptr),
	renderMode(RenderMode::SOLID),
	vertexShader(nullptr)
{
	InitializeAPI();
}

Renderer::~Renderer()
{
}

void Renderer::Render(const Scene& scene)
{
	Clear();

	std::vector<SimpleVertex> vertices = {
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f) }, { XMFLOAT3(-1.0f, +1.0f, -1.0f) },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f) }, { XMFLOAT3(+1.0f, -1.0f, -1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f) }, { XMFLOAT3(-1.0f, +1.0f, +1.0f) },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f) }, { XMFLOAT3(+1.0f, -1.0f, +1.0f) }
	};

	D3D11_BUFFER_DESC bufferDescription = {};

	bufferDescription.ByteWidth = sizeof(SimpleVertex) * vertices.size();
	bufferDescription.Usage = D3D11_USAGE_DEFAULT;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = vertices.data();
	
	ComPtr<ID3D11Buffer> vertexBuffer = nullptr;

	MessageAndThrowIfFailed(
		device->CreateBuffer(
			&bufferDescription,
			&subresourceData,
			vertexBuffer.ReleaseAndGetAddressOf()
		),
		L"Failed to create vertex buffer"
	);

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	//context->Draw(8, 0);

	std::vector<UINT> indices = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 6,
		0, 6, 7,
		0, 7, 8,
		0, 8, 1
	};

	ComPtr<ID3D11Buffer> indexBuffer = nullptr;

	ZeroMemory(&bufferDescription, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.ByteWidth = sizeof(UINT) * 24;
	bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;

	subresourceData.pSysMem = indices.data();

	MessageAndThrowIfFailed(
		device->CreateBuffer(
			&bufferDescription,
			&subresourceData,
			indexBuffer.ReleaseAndGetAddressOf()
		),
		L"Failed to create index buffer!"
	);

	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//context->DrawIndexed(24, 0, 0);

	auto world = Matrix::Identity;

	auto view = Matrix::CreateLookAt(Vector3(2.f, 2.f, 2.f),
		Vector3::Zero, Vector3::UnitY);
	auto projection = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(1920) / float(1080), 0.1f, 10.f);

	

	swapChain->Present(0, 0);
}

void Renderer::InitializeAPI()
{
	CreateDevice();
	CheckMultisamplingSupport();
	CreateSwapChain();
	CreateBackBufferView();
	CreateDepthStencilView();
	BindViewsToPipeline();
	SetViewport();
	CreateShaders();
}

void Renderer::CreateDevice()
{
	UINT flags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif	

	D3D_FEATURE_LEVEL featureLevel;

	MessageAndThrowIfFailed(
		D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			flags,
			0,
			0,
			D3D11_SDK_VERSION,
			&device,
			&featureLevel,
			&context),
		L"Failed to create D3D11 device."
	);
}

void Renderer::CheckMultisamplingSupport()
{
	device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, multisampleCount, &msaaQuality);

	// Look up docs for multisampling quality. This function is likely not implemented properly.
	if (multisampleCount > msaaQuality)
	{
		MessageBox(0, L"Using lowered multisample count.", 0, 0);
		SetMultisampleCount(msaaQuality - 1);
	}
}

void Renderer::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC description;
	description.BufferDesc.Width = width;
	description.BufferDesc.Height = height;
	description.BufferDesc.RefreshRate.Numerator = 60;
	description.BufferDesc.RefreshRate.Denominator = 1;
	description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	description.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// TODO: Check for correctness.
	description.SampleDesc.Count = multisampleCount;
	description.SampleDesc.Quality = msaaQuality - 1;

	description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	description.BufferCount = 1;
	description.OutputWindow = windowHandle;
	description.Windowed = true;
	description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	description.Flags = 0;

	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	MessageAndThrowIfFailed(
		device->QueryInterface<IDXGIDevice>(dxgiDevice.GetAddressOf()),
		L"Failed to query IDXGIDevice interface."
	);

	ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	MessageAndThrowIfFailed(
		dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()),
		L"Failed to get dxgiAdapter."
	);

	ComPtr<IDXGIFactory> dxgiFactory = nullptr;
	MessageAndThrowIfFailed(
		dxgiAdapter->GetParent(
			__uuidof(IDXGIFactory),
			reinterpret_cast<void**>(dxgiFactory.GetAddressOf())),
		L"Failed to get dxgiFactory"
	);


	MessageAndThrowIfFailed(
		dxgiFactory->CreateSwapChain(
			device.Get(),
			&description,
			swapChain.GetAddressOf()),
		L"Swap chain creation failed."
	);
}

void Renderer::CreateBackBufferView()
{
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	MessageAndThrowIfFailed(
		swapChain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(backBuffer.GetAddressOf())),
		L"Failed to get backBuffer."
	);

	MessageAndThrowIfFailed(
		device->CreateRenderTargetView(
			backBuffer.Get(),
			0,
			backBufferView.GetAddressOf()),
		L"Failed to create backBufferView."
	);
}

void Renderer::CreateDepthStencilView()
{
	D3D11_TEXTURE2D_DESC depthStencilDescription;
	depthStencilDescription.Width = width;
	depthStencilDescription.Height = height;
	depthStencilDescription.MipLevels = 1;
	depthStencilDescription.ArraySize = 1;
	depthStencilDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDescription.SampleDesc.Count = multisampleCount;
	depthStencilDescription.SampleDesc.Quality = msaaQuality - 1;
	depthStencilDescription.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDescription.CPUAccessFlags = 0;
	depthStencilDescription.MiscFlags = 0;

	MessageAndThrowIfFailed(
		device->CreateTexture2D(
			&depthStencilDescription,
			nullptr,
			depthStencilBuffer.GetAddressOf()),
		L"Failed to create depthStencilBuffer."
	);

	MessageAndThrowIfFailed(
		device->CreateDepthStencilView(
			depthStencilBuffer.Get(),
			nullptr,
			depthStencilView.GetAddressOf()),
		L"Failed to create depthStencilView"
	);
}

void Renderer::BindViewsToPipeline()
{
	context->OMSetRenderTargets(1, backBufferView.GetAddressOf(), depthStencilView.Get());
}

void Renderer::SetViewport()
{
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(width);
	viewport.Width = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);
}

void Renderer::CreateShaders()
{
	// TODO: Define paths for all different build configurations.
	vertexShader = std::make_unique<VertexShader>(device, L"./x64/Debug/VertexShader.cso");
	pixelShader = std::make_unique<PixelShader>(device, L"./x64/Debug/PixelShader.cso");
}

void Renderer::Clear()
{
	Color clearColor = DirectX::Colors::CornflowerBlue.v;
	context->ClearRenderTargetView(backBufferView.Get(), clearColor);
	context->ClearDepthStencilView(depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, backBufferView.GetAddressOf(),
		depthStencilView.Get());

	BindViewsToPipeline();
}

void Renderer::SetMultisampleCount(UINT count)
{
	if (count >= 1)
		multisampleCount = count;
	else
		multisampleCount = 1;
}