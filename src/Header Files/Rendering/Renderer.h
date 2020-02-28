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
	class Renderer : public DeviceAccess
	{
	public:
		Renderer(HWND windowHandle, int width, int height);
		~Renderer();

		void Render(Scene &scene);
		void SetMultisampleCount(uint32_t count);

	private:

		virtual void IssueRenderCommands() override;
		void Initialize();
		void CheckMultisamplingSupport();
		void CreateSwapChain();
		void CreateBackBufferView();
		void CreateDepthStencilView();
		void BindViewsToPipeline();
		void Clear();

		Scene scene = {};
		uint32_t multisampleCount = 4;
		uint32_t msaaQuality = 1;

		HWND windowHandle;

		int width;
		int height;

		ComPtr<IDXGISwapChain> swapChain = nullptr;
		ComPtr<ID3D11Texture2D> depthStencilBuffer = nullptr;
		ComPtr<ID3D11RenderTargetView> backBufferView = nullptr;
		ComPtr<ID3D11DepthStencilView> depthStencilView = nullptr;

		std::unique_ptr<SplitScreen> splitScreen = nullptr;

		std::shared_ptr<ConstantBuffer<MVPMatricesCBT>> mvpConstantBuffer = nullptr;
		std::shared_ptr<ConstantBuffer<ViewportIndexCBT>> viewportIndexBuffer = nullptr;
		std::shared_ptr<ConstantBuffer<LightingCBT>> lightingConstantBuffer = nullptr;
		std::shared_ptr<ConstantBuffer<MaterialCBT>> materialConstantBuffer = nullptr;
	};
}

