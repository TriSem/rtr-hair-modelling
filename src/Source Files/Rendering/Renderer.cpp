#include "Renderer.h"

using namespace DirectX::SimpleMath;
using DirectX::GeometricPrimitive;
using DirectX::XM_PI;

namespace Rendering
{
	Renderer::Renderer(HWND windowHandle, int width, int height) :
		windowHandle(windowHandle),
		width(width),
		height(height)
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

		LightingCBT lightingData;
		lightingData.directionalLight = scene.GetLight();
		lightingData.viewPoint = scene.GetCamera().Position();
		lightingConstantBuffer->SetData(lightingData);

		vector<std::shared_ptr<SceneObject>>& objects = scene.GetSceneObjects();

		for (auto it = objects.begin(); it != objects.end(); it++)
		{
			Camera& camera = scene.GetCamera();
			std::shared_ptr<SceneObject> object = *it;
			MVPMatricesCBT mvp;
			mvp.model = object->GetTransform().TransformationMatrix();
			mvp.view = camera.ViewMatrix();
			mvp.projection = camera.ProjectionMatrix();
			mvpConstantBuffer->SetData(mvp);
			ViewportIndexCBT viewportIndex;
			viewportIndex.index = 1;
			viewportIndexBuffer->SetData(viewportIndex);

			SetRenderMode(RenderMode::SOLID);
			std::shared_ptr<VertexBuffer<HairVertex>> vertexBuffer = object->GetVertexBuffer();
			std::shared_ptr<IndexBuffer> indexBuffer = object->GetIndexBuffer(); 
			context->IASetInputLayout(vertexShader->GetInputLayout().Get());
			const UINT offsets[] = { 0 };
			context->IASetVertexBuffers(0, 1, vertexBuffer->Data().GetAddressOf(), &vertexBuffer->STRIDE, offsets);
			context->IASetIndexBuffer(indexBuffer->Data().Get(), DXGI_FORMAT_R32_UINT, 0);
			context->VSSetConstantBuffers(0, 1, mvpConstantBuffer->Data().GetAddressOf());
			context->VSSetShader(vertexShader->GetShader().Get(), nullptr, 0);
			context->GSSetConstantBuffers(0, 1, viewportIndexBuffer->Data().GetAddressOf());
			context->GSSetShader(standardGeometryShader->GetShader().Get(), nullptr, 0);
			context->PSSetConstantBuffers(0, 1, lightingConstantBuffer->Data().GetAddressOf());
			context->PSSetShader(litPixelShader->GetShader().Get(), nullptr, 0);
			context->DrawIndexed(indexBuffer->GetIndexCount(), 0, 0);

			// Hair pass
			context->VSSetShader(hairVertexShader->GetShader().Get(), nullptr, 0);
			context->GSSetConstantBuffers(0, 1, viewportIndexBuffer->Data().GetAddressOf());
			context->GSSetShader(hairGeometryShader->GetShader().Get(), nullptr, 0);
			context->PSSetShader(litLinePixelShader->GetShader().Get(), nullptr, 0);
			context->DrawIndexed(indexBuffer->GetIndexCount(), 0, 0);

			SetRenderMode(RenderMode::WIREFRAME);
			viewportIndex.index = 0;
			viewportIndexBuffer->SetData(viewportIndex);
			context->VSSetShader(flatVertexShader->GetShader().Get(), nullptr, 0);
			context->GSGetConstantBuffers(0, 1, viewportIndexBuffer->Data().GetAddressOf());
			context->GSSetShader(standardGeometryShader->GetShader().Get(), nullptr, 0);
			context->PSSetShader(unlitPixelShader->GetShader().Get(), nullptr, 0);
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

		TextureOptions options = { "E:/Programming/DirectX11/RTRHairModelling/ModelData/AngelinaDiffuseTex2048.raw", 2048, 2048 };
		diffuseTexture = std::make_unique<Texture>(options);
		device->GetContext()->PSSetShaderResources(0, 1, diffuseTexture->ResourceView().GetAddressOf());
		device->GetContext()->PSSetSamplers(0, 1, diffuseTexture->SamplerState().GetAddressOf());

		mvpConstantBuffer = std::make_shared<ConstantBuffer<MVPMatricesCBT>>();
		viewportIndexBuffer = std::make_shared<ConstantBuffer<ViewportIndexCBT>>();
		lightingConstantBuffer = std::make_shared<ConstantBuffer<LightingCBT>>();
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
		InputLayoutDescription hairVertexDescription = InputLayoutDescription::HairVertex();
		vertexShader = std::make_shared<VertexShader>(L"StandardVS", hairVertexDescription);
		flatVertexShader = std::make_shared<VertexShader>(L"FlatVS", hairVertexDescription);
		hairVertexShader = std::make_shared<VertexShader>(L"HairVS", hairVertexDescription);
		litPixelShader = std::make_shared<PixelShader>(L"LitPS");
		litLinePixelShader = std::make_shared<PixelShader>(L"LitLinesPS");
		unlitPixelShader = std::make_shared<PixelShader>(L"UnlitPS"); 
		standardGeometryShader = std::make_shared<GeometryShader>(L"StandardGS");
		hairGeometryShader = std::make_shared<GeometryShader>(L"HairGS");
	}

	void Renderer::Clear()
	{
		DirectX::SimpleMath::Color clearColor = DirectX::Colors::CornflowerBlue.v;
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
