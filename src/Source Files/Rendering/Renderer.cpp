#include "Renderer.h"

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
		swapChain(nullptr),
		depthStencilBuffer(nullptr),
		backBufferView(nullptr),
		depthStencilView(nullptr),
		renderMode(RenderMode::SOLID),
		vertexShader(nullptr),
		pixelShader(nullptr),
		splitScreen(nullptr),
		mvpConstantBuffer(nullptr)
	{
		Initialize();
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Render(Scene& scene)
	{
		Clear();
		ID3D11DeviceContext* context = device->GetContext();
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		vector<std::shared_ptr<SceneObject>>& objects = scene.GetSceneObjects();

		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			Camera& camera = scene.GetCamera();
			std::shared_ptr<SceneObject> object = *it;
			MVPMatrices mvp;
			mvp.model = object->GetTransform().TransformationMatrix();
			mvp.view = camera.ViewMatrix();
			mvp.projection = camera.ProjectionMatrix();
			mvpConstantBuffer->SetData(mvp);

			std::shared_ptr<VertexBuffer> vertexBuffer = object->GetVertexBuffer();
			std::shared_ptr<IndexBuffer> indexBuffer = object->GetIndexBuffer(); 
			context->IASetInputLayout(vertexBuffer->GetInputLayout().Get());
			context->IASetVertexBuffers(0, 1, vertexBuffer->GetData().GetAddressOf(), &vertexBuffer->STRIDE, &vertexBuffer->GetOffset());
			context->IASetIndexBuffer(indexBuffer->GetData().Get(), DXGI_FORMAT_R32_UINT, 0);
			context->VSSetShader(vertexShader->GetShader().Get(), nullptr, 0);
			context->VSSetConstantBuffers(0, 1, mvpConstantBuffer->GetAddressOf());
			context->GSSetShader(geometryShader->GetShader().Get(), nullptr, 0);
			context->PSSetShader(pixelShader->GetShader().Get(), nullptr, 0);
			context->DrawIndexed(indexBuffer->GetIndexCount(), 0, 0);
		}

		swapChain->Present(0, 0);
	}

	void Renderer::Initialize()
	{
		CheckMultisamplingSupport();
		CreateSwapChain();
		CreateBackBufferView();
		CreateDepthStencilView();
		BindViewsToPipeline();
		splitScreen = std::make_unique<SplitScreen>(ScreenSectioning::HALVED, static_cast<float>(width), static_cast<float>(height));
		CreateRasterizerStates();
		CreateShaders();

		mvpConstantBuffer = std::make_shared<ConstantBuffer<MVPMatrices>>();
	}

	void Renderer::CheckMultisamplingSupport()
	{
		MessageAndThrowIfFailed(
			device->GetDevice()->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, multisampleCount, &msaaQuality),
			L"Failed to check multisample quality levels."
		);
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
			device->GetDevice()->QueryInterface<IDXGIDevice>(dxgiDevice.GetAddressOf()),
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
				device->GetDevice(),
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
			device->GetDevice()->CreateRenderTargetView(
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
			device->GetDevice()->CreateTexture2D(
				&depthStencilDescription,
				nullptr,
				depthStencilBuffer.GetAddressOf()),
			L"Failed to create depthStencilBuffer."
		);

		MessageAndThrowIfFailed(
			device->GetDevice()->CreateDepthStencilView(
				depthStencilBuffer.Get(),
				nullptr,
				depthStencilView.GetAddressOf()),
			L"Failed to create depthStencilView"
		);
	}

	void Renderer::BindViewsToPipeline()
	{
		device->GetContext()->OMSetRenderTargets(1, backBufferView.GetAddressOf(), depthStencilView.Get());
	}

	void Renderer::CreateRasterizerStates()
	{
		D3D11_RASTERIZER_DESC rasterizerDescription = {};

		rasterizerDescription.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDescription.CullMode = D3D11_CULL_NONE;
		MessageAndThrowIfFailed(
			device->GetDevice()->CreateRasterizerState(&rasterizerDescription, rasterizerStateWireframe.ReleaseAndGetAddressOf()),
			L"Failed to create rasterizer state: WIREFRAME."
		);

		rasterizerDescription.FillMode = D3D11_FILL_SOLID;
		MessageAndThrowIfFailed(
			device->GetDevice()->CreateRasterizerState(&rasterizerDescription, rasterizerStateSolid.ReleaseAndGetAddressOf()),
			L"Failed to create rasterizer state: SOLID."
		);

		if(renderMode == RenderMode::SOLID)
			device->GetContext()->RSSetState(rasterizerStateSolid.Get());
		else
			device->GetContext()->RSSetState(rasterizerStateWireframe.Get());
	}

	void Renderer::CreateShaders()
	{
		vertexShader = std::make_shared<VertexShader>(L"StandardVS");
		pixelShader = std::make_shared<PixelShader>(L"UnlitPS"); 
		geometryShader = std::make_shared<GeometryShader>(L"StandardGS");
	}

	void Renderer::Clear()
	{
		Color clearColor = DirectX::Colors::CornflowerBlue.v;
		device->GetContext()->ClearRenderTargetView(backBufferView.Get(), clearColor);
		device->GetContext()->ClearDepthStencilView(depthStencilView.Get(),
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
				device->GetContext()->RSSetState(rasterizerStateSolid.Get());
			else
				device->GetContext()->RSSetState(rasterizerStateWireframe.Get());
		}
	}

	const ComPtr<ID3D11Device> Renderer::GetDevice() const
	{
		return device->GetDevice();
	}

	const std::shared_ptr<VertexShader> Renderer::GetVertexShader() const
	{
		return vertexShader;
	}
}
