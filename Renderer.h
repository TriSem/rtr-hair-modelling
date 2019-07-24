#pragma once

#include "framework.h"
#include "Graphics.h"
#include "Scene.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

using Microsoft::WRL::ComPtr;

namespace Rendering
{
	class Renderer
	{
	public:
		Renderer(HWND windowHandle, int width, int height);
		~Renderer();

		void Render(Scene &scene);
		void SetMultisampleCount(UINT count);

		const ComPtr<ID3D11Device> GetDevice() const;
		const std::shared_ptr<VertexShader> GetVertexShader() const;

		enum class RenderMode
		{
			SOLID,
			WIREFRAME
		};

	private:

		Scene scene;

		void InitializeAPI();
		void CreateDevice();
		void CheckMultisamplingSupport();
		void CreateSwapChain();
		void CreateBackBufferView();
		void CreateDepthStencilView();
		void BindViewsToPipeline();
		void SetViewport();
		void CreateShaders();

		void Clear();

		UINT multisampleCount;
		UINT msaaQuality;

		HWND windowHandle;

		int width;
		int height;
		RenderMode renderMode;

		D3D11_VIEWPORT viewport;

		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;
		ComPtr<IDXGISwapChain> swapChain;
		ComPtr<ID3D11Texture2D> depthStencilBuffer;
		ComPtr<ID3D11RenderTargetView> backBufferView;
		ComPtr<ID3D11DepthStencilView> depthStencilView;
		ComPtr<ID3D11RasterizerState> rasterizerState;

		std::shared_ptr<VertexShader> vertexShader;
		std::shared_ptr<PixelShader> pixelShader;
	};
}

