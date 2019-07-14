#include "Renderer.h"

using namespace Microsoft::WRL;

Renderer::Renderer(HWND windowHandle) :
	windowHandle(windowHandle),
	multisampleCount(4),
	width(1920),
	height(1280),
	device(nullptr),
	context(nullptr),
	swapChain(nullptr),
	depthStencilBuffer(nullptr),
	backBufferView(nullptr),
	depthStencilView(nullptr)
{
	InitializeAPI();
}

Renderer::~Renderer()
{
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
}

void Renderer::CreateDevice()
{
	UINT flags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif	

	D3D_FEATURE_LEVEL featureLevel;

	ThrowIfFailed(
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
	ThrowIfFailed(
		device->QueryInterface<IDXGIDevice>(dxgiDevice.GetAddressOf()),
		L"Failed to query IDXGIDevice interface."
	);

	ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	ThrowIfFailed(
		dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()),
		L"Failed to get dxgiAdapter."
	);

	ComPtr<IDXGIFactory> dxgiFactory = nullptr;
	ThrowIfFailed(
		dxgiAdapter->GetParent(
			__uuidof(IDXGIFactory),
			reinterpret_cast<void**>(dxgiFactory.GetAddressOf())),
		L"Failed to get dxgiFactory"
	);


	ThrowIfFailed(
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
	ThrowIfFailed(
		swapChain->GetBuffer(
			0,
			__uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(backBuffer.GetAddressOf())),
		L"Failed to get backBuffer."
	);

	ThrowIfFailed(
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

	ThrowIfFailed(
		device->CreateTexture2D(
			&depthStencilDescription,
			nullptr,
			depthStencilBuffer.GetAddressOf()),
		L"Failed to create depthStencilBuffer."
	);

	ThrowIfFailed(
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

void Renderer::SetMultisampleCount(UINT count)
{
	if (count >= 1)
		multisampleCount = count;
	else
		multisampleCount = 1;
}