#include "Renderer.h"
#include "DirectXTK/Inc/GeometricPrimitive.h"
#include <DirectXMath.h>
#include "GeometryDataStructs.h"

using namespace DirectX::SimpleMath;
using DirectX::GeometricPrimitive;
using DirectX::XM_PI;

namespace Rendering
{
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
		vertexShader(nullptr),
		pixelShader(nullptr)
	{
		InitializeAPI();
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Render(Scene& scene)
	{
		Clear();
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		vector<SceneObject>& objects = scene.GetSceneObjects();

		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			ConstantBufferVS constantBufferVS;
			Camera& camera = scene.GetCamera();
			constantBufferVS.mvp = it->GetTransform().TransformationMatrix() * camera.ViewMatrix() * camera.ProjectionMatrix();
			constantBufferVS.modelMatrix = it->GetTransform().TransformationMatrix();

			D3D11_MAPPED_SUBRESOURCE resource;
			context->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
			CopyMemory(resource.pData, &constantBufferVS, sizeof(ConstantBufferVS));
			context->Unmap(constantBuffer.Get(), 0);

			std::shared_ptr<VertexBuffer> vertexBuffer = it->GetVertexBuffer();
			std::shared_ptr<IndexBuffer> indexBuffer = it->GetIndexBuffer(); 
			context->IASetInputLayout(vertexBuffer->GetInputLayout().Get());
			context->IASetVertexBuffers(0, 1, vertexBuffer->GetData().GetAddressOf(), &vertexBuffer->STRIDE, &vertexBuffer->GetOffset());
			context->IASetIndexBuffer(indexBuffer->GetData().Get(), DXGI_FORMAT_R32_UINT, 0);
			context->VSSetShader(vertexShader->GetShader().Get(), nullptr, 0);
			context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
			context->PSSetShader(pixelShader->GetShader().Get(), nullptr, 0);
			context->DrawIndexed(indexBuffer->GetIndexCount(), 0, 0);
		}

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
		CreateRasterizerStates();
		CreateShaders();

		D3D11_BUFFER_DESC description = {};
		description.Usage = D3D11_USAGE_DYNAMIC;
		description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		description.ByteWidth = static_cast<uint32_t>(sizeof(ConstantBufferVS) + (16 - (sizeof(ConstantBufferVS) % 16)));

		MessageAndThrowIfFailed(
			device->CreateBuffer(&description, 0, constantBuffer.GetAddressOf()),
			L"Failed to create constant buffer."
		);
	}

	void Renderer::CreateDevice()
	{
		UINT flags = 0;

	#if defined(_DEBUG)
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
		MessageAndThrowIfFailed(
			device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, multisampleCount, &msaaQuality),
			L"Failed to check multisample quality levels."
		);
	

		// Look up docs for multisampling quality. This function is likely not implemented properly.
		/*if (multisampleCount > msaaQuality)
		{
			MessageBox(0, L"Using lowered multisample count.", 0, 0);
			SetMultisampleCount(msaaQuality - 1);
		}*/
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
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		context->RSSetViewports(1, &viewport);
	}

	void Renderer::CreateRasterizerStates()
	{
		D3D11_RASTERIZER_DESC rasterizerDescription = {};

		rasterizerDescription.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDescription.CullMode = D3D11_CULL_BACK;
		MessageAndThrowIfFailed(
			device->CreateRasterizerState(&rasterizerDescription, rasterizerStateWireframe.ReleaseAndGetAddressOf()),
			L"Failed to create rasterizer state: WIREFRAME."
		);

		rasterizerDescription.FillMode = D3D11_FILL_SOLID;
		MessageAndThrowIfFailed(
			device->CreateRasterizerState(&rasterizerDescription, rasterizerStateSolid.ReleaseAndGetAddressOf()),
			L"Failed to create rasterizer state: SOLID."
		);

		if(renderMode == RenderMode::SOLID)
			context->RSSetState(rasterizerStateSolid.Get());
		else
			context->RSSetState(rasterizerStateWireframe.Get());
	}

	void Renderer::CreateShaders()
	{
		std::wstring shaderPath;

#ifdef _DEBUG
#ifdef _WIN64
		shaderPath = L"./x64/Debug/";
#else
		shaderPath = L"./Win32/Debug/";
#endif
#else
#ifdef _WIN64
		shaderPath = L"./x64/Release/";
#else
		shaderPath = L"./Win32/Debug/";
#endif
#endif

		std::wstring vertexShaderLocation = shaderPath + L"VertexShader.cso";
		std::wstring pixelShaderLocation = shaderPath + L"PixelShader.cso";
		vertexShader = std::make_unique<VertexShader>(device, vertexShaderLocation);
		pixelShader = std::make_unique<PixelShader>(device, pixelShaderLocation);
	}

	void Renderer::Clear()
	{
		Color clearColor = DirectX::Colors::CornflowerBlue.v;
		context->ClearRenderTargetView(backBufferView.Get(), clearColor);
		context->ClearDepthStencilView(depthStencilView.Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void Renderer::SetMultisampleCount(UINT count)
	{
		if (count >= 1)
			multisampleCount = count;
		else
			multisampleCount = 1;
	}

	void Renderer::SetRenderMode(RenderMode renderMode)
	{
		if (this->renderMode == renderMode)
			return;
		else
		{
			this->renderMode = renderMode;
			if (renderMode == RenderMode::SOLID)
				context->RSSetState(rasterizerStateSolid.Get());
			else
				context->RSSetState(rasterizerStateWireframe.Get());
		}
	}

	const ComPtr<ID3D11Device> Renderer::GetDevice() const
	{
		return device;
	}

	const std::shared_ptr<VertexShader> Renderer::GetVertexShader() const
	{
		return vertexShader;
	}
}
