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

		Scene scene;

		void Initialize();
		void CheckMultisamplingSupport();
		void CreateSwapChain();
		void CreateBackBufferView();
		void CreateDepthStencilView();
		void BindViewsToPipeline();
		void CreateRasterizerStates();
		void CreateShaders();
		void Clear();

		UINT multisampleCount;
		UINT msaaQuality;

		HWND windowHandle;

		int width;
		int height;
		RenderMode renderMode;

		ComPtr<IDXGISwapChain> swapChain;
		ComPtr<ID3D11Texture2D> depthStencilBuffer;
		ComPtr<ID3D11RenderTargetView> backBufferView;
		ComPtr<ID3D11DepthStencilView> depthStencilView;
		ComPtr<ID3D11RasterizerState> rasterizerStateSolid;
		ComPtr<ID3D11RasterizerState> rasterizerStateWireframe;

		std::unique_ptr<SplitScreen> splitScreen;

		std::shared_ptr<VertexShader> vertexShader;
		std::shared_ptr<VertexShader> flatVertexShader;
		std::shared_ptr<PixelShader> unlitPixelShader;
		std::shared_ptr<PixelShader> litPixelShader;
		std::shared_ptr<GeometryShader> geometryShader;
		std::shared_ptr<ConstantBuffer<MVPMatricesCBT>> mvpConstantBuffer;
		std::shared_ptr<ConstantBuffer<ViewportIndexCBT>> viewportIndexBuffer;
		std::shared_ptr<ConstantBuffer<LightingCBT>> lightingConstantBuffer;
		std::unique_ptr<Texture> diffuseTexture;

	};
}

