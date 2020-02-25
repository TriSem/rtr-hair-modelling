#pragma once

#include "framework.h"
#include "Graphics.h"
#include "Scene.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "SplitScreen.h"
#include "ConstantBuffer.h"
#include "Texture.h"

using Microsoft::WRL::ComPtr;

namespace Rendering
{
	enum class RenderMode
	{
		SOLID,
		WIREFRAME
	};

	class Renderer : public DeviceAccess
	{
	public:
		Renderer(HWND windowHandle, int width, int height);
		~Renderer();

		void Render(Scene &scene);
		void SetMultisampleCount(UINT count);

		void SetRenderMode(RenderMode renderMode);

		const ComPtr<ID3D11Device> GetDevice() const;
		const std::shared_ptr<VertexShader> GetVertexShader() const;

	private:

		void Initialize();
		void CheckMultisamplingSupport();
		void CreateSwapChain();
		void CreateBackBufferView();
		void CreateDepthStencilView();
		void BindViewsToPipeline();
		void CreateRasterizerStates();
		void CreateShaders();
		void Clear();

		Scene scene = {};
		uint32_t multisampleCount = 4;
		uint32_t msaaQuality = 1;

		HWND windowHandle;

		int width;
		int height;
		RenderMode renderMode = RenderMode::SOLID;

		ComPtr<IDXGISwapChain> swapChain = nullptr;
		ComPtr<ID3D11Texture2D> depthStencilBuffer = nullptr;
		ComPtr<ID3D11RenderTargetView> backBufferView = nullptr;
		ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;
		ComPtr<ID3D11RasterizerState> rasterizerStateSolid = nullptr;
		ComPtr<ID3D11RasterizerState> rasterizerStateWireframe = nullptr;

		std::unique_ptr<SplitScreen> splitScreen = nullptr;
		std::unique_ptr<Texture> diffuseTexture = nullptr;

		std::shared_ptr<VertexShader> vertexShader = nullptr;
		std::shared_ptr<VertexShader> hairVertexShader = nullptr;
		std::shared_ptr<VertexShader> flatVertexShader = nullptr;
		std::shared_ptr<PixelShader> unlitPixelShader = nullptr;
		std::shared_ptr<PixelShader> litPixelShader = nullptr;
		std::shared_ptr<PixelShader> litLinePixelShader = nullptr;
		std::shared_ptr<GeometryShader> standardGeometryShader = nullptr;
		std::shared_ptr<GeometryShader> hairGeometryShader = nullptr;
		std::shared_ptr<ConstantBuffer<MVPMatricesCBT>> mvpConstantBuffer = nullptr;
		std::shared_ptr<ConstantBuffer<ViewportIndexCBT>> viewportIndexBuffer = nullptr;
		std::shared_ptr<ConstantBuffer<LightingCBT>> lightingConstantBuffer = nullptr;

	};
}

